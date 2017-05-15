--
-- q2admin
--
-- q2a_lua_plugman.lua
--
-- copyright 2009 Toni Spets
--

--
-- Q2Admin Lua plugin manager
--

local cfg = {}
local globals = {}
local plugins = {}
local q2a_config

-- ex table consists of extra functionality, declaration moved to q2a_lua.c
--ex = { }
ex.players = { }
local players_tmp = { }

local function q2a_plugin_call(plugin, func, ...)
	if type(plugin.env[func]) ~= 'function' then
		return nil
	end

	success, err, param = pcall(plugin.env[func], ...)
	if not success then
		gi.dprintf("Q2A Lua: Failed to call '%s' in '%s': %s\n", func, plugin.file, err)
		if not plugin.unloading then
			q2a_unload(plugin.file)
		end

		return false
	end

	return err, param
end

function ex.stuffcmd(client, str)
	gi.WriteByte(11)
	gi.WriteString(str)
	if client == nil then
		gi.multicast({})
	else
		gi.unicast(client, true)
	end
end

-- changes the text color to alternative (green by default)
function ex.hilight(str)
	local t = { }

	for i=1,string.len(str) do
		local b = string.byte(str, i)
		if (b >= 0x1B and b <= 0x7F) or ((b > 0x0A and b <= 0x11) and b ~= 0x0D) then
			b = b + 0x80
		end
		table.insert(t, string.char(b))
	end

	return table.concat(t)
end

-- get command arguments as a single string
function ex.args(index, glue)
	local t = { }

	if index == nil then
		index = 1
	end

	if glue == nil then
		glue = ' '
	end

	for i=index,gi.argc() do
		table.insert(t, gi.argv(i))
	end

	return table.concat(t, glue)
end

function ClientConnect(client, userinfo)
	ex.players[client] = { }
	for k, v in string.gmatch(userinfo, "\\([^\\]+)\\([^\\]+)") do
		ex.players[client][k] = v
	end

	local rejmsg = ""
	local success = true

	for i,plugin in pairs(plugins) do
		local ret, lrejmsg = q2a_plugin_call(plugin, "ClientConnect", client)
		if ret == false then
			rejmsg = lrejmsg
			success = false
		end
	end

	players_tmp[client] = ex.players[client]
	ex.players[client] = nil

	return success, rejmsg
end

function ClientUserinfoChanged(client, userinfo)
	if ex.players[client] == nil then
		return
	end

	local userinfo_t = { }
	for k, v in string.gmatch(userinfo, "\\([^\\]+)\\([^\\]+)") do
		userinfo_t[k] = v
	end

	ex.players[client] = userinfo_t

	--[[ needs some adjustment
	local modified = false
	for i,plugin in pairs(plugins) do
		local ret = q2a_plugin_call(plugin, "ClientUserinfoChanged", userinfo_t)
		if ret == true then
			modified = true
		end
	end


	if modified then
		local t = { }
		for k,v in pairs(ex.players[client]) do
			table.insert(t, k);
			table.insert(t, v);
		end
		return "\\"..table.concat(t, "\\")
	end

	return false
	--]]
end

-- not a complete deep copy, metatables are ignored
local function copy_table(orig)
	local new = {}

	for k,v in pairs(orig) do
		new[k] = v
	end

	return new
end

function q2a_init()
	if q2a_config == nil then
		-- quick implementation of *printf family functions
		do
			local dprintf = gi.dprintf
			local cprintf = gi.cprintf
			local bprintf = gi.bprintf
			local centerprintf = gi.centerprintf

			function gi.dprintf(fmt, ...)
				return dprintf(string.format(fmt, ...))
			end

			function gi.cprintf(client, level, fmt, ...)
				return cprintf(client, level, string.format(fmt, ...))
			end

			function gi.bprintf(level, fmt, ...)
				return bprintf(level, string.format(fmt, ...))
			end

			function gi.centerprintf(level, fmt, ...)
				return centerprintf(level, string.format(fmt, ...))
			end
		end

		q2a_config = gi.cvar("q2a_config", "config.lua")
	end

	gi.dprintf("Q2A Lua: Plugin Manager\n")
	gi.dprintf("Q2A Lua: Loading configuration %s\n", q2a_config.string);

	globals = copy_table(_G)
	globals.ex = ex
	globals.ClientConnect = nil
	globals.ClientUserinfoChanged = nil
	globals.q2a_init = nil
	globals.q2a_shutdown = nil
	globals.q2a_load = nil
	globals.q2a_unload = nil
	globals.q2a_reload = nil
	globals.q2a_call = nil

	chunk, err = loadfile(q2a_config.string)
	if chunk == nil then
		gi.dprintf("Q2A Lua: Failed to load configuration from %s (not fatal): %s\n", q2a_config.string, tostring(err))
		return
	else
		setfenv(chunk, cfg)
		success, err = pcall(chunk)
		if not success then
			gi.dprintf("Q2A Lua: Syntax error in config: %s\n", tostring(err))
			return
		end
	end

	if type(cfg.plugins) == 'table' then
		for k,v in pairs(cfg.plugins) do
			if type(v) == 'table' then
				q2a_load("plugins/"..tostring(k)..".lua", v)
                        else
                            gi.dprintf("Q2A Lua: Invalid configuration for plugin %s, expected a table, got %s\n", tostring(k), type(v))
			end
		end
	end
end

function q2a_shutdown()
	gi.dprintf("Q2A Lua: Shutdown in progress...\n")
	for i,plugin in pairs(plugins) do
		q2a_unload(plugin.file)
	end
	gi.dprintf("Q2A Lua: All plugins unloaded!\n")
end

function q2a_load(file, config)
	local success, err, chunk

	local plugin = {}
	plugin.file = file

	chunk, err = loadfile(plugin.file)
	if chunk == nil then
		gi.dprintf("Q2A Lua: Failed to load file %s: %s\n", plugin.file, tostring(err))
		return false
	end

	plugin.env = copy_table(globals)
	setfenv(chunk, plugin.env)
	success, err = pcall(chunk)

	if not success then
		gi.dprintf("Q2A Lua: Failed to compile file %s: %s\n", plugin.file, tostring(err))
		return false
	end

	gi.dprintf("Q2A Lua: Loaded plugin %s\n", plugin.file)
	q2a_plugin_call(plugin, 'q2a_load', config)

	table.insert(plugins, plugin)
	return true
end

function q2a_unload(file)
	for i,plugin in pairs(plugins) do
		if plugin.file == file then
			gi.dprintf("Q2A Lua: unloading %s\n", file)
			plugin.unloading = true
			-- let the plugin know we are unloading it
			q2a_plugin_call(plugin, 'q2a_unload')
			plugins[i] = nil
			return true
		end
	end

	return false
end

-- reload all plugins
function q2a_reload()
	gi.dprintf("Q2a Lua: Reloading...\n")
	q2a_shutdown()
	q2a_init()
end

function q2a_call(func, ...)

	if func == 'ClientBegin' then
		local arg = {...}
		local client = arg[1]
		if players_tmp[client] ~= nil then
			ex.players[client] = players_tmp[client]
			players_tmp[client] = nil
		end
	end

	for i,plugin in pairs(plugins) do
		q2a_plugin_call(plugin, func, ...)
	end

	if func == 'ClientDisconnect' then
		local arg = {...}
		local client = arg[1]
		ex.players[client] = nil
	end
end

function q2a_call_bool(func, def, ...)

	for i,plugin in pairs(plugins) do
		local ret = q2a_plugin_call(plugin, func, ...)
		if ret ~= nil and ret ~= def then
			return ret
		end
	end

	return def
end

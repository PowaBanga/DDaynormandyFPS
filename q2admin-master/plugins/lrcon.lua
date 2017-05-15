--
-- a very limited rcon by hifi <3  version 1.0
--
-- changes by TgT
-- 1.2 disable gamemap (causing crashes) and make sure sv_allow_map is 1
-- 1.1 fixed sv softmap and sv stuffall and maybe lrcon status crash

local version = "1.2"
gi.AddCommandString("sets q2a_lrcon "..version.."\n")

local quit_on_empty 
local cvars

local claimer = nil
local claimer_store = nil

-- set  servers sv_allow_map to 1

gi.cvar_set("sv_allow_map", "1")

function q2a_load(config)
    quit_on_empty = config.quit_on_empty
    cvars = config.cvars

    if quit_on_empty == nil or cvars == nil then
        gi.dprintf("Warning: lrcon config is invalid\n")
    end

    claimer_store = gi.cvar('lua_q2a_lrcon_storage', '')
end

function q2a_unload()
    if claimer then
        gi.cvar_forceset('lua_q2a_lrcon_storage', ex.players[claimer].name..ex.players[claimer].ip)
    else
        gi.cvar_forceset('lua_q2a_lrcon_storage', '')
    end
end

function ClientCommand(client)
    if(gi.argv(1) == 'lrcon') then
        if gi.argc() == 1 then
            gi.cprintf(client, PRINT_HIGH, 'Usage: lrcon <command> [parameters]\n')
            gi.cprintf(client, PRINT_HIGH, 'Type "lrcon help" for more information.\n')
        else
            local plr = ex.players[client]
            local ip = string.match(plr.ip, '^([^:]+)')
            local now = os.time()
            local cmd = gi.argv(2)

            if cmd == 'claim' then
                if claimer == nil then
                    claimer = client
                    gi.cprintf(client, PRINT_HIGH, 'You have claimed the server! Type lrcon <command> to set appropriate settings.\n')
                    gi.bprintf(PRINT_HIGH, '%s claimed the server\n', ex.players[claimer].name)
                else
                    gi.cprintf(client, PRINT_HIGH, 'This server has already been claimed by %s.\n', ex.players[claimer].name)
                end
                return true
            elseif cmd == 'release' then
                if client == claimer then
                    gi.cprintf(client, PRINT_HIGH, 'You released the server.\n')
                    gi.bprintf(PRINT_HIGH, '%s released the server, use "lrcon claim" to re-claim\n', ex.players[claimer].name)
		    claimer = nil
                else
                    gi.cprintf(client, PRINT_HIGH, 'You have not claimed this server.\n')
                end
                return true
            elseif cmd == 'help' then
                gi.cprintf(client, PRINT_HIGH, 'Limited rcon usage:\n')
                gi.cprintf(client, PRINT_HIGH, ' lrcon claim             - claim the server\n')
                gi.cprintf(client, PRINT_HIGH, ' lrcon release           - release the server to be re-claimed\n')
                gi.cprintf(client, PRINT_HIGH, ' lrcon <cvar>            - query cvar value\n')
                gi.cprintf(client, PRINT_HIGH, ' lrcon <cvar> <value>    - set cvar value\n')
                gi.cprintf(client, PRINT_HIGH, ' lrcon status            - get client status information\n')
                gi.cprintf(client, PRINT_HIGH, ' lrcon kick <id>         - kick a player\n')
                gi.cprintf(client, PRINT_HIGH, ' lrcon map <mapname>     - change map\n')
                --gi.cprintf(client, PRINT_HIGH, ' lrcon gamemap <mapname> - change map (keeping state)\n')
                return true
            else
                if client == claimer then
                    local cmd = gi.argv(2)
                    local param

                    if gi.argc() > 2 then
                        param = ex.args(3)
                    end

                    if cmd == 'set' and gi.argc() > 2 then
                        cmd = gi.argv(3)
                        param = ex.args(4)
                    end

                    for k,v in ipairs(cvars) do
                        if v == cmd then
                            local cvar = gi.cvar(cmd, "")
                            if param == nil then
                                gi.cprintf(client, PRINT_HIGH, '%s = "%s"\n', cvar.name, cvar.string)
                            else
                                gi.cvar_set(cvar.name, param)

                                if cvar.latched_string == param then
                                    gi.cprintf(client, PRINT_HIGH, '%s -> "%s" (latched)\n', cvar.name, cvar.latched_string)
                                    gi.bprintf(PRINT_HIGH, '%s changed server settings: %s -> "%s" (latched)\n', ex.players[client].name, cvar.name, cvar.latched_string)
                                else
                                    gi.cprintf(client, PRINT_HIGH, '%s -> "%s"\n', cvar.name, cvar.string)
                                    gi.bprintf(PRINT_HIGH, '%s changed server settings: %s -> "%s"\n', ex.players[client].name, cvar.name, cvar.string)
                                end
                            end
                            return true
                        end
                    end

                    if cmd == 'status' then
			gi.cprintf(client, PRINT_HIGH, 'num  name             address\n')
			gi.cprintf(client, PRINT_HIGH, '---  ---------------  ---------------\n')

			for i,plr in pairs(ex.players) do
				if plr ~= nil
                            		gi.cprintf(client, PRINT_HIGH, "%3d  %-15s  %s\n", i, plr.name, string.match(plr.ip, '^([^:]+)'))
                            	end
			end
                        return true
                    end

                    if cmd == 'sv' and gi.argc() > 2 then
                        cmd = gi.argv(3)
                        param = gi.argv(4)

                        if cmd == 'softmap' then
                            if param == nil or string.len(param) < 1 then
                                gi.cprintf(client, PRINT_HIGH, 'Usage: sv softmap <mapname>\n')
                            else
                                gi.AddCommandString('sv softmap '..param)
                            end
                            return true
                        end

                        if cmd == 'stuffcmd' then
                            if param == nil or string.len(param) < 1 then
                                gi.cprintf(client, PRINT_HIGH, 'Usage: sv stuffcmd <client/all> <command>\n')
                            else
                                local rest = ''
                                for i=5,gi.argc() do
                                    rest = rest..gi.argv(i)..' '
                                end
                                if param == "all" then
                                	gi.AddCommandString('sv stuffcmd all '..rest)
                                else
                                	gi.AddCommandString('sv stuffcmd '..param..' '..rest)
	
                                end
                            end
                            return true
                        end
                    end

                    if cmd == 'map' then
                        if param == nil then
                            gi.cprintf(client, PRINT_HIGH, 'Usage: map <mapname>\n')
                        else
                            gi.AddCommandString('map '..param)
                        end
                        return true
                    end

                    --[[if cmd == 'gamemap' then
                        if param == nil then
                            gi.cprintf(client, PRINT_HIGH, 'Usage: gamemap <mapname>\n')
                        else
                            gi.AddCommandString('gamemap '..param)
                        end
                        return true
                    end--]]

                    if cmd == 'kick' then
			if param == nil or tonumber(param) == nil then
                            gi.cprintf(client, PRINT_HIGH, 'Usage: kick <id>\n')
			else
			    gi.AddCommandString('kick '..tostring(param - 1))
			end
                        return true
                    end
                else
                    gi.cprintf(client, PRINT_HIGH, 'You have not claimed the server yet.\n')
                    return true
                end

                gi.cprintf(client, PRINT_HIGH, 'Unknown or disallowed lrcon command: "%s"\n', cmd)
                gi.cprintf(client, PRINT_HIGH, 'Type "lrcon help" for more information.\n')
            end
        end
        return true
    end

    return false
end

function ClientConnect(client)
    local plr = ex.players[client]

    if ex.players[client].name..ex.players[client].ip == claimer_store.string then
        gi.cvar_forceset('lua_q2a_lrcon_storage', '');
        claimer = client
    end

    return true
end

-- quit server after a match to reset state
function ClientDisconnect(client)
    local numplrs = #ex.players

    if quit_on_empty and numplrs == 1 then
        gi.AddCommandString('quit')
    end

    if client == claimer then
        claimer = nil
    end
end

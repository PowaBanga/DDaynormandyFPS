--[[
simple !version* script by TgT 
TODO
	if version table is empty dont crash -- not yet
CHANGELOG
v1.4
+ prevent !q2client abuse
v1.3
+ alternative for !version !versionsay !versionsayall
v1.2
+ different way to report a version, works with any client (i hope)
+ any word that starts with !version reports a client version, not just 3 configured ones
v1.1
+ added three common responds for version ( !version !versionsay !versionsayall)
v1
+ first attempt to simulate !version with q2admin script
+ works with q2pro clients
]]--

local version = "1.3"
gi.AddCommandString("sets q2a_version "..version.."\n")

local ver_req = {"!versio.+"}
local clver = {}

function ClientBegin(client)
    local plr = ex.players[client]
    allowed = 1
    ex.stuffcmd(client,"!q2client $version")
end

local sm = string.match

function ClientCommand(client)
    local cmd = gi.argv(1)
-- 
    if allowed cmd == "!q2client" then
        local cver = ""
        for i=2,gi.argc() do
            cver = cver .. gi.argv(i) .. " " 
        end

        clver[client] = cver
        allowed = nil
        return true
    end
-- version stuff
	for k,v in pairs(ver_req) do
		word = sm(cmd,v)	    
		if word then
			gi.cprintf(client, PRINT_HIGH, 'num  name             client version\n')
			gi.cprintf(client, PRINT_HIGH, '---  ---------------  ---------------------------------------------\n')
		for i,plr in pairs(ex.players) do
				--if plr.name.clver[i] ~= nil then
			               gi.cprintf(client, PRINT_HIGH, "%3d  %-15s  %s\n",i, plr.name,clver[i])
				--end
	  		end
		return true
		end
	end
end

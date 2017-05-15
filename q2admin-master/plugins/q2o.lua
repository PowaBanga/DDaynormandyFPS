--
-- Q2Online matchmaker experimental authentication plugin
--

local tokens = {}

function q2a_load()

    gi.dprintf("Q2Online match authentication loaded\n")

    -- refresh current players if reloaded during match
    for i,plr in pairs(ex.players) do
        ClientBegin(i)
    end 

end

function ClientBegin(client)
    ex.stuffcmd(client, "_token $_q2o_token")
end

function ClientCommand(client)

    if gi.argv(1) == '_token' then
        tokens[client] = gi.argv(2)
        return true
    end 

    return false

end

function ClientDisconnect(client)
    tokens[client] = nil 
end

function ServerCommand()

    if gi.argv(2) == 'q2o_status' then

        gi.dprintf('num score name            token\n')
        gi.dprintf('--- ----- --------------- ---------------\n')

        for i,plr in pairs(ex.players) do
            gi.dprintf("%3d %5d %-15s %s\n", i-1, ex.ClientStats(i, STAT_FRAGS), plr.name, tokens[i] == nil and '' or tokens[i])
        end 

        return true

    end 

    if gi.argv(2) == 'q2o_centerprintall' then

	local msg = string.gsub(ex.args(3), '\\n', '\n')
        for i,plr in pairs(ex.players) do
            gi.centerprintf(i, msg)
        end 

        return true

    end 

    return false

end

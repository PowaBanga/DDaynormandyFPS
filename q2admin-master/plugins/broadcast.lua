--
-- simple broadcast plugin by TgT v1.0
--
-- DESCRIPTION
--      Broadcast message to clients on server
-- USAGE: broadcast <msg>

-- setup
local version = "1.0"
gi.AddCommandString("sets q2a_broadcast "..version.."\n")
gi.cvar_set("broadcast", "")


function RunFrame()
        -- get content of the broadcast cvar
        local bcast = gi.cvar("broadcast","").string
        -- if broadcast cvar is set
        if bcast ~= '' then
                --gi.dprintf( "*** "..bcast.." \n")

                -- unset broadcast
                gi.cvar_set("broadcast", "")

                -- tell the msg to clients
                gi.bprintf(PRINT_CHAT, '*** '..bcast..'\n')
        end
end

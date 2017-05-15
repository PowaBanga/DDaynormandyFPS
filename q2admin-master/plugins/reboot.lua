--
-- automagic nightly server rebooting
--

-- will try to reboot during this hour, if the server stays occupied it will be postponed 24h
local reboot_utc_hour = 03

local booted = os.time()
local maxclients = gi.cvar("maxclients", "")

function RunFrame()
    local now = os.time()
    local hour = math.floor(now % 86400 / 3600)

    if hour == reboot_utc_hour and now - booted > 3600 then
        -- reboot only after the last player leaves
        if #ex.players > 0 then
            return
        end

        gi.AddCommandString("quit")
    end
end

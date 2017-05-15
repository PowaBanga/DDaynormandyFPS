---
--- Coinflip.lua script by TgT
---

local version = "1.0"
gi.AddCommandString("sets q2a_cointoss "..version.."\n")

-- edit this (seconds)
local flip_flood = 5

-- do not tuch
local flip_last = os.time()
local flip_allow = true

-- HEADS or TAILS

function ClientCommand(client)
	local plr = ex.players[client]
	if(gi.argv(1) == '!flip') then -- 
		--print("got FLIP")
		time_asked = os.time()
		time_asked = tonumber(time_asked)
		if gi.argc() == 1 then

			diff = tonumber(time_asked) - tonumber(flip_last)
			--print(diff)
			if diff > flip_flood  or flip_allow == true then

				flip_allow = nil 

				local number = math.random( 0, 99 )
				math.randomseed( os.time() )
				--gi.bprintf(PRINT_HIGH, time_asked.." ")
				gi.bprintf(PRINT_HIGH, ex.hilight(plr.name))
	                        --gi.bprintf(PRINT_HIGH, "AI ")
			
				if number < 49 then
					gi.bprintf(PRINT_HIGH, ' flipped a coin: '..ex.hilight("HEADS")..'\n')
					flip_allow = false
					--flip_last = os.time()
				elseif number > 50 then
                                	gi.bprintf(PRINT_HIGH, ' flipped a coin: '..ex.hilight("TAILS")..'\n')
					flip_allow = false
				elseif number == 49 then
                                	gi.bprintf(PRINT_HIGH, ' flipped a coin but was stolen by a Angry Bird!\n')
					flip_allow = true
				elseif number == 50 then
                                	gi.bprintf(PRINT_HIGH, ' flipped a coin but it fell on its side!\n')
					flip_allow = true
				--flip_last = time_asked
				end -- prints
			flip_last = time_asked
			else
				gi.cprintf(client, PRINT_HIGH, "Flip again? Cmon, wait for "..flip_flood - diff.." more seconds!\n")
			end -- if time or true
		end -- argc
		return true
	end -- flip
	return false
end -- function

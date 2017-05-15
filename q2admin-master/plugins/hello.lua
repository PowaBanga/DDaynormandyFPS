function q2a_load()
    gi.dprintf("hello.lua: q2a_load()\n")
end

function q2a_unload()
    gi.dprintf("hello.lua: q2a_unload()\n")
end

function ClientConnect(client, userinfo)
    local plr = players[client]
    gi.dprintf("hello.lua: %s@%s is connecting\n", plr.name, plr.ip)
    return true
end

function ClientBegin(client)
    local plr = players[client]
    gi.dprintf("hello.lua: %s@%s joined the game\n", plr.name, plr.ip)
end

function ClientDisconnect(client)
    local plr = players[client]
    gi.dprintf("hello.lua: %s@%s disconnected\n", plr.name, plr.ip)
end

function ClientUserinfoChanged(client, userinfo)
    gi.dprintf("hello.lua: ClientUserinfoChanged(%d)\n", client)
end

function LevelChanged(level)
    gi.dprintf("hello.lua: LevelChanged(%s)\n", level)
end

function RunFrame()
    --gi.dprintf("hello.lua: RunFrame()\n")
end

function ClientThink(client)
    --gi.dprintf("hello.lua: ClientThink()\n")
end

function ClientCommand(client)

    gi.dprintf("hello.lua: ClientCommand(%d)\n", client)

    local cmd = gi.argv(0)
    if cmd == "hello" then
        if gi.argc() > 1 then
            local name = gi.argv(1)
            gi.cprintf(client, PRINT_HIGH, "hello.lua: Hello, %s!\n", name)
        else
            gi.cprintf(client, PRINT_HIGH, "hello.lua: usage: hello <name>\n")
        end

        return true
    end

    if cmd == "ping" then
        gi.centerprintf(client, "Pong!")
        return true
    end

    if cmd == "kickme" then
        gi.cprintf(client, PRINT_HIGH, "Kick requested.\n")
        gi.AddCommandString("kick "..client)
        return true
    end

    return false
end

function ServerCommand(cmd)
    gi.dprintf("hello.lua: ServerCommand(%s)\n", cmd)

    if cmd == "hello" then
        if gi.argc() > 2 then
            local name = gi.argv(2)
            gi.dprintf("hello.lua: Hello, %s!\n", name)
        else
            gi.dprintf("hello.lua: usage: hello <name>\n")
        end

        return true
    end

    return false
end

gi.dprintf("hello.lua: plugin loaded\n")

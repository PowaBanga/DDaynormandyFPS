OBJS = q2a_main.o q2a_util.o q2a_lua.o q2a_lua_import.o q2a_lua_export.o

$(GAME_NAME): q2a_lua_plugman.h $(OBJS)
	$(CC) $(CFLAGS) -rdynamic -shared -o ../$@ $(OBJS) $(LDFLAGS)

clean: 
	/bin/rm -f $(OBJS) q2a_lua_plugman.h q2a_lua_plugman.o bin2hex

$*.o: $*.c
	$(CC) $(CFLAGS) -c $*.c

bin2hex:
	$(CC) -o bin2hex bin2hex.c

q2a_lua_plugman.h: bin2hex
	cat q2a_lua_plugman.lua | ./bin2hex q2a_lua_plugman > q2a_lua_plugman.h

all: $(GAME_NAME)

# DO NOT DELETE

bin2hex: bin2hex.c
q2a_main.o: g_local.h q_shared.h game.h
q2a_util.o: g_local.h q_shared.h game.h
q2a_lua.o: g_local.h q_shared.h game.h q2a_lua_plugman.h
q2a_lua_import.o: g_local.h q_shared.h game.h q2a_lua_plugman.h
q2a_lua_export.o: g_local.h q_shared.h game.h q2a_lua_plugman.h
q2a_lua_plugman.h: q2a_lua_plugman.lua bin2hex.c

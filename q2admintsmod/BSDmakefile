# q2admin.so FreeBSD makefile

# -I/usr/i486-linuxlibc1/include


CC      = gcc
# CC      = /usr/i486-linuxlibc1/bin/gcc
CFLAGS  = -m32 -O2 -fPIC -ffast-math -w -DGAME_INCLUDE -DLINUX -DSTDC_HEADERS -I/usr/include
LDFLAGS = -m32 -lm -shared

OUTFILES = g_main.o zb_spawn.o zb_vote.o zb_ban.o zb_cmd.o zb_flood.o zb_init.o zb_log.o zb_lrcon.o zb_msgqueue.o zb_util.o zb_zbot.o zb_zbotcheck.o regex.o zb_disable.o zb_checkvar.o md4.o

q2admin.so: $(OUTFILES)
	$(CC) $(OUTFILES) $(LDFLAGS) -o gamei386.so

zip: q2admin.so
	strip gamei386.so
	zip -9 q2admin-gamei386.zip gamei386.so

clean:
	rm -f $(OUTFILES) gamei386.so

md4.o: md4.c
g_main.o: g_main.c g_local.h q_shared.h game.h regex.h
zb_ban.o: zb_ban.c g_local.h q_shared.h game.h regex.h
zb_spawn.o: zb_spawn.c g_local.h q_shared.h game.h regex.h
zb_vote.o: zb_vote.c g_local.h q_shared.h game.h regex.h
zb_cmd.o: zb_cmd.c g_local.h q_shared.h game.h regex.h
zb_flood.o: zb_zbot.c g_local.h q_shared.h game.h regex.h
zb_init.o: zb_init.c g_local.h q_shared.h game.h regex.h
zb_log.o: zb_zbot.c g_local.h q_shared.h game.h regex.h
zb_lrcon.o: zb_lrcon.c g_local.h q_shared.h game.h regex.h
zb_msgqueue.o: zb_msgqueue.c g_local.h q_shared.h game.h regex.h
zb_util.o: zb_util.c g_local.h q_shared.h game.h regex.h
zb_zbot.o: zb_zbot.c g_local.h q_shared.h game.h regex.h
zb_zbotcheck.o: zb_zbotcheck.c g_local.h q_shared.h game.h regex.h
regex.o: regex.c g_local.h q_shared.h game.h regex.h
zb_disable.o: zb_disable.c g_local.h q_shared.h game.h regex.h
zb_checkvar.o: zb_checkvar.c g_local.h q_shared.h game.h regex.h


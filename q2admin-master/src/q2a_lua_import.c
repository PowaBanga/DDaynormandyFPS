//
// q2admin
//
// q2a_lua_import.c
//
// copyright 2009 Toni Spets
//

#include "g_local.h"
#include "q2a_lua.h"

void q2a_lua_LogMessage(char *msg)
{
	char *err_msg;
	if(!L) return;

	q2a_fpu_lua();

	lua_getglobal(L, "q2a_call");
	lua_pushstring(L, "LogMessage");
	lua_pushstring(L, msg);

	if(lua_pcall(L, 2, 0, 0) != 0) {
		err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("Lua: LogMessage returned error: %s\n", err_msg);
	}

	q2a_fpu_q2();
}

qboolean q2a_lua_ClientConnect(int client, char *userinfo)
{
	char *err_msg = NULL, *rej_msg = NULL;
	qboolean ret = TRUE;

	if(!L) return ret;

	q2a_fpu_lua();

	lua_getglobal(L, "ClientConnect");
	lua_pushnumber(L, client + 1);
	lua_pushstring(L, userinfo);

	if(lua_pcall(L, 2, 2, 0) == 0) {
		ret = lua_toboolean(L, -2);
		rej_msg = (char *)lua_tostring(L, -1);
	} else {
		err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("Lua: ClientConnect returned error: %s\n", err_msg);
	}

	q2a_fpu_q2();

	if (!ret && rej_msg != NULL) {
		sprintf(userinfo, "\\rejmsg\\%s", rej_msg);
	}

	return ret;
}

void q2a_lua_ClientBegin(int client)
{
	char *err_msg;
	if(!L) return;

	q2a_fpu_lua();

	lua_getglobal(L, "q2a_call");
	lua_pushstring(L, "ClientBegin");
	lua_pushnumber(L, client + 1);

	if(lua_pcall(L, 2, 0, 0) != 0) {
		err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("Lua: ClientBegin returned error: %s\n", err_msg);
	}

	q2a_fpu_q2();
}

void q2a_lua_ClientDisconnect(int client)
{
	char *err_msg;
	if(!L) return;

	q2a_fpu_lua();

	lua_getglobal(L, "q2a_call");
	lua_pushstring(L, "ClientDisconnect");
	lua_pushnumber(L, client + 1);

	if(lua_pcall(L, 2, 0, 0) != 0) {
		err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("Lua: ClientDisconnect returned error: %s\n", err_msg);
	}

	q2a_fpu_q2();
}

void q2a_lua_ClientThink(int client)
{
	char *err_msg;
	if(!L) return;

	q2a_fpu_lua();

	lua_getglobal(L, "q2a_call");
	lua_pushstring(L, "ClientThink");
	lua_pushnumber(L, client + 1);

	if(lua_pcall(L, 2, 0, 0) != 0) {
		err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("Lua: ClientThink returned error: %s\n", err_msg);
	}

	q2a_fpu_q2();
}

void q2a_lua_RunFrame(void)
{
	char *err_msg;
	if(!L) return;

	q2a_fpu_lua();

	lua_getglobal(L, "q2a_call");
	lua_pushstring(L, "RunFrame");

	if(lua_pcall(L, 1, 0, 0) != 0) {
		err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("Lua: RunFrame returned error: %s\n", err_msg);
	}

	q2a_fpu_q2();
}

qboolean q2a_lua_ClientCommand(int client)
{
	char *err_msg;
	qboolean ret = FALSE;

	if(!L) return ret;

	q2a_fpu_lua();

	lua_getglobal(L, "q2a_call_bool");
	lua_pushstring(L, "ClientCommand");
	lua_pushboolean(L, FALSE);
	lua_pushnumber(L, client + 1);

	if(lua_pcall(L, 3, 1, 0) == 0) {
		ret = lua_toboolean(L, -1);
	} else {
		err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("Lua: ClientCommand returned error: %s\n", err_msg);
		ret = FALSE;
	}

	q2a_fpu_q2();

	return ret;
}

qboolean q2a_lua_ServerCommand(const char *cmd)
{
	char *err_msg;
	qboolean ret = FALSE;

	if(!L) return ret;

	q2a_fpu_lua();

	lua_getglobal(L, "q2a_call_bool");
	lua_pushstring(L, "ServerCommand");
	lua_pushboolean(L, FALSE);
	lua_pushstring(L, cmd);

	if(lua_pcall(L, 3, 1, 0) == 0) {
		ret = lua_toboolean(L, -1);
	} else {
		err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("Lua: ServerCommand returned error: %s\n", err_msg);
		ret = FALSE;
	}

	q2a_fpu_q2();

	return ret;
}

void q2a_lua_ClientUserinfoChanged(int client, char *userinfo)
{
	char *err_msg;

	if(!L) return;

	q2a_fpu_lua();

	lua_getglobal(L, "ClientUserinfoChanged");
	lua_pushnumber(L, client + 1);
	lua_pushstring(L, userinfo);

	if(lua_pcall(L, 2, 0, 0) != 0) {
		err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("Lua: ClientUserinfoChanged returned error: %s\n", err_msg);
	}

	q2a_fpu_q2();
}

void q2a_lua_SpawnEntities(char *mapname, char *entities, char *spawnpoint)
{
	char *err_msg;

	if(!L) return;

	q2a_fpu_lua();

	lua_getglobal(L, "q2a_call");
	lua_pushstring(L, "LevelChanged");
	lua_pushstring(L, mapname);
	lua_pushstring(L, entities);
	lua_pushstring(L, spawnpoint);

	if(lua_pcall(L, 4, 0, 0) != 0) {
		err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("Lua: LevelChanged returned error: %s\n", err_msg);
	}

	q2a_fpu_q2();
}

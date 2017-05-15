//
// q2admin
//
// q2a_lua.c
//
// copyright 2009 Toni Spets
//

#include "g_local.h"
#include <dlfcn.h>
#include "q2a_lua.h"

/* include the compiled plugin manager code */
#include "q2a_lua_plugman.h"

lua_State *L = NULL;
void *lua_dll = NULL;

static int q2a_lua_cvar_index(lua_State *L)
{
	cvar_t *cvar = *(cvar_t **)lua_touserdata(L, 1);
	char *key = (char *)luaL_checkstring(L, 2);

	if(!strcmp(key, "name")) {
		lua_pushstring(L, cvar->name);
		return 1;
	}

	if(!strcmp(key, "string")) {
		lua_pushstring(L, cvar->string);
		return 1;
	}

	if(!strcmp(key, "latched_string")) {
		lua_pushstring(L, cvar->latched_string);
		return 1;
	}

	if(!strcmp(key, "flags")) {
		lua_pushnumber(L, cvar->flags);
		return 1;
	}

	if(!strcmp(key, "modified")) {
		lua_pushboolean(L, cvar->modified);
		return 1;
	}

	if(!strcmp(key, "value")) {
		lua_pushnumber(L, cvar->value);
		return 1;
	}

	return 0;
}

void q2a_lua_init(void)
{
	if(L) return;

	lua_dll = dlopen("liblua5.1.so", RTLD_NOW|RTLD_GLOBAL);
	if(!lua_dll) {
		lua_dll = dlopen("liblua.so.5.1", RTLD_NOW|RTLD_GLOBAL);
	}
	if(!lua_dll) {
		lua_dll = dlopen("liblua.so.5", RTLD_NOW|RTLD_GLOBAL);
	}
	if(!lua_dll) {
		lua_dll = dlopen("liblua.so.5.1.0", RTLD_NOW|RTLD_GLOBAL);
	}
	if(!lua_dll) {
		gi.dprintf("Q2A Lua: Loading Lua shared object failed, tried: liblua5.1.so, liblua.so.5.1, liblua.so.5 and liblua.so.5.1.0\n");
		return;
	}

	q2a_fpu_lua();

	L = lua_open();
	luaL_openlibs(L);

	/* register cvar stuff */
	luaL_newmetatable(L, "Cvar");
	lua_pushcfunction(L, q2a_lua_cvar_index);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);

	/* register "constants" */
	lua_pushnumber(L, 0);
	lua_setglobal(L, "PRINT_LOW");
	lua_pushnumber(L, 1);
	lua_setglobal(L, "PRINT_MEDIUM");
	lua_pushnumber(L, 2);
	lua_setglobal(L, "PRINT_HIGH");
	lua_pushnumber(L, 3);
	lua_setglobal(L, "PRINT_CHAT");

	lua_pushnumber(L, CVAR_ARCHIVE);
	lua_setglobal(L, "CVAR_ARCHIVE");
	lua_pushnumber(L, CVAR_USERINFO);
	lua_setglobal(L, "CVAR_USERINFO");
	lua_pushnumber(L, CVAR_SERVERINFO);
	lua_setglobal(L, "CVAR_SERVERINFO");
	lua_pushnumber(L, CVAR_NOSET);
	lua_setglobal(L, "CVAR_NOSET");
	lua_pushnumber(L, CVAR_LATCH);
	lua_setglobal(L, "CVAR_LATCH");

	lua_pushnumber(L, STAT_FRAGS);
	lua_setglobal(L, "STAT_FRAGS");

	/* register gi functions */
	lua_newtable(L); // gi table

	lua_pushcfunction(L, q2a_lua_gi_dprintf);
	lua_setfield(L, 1, "dprintf");

	lua_pushcfunction(L, q2a_lua_gi_bprintf);
	lua_setfield(L, 1, "bprintf");

	lua_pushcfunction(L, q2a_lua_gi_cprintf);
	lua_setfield(L, 1, "cprintf");

	lua_pushcfunction(L, q2a_lua_gi_centerprintf);
	lua_setfield(L, 1, "centerprintf");

	lua_pushcfunction(L, q2a_lua_gi_argc);
	lua_setfield(L, 1, "argc");

	lua_pushcfunction(L, q2a_lua_gi_argv);
	lua_setfield(L, 1, "argv");

	lua_pushcfunction(L, q2a_lua_gi_AddCommandString);
	lua_setfield(L, 1, "AddCommandString");

	lua_pushcfunction(L, q2a_lua_gi_cvar);
	lua_setfield(L, 1, "cvar");

	lua_pushcfunction(L, q2a_lua_gi_cvar_set);
	lua_setfield(L, 1, "cvar_set");

	lua_pushcfunction(L, q2a_lua_gi_cvar_forceset);
	lua_setfield(L, 1, "cvar_forceset");

	lua_pushcfunction(L, q2a_lua_gi_multicast);
	lua_setfield(L, 1, "multicast");

	lua_pushcfunction(L, q2a_lua_gi_unicast);
	lua_setfield(L, 1, "unicast");

	lua_pushcfunction(L, q2a_lua_gi_WriteChar);
	lua_setfield(L, 1, "WriteChar");

	lua_pushcfunction(L, q2a_lua_gi_WriteByte);
	lua_setfield(L, 1, "WriteByte");

	lua_pushcfunction(L, q2a_lua_gi_WriteShort);
	lua_setfield(L, 1, "WriteShort");

	lua_pushcfunction(L, q2a_lua_gi_WriteLong);
	lua_setfield(L, 1, "WriteLong");

	lua_pushcfunction(L, q2a_lua_gi_WriteFloat);
	lua_setfield(L, 1, "WriteFloat");

	lua_pushcfunction(L, q2a_lua_gi_WriteString);
	lua_setfield(L, 1, "WriteString");

	lua_pushcfunction(L, q2a_lua_gi_WriteAngle);
	lua_setfield(L, 1, "WriteAngle");

	lua_setglobal(L, "gi");

	/* register ex functions, these may change without prior notice */
	lua_newtable(L); // ex table

	lua_pushcfunction(L, q2a_lua_ex_ClientStats);
	lua_setfield(L, 1, "ClientStats");

	lua_setglobal(L, "ex");

	gi.dprintf("Q2A Lua: Loading stored Lua code, %d bytes\n", sizeof(q2a_lua_plugman));

	/* load plugin manager code */
	if(luaL_loadbuffer(L, (const char *)q2a_lua_plugman, sizeof(q2a_lua_plugman), "q2a_lua_plugman") != 0) {
		gi.dprintf("Q2A Lua: Plugin manager code load failed, disabling Lua support\n");
		q2a_fpu_q2();
		q2a_lua_shutdown();
		return;
	}
	if(lua_pcall(L, 0, 0, 0) != 0) {
		char *err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("q2a_lua_init: Plugin manager code execution failed, disabling Lua support: %s\n", err_msg);
		q2a_fpu_q2();
		q2a_lua_shutdown();
		return;
	}

	/* run the initialization Lua routine */
	lua_getglobal(L, "q2a_init");
	if(lua_pcall(L, 0, 0, 0) != 0) {
		char *err_msg = (char *)lua_tostring(L, -1);
		gi.dprintf("q2a_lua_init: calling q2a_init failed: %s\n", err_msg);
		q2a_fpu_q2();
		q2a_lua_shutdown();
		return;
	}

	q2a_fpu_q2();
}

void q2a_lua_reload(void)
{
	if(!L) return;

	q2a_fpu_lua();

	lua_getglobal(L, "q2a_reload");
	lua_pcall(L, 0, 0, 0);

	q2a_fpu_q2();
}

void q2a_lua_shutdown(void)
{
	if(!L) return;

	q2a_fpu_lua();

	/* run the shutdown Lua routine */
	lua_getglobal(L, "q2a_shutdown");
	lua_pcall(L, 0, 0, 0);

	lua_close(L);
	L = NULL;

	q2a_fpu_q2();

	dlclose(lua_dll);
}

void lua_tovec3(lua_State *L, int index, vec3_t dst)
{
	if( !lua_istable(L, index) ) {
		gi.dprintf("nope\n");
		return;
	}

	lua_rawgeti(L, index, 1);
	lua_rawgeti(L, index, 2);
	lua_rawgeti(L, index, 3);

	dst[0] = (vec_t)lua_tonumber(L, -3);
	dst[1] = (vec_t)lua_tonumber(L, -2);
	dst[2] = (vec_t)lua_tonumber(L, -1);
}

/* x86 workaround for Lua, fsck you Carmack! */
unsigned short q2a_fpuword = 0;
void q2a_fpu_q2 (void)
{
#ifdef __i386__
	assert(q2a_fpuword != 0);
	__asm__ __volatile__ ("fldcw %0" : : "m" (q2a_fpuword));
	q2a_fpuword = 0;
#endif
}

void q2a_fpu_lua (void)
{
#ifdef __i386__
	unsigned short tmp = 0x37F;
	assert(q2a_fpuword == 0);
	__asm__ __volatile__ ("fnstcw %0" : "=m" (q2a_fpuword));
	__asm__ __volatile__ ("fldcw %0" : : "m" (tmp));
#endif
}

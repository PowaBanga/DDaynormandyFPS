//
// q2admin
//
// q2a_lua.h
//
// copyright 2009 Toni Spets
//

#ifndef Q2A_LUA_H

#include <assert.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

extern lua_State *L;

/* utility functions */
void lua_tovec3(lua_State *L, int index, vec3_t dst);

/* functions exported to Lua */
int q2a_lua_gi_dprintf(lua_State *L);
int q2a_lua_gi_bprintf(lua_State *L);
int q2a_lua_gi_cprintf(lua_State *L);
int q2a_lua_gi_centerprintf(lua_State *L);
int q2a_lua_gi_argc(lua_State *L);
int q2a_lua_gi_argv(lua_State *L);
int q2a_lua_gi_AddCommandString(lua_State *L);
int q2a_lua_gi_cvar(lua_State *L);
int q2a_lua_gi_cvar_set(lua_State *L);
int q2a_lua_gi_cvar_forceset(lua_State *L);
int q2a_lua_gi_multicast(lua_State *L);
int q2a_lua_gi_unicast(lua_State *L);
int q2a_lua_gi_WriteChar(lua_State *L);
int q2a_lua_gi_WriteByte(lua_State *L);
int q2a_lua_gi_WriteShort(lua_State *L);
int q2a_lua_gi_WriteLong(lua_State *L);
int q2a_lua_gi_WriteFloat(lua_State *L);
int q2a_lua_gi_WriteString(lua_State *L);
int q2a_lua_gi_WriteAngle(lua_State *L);

/* very custom, might change without prior notice */
int q2a_lua_ex_ClientStats(lua_State *L);

#define Q2A_LUA_H
#endif

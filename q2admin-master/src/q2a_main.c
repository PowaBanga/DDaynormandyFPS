//
// q2admin
//
// g_main.c
//
// copyright 2000 Shane Powell
// copyright 2009 Toni Spets
//

#include "g_local.h"

#ifdef _WIN32

#include <windows.h>
HINSTANCE hdll;
#define DLLNAME		"gamex86.real.dll"

#else // if not win32

#include <dlfcn.h>
void *hdll = NULL;

#if defined (__x86_64__)
#define DLLNAME		"gamex86_64.real.so"
#elif defined (__i386__)
#define DLLNAME		"gamei386.real.so"
#elif defined (__arm__)
#define DLLNAME         "gamearm.real.so"
#endif

#endif

typedef game_export_t  *GAMEAPI (game_import_t *import);

game_import_t  gi;
game_export_t  globals;
game_export_t  *dllglobals;

char moddir[256];

void Init (void)
{
	if(hdll == NULL) return;

	dllglobals->Init();
	copyDllInfo();
}

void Shutdown (void)
{
	q2a_lua_shutdown();

	if(hdll == NULL) return;

	dllglobals->Shutdown();

#ifdef _WIN32
	FreeLibrary(hdll);
#else
	dlclose(hdll);
#endif

	hdll = NULL;
}

void SpawnEntities (char *mapname, char *entities, char *spawnpoint)
{
	if(hdll == NULL) return;

	q2a_lua_SpawnEntities(mapname, entities, spawnpoint);

	dllglobals->SpawnEntities(mapname, entities, spawnpoint);
	copyDllInfo();
}

qboolean ClientConnect (edict_t *ent, char *userinfo)
{
	qboolean ret;
	int client = getClientOffset(ent);

	if(hdll == NULL) return FALSE;

	// if any lua ClientConnect return false, so do we
	if(!q2a_lua_ClientConnect(client, userinfo))
		return FALSE;

	ret = dllglobals->ClientConnect(ent, userinfo);
	copyDllInfo();
	return ret;
}

void ClientBegin (edict_t *ent)
{
	int client = getClientOffset(ent);

	if(hdll == NULL) return;

	q2a_lua_ClientBegin(client);

	dllglobals->ClientBegin(ent);
	copyDllInfo();
}

void ClientUserinfoChanged (edict_t *ent, char *userinfo)
{
	int client = getClientOffset(ent);

	if(hdll == NULL) return;

	q2a_lua_ClientUserinfoChanged(client, userinfo);

	dllglobals->ClientUserinfoChanged(ent, userinfo);
	copyDllInfo();
}

void ClientDisconnect (edict_t *ent)
{
	int client = getClientOffset(ent);

	if(hdll == NULL) return;

	q2a_lua_ClientDisconnect(client);

	dllglobals->ClientDisconnect(ent);
	copyDllInfo();
}

void ClientCommand (edict_t *ent)
{
	int client = getClientOffset(ent);
	
	if(hdll == NULL) return;

	if(q2a_lua_ClientCommand(client))
		return;

	dllglobals->ClientCommand(ent);
	copyDllInfo();
}

void ClientThink (edict_t *ent, usercmd_t *ucmd)
{
	int client = getClientOffset(ent);
	
	if(hdll == NULL) return;
		
	q2a_lua_ClientThink(client);
	
	dllglobals->ClientThink(ent, ucmd);
	
	copyDllInfo();
}

void RunFrame(void)
{
	if(hdll == NULL) return;

	q2a_lua_RunFrame();

	dllglobals->RunFrame();
	copyDllInfo();
}

void ServerCommand (void)
{
	char *cmd;

	if(hdll == NULL) return;

	cmd = gi.argv(1);

	if(!q2a_strcmp(cmd, "lua_reload")) {
		q2a_lua_reload();
		return;
	}

	if(!q2a_strcmp(cmd, "lua_init")) {
		q2a_lua_init();
		return;
	}

	if(!q2a_strcmp(cmd, "lua_shutdown")) {
		q2a_lua_shutdown();
		return;
	}

	if(!q2a_strcmp(cmd, "lua_restart")) {
		q2a_lua_shutdown();
		q2a_lua_init();
		return;
	}

	if(q2a_lua_ServerCommand(cmd))
		return;
		
	dllglobals->ServerCommand();
	copyDllInfo();
}

void game_bprintf(int printlevel, char *fmt, ...)
{
	va_list args;
	char buf[1024];

	va_start(args, fmt);
	vsnprintf(buf, sizeof buf, fmt, args);
	buf[sizeof buf - 1] = 0;
	va_end(args);

	q2a_lua_LogMessage(buf);

	gi.bprintf(printlevel, "%s", buf);
}

void game_dprintf(char *fmt, ...)
{
	va_list args;
	char buf[1024];

	va_start(args, fmt);
	vsnprintf(buf, sizeof buf, fmt, args);
	buf[sizeof buf - 1] = 0;
	va_end(args);

	q2a_lua_LogMessage(buf);

	gi.dprintf("%s", buf);
}

void game_cprintf(edict_t *ent, int printlevel, char *fmt, ...)
{
	va_list args;
	char buf[1024];

	va_start(args, fmt);
	vsnprintf(buf, sizeof buf, fmt, args);
	buf[sizeof buf - 1] = 0;
	va_end(args);

	if (ent == NULL)
		q2a_lua_LogMessage(buf);

	gi.cprintf(ent, printlevel, "%s", buf);
}

/*
=================
GetGameAPI
 
Returns a pointer to the structure with all entry points
and global variables
=================
*/
game_export_t *GetGameAPI(game_import_t *import)
{
	char dllname[256];
	cvar_t *game;
	GAMEAPI *getapi;
	game_import_t g_import;

	gi = *import;

	game = gi.cvar ("game", "baseq2", 0);
	q2a_strcpy(moddir, game->string);
	
	if(moddir[0] == 0)
		q2a_strcpy(moddir, "baseq2");

	gi.dprintf ("Q2Admin %s running %s\n", Q2A_VERSION, moddir);
	gi.cvar ("*Q2Admin", Q2A_VERSION, CVAR_SERVERINFO|CVAR_NOSET);

	q2a_lua_init();

	sprintf(dllname, "%s/%s", moddir, DLLNAME);
#ifdef _WIN32
	hdll = LoadLibrary(dllname);
#else
	hdll = dlopen(dllname, RTLD_NOW);
#endif
	
	if(hdll == NULL)
	{
		gi.dprintf ("Q2A: Unable to load DLL %s.\n", dllname);
		return &globals;
	}

	gi.dprintf("Q2A: Loaded game DLL: %s.\n", dllname);
		
#ifdef _WIN32
	getapi = (GAMEAPI *)GetProcAddress (hdll, "GetGameAPI");
#else
	getapi = (GAMEAPI *)dlsym(hdll, "GetGameAPI");
#endif
	
	if(getapi == NULL)
	{
#ifdef _WIN32
		FreeLibrary(hdll);
#else
		dlclose(hdll);
#endif
		hdll = NULL;

		gi.dprintf ("Q2A: No \"GetGameApi\" entry in DLL %s.\n", dllname);
		return &globals;
	}

	/* proxy a few imports so we can capture the data */
	memcpy(&g_import, import, sizeof(game_import_t));
	g_import.bprintf = game_bprintf;
	g_import.dprintf = game_dprintf;
	g_import.cprintf = game_cprintf;

	dllglobals = (*getapi)(&g_import);

	globals.apiversion = dllglobals->apiversion;

	globals.Init = Init;
	globals.Shutdown = Shutdown;
	globals.SpawnEntities = SpawnEntities;

	globals.WriteGame = dllglobals->WriteGame;
	globals.ReadGame = dllglobals->ReadGame;

	globals.WriteLevel = dllglobals->WriteLevel;
	globals.ReadLevel = dllglobals->ReadLevel;
	
	globals.ClientConnect = ClientConnect;
	globals.ClientBegin = ClientBegin;
	globals.ClientUserinfoChanged = ClientUserinfoChanged;
	globals.ClientDisconnect = ClientDisconnect;
	globals.ClientCommand = ClientCommand;
	globals.ClientThink = ClientThink;
	
	globals.RunFrame = RunFrame;
	
	globals.ServerCommand = ServerCommand;

	copyDllInfo();

	return &globals;
}

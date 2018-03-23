/*
Copyright (C) 2000 Shane Powell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

//
// q2admin
//
// g_local.h
//
// copyright 2000 Shane Powell
//

// g_local.h -- local definitions for game module
#include "q_shared.h"

// define GAME_INCLUDE so that game.h does not define the
// short, server-visible gclient_t and edict_t structures,
// because we define the full size ones in this file
#define GAME_INCLUDE
#include "game.h"

#include <ctype.h>
#include "regex.h"

//*** UPDATE START ***
#define PRIVATE_COMMANDS  8
#define ALLOWED_MAXCMDS   50
#define ALLOWED_MAXCMDS_SAFETY 45
#define TIMERS_MAX    4
//*** UPDATE END ***

// maximum length of random strings used to check for hacked quake2.exe
#define RANDOM_STRING_LENGTH    20

// the "gameversion" client command will print this plus compile date
#define GAMEVERSION    "baseq2"

// protocol bytes that can be directly added to messages
#define svc_muzzleflash   1
#define svc_muzzleflash2  2
#define svc_temp_entity   3
#define svc_layout    4
#define svc_inventory   5

//==================================================================

// view pitching times
#define DAMAGE_TIME    0.5
#define FALL_TIME    0.3


// edict->spawnflags
// these are set with checkboxes on each entity in the map editor
#define SPAWNFLAG_NOT_EASY   0x00000100
#define SPAWNFLAG_NOT_MEDIUM  0x00000200
#define SPAWNFLAG_NOT_HARD   0x00000400
#define SPAWNFLAG_NOT_DEATHMATCH 0x00000800
#define SPAWNFLAG_NOT_COOP   0x00001000

// edict->flags
#define FL_FLY     0x00000001
#define FL_SWIM     0x00000002 // implied immunity to drowining
#define FL_IMMUNE_LASER   0x00000004
#define FL_INWATER    0x00000008
#define FL_GODMODE    0x00000010
#define FL_NOTARGET    0x00000020
#define FL_IMMUNE_SLIME   0x00000040
#define FL_IMMUNE_LAVA   0x00000080
#define FL_PARTIALGROUND  0x00000100 // not all corners are valid
#define FL_WATERJUMP   0x00000200 // player jumping out of water
#define FL_TEAMSLAVE   0x00000400 // not the first on the team
#define FL_NO_KNOCKBACK   0x00000800
#define FL_POWER_ARMOR   0x00001000 // power armor (if any) is active
#define FL_RESPAWN    0x80000000 // used for item respawning

#define FRAMETIME    0.1

// memory tags to allow dynamic memory to be cleaned up
#define TAG_GAME    765  // clear when unloading the dll
#define TAG_LEVEL    766  // clear when loading a new level


#define MELEE_DISTANCE   80

#define BODY_QUEUE_SIZE   8

typedef enum
{
	DAMAGE_NO,
	DAMAGE_YES,   // will take damage if hit
	DAMAGE_AIM   // auto targeting recognizes this
}

damage_t;

typedef enum
{
	WEAPON_READY,
	WEAPON_ACTIVATING,
	WEAPON_DROPPING,
	WEAPON_FIRING
}

weaponstate_t;

typedef enum
{
	AMMO_BULLETS,
	AMMO_SHELLS,
	AMMO_ROCKETS,
	AMMO_GRENADES,
	AMMO_CELLS,
	AMMO_SLUGS
}

ammo_t;

//deadflag
#define DEAD_NO     0
#define DEAD_DYING    1
#define DEAD_DEAD    2
#define DEAD_RESPAWNABLE  3

//range
#define RANGE_MELEE    0
#define RANGE_NEAR    1
#define RANGE_MID    2
#define RANGE_FAR    3

//gib types
#define GIB_ORGANIC    0
#define GIB_METALLIC   1

//monster ai flags
#define AI_STAND_GROUND   0x00000001
#define AI_TEMP_STAND_GROUND 0x00000002
#define AI_SOUND_TARGET   0x00000004
#define AI_LOST_SIGHT   0x00000008
#define AI_PURSUIT_LAST_SEEN 0x00000010
#define AI_PURSUE_NEXT   0x00000020
#define AI_PURSUE_TEMP   0x00000040
#define AI_HOLD_FRAME   0x00000080
#define AI_GOOD_GUY    0x00000100
#define AI_BRUTAL    0x00000200
#define AI_NOSTEP    0x00000400
#define AI_DUCKED    0x00000800
#define AI_COMBAT_POINT   0x00001000
#define AI_MEDIC    0x00002000
#define AI_RESURRECTING   0x00004000

//monster attack state
#define AS_STRAIGHT    1
#define AS_SLIDING    2
#define AS_MELEE    3
#define AS_MISSILE    4

// armor types
#define ARMOR_NONE    0
#define ARMOR_JACKET   1
#define ARMOR_COMBAT   2
#define ARMOR_BODY    3
#define ARMOR_SHARD    4

// power armor types
#define POWER_ARMOR_NONE  0
#define POWER_ARMOR_SCREEN  1
#define POWER_ARMOR_SHIELD  2

// handedness values
#define RIGHT_HANDED   0
#define LEFT_HANDED    1
#define CENTER_HANDED   2

// game.serverflags values
#define SFL_CROSS_TRIGGER_1  0x00000001
#define SFL_CROSS_TRIGGER_2  0x00000002
#define SFL_CROSS_TRIGGER_3  0x00000004
#define SFL_CROSS_TRIGGER_4  0x00000008
#define SFL_CROSS_TRIGGER_5  0x00000010
#define SFL_CROSS_TRIGGER_6  0x00000020
#define SFL_CROSS_TRIGGER_7  0x00000040
#define SFL_CROSS_TRIGGER_8  0x00000080
#define SFL_CROSS_TRIGGER_MASK 0x000000ff

// noise types for PlayerNoise
#define PNOISE_SELF    0
#define PNOISE_WEAPON   1
#define PNOISE_IMPACT   2

// edict->movetype values
typedef enum
{
	MOVETYPE_NONE,   // never moves
	MOVETYPE_NOCLIP,  // origin and angles change with no interaction
	MOVETYPE_PUSH,   // no clip to world, push on box contact
	MOVETYPE_STOP,   // no clip to world, stops on box contact
    
	MOVETYPE_WALK,   // gravity
	MOVETYPE_STEP,   // gravity, special edge handling
	MOVETYPE_FLY,
	MOVETYPE_TOSS,   // gravity
	MOVETYPE_FLYMISSILE, // extra size to monsters
	MOVETYPE_BOUNCE
}

movetype_t;

// this structure is left intact through an entire game
// it should be initialized at dll load time, and read/written to
// the server.ssv file for savegames
typedef struct
	{
		char  helpmessage1[512];
		char  helpmessage2[512];
		int   helpchanged; // flash F1 icon if non 0, play sound
		// and increment only if 1, 2, or 3
		gclient_t *clients;  // [maxclients]
		
		// can't store spawnpoint in level, because
		// it would get overwritten by the savegame restore
		char  spawnpoint[512]; // needed for coop respawns
		
		// store latched cvars here that we want to get at often
		int   maxclients;
		int   maxentities;
		
		// cross level triggers
		int   serverflags;
		
		// items
		int   num_items;
		
		qboolean autosaved;
	}
	
game_locals_t;

// this structure is cleared as each map is entered
// it is read/written to the level.sav file for savegames
typedef struct
	{
		int   framenum;
		float  time;
		
		char  level_name[MAX_QPATH]; // the descriptive name (Outer Base, etc)
		char  mapname[MAX_QPATH];  // the server name (base1, etc)
		char  nextmap[MAX_QPATH];  // go here when fraglimit is hit
		
		// intermission state
		float  intermissiontime;  // time the intermission was started
		char  *changemap;
		int   exitintermission;
		vec3_t  intermission_origin;
		vec3_t  intermission_angle;
		
		edict_t  *sight_client; // changed once each frame for coop games
		
		edict_t  *sight_entity;
		int   sight_entity_framenum;
		edict_t  *sound_entity;
		int   sound_entity_framenum;
		edict_t  *sound2_entity;
		int   sound2_entity_framenum;
		
		int   pic_health;
		
		int   total_secrets;
		int   found_secrets;
		
		int   total_goals;
		int   found_goals;
		
		int   total_monsters;
		int   killed_monsters;
		
		edict_t  *current_entity; // entity running from G_RunFrame
		int   body_que;   // dead bodies
		
		int   power_cubes;  // ugly necessity for coop
	}
	
level_locals_t;

extern game_locals_t   game;
extern level_locals_t   level;
extern game_import_t   gi;
extern game_export_t   globals;

extern edict_t     *g_edicts;

#define FOFS(x)     (int)&(((edict_t *)0)->x)
#define STOFS(x)    (int)&(((spawn_temp_t *)0)->x)
#define LLOFS(x)    (int)&(((level_locals_t *)0)->x)
#define CLOFS(x)    (int)&(((gclient_t *)0)->x)

#define random()    ((rand () & 0x7fff) / ((float)0x7fff))
#define crandom()    (2.0 * (random() - 0.5))

#define world     (&g_edicts[0])

// item spawnflags
#define ITEM_TRIGGER_SPAWN  0x00000001
#define ITEM_NO_TOUCH   0x00000002
// 6 bits reserved for editor flags
// 8 bits used as power cube id bits for coop games
#define DROPPED_ITEM   0x00010000
#define DROPPED_PLAYER_ITEM  0x00020000
#define ITEM_TARGETS_USED  0x00040000

// fields are needed for spawning from the entity string
// and saving / loading games
#define FFL_SPAWNTEMP   1

typedef enum {
	F_INT,
	F_FLOAT,
	F_LSTRING,   // string on disk, pointer in memory, TAG_LEVEL
	F_GSTRING,   // string on disk, pointer in memory, TAG_GAME
	F_VECTOR,
	F_ANGLEHACK,
	F_EDICT,   // index on disk, pointer in memory
	F_ITEM,    // index on disk, pointer in memory
	F_CLIENT,   // index on disk, pointer in memory
	F_IGNORE
}

fieldtype_t;

typedef struct
	{
		char *name;
		int  ofs;
		fieldtype_t type;
		int  flags;
	}
	
field_t;

// damage flags
#define DAMAGE_RADIUS      0x00000001 // damage was indirect
#define DAMAGE_NO_ARMOR      0x00000002 // armour does not protect from this damage
#define DAMAGE_ENERGY      0x00000004 // damage is from an energy based weapon
#define DAMAGE_NO_KNOCKBACK     0x00000008 // do not affect velocity, just view angles
#define DAMAGE_BULLET      0x00000010  // damage is from a bullet (used for ricochets)
#define DAMAGE_NO_PROTECTION    0x00000020  // armor, shields, invulnerability, and godmode have no effect

#define DEFAULT_BULLET_HSPREAD    300
#define DEFAULT_BULLET_VSPREAD    500
#define DEFAULT_SHOTGUN_HSPREAD    1000
#define DEFAULT_SHOTGUN_VSPREAD    500
#define DEFAULT_DEATHMATCH_SHOTGUN_COUNT 12
#define DEFAULT_SHOTGUN_COUNT    12
#define DEFAULT_SSHOTGUN_COUNT    20

//============================================================================
// client_t->anim_priority
#define ANIM_BASIC  0  // stand / run
#define ANIM_WAVE  1
#define ANIM_JUMP  2
#define ANIM_PAIN  3
#define ANIM_ATTACK  4
#define ANIM_DEATH  5

// this structure is cleared on each PutClientInServer(),
// except for 'client->pers'
struct gclient_s
	{
		// known to server
		player_state_t ps;    // communicated by server to clients
		int     ping;
	};
	
struct edict_s
	{
		entity_state_t  s;
		struct gclient_s *client; // NULL if not a player
		// the server expects the first part
		// of gclient_s to be a player_state_t
		// but the rest of it is opaque
		qboolean inuse;
		int   linkcount;
		
		// FIXME: move these fields to a server private sv_entity_t
		link_t  area;    // linked to a division node or leaf
		
		int   num_clusters;  // if -1, use headnode instead
		int   clusternums[MAX_ENT_CLUSTERS];
		int   headnode;   // unused if num_clusters != -1
		int   areanum, areanum2;
		
		//================================
		
		int   svflags;
		vec3_t  mins, maxs;
		vec3_t  absmin, absmax, size;
		solid_t  solid;
		int   clipmask;
		edict_t  *owner;
		
		// DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
		// EXPECTS THE FIELDS IN THAT ORDER!
		//================================
	};
	
// zbot detector global stuff
struct chatflood_s
	{
		qboolean      chatFloodProtect;
		int           chatFloodProtectNum;
		int           chatFloodProtectSec;
		int           chatFloodProtectSilence;
	};
	
#define MAXIMPULSESTOTEST 256
	
#define RANDCHAR()      (random() < 0.3) ? '0' + (int)(9.9 * random()) : 'A' + (int)(26.9 * random())
	
#define BANLISTFILE      "q2adminban.txt"
#define CFGFILE       "q2admin.txt"
	
#define DEFAULTVOTECOMMAND    "vote"
#define DEFAULTRECONNECTMSG    "Please wait to be reconnected to the server - this is normal for this level of bot protection.\nThe fastest way to do this is not to change any client info e.g. your name or skin."
	
#define DEFAULTUSERDISPLAY    "%s is using a client side proxy."
#define DEFAULTTSDISPLAY    "%s is using a speed cheat."
#define DEFAULTHACKDISPLAY    "%s is using a modified client."
#define DEFAULTSKINCRASHMSG    "%s tried to crash the server."
#define DEFAULTFLOODMSG     "%s changed names too many times."
#define DEFAULTCHATFLOODMSG    "%s is making too much noise."
#define DEFAULTSKINFLOODMSG    "%s changed skin too many times."
#define DEFAULTCL_PITCHSPEED_KICKMSG "cl_pitchspeed changes not allowed on this server."
#define DEFAULTCL_ANGLESPEEDKEY_KICKMSG "cl_anglespeedkey changes not allowed on this server."
#define DEFAULTBANMSG     "You are banned from this server!"
#define DEFAULTCHABANMSG    "Message banned."
#define DEFAULTLOCKOUTMSG    "This server is currently locked."
	
typedef struct banstruct
	{
		regex_t    *r;
		qboolean   exclude;
		byte    type;
		byte    loadType;
		byte    ip[4];
		byte    subnetmask;
		char    nick[80];
		char    password[80];
		char    *msg;
		long    maxnumberofconnects;
		long    numberofconnects;
		long    bannum;
		float    timeout;
		struct chatflood_s floodinfo;
		struct banstruct *next;
	}
	
baninfo_t;

#define NOTUSED   0
#define NICKALL   1
#define NICKEQ   2
#define NICKLIKE  3
#define NICKRE   4
#define NICKBLANK  5

#define LT_PERM   1
#define LT_TEMP   2

typedef struct chatbanstruct
	{
		regex_t     *r;
		byte     type;
		byte     loadType;
		long     bannum;
		char     chat[256];
		char     *msg;
		struct chatbanstruct *next;
	}
	
chatbaninfo_t;

#define CNOTUSED  0
#define CHATLIKE  1
#define CHATRE   2

typedef struct
	{
		byte     command;
		float     timeout;
		unsigned long   data;
		char     *str;
	}
	
CMDQUEUE;

//*** UPDATE START ***
typedef struct
	{
		char   action[256];
		int    start;
	}
	
timers_t;
//*** UPDATE END ***

typedef struct
	{
		qboolean  admin;
		unsigned char retries;
		unsigned char rbotretries;
		CMDQUEUE  cmdQueue[ALLOWED_MAXCMDS]; // command queue - UPDATE
		int    maxCmds;
		unsigned long clientcommand; // internal proxy commands
		char   teststr[9];
		int    charindex;
		//long   logfilereadpos;
		int    logfilenum;
		long   logfilecheckpos;
		char   buffer[256]; // log buffer
		char   ipaddress[40];
		byte   ipaddressBinary[4];
		byte   impulse;
		byte   inuse;
		char   name[16];
		char   skin[40];  // skin/model information.
		int    rate;
		int    maxfps;
		int    cl_pitchspeed;
		float   cl_anglespeedkey;
		baninfo_t  *baninfo;
		long   namechangetimeout;
		int    namechangecount;
		long   skinchangetimeout;
		int    skinchangecount;
		long   chattimeout;
		int    chatcount;
		char   userinfo[MAX_INFO_STRING + 45];
		FILE   *stuffFile;
		int    impulsesgenerated;
		char   lastcmd[8192];
		struct   chatflood_s floodinfo;
		short   zbc_angles[2][2];
		int    zbc_tog;
		int    zbc_jitter;
		float   zbc_jitter_time;
		float   zbc_jitter_last;
		int    votescast;
		int    votetimeout;
		int    msg;
		
		// used to test the alias (and connect) command with random strings
		char   hack_teststring1[RANDOM_STRING_LENGTH+1];
		char   hack_teststring2[RANDOM_STRING_LENGTH+1];
		char   hack_teststring3[RANDOM_STRING_LENGTH+1];
		char   hack_timescale[RANDOM_STRING_LENGTH+1];
		int    hacked_disconnect;
		byte   hacked_disconnect_ip[4];
		int    checked_hacked_exe;
		
		// used to test the variables check list
		char   hack_checkvar[RANDOM_STRING_LENGTH+1];
		int    checkvar_idx;
		
		//*** UPDATE START ***
		char   gl_driver[256];
		int    gl_driver_changes;
		int    pmodver;
		int    pmod;
		int    pmod_noreply_count;
		int    pcmd_noreply_count;
		int    pver;
		int    q2a_admin;
		int    q2a_bypass;
		int    msec_count;
		int    msec_last;
		int    frames_count;
		int    msec_bad;
		float   msec_start;
		int    done_server_and_blocklist;
		int    userinfo_changed_count;
		int    userinfo_changed_start;
		int    private_command;
		int    timescale;
		qboolean  show_fps;
		qboolean  vid_restart;
		qboolean  private_command_got[PRIVATE_COMMANDS];
		char   serverip[16];
		char   cmdlist_stored[256];
		int    cmdlist;
		int    cmdlist_timeout;
		int    userid;
		int    newcmd_timeout;
		timers_t  timers[TIMERS_MAX];
		int    blocklist;
		int    speedfreeze;
		int    enteredgame;
		//*** UPDATE END ***
	}
	
proxyinfo_t;

typedef struct
	{
		byte   inuse;
		char   name[16];
	}
	
proxyreconnectinfo_t;

#define MAXDETECTRETRIES   3

#define CCMD_STARTUPTEST   0x000001
#define CCMD_ZPROXYCHECK2   0x000002
#define CCMD_ZBOTDETECTED   0x000004
#define CCMD_BANNED     0x000008
#define CCMD_NCSILENCE    0x000010
#define CCMD_KICKED     0x000020
#define CCMD_SELECTED    0x000040
#define CCMD_CSILENCE    0x000080
#define CCMD_PCSILENCE    0x000100
#define CCMD_VOTED     0x000200
#define CCMD_VOTEYES    0x000400
#define CCMD_NITRO2PROXY   0x000800
#define CCMD_RATBOTDETECT   0x001000
#define CCMD_RATBOTDETECTNAME  0x002000
#define CCMD_ZBOTCLEAR    0x004000
#define CCMD_RBOTCLEAR    0x008000
#define CCMD_SCSILENCE    0x010000
#define CCMD_RECONNECT    0x020000
#define CCMD_ALIASCHECKSTARTED  0x040000
#define CCMD_WAITFORALIASREPLY1  0x080000
#define CCMD_WAITFORALIASREPLY2  0x100000
#define CCMD_WAITFORCONNECTREPLY 0x200000
#define CCMD_REMEMBERHACK   0x400000
#define CCMD_CLIENTOVERFLOWED  0x800000

#define LEVELCHANGE_KEEP   (CCMD_SCSILENCE | CCMD_CSILENCE | CCMD_PCSILENCE | CCMD_ZBOTDETECTED | CCMD_KICKED | CCMD_NITRO2PROXY | CCMD_ZBOTCLEAR | CCMD_RBOTCLEAR | CCMD_BANNED | CCMD_RECONNECT | CCMD_REMEMBERHACK )
#define BANCHECK     (CCMD_BANNED | CCMD_RECONNECT)

enum _commands
{
	QCMD_STARTUP,
	QCMD_STARTUPTEST,
	QCMD_CLEAR,
	QCMD_DISCONNECT,
	QCMD_CUSTOM,
	QCMD_ZPROXYCHECK1,
	QCMD_ZPROXYCHECK2,
	QCMD_DISPLOGFILE,
	QCMD_DISPLOGFILELIST,
	QCMD_DISPLOGEVENTLIST,
	QCMD_CONNECTCMD,
	QCMD_LOGTOFILE1,
	QCMD_LOGTOFILE2,
	QCMD_LOGTOFILE3,
	QCMD_GETIPALT,
	QCMD_RESTART,
	QCMD_CLIPTOMAXRATE,
	QCMD_CLIPTOMINRATE,
	QCMD_SETUPMAXFPS,
	QCMD_FORCEUDATAUPDATE,
	QCMD_SETMAXFPS,
	QCMD_SETMINFPS,
	QCMD_DISPBANS,
	QCMD_DISPLRCONS,
	QCMD_DISPFLOOD,
	QCMD_DISPSPAWN,
	QCMD_DISPVOTE,
	QCMD_DISPDISABLE,
	QCMD_CHANGENAME,
	QCMD_CHANGESKIN,
	QCMD_BAN,
	QCMD_DISPCHATBANS,
	QCMD_STUFFCLIENT,
	QCMD_TESTADMIN,
	QCMD_TESTADMIN2,  // UPDATE
	QCMD_TESTADMIN3,  // UPDATE
	QCMD_RUNVOTECMD,
	QCMD_TESTRATBOT,
	QCMD_TESTRATBOT2,
	QCMD_TESTRATBOT3,
	QCMD_TESTRATBOT4,
	QCMD_LETRATBOTQUIT,
	QCMD_TESTTIMESCALE,
	QCMD_TESTSTANDARDPROXY,
	QCMD_TESTALIASCMD1,
	QCMD_TESTALIASCMD2,
	QCMD_SETUPCL_PITCHSPEED,
	QCMD_FORCEUDATAUPDATEPS,
	QCMD_SETUPCL_ANGLESPEEDKEY,
	QCMD_FORCEUDATAUPDATEAS,
	QCMD_RECONNECT,
	QCMD_KICK,
	QCMD_MSGDISCONNECT,
	QCMD_DISPCHECKVAR,
	QCMD_CHECKVARTESTS,
	//*** UPDATE START ***
	QCMD_AUTH,
	QCMD_PMODVERTIMEOUT,
	QCMD_PMODVERTIMEOUT_INGAME,
	QCMD_SHOWMOTD,
	QCMD_EXECMAPCFG,
	QCMD_PRIVATECOMMAND,
	QCMD_GL_CHECK,
	QCMD_SETUPTIMESCALE,
	QCMD_SETTIMESCALE,
	QCMD_SPAMBYPASS,
	QCMD_GETCMDQUEUE,
	QCMD_TESTCMDQUEUE
	//*** UPDATE END ***
};

enum zb_logtypesenum
{
	LT_ZBOT,
	LT_ZBOTIMPULSES,
	LT_IMPULSES,
	LT_NAMECHANGE,
	LT_SKINCHANGE,
	LT_CHATBAN,
	LT_CLIENTCONNECT,
	LT_CLIENTBEGIN,
	LT_CLIENTDISCONNECT,
	LT_CLIENTKICK,
	LT_CLIENTCMDS,
	LT_CLIENTLRCON,
	LT_BAN,
	LT_CHAT,
	LT_SERVERSTART,
	LT_SERVERINIT,
	LT_SERVEREND,
	LT_INTERNALWARN,
	LT_PERFORMANCEMONITOR,
	LT_DISABLECMD,
	LT_ENTITYCREATE,
	LT_ENTITYDELETE,
	LT_INVALIDIP,
	LT_ADMINLOG,  // UPDATE
	LT_CLIENTUSERINFO, // UPDATE
	LT_PRIVATELOG,  // UPDATE
};

#define IW_UNEXCEPTEDCMD  1
#define IW_UNKNOWNCMD   2
#define IW_ZBOTDETECT   3
#define IW_STARTUP    4
#define IW_STARTUPTEST   5
#define IW_ZBOTTEST    6
#define IW_OVERFLOWDETECT  7
#define IW_STARTUPFAIL   8
#define IW_Q2ADMINCFGLOAD  9
#define IW_LOGSETUPLOAD   10
#define IW_BANSETUPLOAD   11
#define IW_LRCONSETUPLOAD  12
#define IW_FLOODSETUPLOAD  13
#define IW_SPAWNSETUPLOAD  14
#define IW_VOTESETUPLOAD  15
#define IW_ZBCHECK    16
#define IW_DISABLESETUPLOAD  17
#define IW_CHECKVARSETUPLOAD 18
#define IW_INVALIDIPADDRESS  19

#define MINIMUMTIMEOUT   5
#define MAXSTARTTRY    500

#define getEntOffset(ent)  (((char *)ent - (char *)globals.edicts) / globals.edict_size)
#define getEnt(entnum)   (edict_t *)((char *)globals.edicts + (globals.edict_size * entnum))

// where the command can't be run?
#define CMDWHERE_CFGFILE  0x01
#define CMDWHERE_CLIENTCONSOLE 0x02
#define CMDWHERE_SERVERCONSOLE 0x04

// type of command
#define CMDTYPE_NONE   0
#define CMDTYPE_LOGICAL   1
#define CMDTYPE_NUMBER   2
#define CMDTYPE_STRING   3

typedef void     CMDRUNFUNC(int startarg, edict_t *ent, int client);
typedef void     CMDINITFUNC(char *arg);

typedef struct
	{
		char    *cmdname;
		byte    cmdwhere;
		byte    cmdtype;
		void    *datapoint;
		CMDRUNFUNC   *runfunc;
		CMDINITFUNC   *initfunc;
	}
	
zbotcmd_t;

extern game_import_t gi;
extern game_export_t globals;
extern game_export_t *dllglobals;
extern cvar_t   *rcon_password, *gamedir, *maxclients, *logfile, *rconpassword, *port, *q2admintxt, *q2adminbantxt; // UPDATE

extern char    dllname[256];
extern char    zbotuserdisplay[256];
extern char    timescaleuserdisplay[256];
extern char    hackuserdisplay[256];
extern char    skincrashmsg[256];
extern char    defaultreconnectmessage[256];
extern char    moddir[256];

extern qboolean   soloadlazy;
extern qboolean   dllloaded;
extern qboolean   quake2dirsupport;
extern qboolean   zbotdetect;
extern qboolean   displayzbotuser;
extern qboolean   displaynamechange;
extern qboolean   dopversion;
extern qboolean   disconnectuserimpulse;
extern qboolean   disconnectuser;
extern qboolean   mapcfgexec;
extern qboolean   checkClientIpAddress;
extern qboolean   votecountnovotes;

extern int    votepasspercent;
extern int    voteminclients;
extern int    clientMaxVoteTimeout;
extern int    clientMaxVotes;
extern int    numofdisplays;
extern int    maximpulses;

extern byte    impulsesToKickOn[MAXIMPULSESTOTEST];
extern byte    maxImpulses;

extern qboolean   displayimpulses;

//r1ch 2005-01-26 disable hugely buggy commands BEGIN
/*extern qboolean   play_team_enable;
extern qboolean   play_all_enable;
extern qboolean   play_person_enable;*/
//r1ch 2005-01-26 disable hugely buggy commands END

extern qboolean   printmessageonplaycmds;
extern qboolean   say_person_enable;
extern qboolean   say_group_enable;
extern qboolean   extendedsay_enable;
extern qboolean   spawnentities_enable;
extern qboolean   spawnentities_internal_enable;
extern qboolean   vote_enable;
extern qboolean   consolechat_disable;
extern qboolean   gamemaptomap;
extern qboolean   banOnConnect;
extern qboolean   lockDownServer;
extern qboolean   serverinfoenable;

extern int    clientVoteTimeout;
extern int    clientRemindTimeout;
extern int    randomwaitreporttime;
extern int    proxy_bwproxy;
extern int    proxy_nitro2;
extern int    q2adminrunmode;
extern int    maxMsgLevel;

extern char    *zbotversion;
extern char    zbotmotd[256];
extern char    motd[4096];
extern char    clientVoteCommand[256];

extern int    maxrateallowed;
extern int    minrateallowed;
extern int    maxfpsallowed;
extern int    minfpsallowed;
extern int    zbc_jittermax;
extern int    zbc_jittertime;
extern int    zbc_jittermove;

#define ZBOT_TESTSTRING1   "q2startxx\n"

#define ZBOT_TESTSTRING_TEST1  "q2startxx"
#define ZBOT_TESTSTRING_TEST2  "q2exx"
#define ZBOT_TESTSTRING_TEST3  ".please.disconnect.all.bots"

#define ZBOT_TESTSTRING_TEST1_OLD "q2start"
#define ZBOT_TESTSTRING_TEST2_OLD "q2e"

extern char    zbot_teststring1[];
extern char    zbot_teststring_test1[];
extern char    zbot_teststring_test2[];
extern char    zbot_teststring_test3[];
extern char    zbot_testchar1;
extern char    zbot_testchar2;
extern char    testchars[];

extern int    testcharslength;
extern int    q2adminrunmode;
extern int    maxclientsperframe;
extern int    framesperprocess;

extern char    buffer[0x10000];
extern char    buffer2[256];
extern char    adminpassword[256];
extern char    customServerCmd[256];
extern char    customClientCmd[256];
extern char    customClientCmdConnect[256];
extern char    customServerCmdConnect[256];

//r1ch 2005-01-27 insecure lrcon fix BEGIN
extern qboolean	rcon_insecure;
//r1ch 2005-01-27 insecure lrcon fix END

extern qboolean   rcon_random_password;
extern qboolean   zbc_enable;
extern qboolean   nameChangeFloodProtect;
extern qboolean   skinChangeFloodProtect;

extern char    nameChangeFloodProtectMsg[256];
extern char    skinChangeFloodProtectMsg[256];
extern char    chatFloodProtectMsg[256];

extern int    maxlrcon_cmds;
extern int    lrcon_timeout;
extern int    logfilecheckcount;
extern int    nameChangeFloodProtectNum;
extern int    nameChangeFloodProtectSec;
extern int    nameChangeFloodProtectSilence;
extern int    skinChangeFloodProtectNum;
extern int    skinChangeFloodProtectSec;
extern int    skinChangeFloodProtectSilence;

extern struct   chatflood_s floodinfo;

extern baninfo_t  *banhead;
extern chatbaninfo_t *cbanhead;

extern qboolean   IPBanning_Enable;
extern qboolean   NickBanning_Enable;
extern qboolean   ChatBanning_Enable;
extern qboolean   kickOnNameChange;
extern qboolean   disablecmds_enable;
extern qboolean   checkvarcmds_enable;
extern qboolean   swap_attack_use;
extern qboolean   timescaledetect;

extern char    defaultBanMsg[256];
extern char    defaultChatBanMsg[256];
extern char    *currentBanMsg;

extern proxyinfo_t   *proxyinfo;
extern proxyinfo_t   *proxyinfoBase;
extern proxyreconnectinfo_t *reconnectproxyinfo;
extern zbotcmd_t   zbotCommands[];

extern int    clientsidetimeout;
extern int    zbotdetectactivetimeout;
extern int    lframenum;

extern float   ltime;

extern char    *impulsemessages[];
extern char    cmdpassedvote[2048];
extern char    cl_pitchspeed_kickmsg[256];
extern char    cl_anglespeedkey_kickmsg[256];

extern qboolean   cl_pitchspeed_enable;
extern qboolean   cl_pitchspeed_kick;
extern qboolean   cl_pitchspeed_display;
extern qboolean   cl_anglespeedkey_enable;
extern qboolean   cl_anglespeedkey_kick;
extern qboolean   cl_anglespeedkey_display;
extern qboolean   filternonprintabletext;

extern char    lockoutmsg[256];
extern char    gmapname[MAX_QPATH];
extern char    reconnect_address[256];
extern char    serverip[256];   // UPDATE
extern char    lanip[256];    // UPDATE

extern int    reconnect_time;
extern int    reconnect_checklevel;
extern int    entity_classname_offset;
extern int    checkvar_poll_time;

typedef struct
	{
		long    reconnecttimeout;
		int     retrylistidx;
		char    userinfo[MAX_INFO_STRING + 45];
	}
	
reconnect_info;

typedef struct
	{
		long    retry;
		char    ip[MAX_INFO_STRING + 45];
	}
	
retrylist_info;

extern reconnect_info* reconnectlist;
extern retrylist_info* retrylist;
extern int    maxReconnectList;
extern int    maxretryList;

#define FALSE   0
#define TRUE   1

#define SKIPBLANK(str) \
{\
while(*str == ' ' || *str == '\t') \
{ \
str++; \
} \
}

#define RATBOT_CHANGENAMETEST "pwsnskle"
#define BOTDETECT_CHAR1   'F'
#define BOTDETECT_CHAR2   'U'

#define itoa(x, y, z)   itoaNotAUnixFunction(z, y, z)

#define INITPERFORMANCE(instance) unsigned long performancetimer##instance = 0
#define INITPERFORMANCE_2(instance) \
unsigned long performancetimer##instance = 0; \
static unsigned long totalperformancetimer##instance = 0; \
static int countperformancetimer##instance = 0

#define STARTPERFORMANCE(instance) \
if(isLogEvent(LT_PERFORMANCEMONITOR)) \
{ \
performancetimer##instance = clock(); \
}

#define STOPPERFORMANCE(instance, function, client, ent) \
if(isLogEvent(LT_PERFORMANCEMONITOR)) \
{ \
performancetimer##instance = clock() - performancetimer##instance; \
if(performancetimer##instance) \
{ \
logEvent(LT_PERFORMANCEMONITOR, client, ent, function, 0, (double)performancetimer##instance / CLOCKS_PER_SEC); \
} \
}

#define STOPPERFORMANCE_2(instance, function, client, ent) \
if(isLogEvent(LT_PERFORMANCEMONITOR)) \
{ \
totalperformancetimer##instance += clock() - performancetimer##instance; \
countperformancetimer##instance++; \
if(countperformancetimer##instance >= 100) \
{ \
if(totalperformancetimer##instance) \
{ \
logEvent(LT_PERFORMANCEMONITOR, client, ent, function, 0, (double)totalperformancetimer##instance / (100.0 * CLOCKS_PER_SEC)); \
} \
totalperformancetimer##instance = 0; \
countperformancetimer##instance = 0; \
} \
}

// zb_clib.c
#ifdef Q2ADMINCLIB

char *q2a_strcpy( char *strDestination, const char *strSource );
char *q2a_strncpy( char *strDest, const char *strSource, size_t count );
char *q2a_strcat( char *strDestination, const char *strSource );
char *q2a_strstr( const char *string, const char *strCharSet );
char *q2a_strchr( const char *string, int c );
int  q2a_strcmp( const char *string1, const char *string2 );
size_t q2a_strlen( const char *string );

int  q2a_atoi( const char *string );
double q2a_atof( const char *string );

int  q2a_memcmp( const void *buf1, const void *buf2, size_t count );
void *q2a_memcpy( void *dest, const void *src, size_t count );
void *q2a_memmove( void *dest, const void *src, size_t count );
void *q2a_memset( void *dest, int c, size_t count );

#else

#define q2a_strcpy   strcpy
#define q2a_strncpy   strncpy
#define q2a_strcat   strcat
#define q2a_strcmp   strcmp
#define q2a_strstr   strstr
#define q2a_strchr   strchr
#define q2a_strlen   strlen

#define q2a_atoi   atoi
#define q2a_atof   atof

#define q2a_memcmp   memcmp
#define q2a_memcpy   memcpy
#define q2a_memmove   memmove
#define q2a_memset   memset

#endif

// zb_cmd.c
void  readCfgFiles(void);
void  ClientCommand (edict_t *ent);
void  ServerCommand (void);
void  dprintf_internal (char *fmt, ...);
void  cprintf_internal(edict_t *ent, int printlevel, char *fmt, ...);
void  bprintf_internal(int printlevel, char *fmt, ...);
void  AddCommandString_internal(char *text);
void  stuffNextLine(edict_t *ent, int client);
char  *getArgs(void);
int   getClientsFromArg(int client, edict_t *ent, char *cp, char **text);
edict_t  *getClientFromArg(int client, edict_t *ent, int *cleintret, char *cp, char **text);

// zb_util.c
void  stuffcmd(edict_t *e, char *s);
int   Q_stricmp (char *s1, char *s2);
char  *Info_ValueForKey (char *s, char *key);
void  copyDllInfo(void);
int   breakLine(char *buffer, char *buff1, char *buff2, int buff2size);
int   startContains(char *src, char *cmp);
int   stringContains(char *buff1, char *buff2);
int   isBlank(char *buff1);
char  *processstring(char *output, char *input, int max, char end);
qboolean getLogicalValue(char *arg);
int   getLastLine(char *buffer, FILE *dumpfile, long *fpos);
void  q_strupr(char *c);

// zb_ban.c
void  banRun(int startarg, edict_t *ent, int client);
void  reloadbanfileRun(int startarg, edict_t *ent, int client);
void  readBanLists(void);
int   checkCheckIfBanned(edict_t *ent, int client);
void  listbansRun(int startarg, edict_t *ent, int client);
void  displayNextBan(edict_t *ent, int client, long bannum);
void  delbanRun(int startarg, edict_t *ent, int client);
void  chatbanRun(int startarg, edict_t *ent, int client);
int   checkCheckIfChatBanned(char *txt);
void  listchatbansRun(int startarg, edict_t *ent, int client);
void  displayNextChatBan(edict_t *ent, int client, long chatbannum);
void  delchatbanRun(int startarg, edict_t *ent, int client);
void  freeBanLists(void);

// zb_lrcon.c
void  readLRconLists(void);
void  reloadlrconfileRun(int startarg, edict_t *ent, int client);
void  run_lrcon(edict_t *ent, int client);
void  listlrconsRun(int startarg, edict_t *ent, int client);
void  displayNextLRCon(edict_t *ent, int client, long lrconnum);
void  lrconRun(int startarg, edict_t *ent, int client);
void  lrconDelRun(int startarg, edict_t *ent, int client);
void  freeLRconLists(void);
void  lrcon_reset_rcon_password(int, edict_t *, int);
void  check_lrcon_password(void);

// zb_init.c
void  InitGame (void);
void  SpawnEntities (char *mapname, char *entities, char *spawnpoint);
qboolean ClientConnect (edict_t *ent, char *userinfo);
void  ClientUserinfoChanged (edict_t *ent, char *userinfo);
void  ClientDisconnect (edict_t *ent);
void  ClientBegin (edict_t *ent);
void  WriteGame (char *filename, qboolean autosave);
void  ReadGame (char *filename);
void  WriteLevel (char *filename);
void  ReadLevel (char *filename);

// zb_zbot.c
int   checkForOverflows(edict_t *ent, int client);
void  serverLogZBot(edict_t *ent, int client);
void  ClientThink (edict_t *ent, usercmd_t *ucmd);
void  G_RunFrame (void);
void  Pmove_internal (pmove_t *pmove);
void  generateRandomString(char *buffer, int length);
void  reloadWhoisFileRun(int startarg, edict_t *ent, int client);	//UPDATE
void  reloadLoginFileRun(int startarg, edict_t *ent, int client);	//UPDATE

// zb_msgqueue.c
void  addCmdQueue(int client, byte command, float timeout, unsigned long data, char *str);
qboolean getCommandFromQueue(int client, byte *command, unsigned long *data, char **str);
void  removeClientCommand(int client, byte command);
void  removeClientCommands(int client);

// zb_log.c
void  loadLogList(void);
qboolean isLogEvent(enum zb_logtypesenum ltype);
void  logEvent(enum zb_logtypesenum ltype, int client, edict_t *ent, char *message, int number, float number2);
void  displaylogfileRun(int startarg, edict_t *ent, int client);
void  displayLogFileCont(edict_t *ent, int client, long logfilereadpos);
void  clearlogfileRun(int startarg, edict_t *ent, int client);
void  logfileRun(int startarg, edict_t *ent, int client);
void  displayLogFileListCont(edict_t *ent, int client, long logfilenum);
void  logeventRun(int startarg, edict_t *ent, int client);
void  displayLogEventListCont(edict_t *ent, int client, long logevent, qboolean onetimeonly);

// zb_flood.c
void  freeFloodLists(void);
void  readFloodLists(void);
void  reloadFloodFileRun(int startarg, edict_t *ent, int client);
void  nameChangeFloodProtectInit(char *arg);
void  nameChangeFloodProtectRun(int startarg, edict_t *ent, int client);
void  chatFloodProtectInit(char *arg);
void  chatFloodProtectRun(int startarg, edict_t *ent, int client);
void  muteRun(int startarg, edict_t *ent, int client);
void  clientchatfloodprotectRun(int startarg, edict_t *ent, int client);
qboolean checkForMute(int client, edict_t *ent, qboolean displayMsg);
qboolean checkForFlood(int client);
qboolean checkforfloodcmds(char *cp);
void  listfloodsRun(int startarg, edict_t *ent, int client);
void  displayNextFlood(edict_t *ent, int client, long floodcmd);
void  floodcmdRun(int startarg, edict_t *ent, int client);
void  floodDelRun(int startarg, edict_t *ent, int client);
void  skinChangeFloodProtectInit(char *arg);
void  skinChangeFloodProtectRun(int startarg, edict_t *ent, int client);

// zb_spawn.c
qboolean ReadSpawnFile(char *spawnname, qboolean onelevelflag);
qboolean checkDisabledEntities(char *classname);
void  freeSpawnLists(void);
void  freeOneLevelSpawnLists(void);
void  readSpawnLists(void);
void  reloadSpawnFileRun(int startarg, edict_t *ent, int client);
void  listspawnsRun(int startarg, edict_t *ent, int client);
void  displayNextSpawn(edict_t *ent, int client, long floodcmd);
void  spawncmdRun(int startarg, edict_t *ent, int client);
void  spawnDelRun(int startarg, edict_t *ent, int client);
void  linkentity_internal(edict_t *ent);
void  unlinkentity_internal(edict_t *ent);

// zb_vote.c
void  freeVoteLists(void);
void  readVoteLists(void);
void  reloadVoteFileRun(int startarg, edict_t *ent, int client);
void  listvotesRun(int startarg, edict_t *ent, int client);
void  displayNextVote(edict_t *ent, int client, long floodcmd);
void  votecmdRun(int startarg, edict_t *ent, int client);
void  voteDelRun(int startarg, edict_t *ent, int client);
qboolean checkVoteCommand(char *votecmd);
void  run_vote(edict_t *ent, int client);
void  checkOnVoting(void);

// zb_zbotcheck.c
qboolean zbc_ZbotCheck(int client, usercmd_t *ucmd);

// zb_disable.c
void  freeDisableLists(void);
void  readDisableLists(void);
void  reloadDisableFileRun(int startarg, edict_t *ent, int client);
void  listdisablesRun(int startarg, edict_t *ent, int client);
void  displayNextDisable(edict_t *ent, int client, long floodcmd);
void  disablecmdRun(int startarg, edict_t *ent, int client);
void  disableDelRun(int startarg, edict_t *ent, int client);
qboolean checkDisabledCommand(char *cmd);

// zb_checkvar.c
void  readCheckVarLists(void);
void  reloadCheckVarFileRun(int startarg, edict_t *ent, int client);
void  listcheckvarsRun(int startarg, edict_t *ent, int client);
void  displayNextCheckvar(edict_t *ent, int client, long checkvarcmd);
void  checkvarcmdRun(int startarg, edict_t *ent, int client);
void  checkvarDelRun(int startarg, edict_t *ent, int client);
void  checkVariableTest(edict_t *ent, int client, int idx);
void  checkVariableValid(edict_t *ent, int client, char *value);

//*** UPDATE START ***
// md4.c
unsigned Com_BlockChecksum (void *buffer, int length);

// Pooy's shit
extern char  client_msg[256];
extern qboolean private_command_kick;
extern int  msec_kick_on_bad;
extern int  msec_max;
extern int  speedbot_check_type;
extern int  max_pmod_noreply;
extern int  msec_int;

typedef struct
	{
		char  name[256];
		char  password[256];
		char  ip[256];
		int   level;
	}
	
admin_type;

#define Q2ADMINVERSION   "1.17.44-tsmod-2"
#define DEFAULTQ2AVER   "1.0"
#define DEFAULTQ2AMSG   "\nThis server requires %s anti cheat client.\n"
#define MAX_ADMINS    128
#define ADMIN_AUTH_LEVEL  1
#define MAX_BLOCK_MODELS  26

#define VIRUS_KICK_MSG "%s has not provided adequate authentication, this may be due to a virus.\n"
#define NOMATCH_KICK_MSG "%s has not provided adequate authentication.\n"
#define OUTOFTIME_KICK_MSG "%s failed to authenticate.\n"
#define MOD_KICK_MSG "%s failed the pmodified check on this map, error %d.\n"
#define PRV_KICK_MSG "%s is using a modified client.\n"

#define Q2A_VIRUS_KICK_MSG "Inadequate authentication, may be a virus.\n"
#define Q2A_NOMATCH_KICK_MSG "Inadequate authentication.\n"
#define Q2A_OUTOFTIME_KICK_MSG "Failed to respond.\n"
#define Q2A_MOD_KICK_MSG "Failed the pmodified check on this map.\n"
#define Q2A_PRV_KICK_MSG "Failed the private commands check.\n"
#define FRKQ2_KICK_MSG "Failed the client authentication.\n"

extern admin_type admin_pass[MAX_ADMINS];
extern admin_type q2a_bypass_pass[MAX_ADMINS];
extern int num_admins;
extern int num_q2a_admins;

void Read_Admin_cfg(void);
void List_Admin_Commands(edict_t *ent,int client);
int get_admin_level(char *givenpass,char *givenname);
int get_bypass_level(char *givenpass,char *givenname);

void ADMIN_dumpuser(edict_t *ent,int client,int user,qboolean check);
int  ADMIN_process_command(edict_t *ent,int client);

extern int   client_map_cfg;
extern qboolean  do_franck_check;
extern qboolean  q2a_command_check;
extern qboolean  do_vid_restart;

extern int   gl_driver_check;
extern int   USERINFOCHANGE_TIME;
extern int   USERINFOCHANGE_COUNT;
extern int   gl_driver_max_changes;

typedef struct
	{
		char  command[256];
	}
	
priv_t;

extern   priv_t private_commands[PRIVATE_COMMANDS];
void   stuff_private_commands(int client,edict_t *ent);

typedef struct
	{
		char  name[16];
	}
	
user_dyn;

typedef struct
	{
		int   id;
		char  ip[22];
		char  seen[32];
		user_dyn dyn[10];
	}
	
user_details;

extern int   WHOIS_COUNT;
extern int   whois_active;
extern user_details* whois_details;
extern qboolean  timers_active;
extern int   timers_min_seconds;
extern int   timers_max_seconds;
qboolean   can_do_new_cmds(int client);
void    whois_write_file(void);
void    whois_read_file(void);
void    whois_getid(int client,edict_t *ent);
void    whois(int client,edict_t *ent);
void    whois_adduser(int client,edict_t *ent);
void    whois_newname(int client,edict_t *ent);
void    whois_update_seen(int client,edict_t *ent);
void    whois_dumpdetails(int client,edict_t *ent,int userid);
void    timer_action(int client,edict_t *ent);
void    timer_stop(int client,edict_t *ent);
void    timer_start(int client,edict_t *ent);

typedef struct
	{
		char   *model_name;
	}
	
block_model;

extern block_model block_models[MAX_BLOCK_MODELS];
//*** UPDATE END ***

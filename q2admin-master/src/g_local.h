//
// q2admin
//
// g_local.h
//
// copyright 2000 Shane Powell
// copyright 2009 Toni Spets
//

// g_local.h -- local definitions for game module
#include "q_shared.h"

// define GAME_INCLUDE so that game.h does not define the
// short, server-visible gclient_t and edict_t structures,
// because we define the full size ones in this file
#define GAME_INCLUDE
#include "game.h"

#include <ctype.h>
#include <stdint.h>

// the "gameversion" client command will print this plus compile date
#define GAMEVERSION    "baseq2"

// protocol bytes that can be directly added to messages
#define svc_muzzleflash		1
#define svc_muzzleflash2	2
#define svc_temp_entity		3
#define svc_layout		4
#define svc_inventory		5

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
} movetype_t;

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
} game_locals_t;

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

extern game_locals_t	game;
extern level_locals_t	level;

extern edict_t		*g_edicts;

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
} field_t;

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
	
#define MAXIMPULSESTOTEST 256
	
#define RANDCHAR()      (random() < 0.3) ? '0' + (int)(9.9 * random()) : 'A' + (int)(26.9 * random())

#define getClientOffset(ent)	(((char *)ent - (char *)globals.edicts) / globals.edict_size) - 1
#define getClient(entnum)	(edict_t *)((char *)globals.edicts + (globals.edict_size * entnum))

extern game_import_t	gi;
extern game_export_t	globals;
extern game_export_t	*dllglobals;

#define FALSE	0
#define TRUE	1

#define SKIPBLANK(str) \
{\
while(*str == ' ' || *str == '\t') \
{ \
str++; \
} \
}

#define itoa(x, y, z)   itoaNotAUnixFunction(z, y, z)

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

// q2a_util.c
void  stuffcmd(edict_t *e, char *s);
int   Q_stricmp (char *s1, char *s2);
char  *Info_ValueForKey (char *s, char *key);
void  copyDllInfo(void);
qboolean getLogicalValue(char *arg);
void  q_strupr(char *c);

char *q2a_malloc (int size);
char *q2a_realloc (char *oldmem, int newsize);
void q2a_free (char *mem);

// q2a_lua.c
void q2a_fpu_lua (void);
void q2a_fpu_q2 (void);

void q2a_lua_init(void);
void q2a_lua_shutdown(void);
void q2a_lua_reload(void);
void q2a_lua_load(const char *file);

void q2a_lua_LogMessage(char *msg);
qboolean q2a_lua_ClientConnect(int client, char *);
void q2a_lua_ClientBegin(int client);
void q2a_lua_ClientDisconnect(int client);
void q2a_lua_ClientThink(int client);
void q2a_lua_RunFrame(void);
qboolean q2a_lua_ClientCommand(int client);
qboolean q2a_lua_ServerCommand(const char *cmd);
void q2a_lua_ClientUserinfoChanged(int client, char *userinfo);
void q2a_lua_SpawnEntities(char *mapname, char *entities, char *spawnpoint);

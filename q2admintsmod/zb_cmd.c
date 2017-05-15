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
// zb_cmd.c
//
// copyright 2000 Shane Powell
//

#include "g_local.h"
//#include "md4.c"

int    lastClientCmd = -1;

// prototypes for command structure
void   clientsidetimeoutInit(char *arg);
void   zbotversionRun(int startarg, edict_t *ent, int client);
void   clientsidetimeoutRun(int startarg, edict_t *ent, int client);
void   setadminRun(int startarg, edict_t *ent, int client);
void   maxrateallowedRun(int startarg, edict_t *ent, int client);
void   minrateallowedRun(int startarg, edict_t *ent, int client);
void   maxfpsallowedRun(int startarg, edict_t *ent, int client);
void   maxfpsallowedInit(char *arg);
void   minfpsallowedRun(int startarg, edict_t *ent, int client);
void   minfpsallowedInit(char *arg);
void   impulsesToKickOnRun(int startarg, edict_t *ent, int client);
void   impulsesToKickOnInit(char *arg);
void   zbotmotdRun(int startarg, edict_t *ent, int client);
void   stuffClientRun(int startarg, edict_t *ent, int client);
void   sayGroupRun(int startarg, edict_t *ent, int client);
void   sayPersonRun(int startarg, edict_t *ent, int client);
void   ipRun(int startarg, edict_t *ent, int client);
void   kickRun(int startarg, edict_t *ent, int client);
void   cvarsetRun(int startarg, edict_t *ent, int client);
void   cl_pitchspeed_enableRun(int startarg, edict_t *ent, int client);
void   cl_anglespeedkey_enableRun(int startarg, edict_t *ent, int client);
void   lockDownServerRun(int startarg, edict_t *ent, int client);

#define ZBOTCOMMANDSSIZE (sizeof(zbotCommands) / sizeof(zbotCommands[0]))

//*** UPDATE START ***
block_model block_models[MAX_BLOCK_MODELS] =
	{
		//projected model wallhack protection list.
		{
			"models/items/adrenal/tris.md2"
		},
		{
			"models/items/armor/body/tris.md2"
		},
		{
			"models/items/armor/combat/tris.md2"
		},
		{
			"models/items/armor/jacket/tris.md2"
		},
		{
			"models/items/armor/shield/tris.md2"
		},
		{
			"models/items/band/tris.md2"
		},
		{
			"models/items/invulner/tris.md2"
		},
		{
			"models/items/mega_h/tris.md2"
		},
		{
			"models/items/quaddama/tris.md2"
		},
		{
			"models/objects/rocket/tris.md2"
		},
		{
			"models/ctf/strength/tris.md2"
		},
		{
			"models/ctf/haste/tris.md2"
		},
		{
			"models/ctf/resistance/tris.md2"
		},
		{
			"models/ctf/regeneration/tris.md2"
		},
		{
			"players/male/flag2.md2"
		},
		{
			"players/male/flag1.md2"
		},
		{
			"models/objects/grenade2/tris.md2"
		},
		{
			"models/weapons/g_machn/tris.md2"
		},
		{       "models/weapons/g_rocket/tris.md2"
		},
        
		{       "models/weapons/g_hyperb/tris.md2"
		},
		{
			"models/weapons/g_shotg/tris.md2"
		},
		{
			"models/weapons/g_chain/tris.md2"
		},
		{
			"models/weapons/g_rail/tris.md2"
		},
		{
			"models/weapons/g_shotg2/tris.md2"
		},
		{
			"models/weapons/g_launch/tris.md2"
		},
        
		{
			"models/items/armor/shard/tris.md2"
		}
	};
//*** UPDATE END ***

zbotcmd_t zbotCommands[] =
	{
		{
			"adminpassword",
			CMDWHERE_CFGFILE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			adminpassword
		},
		{
			"ban",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			banRun
		},
		{
			"banonconnect",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&banOnConnect
		},
		{
			"chatban",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			chatbanRun
		},
		{
			"chatbanning_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&ChatBanning_Enable
		},
		{
			"chatfloodprotect",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			chatFloodProtectRun,
			chatFloodProtectInit
		},
		{
			"chatfloodprotectmsg",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			chatFloodProtectMsg
		},
		{
			"checkclientipaddress",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&checkClientIpAddress
		},
		{
			"checkvar_poll_time",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&checkvar_poll_time
		},
		{
			"checkvarcmd",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			checkvarcmdRun
		},
		{
			"checkvarcmds_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&checkvarcmds_enable
		},
		{
			"checkvardel",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			checkvarDelRun
		},
		{
			"clearlogfile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			clearlogfileRun
		},
		{
			"clientchatfloodprotect",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			clientchatfloodprotectRun
		},
		{
			"clientremindtimeout",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&clientRemindTimeout,
		},
		{
			"clientsidetimeout",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&clientsidetimeout,
			clientsidetimeoutRun,
			clientsidetimeoutInit,
		},
		{
			"clientvotecommand",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			clientVoteCommand
		},
		{
			"clientvotetimeout",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&clientVoteTimeout,
		},
		{
			"cl_anglespeedkey_display",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&cl_anglespeedkey_display,
		},
		{
			"cl_anglespeedkey_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&cl_anglespeedkey_enable,
			cl_anglespeedkey_enableRun,
		},
		{
			"cl_anglespeedkey_kick",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&cl_anglespeedkey_kick,
		},
		{
			"cl_anglespeedkey_kickmsg",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			cl_anglespeedkey_kickmsg,
		},
		{
			"cl_pitchspeed_display",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&cl_pitchspeed_display,
		},
		{
			"cl_pitchspeed_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&cl_pitchspeed_enable,
			cl_pitchspeed_enableRun,
		},
		{
			"cl_pitchspeed_kick",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&cl_pitchspeed_kick,
		},
		{
			"cl_pitchspeed_kickmsg",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			cl_pitchspeed_kickmsg,
		},
		{
			"consolechat_disable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&consolechat_disable
		},
		{
			"customclientcmd",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			customClientCmd
		},
		{
			"customclientcmdconnect",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			customClientCmdConnect
		},
		{
			"customservercmd",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			customServerCmd
		},
		{
			"customservercmdconnect",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			customServerCmdConnect
		},
		{
			"cvarset",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			cvarsetRun
		},
		{
			"defaultbanmsg",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			defaultBanMsg
		},
		{
			"defaultchatbanmsg",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			defaultChatBanMsg
		},
		{
			"defaultreconnectmessage",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			defaultreconnectmessage
		},
		{
			"delban",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			delbanRun
		},
		{
			"delchatban",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			delchatbanRun
		},
		{
			"disablecmd",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			disablecmdRun
		},
		{
			"disablecmds_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&disablecmds_enable
		},
		{
			"disabledel",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			disableDelRun
		},
		{
			"disconnectuser",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&disconnectuser
		},
		{
			"disconnectuserimpulse",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&disconnectuserimpulse
		},
		{
			"displayimpulses",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&displayimpulses
		},
		{
			"displaylogfile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			displaylogfileRun
		},
		{
			"displaynamechange",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&displaynamechange
		},
		{
			"dopversion",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&dopversion
		},
		{
			"displayzbotuser",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&displayzbotuser
		},
		{
			"entity_classname_offset",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&entity_classname_offset,
		},
		{
			"extendedsay_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&extendedsay_enable
		},
		{
			"filternonprintabletext",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&filternonprintabletext
		},
		{
			"floodcmd",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			floodcmdRun
		},
		{
			"flooddel",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			floodDelRun
		},
		{
			"framesperprocess",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&framesperprocess
		},
		{
			"gamemaptomap",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&gamemaptomap
		},
		{
			"hackuserdisplay",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			hackuserdisplay
		},		
		{
			"impulsestokickon",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&impulsesToKickOn,
			impulsesToKickOnRun,
			impulsesToKickOnInit
		},
		{
			"ip",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			ipRun
		},
		{
			"ipbanning_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&IPBanning_Enable
		},
		{
			"kick",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			kickRun
		},
		{
			"kickonnamechange",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&kickOnNameChange
		},
		{
			"listbans",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			listbansRun
		},
		{
			"listchatbans",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			listchatbansRun
		},
		{
			"listcheckvar",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			listcheckvarsRun
		},
		{
			"listdisable",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			listdisablesRun
		},
		{
			"listfloods",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			listfloodsRun
		},
		{
			"listlrcons",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			listlrconsRun
		},
		{
			"listspawns",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			listspawnsRun
		},
		{
			"listvotes",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			listvotesRun
		},
		{
			"lock",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&lockDownServer,
			lockDownServerRun,
		},
		{
			"lockoutmsg",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			lockoutmsg,
		},
		{
			"logevent",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			logeventRun
		},
		{
			"logfile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			logfileRun
		},
		{
			"lrcon_timeout",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&lrcon_timeout
		},
		{
			"lrcon",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			lrconRun
		},
		{
			"lrcondel",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			lrconDelRun
		},
		{
			"mapcfgexec",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&mapcfgexec
		},
		{
			"maxclientsperframe",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&maxclientsperframe
		},
		{
			"maxfps",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&maxfpsallowed,
			maxfpsallowedRun,
			maxfpsallowedInit
		},
		{
			"maximpulses",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&maximpulses
		},
		{
			"maxmsglevel",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&maxMsgLevel
		},
		{
			"maxrate",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&maxrateallowed,
			maxrateallowedRun
		},
		{
			"minfps",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&minfpsallowed,
			minfpsallowedRun,
			minfpsallowedInit
		},
		{
			"minrate",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&minrateallowed,
			minrateallowedRun
		},
		{
			"mute",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			muteRun
		},
		{
			"namechangefloodprotect",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			NULL,
			nameChangeFloodProtectRun,
			nameChangeFloodProtectInit
		},
		{
			"namechangefloodprotectmsg",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			nameChangeFloodProtectMsg
		},
		{
			"nickbanning_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&NickBanning_Enable
		},
		{
			"numofdisplays",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&numofdisplays
		},

		//r1ch 2005-01-26 disable hugely buggy commands BEGIN
		/*{
			"play_all_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&play_all_enable
		},
		{
			"play_team_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&play_team_enable
		},
		{
			"play_person_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&play_person_enable
		},*/
		//r1ch 2005-01-26 disable hugely buggy commands END
		{
			"printmessageonplaycmds",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&printmessageonplaycmds
		},
		{
			"proxy_bwproxy",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&proxy_bwproxy
		},
		{
			"proxy_nitro2",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&proxy_nitro2
		},
		{
			"quake2dirsupport",
			CMDWHERE_CFGFILE,
			CMDTYPE_LOGICAL,
			&quake2dirsupport
		},
		{
			"q2adminrunmode",
			CMDWHERE_CFGFILE,
			CMDTYPE_NUMBER,
			&q2adminrunmode
		},
		{
			"randomwaitreporttime",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&randomwaitreporttime
		},
		{
			"rcon_random_password",
			CMDWHERE_CFGFILE,
			CMDTYPE_LOGICAL,
			&rcon_random_password
		},
//r1ch 2005-01-27 insecure lrcon fix BEGIN
		{
			"rcon_insecure",
			CMDWHERE_CFGFILE,
			CMDTYPE_LOGICAL,
			&rcon_insecure
		},
//r1ch 2005-01-27 insecure lrcon fix END
		{
			"reconnect_address",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			reconnect_address
		},
		{
			"reconnect_checklevel",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&reconnect_checklevel
		},
		{
			"reconnect_time",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&reconnect_time
		},
		{
			"reloadbanfile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			reloadbanfileRun,
		},
		{
			"reloadcheckvarfile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			reloadCheckVarFileRun,
		},
		{
			"reloaddisablefile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			reloadDisableFileRun,
		},
		{
			"reloadfloodfile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			reloadFloodFileRun,
		},
		{
			"reloadlrconfile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			reloadlrconfileRun,
		},
		{
			"reloadspawnfile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			reloadSpawnFileRun,
		},
		{
			"reloadvotefile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			reloadVoteFileRun,
		},
		{
			"resetrcon",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			lrcon_reset_rcon_password,
		},
		{
			"say_group",
			CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			sayGroupRun,
		},
		{
			"say_group_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&say_group_enable
		},
		{
			"say_person",
			CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			sayPersonRun,
		},
		{
			"say_person_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&say_person_enable
		},
		{
			"serverinfoenable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&serverinfoenable
		},
		{
			"setadmin",
			CMDWHERE_CLIENTCONSOLE,
			CMDTYPE_NONE,
			NULL,
			setadminRun
		},
		{
			"setmotd",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			zbotmotd,
			zbotmotdRun,
		},
		{
			"skinchangefloodprotect",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			NULL,
			skinChangeFloodProtectRun,
			skinChangeFloodProtectInit
		},
		{
			"skinchangefloodprotectmsg",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			skinChangeFloodProtectMsg
		},
		{
			"skincrashmsg",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			skincrashmsg
		},
		{
			"soloadlazy",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&soloadlazy
		},
		{
			"spawncmd",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			spawncmdRun
		},
		{
			"spawndel",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			spawnDelRun
		},
		{
			"spawnentities_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&spawnentities_enable
		},
		{
			"spawnentities_internal_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&spawnentities_internal_enable
		},
		{
			"stuff",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			stuffClientRun,
		},
		{
			"swap_attack_use",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&swap_attack_use
		},
		{
			"timescaledetect",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&timescaledetect
		},
		{
			"timescaleuserdisplay",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			timescaleuserdisplay
		},
		{
			"version",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			zbotversionRun
		},
		{
			"votecmd",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			votecmdRun
		},
		{
			"voteclientmaxvotes",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&clientMaxVotes
		},
		{
			"voteclientmaxvotetimeout",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&clientMaxVoteTimeout
		},
		{
			"votecountnovotes",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&votecountnovotes
		},
		{
			"votedel",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			voteDelRun
		},
		{
			"voteminclients",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&voteminclients
		},
		{
			"votepasspercent",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&votepasspercent
		},
		{
			"vote_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&vote_enable
		},
		{
			"zbc_enable",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&zbc_enable
		},
		{
			"zbc_jittermax",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&zbc_jittermax
		},
		{
			"zbc_jittermove",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&zbc_jittermove
		},
		{
			"zbc_jittertime",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&zbc_jittertime
		},
		{
			"zbotdetect",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_LOGICAL,
			&zbotdetect
		},
		{
			"zbotdetectactivetimeout",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&zbotdetectactivetimeout
		},
		{
			"zbotuserdisplay",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_STRING,
			zbotuserdisplay
		},
//*** UPDATE START ***
/*		{
			"client_check",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NUMBER,
			&client_check
		},*/
		{
			"client_map_cfg", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&client_map_cfg
		},
		{ 
			"client_msg", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			&client_msg
		},
		{ 
			"do_franck_check", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_LOGICAL,
			&do_franck_check
		},
		{ 
			"do_vid_restart", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_LOGICAL,
			&do_vid_restart
		},
		{ 
			"gl_driver_check", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&gl_driver_check
		},
		{ 
			"gl_driver_max_changes", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&gl_driver_max_changes
		},
		{ 
			"inverted_command1", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			&private_commands[4].command
		},
		{ 
			"inverted_command2", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			&private_commands[5].command
		},
		{ 
			"inverted_command3", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			&private_commands[6].command
		},
		{ 
			"inverted_command4", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			&private_commands[7].command
		},
		{
			"lanip", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			lanip
		},
		{ 
			"max_pmod_noreply", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&max_pmod_noreply
		},
		{ 
			"msec_int", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&msec_int
		},
		{ 
			"msec_kick_on_bad", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&msec_kick_on_bad
		},
		{ 
			"msec_max", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&msec_max
		},
		{ 
			"private_command1", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			&private_commands[0].command
		},
		{ 
			"private_command2", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			&private_commands[1].command
		},
		{ 
			"private_command3", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			&private_commands[2].command
		},
		{ 
			"private_command4", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			&private_commands[3].command
		},
		{
			"private_command_kick",
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_LOGICAL,
			&private_command_kick
		},
		{ 
			"q2a_command_check", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_LOGICAL,
			&q2a_command_check
		},
		{
			"reloadloginfile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			reloadLoginFileRun,
		},
		{
			"reloadwhoisfile",
			CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE,
			CMDTYPE_NONE,
			NULL,
			reloadWhoisFileRun,
		},
		{
			"serverip", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			serverip
		},
		{ 
			"speedbot_check_type", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&speedbot_check_type
		},
		{ 
			"timers_active", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_LOGICAL,
			&timers_active
		},
		{ 
			"timers_max_seconds", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&timers_max_seconds
		},
		{ 
			"timers_min_seconds", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&timers_min_seconds
		},
		{ 
			"userinfochange_count", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&USERINFOCHANGE_COUNT
		},
		{ 
			"userinfochange_time", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_NUMBER,
			&USERINFOCHANGE_TIME
		},
/*		{ 
			"version_check", 
			CMDWHERE_CFGFILE | CMDWHERE_CLIENTCONSOLE | CMDWHERE_SERVERCONSOLE, 
			CMDTYPE_STRING,
			&version_check
		},*/
		{ 
			"whois_active", 
			CMDWHERE_CFGFILE,	//Only allocates memory at InitGame: can only be read from config
			CMDTYPE_NUMBER,
			&whois_active
		},
//*** UPDATE END ***
	};
    
//===================================================================
char mutedText[8192] = "";

void dprintf_internal (char *fmt, ...)
{
	char cbuffer[8192];
	va_list arglist;
	int clienti = lastClientCmd;
	
	// convert to string
	va_start(arglist, fmt);
	vsprintf(cbuffer, fmt, arglist);
	va_end(arglist);
	
	if(q2adminrunmode == 0 || !proxyinfo)
		{
			gi.dprintf("%s", cbuffer);
			return;
		}
		
	if(clienti == -1)
		{
			unsigned int i;
			
			if(maxclients)
				{
					for(i = 0; i < maxclients->value; i++)
						{
							if(proxyinfo[i].inuse && startContains (cbuffer, proxyinfo[i].name))
								{
									if(q2a_strstr(cbuffer, proxyinfo[i].lastcmd))
										{
											if(consolechat_disable)
												{
													return;
												}
												
											clienti = i;
											break;
										}
								}
						}
				}
		}
	else if(proxyinfo[clienti].inuse && !q2a_strstr(cbuffer, proxyinfo[clienti].name) || !q2a_strstr(cbuffer, proxyinfo[clienti].lastcmd))
		{
			clienti = -1;
		}
	else if(consolechat_disable && q2a_strstr(cbuffer, proxyinfo[clienti].lastcmd))
		{
			return;
		}
		
	if(clienti != -1)
		{
			if(checkForMute(clienti, getEnt((clienti + 1)), TRUE))
				{
					q2a_strcpy(mutedText, cbuffer);
					return;
				}
				
			mutedText[0] = 0;
			
			logEvent(LT_CHAT, clienti, getEnt((clienti + 1)), cbuffer, 0, 0.0);
		}
		
	if(filternonprintabletext)
		{
			char *cp = cbuffer;
			
			while(*cp)
				{
					if(!isprint(*cp) && *(cp + 1) != 0)
						{
							*cp = ' ';
						}
						
					cp++;
				}
		}
		
	gi.dprintf("%s", cbuffer);
	
	if(clienti != -1 && (floodinfo.chatFloodProtect || proxyinfo[clienti].floodinfo.chatFloodProtect))
		{
			if(checkForFlood(clienti))
				{
					return;
				}
		}
}


void cprintf_internal(edict_t *ent, int printlevel, char *fmt, ...)
{
	char cbuffer[8192];
	va_list arglist;
	char *cp;
	int clienti = lastClientCmd;
	
	// convert to string
	va_start(arglist, fmt);
	vsprintf(cbuffer, fmt, arglist);
	va_end(arglist);
	
	if(q2adminrunmode == 0)
		{
			gi.cprintf(ent, printlevel, "%s", cbuffer);
			return;
		}
		
	cp = q2a_strstr(cbuffer, "swpplay ");
	if(cp)
		{
			// found a play_team command, play sound at client console instead.
			//r1ch 2005-01-26 fix fucking huge security hole BEGIN
			//stuffcmd(ent, cp + 3);
			//r1ch 2005-01-26 fix fucking huge security hole END
			return;
		}
		
	/*
	// check for banned chat words
	if(printlevel == PRINT_CHAT && checkCheckIfChatBanned(cbuffer))
	{
	logEvent(LT_CHATBAN, getEntOffset(ent) - 1, ent, cbuffer, 0);
	return;
	}
	*/
	
	if(q2a_strcmp(mutedText, cbuffer)== 0)
		{
			return;
		}
		
	if(printlevel== PRINT_CHAT && clienti==-1)
		{
			unsigned int i;
			
			for(i= 0; i< maxclients->value; i++)
				{
					if(proxyinfo[i].inuse && startContains(cbuffer, proxyinfo[i].name))
						{
							if(q2a_strstr(cbuffer, proxyinfo[i].lastcmd))
								{
									if(consolechat_disable)
										{
											return;
										}
										
									clienti= i;
									break;
								}
						}
				}
		}
		
	if(printlevel == PRINT_CHAT && clienti != -1 && consolechat_disable && q2a_strstr(cbuffer, proxyinfo[clienti].lastcmd))
		{
			return;
		}
		
	if(printlevel == PRINT_CHAT && clienti != -1)
		{
			if(checkForMute(clienti, getEnt((clienti + 1)),(ent == NULL)))
				{
					return;
				}
		}
		
	if(printlevel== PRINT_CHAT && filternonprintabletext)
		{
			cp= cbuffer;
			
			while( *cp)
				{
					if(!isprint( *cp) && *(cp+ 1) != 0)
						{
							*cp=' ';
						}
						
					*cp++;
				}
		}
		
	if(printlevel== PRINT_CHAT && ent == NULL)
		{
			if(clienti == -1)
				{
					logEvent(LT_CHAT, 0, 0, cbuffer, 0, 0.0);
				}
			else
				{
					logEvent(LT_CHAT, clienti, getEnt((clienti+ 1)), cbuffer, 0, 0.0);
				}
		}
		
	gi.cprintf(ent, printlevel,"%s", cbuffer);
	
	if(printlevel== PRINT_CHAT && clienti!=-1 && ent == NULL &&(floodinfo.chatFloodProtect|| proxyinfo[clienti].floodinfo.chatFloodProtect))
		{
			if(checkForFlood(clienti))
				{
					return;
				}
		}
}




void bprintf_internal(int printlevel, char *fmt,...)
{
	char cbuffer[8192];
	va_list arglist;
	int clienti = lastClientCmd;
	
	// convert to string
	va_start(arglist, fmt);
	vsprintf(cbuffer, fmt, arglist);
	va_end(arglist);
	
	if(q2adminrunmode == 0)
		{
			gi.bprintf(printlevel, "%s", cbuffer);
			return;
		}
		
	if(q2a_strcmp(mutedText, cbuffer) == 0)
		{
			return;
		}
		
	if(printlevel == PRINT_CHAT && clienti == -1)
		{
			unsigned int i;
			
			for(i = 0; i < maxclients->value; i++)
				{
					if(proxyinfo[i].inuse && startContains (cbuffer, proxyinfo[i].name))
						{
							if(q2a_strstr(cbuffer, proxyinfo[i].lastcmd))
								{
									clienti = i;
									break;
								}
						}
				}
		}
		
	if(printlevel == PRINT_CHAT && clienti != -1)
		{
			if(checkForMute(clienti, getEnt((clienti + 1)), TRUE))
				{
					return;
				}
		}
		
	if(printlevel == PRINT_CHAT)
		{
			if(filternonprintabletext)
				{
					char *cp = cbuffer;
					
					while(*cp)
						{
							if(!isprint(*cp) && *(cp + 1) != 0)
								{
									*cp = ' ';
								}
								
							*cp++;
						}
				}
				
			if(clienti == -1)
				{
					logEvent(LT_CHAT, 0, 0, cbuffer, 0, 0.0);
				}
			else
				{
					logEvent(LT_CHAT, clienti, getEnt((clienti + 1)), cbuffer, 0, 0.0);
				}
		}
		
	gi.bprintf(printlevel, "%s", cbuffer);
	
	if(printlevel == PRINT_CHAT && clienti != -1 && (floodinfo.chatFloodProtect || proxyinfo[clienti].floodinfo.chatFloodProtect))
		{
			if(checkForFlood(clienti))
				{
					return;
				}
		}
}




void AddCommandString_internal(char *text)
{
	char *str;
	qboolean mapChangeFound = FALSE;
	
	if(q2adminrunmode == 0)
		{
			gi.AddCommandString(text);
			return;
		}
		
	if(gamemaptomap)
		{
			// check for gamemap in string.
			q2a_strcpy(buffer, text);
			q_strupr(buffer);
			
			str = q2a_strstr(buffer, "GAMEMAP");
			
			// double check the string is correct
			if(str && (str == buffer || *(str - 1) == ' ') && *(str + 7) == ' ')
				{
					// change to a map command
					if(str != buffer)
						{
							q2a_memcpy(buffer, text, str - buffer);
						}
						
					q2a_memcpy(str, text + ((str + 4) - buffer), q2a_strlen(text) - ((str + 4) - buffer) + 1);
					
					text = buffer;
				}
		}
		
	q2a_strcpy(buffer, text);
	q_strupr(buffer);
	
	str = q2a_strstr(buffer, "GAMEMAP");
	if(str && (str == buffer || *(str - 1) == ' ') && *(str + 7) == ' ')
		{
			// gamemap found, find map name
			str += 7;
			while(*str == ' ')
				{
					str++;
				}
				
			if(*str == '\"')
				{
					str++;
					str = text + (str - buffer);
					mapChangeFound = TRUE;
				}
		}
	else
		{
			str = q2a_strstr(buffer, "MAP");
			
			if(str && (str == buffer || *(str - 1) == ' ') && *(str + 3) == ' ')
				{
					// map found, find map name
					str += 3;
					while(*str == ' ')
						{
							str++;
						}
						
					if(*str == '\"')
						{
							str++;
							str = text + (str - buffer);
							mapChangeFound = TRUE;
						}
				}
		}
		
	if(mapChangeFound)
		{
			if ( mapcfgexec )
				{
					char *nameBuffer;
					
					q2a_strcpy(buffer, "exec mapcfg/");
					q2a_strcat(buffer, gmapname);
					q2a_strcat(buffer, "-end.cfg\n");
					gi.AddCommandString(buffer);
					
					q2a_strcpy(buffer, "exec ");
					
					nameBuffer = buffer + q2a_strlen(buffer);
					while(*str && *str != '\"')
						{
							*nameBuffer++ = *str++;
						}
						
					*nameBuffer = 0;
					
					q2a_strcat(buffer, "-pre.cfg\n");
					gi.AddCommandString(buffer);
				}
		}
		
	gi.AddCommandString(text);
}


//===================================================================

char argtext[2048];

char *getArgs(void)
{
	char *p;
	
	p = gi.args();
	q2a_strcpy(argtext, p);
	p = argtext;
	
	if (*p == '"')
		{
			p++;
			p[q2a_strlen(p)-1] = 0;
		}
		
	return p;
}




void processCommand(int cmdidx, int startarg, edict_t *ent)
{
	if (gi.argc() > startarg)
		{
			switch(zbotCommands[cmdidx].cmdtype)
				{
				case CMDTYPE_LOGICAL:
					*((qboolean *)zbotCommands[cmdidx].datapoint) = getLogicalValue(gi.argv(startarg));
					break;
					
				case CMDTYPE_NUMBER:
					*((int *)zbotCommands[cmdidx].datapoint) = q2a_atoi(gi.argv(startarg));
					break;
					
				case CMDTYPE_STRING:
					processstring(zbotCommands[cmdidx].datapoint, gi.argv(startarg), 255, 0);
					break;
				}
		}
		
	switch(zbotCommands[cmdidx].cmdtype)
		{
		case CMDTYPE_LOGICAL:
			gi.cprintf (ent, PRINT_HIGH, "%s = %s\n", zbotCommands[cmdidx].cmdname, *((qboolean *)zbotCommands[cmdidx].datapoint) ? "Yes" : "No");
			break;
			
		case CMDTYPE_NUMBER:
			gi.cprintf (ent, PRINT_HIGH, "%s = %d\n", zbotCommands[cmdidx].cmdname, *((int *)zbotCommands[cmdidx].datapoint));
			break;
			
		case CMDTYPE_STRING:
			gi.cprintf (ent, PRINT_HIGH, "%s = %s\n", zbotCommands[cmdidx].cmdname, (char *)zbotCommands[cmdidx].datapoint);
			break;
		}
}



qboolean readCfgFile(char *cfgfilename)
{
	FILE *cfgfile;
	char buff1[256];
	char buff2[256];
	
	cfgfile = fopen(cfgfilename, "rt");
	if(!cfgfile) return FALSE;
	
	while(fgets(buffer, 256, cfgfile) != NULL)
		{
			char *cp = buffer;
			
			SKIPBLANK(cp);
			
			if(!(cp[0] == ';' || cp[0] == '\n' || isBlank (cp)))
				{
					if(breakLine(cp, buff1, buff2, sizeof(buff2) - 1))
						{
							unsigned int i;
							
							for(i = 0; i < ZBOTCOMMANDSSIZE; i++)
								{
									if((zbotCommands[i].cmdwhere & CMDWHERE_CFGFILE) &&  startContains (zbotCommands[i].cmdname, buff1))
										{
											if(zbotCommands[i].initfunc)
												{
													(*zbotCommands[i].initfunc)(buff2);
												}
											else switch(zbotCommands[i].cmdtype)
													{
													case CMDTYPE_LOGICAL:
														*((qboolean *)zbotCommands[i].datapoint) = getLogicalValue(buff2);
														break;
														
													case CMDTYPE_NUMBER:
														*((int *)zbotCommands[i].datapoint) = q2a_atoi(buff2);
														break;
														
													case CMDTYPE_STRING:
														q2a_strcpy(zbotCommands[i].datapoint, buff2);
														break;
													}
													
											break;
										}
								}
						}
				}
		}
		
	fclose(cfgfile);
	
	return TRUE;
}


void readCfgFiles(void)
{
	char cfgFile[100];
	qboolean ret;
	
	if(!q2admintxt || isBlank(q2admintxt->string))
		{
			q2a_strcpy(cfgFile, CFGFILE);
		}
	else
		{
			q2a_strcpy(cfgFile, q2admintxt->string);
		}
		
	ret = readCfgFile(cfgFile);
	
	sprintf(buffer, "%s/%s", moddir, cfgFile);
	if(readCfgFile(buffer))
		{
			ret = TRUE;
		}
		
	if(!ret)
		{
			gi.dprintf ("WARNING: " CFGFILE " could not be found\n");
			//    logEvent(LT_INTERNALWARN, 0, NULL, CFGFILE " could not be found", IW_Q2ADMINCFGLOAD, 0.0);
		}
}



int getClientsFromArg(int client, edict_t *ent, char *cp, char **text)
{
	int clienti;
	unsigned int like, maxi;
	regex_t r;
	char strbuffer[sizeof(buffer)];
	
	maxi = 0;
	
	if(startContains(cp, "LIKE"))
		{
			like = 1;
			
			cp += 5;
			SKIPBLANK(cp);
			
			if(*cp == '\"')
				{
					cp++;
					cp = processstring(strbuffer, cp, sizeof(strbuffer), '\"');
					cp++;
				}
			else
				{
					cp = processstring(strbuffer, cp, sizeof(strbuffer), ' ');
				}
			SKIPBLANK(cp);
		}
	else if(startContains(cp, "RE"))
		{
			like = 2;
			
			cp += 2;
			SKIPBLANK(cp);
			
			if(*cp == '\"')
				{
					cp++;
					cp = processstring(strbuffer, cp, sizeof(strbuffer), '\"');
					cp++;
				}
			else
				{
					cp = processstring(strbuffer, cp, sizeof(strbuffer), ' ');
				}
			SKIPBLANK(cp);
			
			q_strupr(strbuffer);
			
			q2a_memset(&r, 0x0, sizeof(r));
			//    if(regcomp(&r, strbuffer, REG_EXTENDED))
			if(regcomp(&r, strbuffer, 0))
				{
					gi.cprintf(ent, PRINT_HIGH, "Regular Expression Incorrect.\n");
					return 0;
				}
		}
	else if(startContains(cp, "CL"))
		{
			like = 3;
			
			cp += 2;
			SKIPBLANK(cp);
			
			if(!isdigit(*cp))
				{
					return 0;
				}
				
			for(clienti = 0; clienti < maxclients->value; clienti++)
				{
					proxyinfo[clienti].clientcommand &= ~CCMD_SELECTED;
				}
				
			if(isdigit(*cp))
				{
					while(*cp)
						{
							clienti = q2a_atoi(cp);
							
							if(clienti >= 0 && clienti < maxclients->value && proxyinfo[clienti].inuse)
								{
									proxyinfo[clienti].clientcommand |= CCMD_SELECTED;
									maxi++;
								}
								
							while(isdigit(*cp))
								{
									cp++;
								}
								
							SKIPBLANK(cp);
							
							if(*cp && *cp != '+')
								{
									break;
								}
								
							if(*cp == '+')
								{
									cp++;
								}
								
							SKIPBLANK(cp);
							
							if(*cp && !isdigit(*cp))
								{
									break;
								}
						}
				}
				
			SKIPBLANK(cp);
		}
	else
		{
			like = 0;
			
			if(*cp == '\"')
				{
					cp++;
					cp = processstring(strbuffer, cp, sizeof(strbuffer), '\"');
					cp++;
				}
			else
				{
					cp = processstring(strbuffer, cp, sizeof(strbuffer), ' ');
				}
			SKIPBLANK(cp);
		}
		
		
		
		
	if(like < 3)
		{
			for(clienti = 0; clienti < maxclients->value; clienti++)
				{
					proxyinfo[clienti].clientcommand &= ~CCMD_SELECTED;
					
					//    if(clienti == client)
					//    {
					//      continue;
					//    }
					
					if(proxyinfo[clienti].inuse)
						{
							switch(like)
								{
								case 0:
									if(Q_stricmp(proxyinfo[clienti].name, strbuffer) == 0)
										{
											maxi++;
											proxyinfo[clienti].clientcommand |= CCMD_SELECTED;
										}
									break;
									
								case 1:
									if(stringContains(proxyinfo[clienti].name, strbuffer))
										{
											maxi++;
											proxyinfo[clienti].clientcommand |= CCMD_SELECTED;
										}
									break;
									
								case 2:
									q2a_strcpy(strbuffer, proxyinfo[clienti].name);
									q_strupr(strbuffer);
									if(regexec(&r, strbuffer, 0, 0, 0) != REG_NOMATCH)
										{
											maxi++;
											proxyinfo[clienti].clientcommand |= CCMD_SELECTED;
										}
									break;
								}
						}
				}
				
			if(like == 2)
				{
					regfree(&r);
				}
		}
		
	if(maxi)
		{
			*text = cp;
			return maxi;
		}
	else
		{
			gi.cprintf(ent, PRINT_HIGH, "no player name matches found.\n");
		}
		
	return 0;
}



edict_t *getClientFromArg(int client, edict_t *ent, int *cleintret, char *cp, char **text)
{
	int clienti, foundclienti;
	unsigned int like;
	regex_t r;
	char strbuffer[sizeof(buffer)];
	
	foundclienti = -1;
	
	if(startContains(cp, "LIKE"))
		{
			like = 1;
			
			cp += 5;
			SKIPBLANK(cp);
			
			if(*cp == '\"')
				{
					cp++;
					cp = processstring(strbuffer, cp, sizeof(strbuffer), '\"');
					cp++;
				}
			else
				{
					cp = processstring(strbuffer, cp, sizeof(strbuffer), ' ');
				}
			SKIPBLANK(cp);
		}
	else if(startContains(cp, "RE"))
		{
			like = 2;
			
			cp += 2;
			SKIPBLANK(cp);
			
			if(*cp == '\"')
				{
					cp++;
					cp = processstring(strbuffer, cp, sizeof(strbuffer), '\"');
					cp++;
				}
			else
				{
					cp = processstring(strbuffer, cp, sizeof(strbuffer), ' ');
				}
			SKIPBLANK(cp);
			
			q_strupr(strbuffer);
			
			q2a_memset(&r, 0x0, sizeof(r));
			//    if(regcomp(&r, strbuffer, REG_EXTENDED))
			if(regcomp(&r, strbuffer, 0))
				{
					gi.cprintf(ent, PRINT_HIGH, "Regular Expression Incorrect.\n");
					return NULL;
				}
		}
	else if(startContains(cp, "CL"))
		{
			like = 3;
			
			cp += 2;
			SKIPBLANK(cp);
			
			if(!isdigit(*cp))
				{
					return NULL;
				}
				
			foundclienti = q2a_atoi(cp);
			
			while(isdigit(*cp))
				{
					cp++;
				}
				
			SKIPBLANK(cp);
			
			if(foundclienti < 0 || foundclienti > maxclients->value || !proxyinfo[foundclienti].inuse)
				{
					foundclienti = -1;
				}
		}
	else
		{
			like = 0;
			
			if(*cp == '\"')
				{
					cp++;
					cp = processstring(strbuffer, cp, sizeof(strbuffer), '\"');
					cp++;
				}
			else
				{
					cp = processstring(strbuffer, cp, sizeof(strbuffer), ' ');
				}
			SKIPBLANK(cp);
		}
		
		
	if(like < 3)
		{
			for(clienti = 0; clienti < maxclients->value; clienti++)
				{
					//      if(clienti == client)
					//      {
					//        continue;
					//      }
					if(proxyinfo[clienti].inuse)
						{
							switch(like)
								{
								case 0:
									if(Q_stricmp(proxyinfo[clienti].name, strbuffer) == 0)
										{
											if(foundclienti != -1)
												{
													gi.cprintf(ent, PRINT_HIGH, "2 or more player name matches.\n");
													return NULL;
												}
												
											foundclienti = clienti;
										}
									break;
									
								case 1:
									if(stringContains(proxyinfo[clienti].name, strbuffer))
										{
											if(foundclienti != -1)
												{
													gi.cprintf(ent, PRINT_HIGH, "2 or more player name matches.\n");
													return NULL;
												}
												
											foundclienti = clienti;
										}
									break;
									
								case 2:
									q2a_strcpy(strbuffer, proxyinfo[clienti].name);
									q_strupr(strbuffer);
									if(regexec(&r, strbuffer, 0, 0, 0) != REG_NOMATCH)
										{
											if(foundclienti != -1)
												{
													regfree(&r);
													gi.cprintf(ent, PRINT_HIGH, "2 or more player name matches.\n");
													return NULL;
												}
												
											foundclienti = clienti;
										}
									break;
								}
						}
				}
				
			if(like == 2)
				{
					regfree(&r);
				}
		}
		
	if(foundclienti != -1)
		{
			*text = cp;
			*cleintret = foundclienti;
			return getEnt((foundclienti + 1));
		}
	else
		{
			gi.cprintf(ent, PRINT_HIGH, "no player name matches found.\n");
		}
		
	return NULL;
}



qboolean sayPersonCmd(edict_t *ent, int client, char *args)
{
	char *cp = args, *text;
	edict_t *enti;
	int clienti;
	char tmptext[2100];
	
	SKIPBLANK(cp);
	
	enti = getClientFromArg(client, ent, &clienti, cp, &text);
	
	if(enti)
		{
			// make sure the text doesn't overflow the internal buffer...
			if(q2a_strlen(text) > 2000)
				{
					text[2000] = 0;
				}
				
			// check for banned chat words
			if(checkCheckIfChatBanned(text))
				{
					gi.cprintf(NULL, PRINT_HIGH, "%s: %s\n", proxyinfo[client].name, currentBanMsg);
					gi.cprintf(ent, PRINT_HIGH, "%s\n", currentBanMsg);
					logEvent(LT_CHATBAN, getEntOffset(ent) - 1, ent, text, 0, 0.0);
					return FALSE;
				}
				
			sprintf(tmptext, "(%s)(private message to: %s) %s\n", proxyinfo[client].name, proxyinfo[clienti].name, text);
			cprintf_internal(NULL, PRINT_CHAT, "%s", tmptext);
			cprintf_internal(ent, PRINT_CHAT, "%s", tmptext);
			
			sprintf(tmptext, "(%s)(private message) %s\n", proxyinfo[client].name, text);
			cprintf_internal(enti, PRINT_CHAT, "%s", tmptext);
			
			return FALSE;
		}
		
	return TRUE;
}

qboolean sayGroupCmd(edict_t *ent, int client, char *args)
{
	char *cp = args, *text;
	edict_t *enti;
	int clienti;
	char  tmptext[2100];
	int max;
	
	SKIPBLANK(cp);
	
	max = getClientsFromArg(client, ent, cp, &text);
	
	if(max)
		{
			// make sure the text doesn't overflow the internal buffer...
			if(q2a_strlen(text) > 2000)
				{
					text[2000] = 0;
				}
				
			// check for banned chat words
			if(checkCheckIfChatBanned(text))
				{
					gi.cprintf(NULL, PRINT_HIGH, "%s: %s\n", proxyinfo[client].name, currentBanMsg);
					gi.cprintf(ent, PRINT_HIGH, "%s\n", currentBanMsg);
					logEvent(LT_CHATBAN, getEntOffset(ent) - 1, ent, text, 0, 0.0);
					return FALSE;
				}
				
			for(clienti = 0; clienti < maxclients->value; clienti++)
				{
					if(proxyinfo[clienti].clientcommand & CCMD_SELECTED)
						{
							enti = getEnt((clienti + 1));
							
							sprintf(tmptext, "(%s)(private message to: %s) %s\n", proxyinfo[client].name, proxyinfo[clienti].name, text);
							cprintf_internal(NULL, PRINT_CHAT, "%s", tmptext);
							cprintf_internal(ent, PRINT_CHAT, "%s", tmptext);
							
							sprintf(tmptext, "(%s)(private message) %s\n", proxyinfo[client].name, text);
							cprintf_internal(enti, PRINT_CHAT, "%s", tmptext);
						}
				}
				
			return FALSE;
		}
		
	return TRUE;
}



void proxyDetected(edict_t *ent, int client)
{
	proxyinfo[client].charindex = -6;
	removeClientCommand(client, QCMD_TESTRATBOT2);
	removeClientCommand(client, QCMD_ZPROXYCHECK2);
	serverLogZBot(ent, client);
	
	proxyinfo[client].clientcommand &= ~(CCMD_RATBOTDETECT | CCMD_ZPROXYCHECK2);
	proxyinfo[client].clientcommand |= CCMD_ZBOTDETECTED;
	
	if(displayzbotuser)
		{
			unsigned int i;
			
			q2a_strcpy(buffer, zbotuserdisplay);
			q2a_strcat(buffer, "\n");
			
			for(i = 0; i < numofdisplays; i++)
				{
					gi.bprintf (PRINT_HIGH, buffer, proxyinfo[client].name);
				}
		}
		
	if(customClientCmd[0])
		{
			addCmdQueue(client, QCMD_CUSTOM, 0, 0, 0);
		}
		
	if(disconnectuser)
		{
			addCmdQueue(client, QCMD_DISCONNECT, 1, 0, zbotuserdisplay);
		}
}

void ratbotDetected(edict_t *ent, int client)
{
	proxyinfo[client].charindex = -3;
	removeClientCommand(client, QCMD_TESTRATBOT2);
	removeClientCommand(client, QCMD_ZPROXYCHECK2);
	serverLogZBot(ent, client);
	
	proxyinfo[client].clientcommand &= ~(CCMD_RATBOTDETECT | CCMD_ZPROXYCHECK2);
	proxyinfo[client].clientcommand |= CCMD_ZBOTDETECTED;
	
	if(displayzbotuser)
		{
			unsigned int i;
			
			q2a_strcpy(buffer, zbotuserdisplay);
			q2a_strcat(buffer, "\n");
			
			for(i = 0; i < numofdisplays; i++)
				{
					gi.bprintf (PRINT_HIGH, buffer, proxyinfo[client].name);
				}
		}
		
	if(customClientCmd[0])
		{
			addCmdQueue(client, QCMD_CUSTOM, 0, 0, 0);
		}
		
	if(disconnectuser)
		{
			addCmdQueue(client, QCMD_DISCONNECT, 1, 0, zbotuserdisplay);
		}
}

void timescaleDetected(edict_t *ent, int client)
{
	proxyinfo[client].charindex = -5;
	removeClientCommand(client, QCMD_TESTRATBOT2);
	removeClientCommand(client, QCMD_ZPROXYCHECK2);
	serverLogZBot(ent, client);
	
	proxyinfo[client].clientcommand &= ~(CCMD_RATBOTDETECT | CCMD_ZPROXYCHECK2);
	proxyinfo[client].clientcommand |= CCMD_ZBOTDETECTED;
	
	if(displayzbotuser)
		{
			unsigned int i;
			
			q2a_strcpy(buffer, timescaleuserdisplay);
			q2a_strcat(buffer, "\n");
			
			for(i = 0; i < numofdisplays; i++)
				{
					gi.bprintf (PRINT_HIGH, buffer, proxyinfo[client].name);
				}
		}
		
	if(customClientCmd[0])
		{
			addCmdQueue(client, QCMD_CUSTOM, 0, 0, 0);
		}
		
	if(disconnectuser)
		{
			addCmdQueue(client, QCMD_DISCONNECT, 1, 0, timescaleuserdisplay);
		}
}

void hackDetected(edict_t *ent, int client)
{
	proxyinfo[client].charindex = -8;
	removeClientCommand(client, QCMD_TESTRATBOT2);
	removeClientCommand(client, QCMD_ZPROXYCHECK2);
	removeClientCommand(client, QCMD_TESTALIASCMD2);
	
	proxyinfo[client].clientcommand &= ~(CCMD_RATBOTDETECT | CCMD_ZPROXYCHECK2 | CCMD_WAITFORALIASREPLY1 | CCMD_WAITFORALIASREPLY2 | CCMD_WAITFORCONNECTREPLY);
	proxyinfo[client].clientcommand |= CCMD_ZBOTDETECTED;
	
	q2a_strcpy(buffer, hackuserdisplay);
	q2a_strcat(buffer, "\n");
	
	gi.bprintf (PRINT_HIGH, buffer, proxyinfo[client].name);
	
	if(customClientCmd[0])
		{
			addCmdQueue(client, QCMD_CUSTOM, 0, 0, 0);
		}
		
	if(disconnectuser)
		{
			addCmdQueue(client, QCMD_DISCONNECT, 1, 0, hackuserdisplay);
		}
}

/*
 * quadz 10mar07 - added check for possible mod exploit commands,
 * 		   based on r1ch's info at:
 * 		   http://secur1ty.net/advisories/002-Multiple_Vulnerabilities_In_OSP_Tourney_For_Quake_II.txt
 */
static qboolean client_command_is_mod_exploit (edict_t *ent, int client)
{
	/*
	 * 1. CRITICAL: Exploitable buffer overflow in the 'teamskin' command
	 * ==================================================================
	 * OSP Tourney allows a team captain to set their team skin using the teamskin
	 * command. Proper validation is not performed before performing a sprintf with
	 * the user supplied value into a 256 byte buffer on the stack. This leads to a
	 * reliable user controlled stack overflow which allows for arbitrary code
	 * execution.
	 *
	 * 2. CRITICAL: Exploitable buffer overflow in the 'kickplayer' command
	 * ====================================================================
	 * The length of the username or player ID to be kicked is not checked before it
	 * is copied into a 32 byte buffer on the stack. Reliable remote code execution
	 * is possible.
	 */

	// fprintf(stderr, "ClientCommand: %s\n", cmd);
	
	char *cmd = gi.argv(0);
	int enforce_arg_maxlen = -1;

	if (Q_stricmp(cmd, "teamskin") == 0) {
		enforce_arg_maxlen = 127;
	}
	else if (Q_stricmp(cmd, "kickplayer") == 0  ||
		 Q_stricmp(cmd, "removeplayer") == 0  ||
		 Q_stricmp(cmd, "remove") == 0) {
		enforce_arg_maxlen = 20;
	}

	if (enforce_arg_maxlen >= 0) {
		char *args = getArgs();
		if (strlen(args) > enforce_arg_maxlen) {
			gi.cprintf(ent, PRINT_HIGH, "Error: Arguments to %s command must not exceed %d characters in length.\n",
					cmd, enforce_arg_maxlen);
			return TRUE;
		}
	}

	return FALSE;
}

qboolean doClientCommand(edict_t *ent, int client, qboolean *checkforfloodafter)
{
//*** UPDATE START ***
	unsigned int i, cnt, sameip;
	char abuffer[256];
	char stemp[1024];
	char response[2048];
	int alevel, slen;
	int q2a_admin_command = 0;
	qboolean dont_print;
//*** UPDATE END ***
	char  *cmd;
	char  text[2048];

	//r1ch 2005-01-26 disable hugely buggy commands BEGIN
	//edict_t *enti;
	//int clienti;
	//r1ch 2005-01-26 disable hugely buggy commands END

	if(client >= maxclients->value)
		return FALSE;
	
	if (client_command_is_mod_exploit(ent, client))
		return FALSE;

	cmd = gi.argv(0);

//*** UPDATE START ***
	if(gi.argc() > 1)
	{
		q2a_strcpy (response, "");
		q2a_strcat (response, gi.args());
	}
	else
	{
		q2a_strcpy (response, cmd);
	}

	if (*(rcon_password->string))
	{
			if (strstr(response, rcon_password->string))
			{
					//gi.cprintf(NULL, PRINT_HIGH, "%s: Tried to run disabledcommand: (%s)\n", proxyinfo[client].name, response);
					//logEvent(LT_DISABLECMD, getEntOffset(ent) - 1, ent,response, 0, 0.0);
					//stuffcmd(ent, "echo YOU HAVE BEEN LOGGED FOR THAT ACTION!!!!\n");

					//r1ch: buffer overflow fix
					snprintf(abuffer, sizeof(abuffer)-1, "EXPLOIT - %s", response);
					abuffer[sizeof(abuffer)-1] = 0;
					logEvent(LT_ADMINLOG, client, ent, abuffer, 0, 0.0);
					gi.dprintf("%s\n",abuffer);
	                
					return FALSE;
			}
	}
//*** UPDATE END ***
	
	if(Q_stricmp (cmd, zbot_teststring_test1) == 0)
		{
			if(proxyinfo[client].inuse && proxyinfo[client].clientcommand & CCMD_STARTUPTEST)
				{
					proxyinfo[client].clientcommand &= ~CCMD_STARTUPTEST;
					removeClientCommand(client, QCMD_STARTUPTEST);
					proxyinfo[client].retries = 0;
					proxyinfo[client].rbotretries = 0;
					
					if(zbotdetect)
						{
							addCmdQueue(client, QCMD_RESTART, 1, 0, 0);
							
							stuffcmd(ent, "set msg 0 u\n");
							
							addCmdQueue(client, QCMD_LETRATBOTQUIT, 1, 0, 0);
							
							if(!(proxyinfo[client].clientcommand & CCMD_RBOTCLEAR))
								{
									addCmdQueue(client, QCMD_TESTRATBOT, 12, 0, 0);
								}
								
							// standard proxy detection.
							if(!(proxyinfo[client].clientcommand & CCMD_NITRO2PROXY))
								{
									addCmdQueue(client, QCMD_TESTSTANDARDPROXY, 10, 0, 0);
								}
								
							// Test if alias and connect command are OK
							if(!(proxyinfo[client].clientcommand & CCMD_ALIASCHECKSTARTED))
								{
									addCmdQueue(client, QCMD_TESTALIASCMD1, 1, 0, 0);
								}
						}
				}
			return FALSE;
		}
	else if(proxyinfo[client].clientcommand & CCMD_ZPROXYCHECK2) // check for proxy string
		{
			char *a1 = gi.argv(1);
			char *a2 = gi.argv(2);
			
			if(!zbotdetect || !proxyinfo[client].inuse)
				{
					return FALSE;
				}
				
			if(proxyinfo[client].teststr[0] && Q_stricmp (cmd, proxyinfo[client].teststr) == 0)
				{
					if(!proxyinfo[client].inuse)
						{
							return FALSE;
						}
						
					// we have passed the test!!
					// get next char to test
					proxyinfo[client].charindex++;
					
					// check if it's a NITRO2 proxy client and skip the test for the '.' or ',' characters.
					while((proxy_bwproxy == 2 || proxy_nitro2 == 2 || (proxyinfo[client].clientcommand & CCMD_NITRO2PROXY)) &&
						  (testchars[proxyinfo[client].charindex] == '.' || testchars[proxyinfo[client].charindex] == ','))
						{
							proxyinfo[client].charindex++;
						}
						
						
					proxyinfo[client].clientcommand &= ~CCMD_ZPROXYCHECK2;
					removeClientCommand(client, QCMD_ZPROXYCHECK2);
					return FALSE;
				}
			else if(Q_stricmp (cmd, zbot_teststring_test3) == 0)
				{
					return FALSE;
				}
			else if(Q_stricmp (cmd, zbot_teststring_test2) == 0)
				{
					if(!zbotdetect || !proxyinfo[client].inuse || (proxyinfo[client].clientcommand & CCMD_ZBOTDETECTED) || checkForOverflows(ent, client))
						{
							return FALSE;
						}
						
					if(proxyinfo[client].retries < MAXDETECTRETRIES)
						{
							// try and get "unknown command" off the screen as fast as possible
							gi.cprintf (ent, PRINT_HIGH, "\n\n\n\n\n\n");
							
							proxyinfo[client].clientcommand &= ~CCMD_ZPROXYCHECK2;
							removeClientCommand(client, QCMD_ZPROXYCHECK2);
							addCmdQueue(client, QCMD_CLEAR, 0, 0, 0);
							addCmdQueue(client, QCMD_RESTART, 2 + (3 * random()), 0, 0);
							proxyinfo[client].retries++;
							return FALSE;
						}
						
					// log zbot user
					serverLogZBot(ent, client);
					
					// setup to clear client console, timeout and not add to server log again
					//        proxyinfo[client].clientcommand &= ~CCMD_ZPROXYCHECK2;
					removeClientCommand(client, QCMD_ZPROXYCHECK2);
					addCmdQueue(client, QCMD_CLEAR, 0, 0, 0);
					proxyinfo[client].clientcommand |= CCMD_ZPROXYCHECK2;
					addCmdQueue(client, QCMD_ZPROXYCHECK2, (zbotdetectactivetimeout < 0) ? 5 + (randomwaitreporttime * random()) : zbotdetectactivetimeout, IW_ZBOTDETECT, 0);
					proxyinfo[client].clientcommand |= CCMD_ZBOTDETECTED;
					
					// try and get "unknown command" off the screen as fast as possible
					gi.cprintf (ent, PRINT_HIGH, "\n\n\n\n\n\n");
					return FALSE;
				}
		}
	else if(Q_stricmp (cmd, zbot_teststring_test3) == 0)
		{
			return FALSE;
		}
	else if(Q_stricmp (cmd, zbot_teststring_test2) == 0) // check for end proxy string
		{
			if(!proxyinfo[client].inuse)
				{
					return FALSE;
				}
				
			// do we have more char's to check?
			if(zbotdetect && proxyinfo[client].charindex < testcharslength)
				{
					addCmdQueue(client, QCMD_ZPROXYCHECK1, 0, 0, 0);
				}
			else
				{
					proxyinfo[client].clientcommand |= CCMD_ZBOTCLEAR;
					//        gi.cprintf (ent, PRINT_HIGH, "Finished checking...");
				}
				
			return FALSE;
			
		}
		
	if(q2a_strcmp (cmd, proxyinfo[client].hack_timescale) == 0)
		{
			if(!proxyinfo[client].inuse)
				{
					return FALSE;
				}
				
			if(atoi(gi.argv(1)) != 1)
				{
					timescaleDetected(ent, client);
				}
			else
				{
					if(gi.argv(1)[1] == '.')
						{
							timescaleDetected(ent, client);
						}
				}
				
			return FALSE;
		}
		
	if(q2a_strcmp(cmd, proxyinfo[client].hack_checkvar) == 0)
		{
			if(!proxyinfo[client].inuse)
				{
					return FALSE;
				}
				
			checkVariableValid(ent, client, gi.argv(1));
			return FALSE;
		}
		
	if(proxyinfo[client].clientcommand & CCMD_RATBOTDETECT)
		{
			if(Q_stricmp(cmd, "Please") == 0)
				{
					char *args = getArgs();
					
					if(Q_stricmp(args, "help me : What is a Bot ??") == 0)
						{
							ratbotDetected(ent, client);
							return FALSE;
						}
				}
			else if(Q_stricmp(cmd, "Yeah") == 0)
				{
					char *args = getArgs();
					
					if(Q_stricmp(args, "!!! I am a R A T B O T !!!!! ??") == 0)
						{
							ratbotDetected(ent, client);
							return FALSE;
						}
				}
		}
		
	if((startContains(cmd, ZBOT_TESTSTRING_TEST1_OLD) && isdigit(cmd[7]) && isdigit(cmd[8]) && cmd[9] == 0) ||
		(startContains(cmd, ZBOT_TESTSTRING_TEST2_OLD) && isdigit(cmd[3]) && isdigit(cmd[4]) && cmd[5] == 0) ||
		(cmd[1] == BOTDETECT_CHAR1 && cmd[2] == BOTDETECT_CHAR2 && isdigit(cmd[3]) && isdigit(cmd[4]) && (cmd[7] == 0 || cmd[8] == 0)))
		{
			// clear retries just in case...
			proxyinfo[client].retries = 0;
			
			return FALSE; //ignore because it's from a older level or something
		}
	else if(startContains(cmd, ZBOT_TESTSTRING_TEST1_OLD) || startContains(cmd, ZBOT_TESTSTRING_TEST2_OLD) ||
			(cmd[1] == BOTDETECT_CHAR1 && cmd[2] == BOTDETECT_CHAR2 && (cmd[7] == 0 || cmd[8] == 0)))
		{
			if(!zbotdetect || !proxyinfo[client].inuse)
				{
					return FALSE;
				}
				
			sprintf(text, "I(%d) Cmd(%s) Exp(%s) (unexcepted cmd)", proxyinfo[client].charindex, cmd, proxyinfo[client].teststr);
			logEvent(LT_INTERNALWARN, client, ent, text, IW_UNEXCEPTEDCMD, 0.0);
			
			// clear retries just in case...
			proxyinfo[client].retries = 0;
			return FALSE;
		}
	else if(cmd[1] == BOTDETECT_CHAR1 && cmd[2] == BOTDETECT_CHAR2)
		{
			if(!zbotdetect || !proxyinfo[client].inuse)
				{
					return FALSE;
				}
				
			sprintf(text, "I(%d) Cmd(%s) Exp(%s) (unknown cmd)", proxyinfo[client].charindex, cmd, proxyinfo[client].teststr);
			logEvent(LT_INTERNALWARN, client, ent, text, IW_UNKNOWNCMD, 0.0);
		}
		
	if(proxyinfo[client].clientcommand & CCMD_WAITFORALIASREPLY1)
		{
			if(Q_stricmp (cmd, "alias") == 0)
				{
					proxyinfo[client].clientcommand |= CCMD_ALIASCHECKSTARTED;
					hackDetected(ent, client);
					return FALSE;
				}
				
			if(proxyinfo[client].hacked_disconnect == 1)
				{
					sameip = 1;
					if((proxyinfo[client].hacked_disconnect_ip[0] != proxyinfo[client].ipaddressBinary[0]) ||
						(proxyinfo[client].hacked_disconnect_ip[1] != proxyinfo[client].ipaddressBinary[1]) ||
						(proxyinfo[client].hacked_disconnect_ip[2] != proxyinfo[client].ipaddressBinary[2]) ||
						(proxyinfo[client].hacked_disconnect_ip[3] != proxyinfo[client].ipaddressBinary[3]))
						{
							sameip = 0;
						}
					if (sameip == 1)
						{
							proxyinfo[client].hacked_disconnect = 0;
							hackDetected(ent, client);
							return FALSE;
						}
					proxyinfo[client].hacked_disconnect = 0;
				}
				
			if(proxyinfo[client].checked_hacked_exe == 0)
				{
					char *ratte = Info_ValueForKey(proxyinfo[client].userinfo, "rate");
					proxyinfo[client].checked_hacked_exe = 1;
					if(*ratte == 0)
						{
							hackDetected(ent, client);
							return FALSE;
						}
				}
				
		}
		
	if(proxyinfo[client].clientcommand & CCMD_WAITFORALIASREPLY2)
		{
			if(Q_stricmp (cmd, proxyinfo[client].hack_teststring1) == 0)
				{
					hackDetected(ent, client);
					return FALSE;
				}
			if(Q_stricmp (cmd, proxyinfo[client].hack_teststring2) == 0)
				{
					proxyinfo[client].clientcommand &= ~CCMD_WAITFORALIASREPLY2;
					return FALSE;
				}
		}
		
	if((proxyinfo[client].clientcommand & CCMD_WAITFORCONNECTREPLY) &&
		Q_stricmp (cmd, proxyinfo[client].hack_teststring3) == 0)
		{
			proxyinfo[client].clientcommand &= ~CCMD_WAITFORCONNECTREPLY;
			proxyinfo[client].hacked_disconnect = 1;
			proxyinfo[client].hacked_disconnect_ip[0] = proxyinfo[client].ipaddressBinary[0];
			proxyinfo[client].hacked_disconnect_ip[1] = proxyinfo[client].ipaddressBinary[1];
			proxyinfo[client].hacked_disconnect_ip[2] = proxyinfo[client].ipaddressBinary[2];
			proxyinfo[client].hacked_disconnect_ip[3] = proxyinfo[client].ipaddressBinary[3];
			return FALSE;
		}
		
	if(gi.argc() > 1)
		{
			q2a_strcpy (proxyinfo[client].lastcmd, cmd);
			q2a_strcat (proxyinfo[client].lastcmd, " ");
			q2a_strcat (proxyinfo[client].lastcmd, gi.args());
		}
	else
		{
			q2a_strcpy (proxyinfo[client].lastcmd, cmd);
		}
		
	// check for disabled command.
	if(disablecmds_enable && checkDisabledCommand(proxyinfo[client].lastcmd))
		{
			gi.cprintf(NULL, PRINT_HIGH, "%s: Tried to run disabled command: %s\n", proxyinfo[client].name, proxyinfo[client].lastcmd);
			logEvent(LT_DISABLECMD, getEntOffset(ent) - 1, ent, proxyinfo[client].lastcmd, 0, 0.0);
			return FALSE;
		}

//*** UPDATE START ***
	if(     Q_stricmp (cmd, "admin") == 0 ||  
			Q_stricmp (cmd, "referee") == 0 ||
			Q_stricmp (cmd, "ref") == 0 ||
			stringContains (cmd, "r_") == 1)
		{
			//r1ch 2005-05-11: snprintf to avoid buffer overflow BEGIN
			snprintf (abuffer, sizeof(abuffer)-1, "REFEREE - %s: %s", cmd, gi.argv(1));
			abuffer[sizeof(abuffer)-1] = 0;
			//r1ch 2005-05-11: snprintf to avoid buffer overflow END
			logEvent(LT_ADMINLOG, client, ent, abuffer, 0, 0.0);
			gi.dprintf("%s\n",abuffer);
		}

	if (proxyinfo[client].private_command>ltime)
	{
		//check this command for one of our private commands
		for (i = 0; i < PRIVATE_COMMANDS; i++)
		{
			if (private_commands[i].command[0])
			{
				if (Q_stricmp(proxyinfo[client].lastcmd,private_commands[i].command)==0)
				{
					//we got a response on this command and don't spam
					proxyinfo[client].private_command_got[i] = true;
					return FALSE;
				}
			}
		}
	}

	if(Q_stricmp (cmd, "say") == 0 || Q_stricmp (cmd, "say_team") == 0 || Q_stricmp (cmd, "say_world") == 0)
	{
		strcpy(stemp,gi.args());
		slen = strlen(stemp);
		cnt = 0;
		for (i = 0; i < slen; i++)
		{
			if (stemp[i]=='%')
			{
				cnt++;
			}
		}
		if (cnt>5)
			return FALSE;
			//this check is for non standard p_Ver/p_mod replies
			//pooy, check return string to match q2ace response
			//if they normal, dont spam
		if (proxyinfo[client].cmdlist_timeout>ltime)
		{
			if (q2a_strstr(gi.args(),"BLOCKED_MODEL"))		
			{			
				//1.20
				i = atoi(gi.argv(5));
				if (i!=proxyinfo[client].blocklist)
				{
					//KICK
					gi.bprintf(PRINT_HIGH,MOD_KICK_MSG,proxyinfo[client].name,i);
					//sprintf(abuffer,client_msg,version_check);
					//gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",abuffer);
					addCmdQueue(client, QCMD_DISCONNECT, 1, 0, Q2A_MOD_KICK_MSG);
				}
				else
				{
					//gi.dprintf("%s %s\n",block_models[i].model_name,gi.argv(6));
					if (i<0 || i>=MAX_BLOCK_MODELS)
					{
						//KICK
						gi.bprintf(PRINT_HIGH,MOD_KICK_MSG,proxyinfo[client].name,i);
						//sprintf(abuffer,client_msg,version_check);
						//gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",abuffer);
						addCmdQueue(client, QCMD_DISCONNECT, 1, 0, Q2A_MOD_KICK_MSG);
					}
					else
					{
						//it matched so compare the string now
						if (q2a_strcmp(block_models[i].model_name,gi.argv(6)))
						{
							//didnt match, kick
							gi.bprintf(PRINT_HIGH,MOD_KICK_MSG,proxyinfo[client].name,256);
							//sprintf(abuffer,client_msg,version_check);
							//gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",abuffer);
							addCmdQueue(client, QCMD_DISCONNECT, 1, 0, Q2A_MOD_KICK_MSG);
						}
						else
						{
							//all ok so mark
							proxyinfo[client].cmdlist |= 2;
						}
					}				
				}
				return FALSE; //dont spam
			}
			else if (q2a_strstr(gi.args(),"SERVERIP"))
			{
				//gi.dprintf("pooy test : %s , %s\n",proxyinfo[client].serverip,gi.args());
				//1.20
				if (!*serverip)
				{
					proxyinfo[client].cmdlist |= 4;
					return FALSE;
				}
				//compare random char with what we gave them
				if (strcmp(gi.argv(5),proxyinfo[client].serverip)==0)
				{
					//k its not been tampered with, now check the ip
					if (strcmp(gi.argv(6),serverip)==0)
					{
						//ip correct
						proxyinfo[client].cmdlist |= 4;
					}
					else
					{
						gi.bprintf(PRINT_HIGH,MOD_KICK_MSG,proxyinfo[client].name,1);
						//sprintf(abuffer,client_msg,version_check);
						//gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",abuffer);
						addCmdQueue(client, QCMD_DISCONNECT, 1, 0, Q2A_MOD_KICK_MSG);
					}
				}
				else
				{
					gi.bprintf(PRINT_HIGH,MOD_KICK_MSG,proxyinfo[client].name,proxyinfo[client].pmod);
					//sprintf(abuffer,client_msg,version_check);
					//gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",abuffer);
					addCmdQueue(client, QCMD_DISCONNECT, 1, 0, Q2A_MOD_KICK_MSG);
				}
				return FALSE;
			}
		}
		
			if (proxyinfo[client].pmodver>ltime)
			{
				if (gl_driver_check & 1)
				{
					if (q2a_strstr(gi.args(),"Q2ADMIN_GL_DRIVER_CHECK"))
					{
						dont_print = true;
						//got gl_driver response
						if (strlen(proxyinfo[client].gl_driver))
						{
							//we have a response
							if (strcmp(proxyinfo[client].gl_driver,gi.args())==0)
							{
								//if they match ignore
							}
							else
							{
								//if they dont
								strcpy(proxyinfo[client].gl_driver,gi.args());
								proxyinfo[client].gl_driver_changes++;
								dont_print = false;
								if (gl_driver_check & 4)
				  					gi.dprintf ("%s %s.\n", proxyinfo[client].name,gi.args());
							}
						}
						else
						{
							strcpy(proxyinfo[client].gl_driver,gi.args());
							proxyinfo[client].gl_driver_changes++;
							dont_print = false;
							if (gl_driver_check & 4)
					  			gi.dprintf ("%s %s.\n", proxyinfo[client].name,gi.args());
						}

						if (gl_driver_max_changes)
						{
							if (proxyinfo[client].gl_driver_changes>gl_driver_max_changes)
							{
								//sprintf(abuffer,client_msg,version_check);
								//gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",abuffer);
								addCmdQueue(client, QCMD_DISCONNECT, 1, 0, "Too many gl_driver changes.");
							}
						}
						if (gl_driver_check & 2)
						{
							if (dont_print)
								return FALSE; //dont spam
						}
						else
						{
							//dont print ever
							return FALSE;
						}
					}
				}
			}
			//r1ch: removed NC 2.34 client forcing
		}
//*** UPDATE END ***

	if(Q_stricmp (cmd, "say") == 0 || Q_stricmp (cmd, "say_team") == 0)
		{
			if(strcmp(gi.argv(1), "XANIA") == 0 || strcmp(gi.argv(1), "Nitro2") ==  0)
				{
					if(proxy_nitro2)
						{
							proxyinfo[client].clientcommand  |= CCMD_NITRO2PROXY;
						}
					else
						{
							proxyDetected(ent, client);
							return FALSE;
						}
				}
				
			if(checkForMute(client, ent, TRUE))
				{
					return FALSE;
				}
				
			if(extendedsay_enable)
				{
					char *args = getArgs();
					
					if(say_person_enable && startContains(args, "!p")) // say_person
						{
							if(sayPersonCmd(ent, client, args + 2))
								{
									gi.cprintf(ent, PRINT_HIGH, "say !p [LIKE/RE/CL] name message\n");
								}
								
							return FALSE;
						}
					else if(say_group_enable && startContains(args, "!g")) // say_group
						{
							if(sayGroupCmd(ent, client, args + 2))
								{
									gi.cprintf(ent, PRINT_HIGH, "say !g [LIKE/RE/CL] name message\n");
								}
								
							return FALSE;
						}
					//r1ch 2005-01-26 disable hugely buggy commands BEGIN
					/*else if(play_team_enable && startContains(args, "!t"))  // play_team
						{
							args += 2;
							SKIPBLANK(args);
							
							if(*args && q2a_strchr(args, ';') == NULL)
								{
									if ( printmessageonplaycmds )
										{
											sprintf(buffer, "say_team \"Playing Team Sound %s\"\n", args);
											stuffcmd(ent, buffer);
										}
									sprintf(buffer, "say_team \"swpplay %s\"\n", args);
									stuffcmd(ent, buffer);
								}
							else
								{
									gi.cprintf(ent, PRINT_HIGH, "say !t wavfile\n");
								}
								
							return FALSE;
						}
					else if(play_all_enable && startContains(args, "!a"))  // play_all
						{
							args += 2;
							SKIPBLANK(args);
							
							if(*args && q2a_strchr(args, ';') == NULL)
								{
									if ( printmessageonplaycmds )
										{
											sprintf(buffer, "say \"Playing Sound %s\"\n", args);
											stuffcmd(ent, buffer);
										}
									sprintf(buffer, "play %s\n", args);
									
									for(clienti = 0; clienti < maxclients->value; clienti++)
										{
											if(proxyinfo[clienti].inuse)
												{
													stuffcmd(getEnt((clienti + 1)), buffer);
												}
										}
								}
							else
								{
									gi.cprintf(ent, PRINT_HIGH, "say !a wavfile\n");
								}
								
							return FALSE;
						}
					else if(play_person_enable && startContains(args, "!w"))  // play_person
						{
							char *txt;
							
							args += 2;
							SKIPBLANK(args);
							
							enti = getClientFromArg(client, ent, &clienti, args, &txt);
							
							if(enti)
								{
									if(q2a_strlen(txt) > sizeof(buffer) - 10)
										{
											txt[sizeof(buffer) - 10] = 0;
										}
										
									if(q2a_strchr(args, ';') == NULL)
										{
											if ( printmessageonplaycmds )
												{
													sprintf(buffer, "(%s)(private play sound to: %s) %s\n", proxyinfo[client].name, proxyinfo[clienti].name, txt);
													cprintf_internal(NULL, PRINT_CHAT, "%s", buffer);
													cprintf_internal(ent, PRINT_CHAT, "%s", buffer);
													
													sprintf(buffer, "(%s)(private play sound) %s\n", proxyinfo[client].name, txt);
													cprintf_internal(enti, PRINT_CHAT, "%s", buffer);
												}
												
											sprintf(buffer, "play %s\n", txt);
											stuffcmd(enti, buffer);
										}
									else
										{
											gi.cprintf(ent, PRINT_HIGH, "say !w [LIKE/RE/CL] name wavfile\n");
										}
								}
							else
								{
									gi.cprintf(ent, PRINT_HIGH, "say !w [LIKE/RE/CL] name wavfile\n");
								}
								
							return FALSE;
						}
						*/
						//r1ch 2005-01-26 disable hugely buggy commands BEGIN
				}
		}
	else if(checkforfloodcmds(cmd))
		{
			if(checkForMute(client, ent, TRUE))
				{
					return FALSE;
				}
				
			*checkforfloodafter = TRUE;
		}
		
		
//	if(adminpassword[0] && proxyinfo[client].admin && cmd[0] == '!')
//		{
//*** UPDATE START ***
	if(cmd[0] == '!')
	{
		if(proxyinfo[client].q2a_admin)
		{
			q2a_admin_command = ADMIN_process_command(ent,client);
			if (q2a_admin_command)
				return FALSE;
		}

		if(Q_stricmp (cmd, "!admin") == 0)
		{
			//pooy admin
			if(num_admins)
			{
				if (gi.argc() != 3)
				{
					return FALSE;
				}
				alevel = get_admin_level(gi.argv(2),gi.argv(1));
				if (alevel)
				{
					sprintf(abuffer,"ADMIN - %s %s %d",gi.argv(2),gi.argv(1),alevel);
					logEvent(LT_ADMINLOG, client, ent, abuffer, 0, 0.0);
					gi.dprintf("%s\n",abuffer);

					proxyinfo[client].q2a_admin = alevel;
					//gi.bprintf(PRINT_HIGH, "%s has become a level %d admin.\n", proxyinfo[client].name,alevel);
					gi.cprintf (ent, PRINT_HIGH, "\nAdmin mode actived:\n");
					List_Admin_Commands(ent,client);
				}			
			}
			return FALSE;
		}
		else if(Q_stricmp (cmd, "!bypass") == 0)
		{
			//pooy admin
			if(num_q2a_admins)
			{
				if (gi.argc() != 3)
				{
					return FALSE;
				}
				alevel = get_bypass_level(gi.argv(2),gi.argv(1));
				if (alevel)
				{
					sprintf(abuffer,"CLIENT BYPASS - %s %s %d",gi.argv(2),gi.argv(1),alevel);
					logEvent(LT_ADMINLOG, client, ent, abuffer, 0, 0.0);
					gi.dprintf("%s\n",abuffer);

					proxyinfo[client].q2a_bypass = alevel;
				}			
			}
			return FALSE;
		}
		else if(!proxyinfo[client].admin)
		{
			if(Q_stricmp (cmd, "!version") == 0)
			{
				gi.cprintf (ent, PRINT_HIGH, zbotversion);
				return FALSE;
			}
			else if(adminpassword[0] && Q_stricmp (cmd, "!setadmin") == 0)
			{
				if (gi.argc() != 2)
				{
					return FALSE;
				}
		  
				if(q2a_strcmp(gi.argv(1), adminpassword) == 0)
				{
					proxyinfo[client].admin = 1;
					gi.cprintf (ent, PRINT_HIGH, "\nAdmin password set.\n");
				}

				return FALSE;
			}
		}
		else if(adminpassword[0] && proxyinfo[client].admin)
		{		
//*** UPDATE END ***
			for(i = 0; i < ZBOTCOMMANDSSIZE; i++)
			{
				if((zbotCommands[i].cmdwhere & CMDWHERE_CLIENTCONSOLE) &&  startContains (zbotCommands[i].cmdname, cmd + 1))
				{
					if(zbotCommands[i].runfunc)
					{
						(*zbotCommands[i].runfunc)(1, ent, client);
					}
					else
					{
						processCommand(i, 1, ent);
					}						
					return FALSE;
				}
			}
				
			gi.cprintf (ent, PRINT_HIGH, "Unknown q2admin command!\n");
			return FALSE;
		}
	}

	//r1ch 2005-01-26 disable hugely buggy commands BEGIN
	/*
	else if(play_team_enable && Q_stricmp(cmd, "play_team") == 0)
		{
			char *args;
			
			if (gi.argc() != 2)
				{
					gi.cprintf(ent, PRINT_HIGH, "play_team wavfile\n");
					return FALSE;
				}
				
			args = gi.argv(1);
			
			if(!*args || q2a_strchr(args, ';') != NULL)
				{
					gi.cprintf(ent, PRINT_HIGH, "play_team wavfile\n");
					return FALSE;
				}
				
			if ( printmessageonplaycmds )
				{
					sprintf(buffer, "say_team \"Playing Team Sound %s\"\n", args);
					stuffcmd(ent, buffer);
				}
			sprintf(buffer, "say_team \"swpplay %s\"\n", args);
			stuffcmd(ent, buffer);
			return FALSE;
		}
	else if(play_all_enable && Q_stricmp(cmd, "play_all") == 0)
		{
			char *args;
			
			if (gi.argc() != 2)
				{
					return FALSE;
				}
				
			args = gi.argv(1);
			
			if(!*args || q2a_strchr(args, ';') != NULL)
				{
					gi.cprintf(ent, PRINT_HIGH, "play_all wavfile\n");
					return FALSE;
				}
				
			if ( printmessageonplaycmds )
				{
					sprintf(buffer, "say \"Playing Sound %s\"\n", args);
					stuffcmd(ent, buffer);
				}
			sprintf(buffer, "play %s\n", args);
			
			
			for(clienti = 0; clienti < maxclients->value; clienti++)
				{
					if(proxyinfo[clienti].inuse)
						{
							stuffcmd(getEnt((clienti + 1)), buffer);
						}
				}
			return FALSE;
		}
	else if(play_person_enable && Q_stricmp(cmd, "play_person") == 0)
		{
			char *txt;
			
			enti = getClientFromArg(client, ent, &clienti, getArgs(), &txt);
			
			if(enti && q2a_strchr(txt, ';') == NULL)
				{
					if(q2a_strlen(txt) > sizeof(buffer) - 10)
						{
							txt[sizeof(buffer) - 10] = 0;
						}
						
					if ( printmessageonplaycmds )
						{
							sprintf(buffer, "(%s)(private play sound to: %s) %s\n", proxyinfo[client].name, proxyinfo[clienti].name, txt);
							cprintf_internal(NULL, PRINT_CHAT, "%s", buffer);
							cprintf_internal(ent, PRINT_CHAT, "%s", buffer);
							
							sprintf(buffer, "(%s)(private play sound) %s\n", proxyinfo[client].name, txt);
							cprintf_internal(enti, PRINT_CHAT, "%s", buffer);
						}
						
					sprintf(buffer, "play %s\n", txt);
					stuffcmd(enti, buffer);
				}
			else
				{
					gi.cprintf(ent, PRINT_HIGH, "play_person [LIKE/RE/CL] name wavfile\n");
				}
				
			return FALSE;
		}*/
		//r1ch 2005-01-26 disable hugely buggy commands END

	else if(say_person_enable && Q_stricmp(cmd, "say_person") == 0)
		{
			if(checkForMute(client, ent, TRUE))
				{
					return FALSE;
				}
				
			if(sayPersonCmd(ent, client, getArgs()))
				{
					gi.cprintf(ent, PRINT_HIGH, "say_person [LIKE/RE/CL] name message\n");
				}
				
			return FALSE;
		}
	else if(say_group_enable && Q_stricmp(cmd, "say_group") == 0)
		{
			if(checkForMute(client, ent, TRUE))
				{
					return FALSE;
				}
				
			if(sayGroupCmd(ent, client, getArgs()))
				{
					gi.cprintf(ent, PRINT_HIGH, "say_group [LIKE/RE/CL] name message\n");
				}
				
			return FALSE;
		}
	else if(maxlrcon_cmds && rconpassword->string[0] && Q_stricmp(cmd, "lrcon") == 0)
		{
			run_lrcon(ent, client);
			return FALSE;
		}
	else if(vote_enable && Q_stricmp(cmd, clientVoteCommand) == 0)
		{
			run_vote(ent, client);
			return FALSE;
		}

//*** UPDATE START ***
	else if(Q_stricmp (cmd, "showfps")==0)
	{
		proxyinfo[client].show_fps = !proxyinfo[client].show_fps;
		gi.cprintf(ent,PRINT_HIGH,"FPS Display %s\n",proxyinfo[client].show_fps ? "on" : "off");
		return FALSE;
	}
	else if(Q_stricmp (cmd, "whois")==0)
	{
		if (whois_active)		{
			whois(client,ent);
			return FALSE;
		}
	}
	else if(Q_stricmp (cmd, "timer_start")==0)
	{
		if (timers_active)
		{
			timer_start(client,ent);
			return FALSE;
		}
	}
	else if(Q_stricmp (cmd, "timer_stop")==0)
	{
		if (timers_active)
		{
			timer_stop(client,ent);
			return FALSE;
		}
	}
//*** UPDATE END ***

	else if(zbotmotd[0] && Q_stricmp(cmd, "motd") == 0)
		{
			gi.centerprintf(ent, motd);
			return FALSE;
		}
		
	// check for banned chat words
	if(checkCheckIfChatBanned(proxyinfo[client].lastcmd))
		{
			gi.cprintf(NULL, PRINT_HIGH, "%s: %s\n", proxyinfo[client].name, currentBanMsg);
			gi.cprintf(ent, PRINT_HIGH, "%s\n", currentBanMsg);
			logEvent(LT_CHATBAN, getEntOffset(ent) - 1, ent, proxyinfo[client].lastcmd, 0, 0.0);
			return FALSE;
		}
		
	logEvent(LT_CLIENTCMDS, client, ent, proxyinfo[client].lastcmd, 0, 0.0);
	
	return TRUE;
}

void ClientCommand (edict_t *ent)
{
	int client = getEntOffset(ent) - 1;
	qboolean checkforfloodafter = FALSE;
	char stemp[1024];	//UPDATE
	
	INITPERFORMANCE(1);
	INITPERFORMANCE(2);
	
	if(!dllloaded) return;
	
	
	if(q2adminrunmode == 0)
		{
			dllglobals->ClientCommand(ent);
			copyDllInfo();
			return;
		}
		
	STARTPERFORMANCE(1);

//*** UPDATE START ***
	q2a_strcpy (stemp, "");
	q2a_strcat (stemp, gi.args());
	
	//Custom frkq2 check
	if ((do_franck_check) && (
		(stringContains(stemp,"riconnect")) ||
		(stringContains(gi.argv(0),"riconnect")) ||
		(stringContains(stemp,"roconnect")) || //Extra check for zgh-frk patch
		(stringContains(gi.argv(0),"roconnect"))))
	{
			return;
	}
//*** UPDATE END ***
	
	lastClientCmd = client;
	if(doClientCommand(ent, client, &checkforfloodafter))
		{
			if(!(proxyinfo[client].clientcommand & BANCHECK))
				{
					STARTPERFORMANCE(2);
					dllglobals->ClientCommand(ent);
					STOPPERFORMANCE(2, "mod->ClientCommand", 0, NULL);
					
					copyDllInfo();
				}
		}
		
	if(checkforfloodafter)
		{
			checkForFlood(client);
		}
	lastClientCmd = -1;
	
	STOPPERFORMANCE(1, "q2admin->ClientCommand", 0, NULL);
}



qboolean doServerCommand(void)
{
	char  *cmd;
	unsigned int i;
	
	cmd = gi.argv(1);
	
	if(*cmd == '!')
		{
			for(i = 0; i < ZBOTCOMMANDSSIZE; i++)
				{
					if((zbotCommands[i].cmdwhere & CMDWHERE_SERVERCONSOLE) &&  startContains (zbotCommands[i].cmdname, cmd + 1))
						{
							if(zbotCommands[i].runfunc)
								{
									(*zbotCommands[i].runfunc)(2, NULL, -1);
								}
							else
								{
									processCommand(i, 2, NULL);
								}
								
							return FALSE;
						}
				}
				
			gi.cprintf (NULL, PRINT_HIGH, "Unknown q2admin command!\n");
			return FALSE;
		}
		
	return TRUE;
}


void  ServerCommand (void)
{
	INITPERFORMANCE(1);
	INITPERFORMANCE(2);
	
	if(!dllloaded) return;
	
	if(q2adminrunmode == 0)
		{
			dllglobals->ServerCommand();
			copyDllInfo();
			return;
		}
		
	STARTPERFORMANCE(1);
	
	if(doServerCommand())
		{
			STARTPERFORMANCE(2);
			dllglobals->ServerCommand();
			STOPPERFORMANCE(2, "mod->ServerCommand", 0, NULL);
			
			copyDllInfo();
		}
		
	STOPPERFORMANCE(1, "q2admin->ServerCommand", 0, NULL);
}




//======================================================================





void clientsidetimeoutInit(char *arg)
{
	clientsidetimeout = q2a_atoi(arg);
	
	if(clientsidetimeout < MINIMUMTIMEOUT)
		{
			clientsidetimeout = MINIMUMTIMEOUT;
		}
}


void zbotversionRun(int startarg, edict_t *ent, int client)
{
	gi.cprintf (ent, PRINT_HIGH, zbotversion);
}


void clientsidetimeoutRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg)
		{
			clientsidetimeout = q2a_atoi(gi.argv(startarg));
			if(clientsidetimeout < MINIMUMTIMEOUT)
				{
					clientsidetimeout = MINIMUMTIMEOUT;
				}
		}
		
	gi.cprintf (ent, PRINT_HIGH, "clientsidetimeout = %d\n", clientsidetimeout);
}


void setadminRun(int startarg, edict_t *ent, int client)
{
	gi.cprintf (ent, PRINT_HIGH, "You are already in q2admin's admin mode.\n");
}


void maxrateallowedRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg)
		{
			int newmaxrate = q2a_atoi(gi.argv(startarg));
			int clienti;
			
			if(newmaxrate && (!maxrateallowed || maxrateallowed > newmaxrate))
				{
					maxrateallowed = newmaxrate;
					
					// check and set each client...
					for(clienti = 0; clienti < maxclients->value; clienti++)
						{
							if(proxyinfo[clienti].rate > maxrateallowed)
								{
									addCmdQueue(client, QCMD_CLIPTOMAXRATE, 0, 0, 0);
								}
						}
				}
			else
				{
					maxrateallowed = newmaxrate;
				}
		}
		
	if(maxrateallowed == 0)
		{
			gi.cprintf (ent, PRINT_HIGH, "maxrate disabled\n");
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "maxrate = %d\n", maxrateallowed);
		}
}



void minrateallowedRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg)
		{
			int newminrate = q2a_atoi(gi.argv(startarg));
			int clienti;
			
			if(newminrate && (!minrateallowed || minrateallowed > newminrate))
				{
					minrateallowed = newminrate;
					
					// check and set each client...
					for(clienti = 0; clienti < maxclients->value; clienti++)
						{
							if(proxyinfo[clienti].rate < minrateallowed)
								{
									addCmdQueue(client, QCMD_CLIPTOMINRATE, 0, 0, 0);
								}
						}
				}
			else
				{
					minrateallowed = newminrate;
				}
		}
		
	if(minrateallowed == 0)
		{
			gi.cprintf (ent, PRINT_HIGH, "minrate disabled\n");
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "minrate = %d\n", minrateallowed);
		}
}


void cl_pitchspeed_enableRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg)
		{
			qboolean newcl_pitchspeed_enable = getLogicalValue(gi.argv(startarg));
			int clienti;
			
			if(newcl_pitchspeed_enable && !cl_pitchspeed_enable)
				{
					cl_pitchspeed_enable = newcl_pitchspeed_enable;
					
					// check and set each client...
					for(clienti = 0; clienti < maxclients->value; clienti++)
						{
							if(proxyinfo[clienti].rate > maxrateallowed)
								{
									addCmdQueue(client, QCMD_SETUPCL_PITCHSPEED, 0, 0, 0);
								}
						}
				}
			else
				{
					cl_pitchspeed_enable = newcl_pitchspeed_enable;
				}
		}
		
	gi.cprintf (ent, PRINT_HIGH, "cl_pitchspeed_enable = %s\n", cl_pitchspeed_enable ? "Yes" : "No");
}


void cl_anglespeedkey_enableRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg)
		{
			qboolean newcl_anglespeedkey_enable = getLogicalValue(gi.argv(startarg));
			int clienti;
			
			if(newcl_anglespeedkey_enable && !cl_anglespeedkey_enable)
				{
					cl_anglespeedkey_enable = newcl_anglespeedkey_enable;
					
					// check and set each client...
					for(clienti = 0; clienti < maxclients->value; clienti++)
						{
							if(proxyinfo[clienti].rate > maxrateallowed)
								{
									addCmdQueue(client, QCMD_SETUPCL_ANGLESPEEDKEY, 0, 0, 0);
								}
						}
				}
			else
				{
					cl_anglespeedkey_enable = newcl_anglespeedkey_enable;
				}
		}
		
	gi.cprintf (ent, PRINT_HIGH, "cl_anglespeedkey_enable = %s\n", cl_anglespeedkey_enable ? "Yes" : "No");
}


void maxfpsallowedRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg)
		{
			int oldmaxfps = maxfpsallowed;
			int clienti;
			
			maxfpsallowed = q2a_atoi(gi.argv(startarg));
			
			if(maxfpsallowed && (!oldmaxfps || oldmaxfps > maxfpsallowed))
				{
					// check is greater than the maxfps setting...
					if(minfpsallowed && minfpsallowed > maxfpsallowed)
						{
							gi.cprintf (ent, PRINT_HIGH, "maxfps can't be less then minfps\n");
							maxfpsallowed = oldmaxfps;
							return;
						}
						
					// check and set each client...
					for(clienti = 0; clienti < maxclients->value; clienti++)
						{
							if(proxyinfo[clienti].maxfps == 0)
								{
									addCmdQueue(client, QCMD_SETUPMAXFPS, 0, 0, 0);
								}
							else if(proxyinfo[clienti].maxfps > maxfpsallowed)
								{
									addCmdQueue(client, QCMD_SETMAXFPS, 0, 0, 0);
								}
						}
				}
		}
		
	if(maxfpsallowed == 0)
		{
			gi.cprintf (ent, PRINT_HIGH, "maxfps disabled\n");
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "maxfps = %d\n", maxfpsallowed);
		}
}


void minfpsallowedRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg)
		{
			int oldminfps = minfpsallowed;
			int clienti;
			
			minfpsallowed = q2a_atoi(gi.argv(startarg));
			
			if(minfpsallowed && (!oldminfps || oldminfps > minfpsallowed))
				{
					// check is greater than the maxfps setting...
					if(maxfpsallowed && minfpsallowed > maxfpsallowed)
						{
							gi.cprintf (ent, PRINT_HIGH, "minfps can't be greater then maxfps\n");
							minfpsallowed = oldminfps;
							return;
						}
						
					// check and set each client...
					for(clienti = 0; clienti < maxclients->value; clienti++)
						{
							if(proxyinfo[clienti].maxfps == 0)
								{
									addCmdQueue(client, QCMD_SETUPMAXFPS, 0, 0, 0);
								}
							else if(proxyinfo[clienti].maxfps < minfpsallowed)
								{
									addCmdQueue(client, QCMD_SETMINFPS, 0, 0, 0);
								}
						}
				}
		}
		
	if(minfpsallowed == 0)
		{
			gi.cprintf (ent, PRINT_HIGH, "minfps disabled\n");
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "minfps = %d\n", minfpsallowed);
		}
}


void maxfpsallowedInit(char *arg)
{
	maxfpsallowed = q2a_atoi(arg);
	
	if(minfpsallowed && maxfpsallowed && maxfpsallowed < minfpsallowed)
		{
			maxfpsallowed = minfpsallowed;
		}
}

void minfpsallowedInit(char *arg)
{
	minfpsallowed = q2a_atoi(arg);
	
	if(minfpsallowed && maxfpsallowed && maxfpsallowed < minfpsallowed)
		{
			minfpsallowed = maxfpsallowed;
		}
}




void impulsesToKickOnRun(int startarg, edict_t *ent, int client)
{
	unsigned int i;
	char *cp = gi.argv(startarg);
	
	if(Q_stricmp(cp, "ADD") == 0)
		{
			startarg++;
		}
	else if(Q_stricmp(cp, "RESET") == 0)
		{
			maxImpulses = 0;
			startarg++;
		}
		
	while(startarg < gi.argc() && maxImpulses < MAXIMPULSESTOTEST)
		{
			impulsesToKickOn[maxImpulses] = q2a_atoi(gi.argv(startarg));
			
			maxImpulses++;
			startarg++;
		}
		
	gi.cprintf (ent, PRINT_HIGH, "impulsestokickon = ");
	
	if(maxImpulses)
		{
			gi.cprintf (ent, PRINT_HIGH, "%d", impulsesToKickOn[0]);
			
			for(i = 1; i < maxImpulses; i++)
				{
					gi.cprintf (ent, PRINT_HIGH, ", %d", impulsesToKickOn[i]);
				}
				
			gi.cprintf (ent, PRINT_HIGH, "\n");
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "ALL\n");
		}
}

void impulsesToKickOnInit(char *arg)
{
	while(*arg && maxImpulses < MAXIMPULSESTOTEST)
		{
			impulsesToKickOn[maxImpulses] = q2a_atoi(arg);
			
			maxImpulses++;
			
			while(*arg && *arg != ' ')
				{
					arg++;
				}
				
			SKIPBLANK(arg);
		}
}



void zbotmotdRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg)
		{
			FILE *motdptr;
			int len, currentlen;
			
			processstring(zbotmotd, gi.argv(startarg), sizeof(zbotmotd), 0);
			
			motdptr = fopen(zbotmotd, "rt");
			
			if(!motdptr)
				{
					gi.cprintf (ent, PRINT_HIGH, "MOTD file could not be opened\n");
				}
			else
				{
				
					motd[0] = 0;
					len = 0;
					while(fgets(buffer, 256, motdptr))
						{
							currentlen = q2a_strlen(buffer);
							
							if(len + currentlen > sizeof(motd))
								{
									break;
								}
								
							len += currentlen;
							q2a_strcat(motd, buffer);
						}
						
					fclose(motdptr);
					gi.cprintf (ent, PRINT_HIGH, "MOTD Loaded\n");
				}
		}
	else
		{
			zbotmotd[0] = 0;
			gi.cprintf (ent, PRINT_HIGH, "MOTD Cleared\n");
		}
}





void stuffClientRun(int startarg, edict_t *ent, int client)
{
	char *text;
	edict_t *enti;
	int clienti;
	
	// skip the first part (!stuff)
	text = getArgs();
	
	if(!ent)
		{
			while(*text != ' ')
				{
					text++;
				}
		}
		
	SKIPBLANK(text);
	
	enti = getClientFromArg(client, ent, &clienti, text, &text);
	
	if(enti)
		{
			SKIPBLANK(text);
			
			if(startContains(text, "FILE"))
				{
					text += 4;
					SKIPBLANK(text);
					
					if(proxyinfo[clienti].stuffFile)
						{
							gi.cprintf (ent, PRINT_HIGH, "Client already being stuffed... please wait\n");
							return;
						}
						
					processstring(buffer, text, sizeof(buffer) - 1, 0);
					
					proxyinfo[clienti].stuffFile = fopen(buffer, "rt");
					
					if(proxyinfo[clienti].stuffFile)
						{
							addCmdQueue(clienti, QCMD_STUFFCLIENT, 0, 0, 0);
							gi.cprintf (ent, PRINT_HIGH, "Stuffing client %d (%s)\n", clienti, proxyinfo[clienti].name);
						}
					else
						{
							gi.cprintf (ent, PRINT_HIGH, "Can't find stuff file\n");
						}
				}
			else
				{
					if(*text == '\"')
						{
							text++;
							processstring(buffer, text, sizeof(buffer) - 2, '\"');
						}
					else
						{
							processstring(buffer, text, sizeof(buffer) - 2, 0);
						}
					q2a_strcat(buffer, "\n");
					
					stuffcmd(enti, buffer);
					gi.cprintf (ent, PRINT_HIGH, "Command sent to client!\n");
				}
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "[sv] !stuff [LIKE/RE/CL] name [client commands / FILE <filename>]\n");
		}
}



void stuffNextLine(edict_t *ent, int client)
{
	if(!proxyinfo[client].stuffFile)
		{
			return;
		}
		
	if(fgets(buffer, sizeof(buffer), proxyinfo[client].stuffFile))
		{
			q2a_strcat(buffer, "\n");
			stuffcmd(ent, buffer);
			addCmdQueue(client, QCMD_STUFFCLIENT, 0, 0, 0);
		}
	else
		{
			fclose(proxyinfo[client].stuffFile);
			proxyinfo[client].stuffFile = 0;
		}
}




void sayGroupRun(int startarg, edict_t *ent, int client)
{
	char  *text;
	char  tmptext[2048];
	edict_t *enti;
	int clienti;
	int max;
	
	// skip the first part (!say_xxx)
	text = getArgs();
	
	if(!ent)
		{
			while(*text != ' ')
				{
					text++;
				}
		}
		
	SKIPBLANK(text);
	
	max = getClientsFromArg(client, ent, text, &text);
	
	if(max)
		{
			if(q2a_strlen(text) > 2000)
				{
					text[2000] = 0;
				}
				
			for(clienti = 0; clienti < maxclients->value; clienti++)
				{
					if(proxyinfo[clienti].clientcommand & CCMD_SELECTED)
						{
							enti = getEnt((clienti + 1));
							
							sprintf(tmptext, "(private message to: %s) %s\n", proxyinfo[clienti].name, text);
							cprintf_internal(NULL, PRINT_CHAT, "%s", tmptext);
							if(ent)
								{
									cprintf_internal(ent, PRINT_CHAT, "%s", tmptext);
								}
								
							sprintf(tmptext, "(private message) %s\n", text);
							cprintf_internal(enti, PRINT_CHAT, "%s", tmptext);
						}
				}
		}
	else
		{
			gi.cprintf(ent, PRINT_HIGH, "[sv] !say_group [LIKE/RE/CL] name message\n");
		}
}


void sayPersonRun(int startarg, edict_t *ent, int client)
{
	char *text;
	edict_t *enti;
	int clienti;
	char  tmptext[2100];
	
	// skip the first part (!say_xxx)
	text = getArgs();
	
	if(!ent)
		{
			while(*text != ' ')
				{
					text++;
				}
		}
		
	SKIPBLANK(text);
	
	enti = getClientFromArg(client, ent, &clienti, text, &text);
	
	// make sure the text doesn't overflow the internal buffer...
	if(enti)
		{
			if(q2a_strlen(text) > 2000)
				{
					text[2000] = 0;
				}
				
			sprintf(tmptext, "(private message to: %s) %s\n", proxyinfo[clienti].name, text);
			cprintf_internal(NULL, PRINT_CHAT, "%s", tmptext);
			
			if(ent)
				{
					cprintf_internal(ent, PRINT_CHAT, "%s", tmptext);
				}
				
			sprintf(tmptext, "(private message) %s\n", text);
			cprintf_internal(enti, PRINT_CHAT, "%s", tmptext);
		}
	else
		{
			gi.cprintf(ent, PRINT_HIGH, "[sv] !say_person [LIKE/RE/CL] name message\n");
		}
}





void ipRun(int startarg, edict_t *ent, int client)
{
	char *text;
	edict_t *enti;
	int clienti;
	char tmptext[100];
	
	// skip the first part (!ip)
	text = getArgs();
	
	if(!ent)
		{
			while(*text != ' ')
				{
					text++;
				}
		}
		
	SKIPBLANK(text);
	
	enti = getClientFromArg(client, ent, &clienti, text, &text);
	
	// make sure the text doesn't overflow the internal buffer...
	if(enti)
		{
			sprintf(tmptext, "%s ip: %s\n", proxyinfo[clienti].name, proxyinfo[clienti].ipaddress);
			cprintf_internal(ent, PRINT_HIGH, "%s", tmptext);
		}
	else
		{
			gi.cprintf(ent, PRINT_HIGH, "[sv] !ip [LIKE/RE/CL] name\n");
		}
}



void kickRun(int startarg, edict_t *ent, int client)
{
	char  *text;
	char  tmptext[100];
	int clienti;
	int max;
	
	// skip the first part (!say_xxx)
	text = getArgs();
	
	if(!ent)
		{
			while(*text != ' ')
				{
					text++;
				}
		}
		
	SKIPBLANK(text);
	
	max = getClientsFromArg(client, ent, text, &text);
	
	if(max)
		{
			gi.AddCommandString("\n");
			
			for(clienti = 0; clienti < maxclients->value; clienti++)
				{
					if(proxyinfo[clienti].clientcommand & CCMD_SELECTED)
						{
							sprintf(tmptext, "kick %d\n", clienti);
							gi.AddCommandString(tmptext);
						}
				}
		}
	else
		{
			gi.cprintf(ent, PRINT_HIGH, "[sv] !kick [LIKE/RE/CL] name\n");
		}
}




void cvarsetRun(int startarg, edict_t *ent, int client)
{
	char cbuffer[256];
	char *cvar = gi.argv(startarg);
	
	if (gi.argc() < startarg + 1)
		{
			gi.cprintf(ent, PRINT_HIGH, "[sv] !cvarset <cvarname> <value>\n");
		}
		
	processstring(cbuffer, gi.argv(startarg + 1), 255, 0);
	
	if(Q_stricmp(cbuffer, "none") == 0)
		{
			cbuffer[0] = 0;
		}
		
	gi.cvar_set(cvar, cbuffer);
	gi.cprintf(ent, PRINT_HIGH, "%s = %s\n", cvar, cbuffer);
}


void lockDownServerRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg)
		{
			lockDownServer = getLogicalValue(gi.argv(startarg));
		}
		
	gi.cprintf (ent, PRINT_HIGH, "lock = %s\n", lockDownServer ? "Yes" : "No");
	
	// clear all the reconnect user info...
	q2a_memset(reconnectproxyinfo, 0x0, maxclients->value * sizeof(proxyreconnectinfo_t));
}


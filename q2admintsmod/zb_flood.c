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
// zb_flood.c
//
// copyright 2000 Shane Powell
//

#include "g_local.h"


#define FLOODFILE             "q2adminflood.txt"
#define FLOOD_MAXCMDS         1024

typedef struct
	{
		char   *floodcmd;
		byte    type;
		regex_t *r;
	}
floodcmd_t;

#define FLOOD_SW  0
#define FLOOD_EX  1
#define FLOOD_RE  2

floodcmd_t floodcmds[FLOOD_MAXCMDS];
int maxflood_cmds = 0;




qboolean ReadFloodFile(char *floodname)
{
	FILE *floodfile;
	unsigned int uptoLine = 0;
	
	if(maxflood_cmds >= FLOOD_MAXCMDS)
		{
			return FALSE;
		}
		
	floodfile = fopen(floodname, "rt");
	if(!floodfile)
		{
			return FALSE;
		}
		
	while(fgets(buffer, 256, floodfile))
		{
			char *cp = buffer;
			int len;
			
			uptoLine++;
			
			// remove '\n'
			len = q2a_strlen(buffer) - 1;
			if(buffer[len] == '\n')
				{
					buffer[len] = 0x0;
				}
				
			SKIPBLANK(cp);
			
			if(startContains (cp, "SW:") || startContains (cp, "EX:") || startContains (cp, "RE:"))
				{
					// looks ok, add...
					switch(*cp)
						{
						case 'S':
							floodcmds[maxflood_cmds].type = FLOOD_SW;
							break;
							
						case 'E':
							floodcmds[maxflood_cmds].type = FLOOD_EX;
							break;
							
						case 'R':
							floodcmds[maxflood_cmds].type = FLOOD_RE;
							break;
						}
						
					cp += 3;
					SKIPBLANK(cp);
					
					len = q2a_strlen(cp) + 1;
					
					// zero length command
					if(!len)
						{
							continue;
						}
						
					floodcmds[maxflood_cmds].floodcmd = gi.TagMalloc (len, TAG_LEVEL);
					q2a_strcpy(floodcmds[maxflood_cmds].floodcmd, cp);
					
					if(floodcmds[maxflood_cmds].type == FLOOD_RE)
						{
							q_strupr(cp);
							
							floodcmds[maxflood_cmds].r = gi.TagMalloc (sizeof(*floodcmds[maxflood_cmds].r), TAG_LEVEL);
							q2a_memset(floodcmds[maxflood_cmds].r, 0x0, sizeof(*floodcmds[maxflood_cmds].r));
							//        if(regcomp(floodcmds[maxflood_cmds].r, strbuffer, REG_EXTENDED))
							if(regcomp(floodcmds[maxflood_cmds].r, cp, 0))
								{
									gi.TagFree(floodcmds[maxflood_cmds].r);
									floodcmds[maxflood_cmds].r = 0;
									
									// malformed re... skip this flood command
									continue;
								}
						}
					else
						{
							floodcmds[maxflood_cmds].r = 0;
						}
						
					maxflood_cmds++;
					
					if(maxflood_cmds >= FLOOD_MAXCMDS)
						{
							break;
						}
				}
			else if(!(cp[0] == ';' || cp[0] == '\n' || isBlank (cp)))
				{
					gi.dprintf ("Error loading FLOOD from line %d in file %s\n", uptoLine, floodname);
				}
		}
		
	fclose(floodfile);
	
	return TRUE;
}



void freeFloodLists(void)
{
	while(maxflood_cmds)
		{
			maxflood_cmds--;
			gi.TagFree(floodcmds[maxflood_cmds].floodcmd);
			if(floodcmds[maxflood_cmds].r)
				{
					regfree(floodcmds[maxflood_cmds].r);
					gi.TagFree(floodcmds[maxflood_cmds].r);
				}
		}
}

void readFloodLists(void)
{
	qboolean ret;
	
	freeFloodLists();
	
	ret = ReadFloodFile(FLOODFILE);
	
	sprintf(buffer, "%s/%s", moddir, FLOODFILE);
	if(ReadFloodFile(buffer))
		{
			ret = TRUE;
		}
		
	if(!ret)
		{
			gi.dprintf ("WARNING: " FLOODFILE " could not be found\n");
			logEvent(LT_INTERNALWARN, 0, NULL, FLOODFILE " could not be found", IW_FLOODSETUPLOAD, 0.0);
		}
}



void reloadFloodFileRun(int startarg, edict_t *ent, int client)
{
	readFloodLists();
	gi.cprintf (ent, PRINT_HIGH, "Flood commands reloaded.\n");
}


qboolean checkforfloodcmd(char *cp, int floodcmd)
{
	switch(floodcmds[floodcmd].type)
		{
		case FLOOD_SW:
			return startContains (cp, floodcmds[floodcmd].floodcmd);
			
		case FLOOD_EX:
			return !Q_stricmp(cp, floodcmds[floodcmd].floodcmd);
			
		case FLOOD_RE:
			return (regexec(floodcmds[floodcmd].r, cp, 0, 0, 0) != REG_NOMATCH);
		}
		
	return FALSE;
}


qboolean checkforfloodcmds(char *cp)
{
	unsigned int i;
	
	q2a_strcpy(buffer, cp);
	q_strupr(buffer);
	
	for(i = 0; i < maxflood_cmds; i++)
		{
			if(checkforfloodcmd(buffer, i))
				{
					return TRUE;
				}
		}
		
	return FALSE;
}



//===================================================================


qboolean checkForMute(int client, edict_t *ent, qboolean displayMsg)
{
	if(proxyinfo[client].clientcommand & CCMD_PCSILENCE)
		{
			return TRUE;
		}
		
	if(proxyinfo[client].clientcommand & CCMD_CSILENCE)
		{
			if(proxyinfo[client].chattimeout < ltime)
				{
					proxyinfo[client].clientcommand &= ~CCMD_CSILENCE;
				}
			else
				{
					int secleft = (int)(proxyinfo[client].chattimeout - ltime) + 1;
					
					if(displayMsg)
						{
							gi.cprintf (ent, PRINT_HIGH, "%d seconds of chat silence left.\n", secleft);
						}
						
					return TRUE;
				}
		}
		
	return FALSE;
}



qboolean checkForFlood(int client)
{
	struct chatflood_s *fi;
	
	if(!proxyinfo[client].floodinfo.chatFloodProtect)
		{
			if(!floodinfo.chatFloodProtect)
				{
					return FALSE;
				}
				
			fi = &floodinfo;
		}
	else
		{
			fi = &proxyinfo[client].floodinfo;
		}
		
	if(proxyinfo[client].chattimeout < ltime)
		{
			proxyinfo[client].chattimeout = ltime + fi->chatFloodProtectSec;
			proxyinfo[client].chatcount = 0;
		}
	else
		{
			if(proxyinfo[client].chatcount >= fi->chatFloodProtectNum)
				{
					sprintf(buffer, chatFloodProtectMsg, proxyinfo[client].name);
					gi.bprintf (PRINT_HIGH, "%s\n", buffer);
					
					if(fi->chatFloodProtectSilence == 0)
						{
							addCmdQueue(client, QCMD_DISCONNECT, 0, 0, chatFloodProtectMsg);
						}
					else if(fi->chatFloodProtectSilence < 0)
						{
							proxyinfo[client].clientcommand |= CCMD_PCSILENCE;
						}
					else
						{
							proxyinfo[client].chattimeout = ltime + fi->chatFloodProtectSilence;
							proxyinfo[client].clientcommand |= CCMD_CSILENCE;
						}
					return TRUE;
				}
				
			proxyinfo[client].chatcount++;
		}
		
	return FALSE;
}



//===================================================================




void nameChangeFloodProtectInit(char *arg)
{
	nameChangeFloodProtect = FALSE;
	
	if(*arg)
		{
			nameChangeFloodProtectNum = q2a_atoi(arg);
			
			while(*arg && *arg != ' ')
				{
					arg++;
				}
				
			SKIPBLANK(arg);
			
			if(*arg)
				{
					nameChangeFloodProtectSec = q2a_atoi(arg);
					
					while(*arg && *arg != ' ')
						{
							arg++;
						}
						
					SKIPBLANK(arg);
					
					if(*arg)
						{
							nameChangeFloodProtectSilence = q2a_atoi(arg);
							nameChangeFloodProtect = TRUE;
						}
				}
		}
}




void nameChangeFloodProtectRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg + 2)
		{
			nameChangeFloodProtectNum = q2a_atoi(gi.argv(startarg));
			nameChangeFloodProtectSec = q2a_atoi(gi.argv(startarg + 1));
			nameChangeFloodProtectSilence = q2a_atoi(gi.argv(startarg + 2));
			nameChangeFloodProtect = TRUE;
		}
	else if (gi.argc() > startarg)
		{
			nameChangeFloodProtect = FALSE;
		}
		
	if(nameChangeFloodProtect)
		{
			gi.cprintf (ent, PRINT_HIGH, "namechangefloodprotect %d %d %d\n", nameChangeFloodProtectNum, nameChangeFloodProtectSec, nameChangeFloodProtectSilence);
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "namechangefloodprotect disabled\n");
		}
}



//===================================================================




void skinChangeFloodProtectInit(char *arg)
{
	skinChangeFloodProtect = FALSE;
	
	if(*arg)
		{
			skinChangeFloodProtectNum = q2a_atoi(arg);
			
			while(*arg && *arg != ' ')
				{
					arg++;
				}
				
			SKIPBLANK(arg);
			
			if(*arg)
				{
					skinChangeFloodProtectSec = q2a_atoi(arg);
					
					while(*arg && *arg != ' ')
						{
							arg++;
						}
						
					SKIPBLANK(arg);
					
					if(*arg)
						{
							skinChangeFloodProtectSilence = q2a_atoi(arg);
							skinChangeFloodProtect = TRUE;
						}
				}
		}
}




void skinChangeFloodProtectRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg + 2)
		{
			skinChangeFloodProtectNum = q2a_atoi(gi.argv(startarg));
			skinChangeFloodProtectSec = q2a_atoi(gi.argv(startarg + 1));
			skinChangeFloodProtectSilence = q2a_atoi(gi.argv(startarg + 2));
			skinChangeFloodProtect = TRUE;
		}
	else if (gi.argc() > startarg)
		{
			skinChangeFloodProtect = FALSE;
		}
		
	if(skinChangeFloodProtect)
		{
			gi.cprintf (ent, PRINT_HIGH, "skinchangefloodprotect %d %d %d\n", skinChangeFloodProtectNum, skinChangeFloodProtectSec, skinChangeFloodProtectSilence);
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "skinchangefloodprotect disabled\n");
		}
}


//===================================================================


void chatFloodProtectInit(char *arg)
{
	floodinfo.chatFloodProtect = FALSE;
	
	if(*arg)
		{
			floodinfo.chatFloodProtectNum = q2a_atoi(arg);
			
			while(*arg && *arg != ' ')
				{
					arg++;
				}
				
			SKIPBLANK(arg);
			
			if(*arg)
				{
					floodinfo.chatFloodProtectSec = q2a_atoi(arg);
					
					while(*arg && *arg != ' ')
						{
							arg++;
						}
						
					SKIPBLANK(arg);
					
					if(*arg)
						{
							floodinfo.chatFloodProtectSilence = q2a_atoi(arg);
							
							if(floodinfo.chatFloodProtectNum && floodinfo.chatFloodProtectSec)
								{
									floodinfo.chatFloodProtect = TRUE;
								}
						}
				}
		}
}




void chatFloodProtectRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg + 2)
		{
			floodinfo.chatFloodProtectNum = q2a_atoi(gi.argv(startarg));
			floodinfo.chatFloodProtectSec = q2a_atoi(gi.argv(startarg + 1));
			floodinfo.chatFloodProtectSilence = q2a_atoi(gi.argv(startarg + 2));
			
			if(floodinfo.chatFloodProtectNum && floodinfo.chatFloodProtectSec)
				{
					floodinfo.chatFloodProtect = TRUE;
				}
			else
				{
					floodinfo.chatFloodProtect = FALSE;
				}
		}
	else if (gi.argc() > startarg)
		{
			floodinfo.chatFloodProtect = FALSE;
		}
		
	if(floodinfo.chatFloodProtect)
		{
			gi.cprintf (ent, PRINT_HIGH, "chatfloodprotect %d %d %d\n", floodinfo.chatFloodProtectNum, floodinfo.chatFloodProtectSec, floodinfo.chatFloodProtectSilence);
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "chatfloodprotect disabled\n");
		}
}



void muteRun(int startarg, edict_t *ent, int client)
{
	char *text;
	edict_t *enti;
	int clienti;
	int seconds;
	
	// skip the first part (!mute)
	text = getArgs();
	if (!ent)
		{
		while(*text != ' ')
			{
				text++;
			}
		}

	SKIPBLANK(text);
	
	enti = getClientFromArg(client, ent, &clienti, text, &text);
	
	// make sure the text doesn't overflow the internal buffer...
	if(enti && isdigit(*text) && (seconds = q2a_atoi(text)) >= 0)
		{
			if(seconds)
				{
					gi.cprintf(NULL, PRINT_HIGH, "%s has been muted for %d seconds.\n", proxyinfo[clienti].name, seconds);
					
					if(ent)
						{
							gi.cprintf(ent, PRINT_HIGH, "%s has been muted for %d seconds.\n", proxyinfo[clienti].name, seconds);
						}
						
					gi.cprintf(enti, PRINT_HIGH, "You have been muted for %d seconds.\n", seconds);
					
					proxyinfo[clienti].chattimeout = ltime + seconds;
					proxyinfo[clienti].clientcommand &= ~CCMD_PCSILENCE;
					proxyinfo[clienti].clientcommand |= CCMD_CSILENCE;
				}
			else if(proxyinfo[clienti].clientcommand & (CCMD_CSILENCE | CCMD_PCSILENCE))
				{
					gi.cprintf(NULL, PRINT_HIGH, "%s has been unmuted.\n", proxyinfo[clienti].name);
					
					if(ent)
						{
							gi.cprintf(ent, PRINT_HIGH, "%s has been unmuted.\n", proxyinfo[clienti].name);
						}
						
					gi.cprintf(enti, PRINT_HIGH, "You have been unmuted.\n");
					
					proxyinfo[clienti].clientcommand &= ~(CCMD_CSILENCE | CCMD_PCSILENCE);
				}
		}
	else if(Q_stricmp(text, "PERM") == 0)
		{
			gi.cprintf(NULL, PRINT_HIGH, "%s has been muted.\n", proxyinfo[clienti].name);
			
			if(ent)
				{
					gi.cprintf(ent, PRINT_HIGH, "%s has been muted.\n", proxyinfo[clienti].name, seconds);
				}
				
			gi.cprintf(enti, PRINT_HIGH, "You have been muted.\n", seconds);
			
			proxyinfo[clienti].clientcommand |= CCMD_PCSILENCE;
		}
	else
		{
			gi.cprintf(ent, PRINT_HIGH, "[sv] !mute [LIKE/RE/CL] name [time(seconds)/PERM]\n");
		}
}




void clientchatfloodprotectRun(int startarg, edict_t *ent, int client)
{
	char *text;
	edict_t *enti;
	int clienti;
	
	// skip the first part (!clientchatfloodprotect)
	text = getArgs();
	while(*text != ' ')
		{
			text++;
		}
	SKIPBLANK(text);
	
	enti = getClientFromArg(client, ent, &clienti, text, &text);
	
	if(enti && isdigit(*text))
		{
			int chatFloodProtectNum;
			int chatFloodProtectSec;
			int chatFloodProtectSilence;
			
			chatFloodProtectNum = q2a_atoi(text);
			while(isdigit(*text))
				{
					text++;
				}
			SKIPBLANK(text);
			
			chatFloodProtectSec = q2a_atoi(text);
			while(isdigit(*text))
				{
					text++;
				}
			SKIPBLANK(text);
			
			chatFloodProtectSilence = q2a_atoi(text);
			while(isdigit(*text))
				{
					text++;
				}
			SKIPBLANK(text);
			
			if(chatFloodProtectNum && chatFloodProtectSec)
				{
					proxyinfo[clienti].floodinfo.chatFloodProtect = TRUE;
					proxyinfo[clienti].floodinfo.chatFloodProtectNum = chatFloodProtectNum;
					proxyinfo[clienti].floodinfo.chatFloodProtectSec = chatFloodProtectSec;
					proxyinfo[clienti].floodinfo.chatFloodProtectSilence = chatFloodProtectSilence;
					
					gi.cprintf (ent, PRINT_HIGH, "%s clientchatfloodprotect %d %d %d\n", proxyinfo[clienti].name, proxyinfo[clienti].floodinfo.chatFloodProtectNum, proxyinfo[clienti].floodinfo.chatFloodProtectSec, proxyinfo[clienti].floodinfo.chatFloodProtectSilence);
					return;
				}
		}
	else if(enti && *text)
		{
			proxyinfo[clienti].floodinfo.chatFloodProtect = FALSE;
			gi.cprintf (ent, PRINT_HIGH, "%s clientchatfloodprotect disabled\n", proxyinfo[clienti].name);
			return;
		}
	else if(enti)
		{
			if(proxyinfo[clienti].floodinfo.chatFloodProtect)
				{
					gi.cprintf (ent, PRINT_HIGH, "%s clientchatfloodprotect %d %d %d\n", proxyinfo[clienti].name, proxyinfo[clienti].floodinfo.chatFloodProtectNum, proxyinfo[clienti].floodinfo.chatFloodProtectSec, proxyinfo[clienti].floodinfo.chatFloodProtectSilence);
				}
			else
				{
					gi.cprintf (ent, PRINT_HIGH, "%s clientchatfloodprotect disabled\n", proxyinfo[clienti].name);
				}
				
			return;
		}
		
	gi.cprintf(ent, PRINT_HIGH, "[sv] !clientchatfloodprotect [LIKE/RE/CL] name [xxx(num) xxx(sec) xxx(silence) / disable]\n");
}








//===================================================================




void listfloodsRun(int startarg, edict_t *ent, int client)
{
	addCmdQueue(client, QCMD_DISPFLOOD, 0, 0, 0);
	
	gi.cprintf (ent, PRINT_HIGH, "Start flood commands List:\n");
}


void displayNextFlood(edict_t *ent, int client, long floodcmd)
{
	if(floodcmd < maxflood_cmds)
		{
			switch(floodcmds[floodcmd].type)
				{
				case FLOOD_SW:
					gi.cprintf (ent, PRINT_HIGH, "%4d SW:\"%s\"\n", floodcmd + 1, floodcmds[floodcmd].floodcmd);
					break;
					
				case FLOOD_EX:
					gi.cprintf (ent, PRINT_HIGH, "%4d EX:\"%s\"\n", floodcmd + 1, floodcmds[floodcmd].floodcmd);
					break;
					
				case FLOOD_RE:
					gi.cprintf (ent, PRINT_HIGH, "%4d RE:\"%s\"\n", floodcmd + 1, floodcmds[floodcmd].floodcmd);
					break;
				}
			floodcmd++;
			addCmdQueue(client, QCMD_DISPFLOOD, 0, floodcmd, 0);
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "End flood commands List\n");
		}
}



#define FLOODCMD     "[sv] !floodcmd [SW/EX/RE] \"command\"\n"

void floodcmdRun(int startarg, edict_t *ent, int client)
{
	char *cmd;
	int len;
	
	if(maxflood_cmds >= FLOOD_MAXCMDS)
		{
			gi.cprintf (ent, PRINT_HIGH, "Sorry, maximum number of flood commands has been reached.\n");
			return;
		}
		
	if (gi.argc() <= startarg + 1)
		{
			gi.cprintf (ent, PRINT_HIGH, FLOODCMD);
			return;
		}
		
	cmd = gi.argv(startarg);
	
	if(Q_stricmp(cmd, "SW") == 0)
		{
			floodcmds[maxflood_cmds].type = FLOOD_SW;
		}
	else if(Q_stricmp(cmd, "EX") == 0)
		{
			floodcmds[maxflood_cmds].type = FLOOD_EX;
		}
	else if(Q_stricmp(cmd, "RE") == 0)
		{
			floodcmds[maxflood_cmds].type = FLOOD_RE;
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, FLOODCMD);
			return;
		}
		
	cmd = gi.argv(startarg + 1);
	
	if(isBlank(cmd))
		{
			gi.cprintf (ent, PRINT_HIGH, FLOODCMD);
			return;
		}
		
	len = q2a_strlen(cmd) + 20;
	
	floodcmds[maxflood_cmds].floodcmd = gi.TagMalloc (len, TAG_LEVEL);
	processstring(floodcmds[maxflood_cmds].floodcmd, cmd, len - 1, 0);
	//  q2a_strcpy(floodcmds[maxflood_cmds].floodcmd, cmd);
	
	if(floodcmds[maxflood_cmds].type == FLOOD_RE)
		{
			q_strupr(cmd);
			
			floodcmds[maxflood_cmds].r = gi.TagMalloc (sizeof(*floodcmds[maxflood_cmds].r), TAG_LEVEL);
			q2a_memset(floodcmds[maxflood_cmds].r, 0x0, sizeof(*floodcmds[maxflood_cmds].r));
			//        if(regcomp(floodcmds[maxflood_cmds].r, cmd, REG_EXTENDED))
			if(regcomp(floodcmds[maxflood_cmds].r, cmd, 0))
				{
					gi.TagFree(floodcmds[maxflood_cmds].floodcmd);
					gi.TagFree(floodcmds[maxflood_cmds].r);
					floodcmds[maxflood_cmds].r = 0;
					
					// malformed re...
					gi.cprintf (ent, PRINT_HIGH, "Regular expression couldn't compile!\n");
					return;
				}
		}
	else
		{
			floodcmds[maxflood_cmds].r = 0;
		}
		
	switch(floodcmds[maxflood_cmds].type)
		{
		case FLOOD_SW:
			gi.cprintf (ent, PRINT_HIGH, "%4d SW:\"%s\" added\n", maxflood_cmds + 1, floodcmds[maxflood_cmds].floodcmd);
			break;
			
		case FLOOD_EX:
			gi.cprintf (ent, PRINT_HIGH, "%4d EX:\"%s\" added\n", maxflood_cmds + 1, floodcmds[maxflood_cmds].floodcmd);
			break;
			
		case FLOOD_RE:
			gi.cprintf (ent, PRINT_HIGH, "%4d RE:\"%s\" added\n", maxflood_cmds + 1, floodcmds[maxflood_cmds].floodcmd);
			break;
		}
		
	maxflood_cmds++;
}



#define FLOODDELCMD     "[sv] !flooddel floodnum\n"


void floodDelRun(int startarg, edict_t *ent, int client)
{
	int flood;
	
	if (gi.argc() <= startarg)
		{
			gi.cprintf (ent, PRINT_HIGH, FLOODDELCMD);
			return;
		}
		
	flood = q2a_atoi(gi.argv(startarg));
	
	if(flood < 1 || flood > maxflood_cmds)
		{
			gi.cprintf (ent, PRINT_HIGH, FLOODDELCMD);
			return;
		}
		
	flood--;
	
	gi.TagFree(floodcmds[flood].floodcmd);
	if(floodcmds[flood].r)
		{
			regfree(floodcmds[flood].r);
			gi.TagFree(floodcmds[flood].r);
		}
		
	if(flood + 1 < maxflood_cmds)
		{
			q2a_memmove((floodcmds + flood), (floodcmds + flood + 1), sizeof(floodcmd_t) * (maxflood_cmds - flood));
		}
		
	maxflood_cmds--;
	
	gi.cprintf (ent, PRINT_HIGH, "flood command deleted\n");
}



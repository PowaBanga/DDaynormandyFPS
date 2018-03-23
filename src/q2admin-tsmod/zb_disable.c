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
// zb_disable.c
//
// copyright 2000 Shane Powell
//

#include "g_local.h"


#define DISABLEFILE             "q2admindisable.txt"
#define DISABLE_MAXCMDS         50

typedef struct
	{
		char    *disablecmd;
		byte     type;
		regex_t *r;
	}
disablecmd_t;

#define DISABLE_SW  0
#define DISABLE_EX  1
#define DISABLE_RE  2

disablecmd_t disablecmds[DISABLE_MAXCMDS];
int maxdisable_cmds = 0;

qboolean disablecmds_enable = FALSE;




qboolean ReadDisableFile(char *disablename)
{
	FILE *disablefile;
	unsigned int uptoLine = 0;
	
	if(maxdisable_cmds >= DISABLE_MAXCMDS)
		{
			return FALSE;
		}
		
	disablefile = fopen(disablename, "rt");
	if(!disablefile)
		{
			return FALSE;
		}
		
	while(fgets(buffer, 256, disablefile))
		{
			char *cp = buffer;
			int len;
			
			// remove '\n'
			len = q2a_strlen(buffer) - 1;
			if(buffer[len] == '\n')
				{
					buffer[len] = 0x0;
				}
				
			SKIPBLANK(cp);
			
			uptoLine++;
			
			if(startContains (cp, "SW:") || startContains (cp, "EX:") || startContains (cp, "RE:"))
				{
					// looks ok, add...
					switch(*cp)
						{
						case 'S':
							disablecmds[maxdisable_cmds].type = DISABLE_SW;
							break;
							
						case 'E':
							disablecmds[maxdisable_cmds].type = DISABLE_EX;
							break;
							
						case 'R':
							disablecmds[maxdisable_cmds].type = DISABLE_RE;
							break;
						}
						
					cp += 3;
					SKIPBLANK(cp);
					
					len = q2a_strlen(cp) + 1;
					
					// zero length command
					if(!len)
						{
							gi.dprintf ("Error loading DISABLE from line %d in file %s\n", uptoLine, disablename);
							continue;
						}
						
					disablecmds[maxdisable_cmds].disablecmd = gi.TagMalloc (len, TAG_LEVEL);
					q2a_strcpy(disablecmds[maxdisable_cmds].disablecmd, cp);
					
					if(disablecmds[maxdisable_cmds].type == DISABLE_RE)
						{
							q_strupr(cp);
							
							disablecmds[maxdisable_cmds].r = gi.TagMalloc (sizeof(*disablecmds[maxdisable_cmds].r), TAG_LEVEL);
							q2a_memset(disablecmds[maxdisable_cmds].r, 0x0, sizeof(*disablecmds[maxdisable_cmds].r));
							//        if(regcomp(disablecmds[maxdisable_cmds].r, strbuffer, REG_EXTENDED))
							if(regcomp(disablecmds[maxdisable_cmds].r, cp, 0))
								{
									gi.TagFree(disablecmds[maxdisable_cmds].r);
									disablecmds[maxdisable_cmds].r = 0;
									
									// malformed re... skip this disable command
									gi.dprintf ("Error loading DISABLE from line %d in file %s\n", uptoLine, disablename);
									continue;
								}
						}
					else
						{
							disablecmds[maxdisable_cmds].r = 0;
						}
						
					maxdisable_cmds++;
					
					if(maxdisable_cmds >= DISABLE_MAXCMDS)
						{
							break;
						}
				}
			else if(!(cp[0] == ';' || cp[0] == '\n' || isBlank (cp)))
				{
					gi.dprintf ("Error loading DISABLE from line %d in file %s\n", uptoLine, disablename);
				}
		}
		
	fclose(disablefile);
	
	return TRUE;
}



void freeDisableLists(void)
{
	while(maxdisable_cmds)
		{
			maxdisable_cmds--;
			gi.TagFree(disablecmds[maxdisable_cmds].disablecmd);
			if(disablecmds[maxdisable_cmds].r)
				{
					regfree(disablecmds[maxdisable_cmds].r);
					gi.TagFree(disablecmds[maxdisable_cmds].r);
				}
		}
}


void readDisableLists(void)
{
	qboolean ret;
	
	freeDisableLists();
	
	ret = ReadDisableFile(DISABLEFILE);
	
	sprintf(buffer, "%s/%s", moddir, DISABLEFILE);
	if(ReadDisableFile(buffer))
		{
			ret = TRUE;
		}
		
	if(!ret)
		{
			gi.dprintf ("WARNING: " DISABLEFILE " could not be found\n");
			logEvent(LT_INTERNALWARN, 0, NULL, DISABLEFILE " could not be found", IW_DISABLESETUPLOAD, 0.0);
		}
}



void reloadDisableFileRun(int startarg, edict_t *ent, int client)
{
	readDisableLists();
	gi.cprintf (ent, PRINT_HIGH, "Disbled commands reloaded.\n");
}


qboolean checkfordisablecmd(char *cp, int disablecmd)
{
	switch(disablecmds[disablecmd].type)
		{
		case DISABLE_SW:
			return startContains (cp, disablecmds[disablecmd].disablecmd);
			
		case DISABLE_EX:
			return !Q_stricmp(cp, disablecmds[disablecmd].disablecmd);
			
		case DISABLE_RE:
			return (regexec(disablecmds[disablecmd].r, cp, 0, 0, 0) != REG_NOMATCH);
		}
		
	return FALSE;
}


qboolean checkDisabledCommand(char *cmd)
{
	unsigned int i;
	
	q2a_strcpy(buffer, cmd);
	q_strupr(buffer);
	
	for(i = 0; i < maxdisable_cmds; i++)
		{
			if(checkfordisablecmd(buffer, i))
				{
					return TRUE;
				}
		}
		
	return FALSE;
}



//===================================================================






void listdisablesRun(int startarg, edict_t *ent, int client)
{
	addCmdQueue(client, QCMD_DISPDISABLE, 0, 0, 0);
	
	gi.cprintf (ent, PRINT_HIGH, "Start disbled-entities List:\n");
}


void displayNextDisable(edict_t *ent, int client, long disablecmd)
{
	if(disablecmd < maxdisable_cmds)
		{
			switch(disablecmds[disablecmd].type)
				{
				case DISABLE_SW:
					gi.cprintf (ent, PRINT_HIGH, "%4d SW:\"%s\"\n", disablecmd + 1, disablecmds[disablecmd].disablecmd);
					break;
					
				case DISABLE_EX:
					gi.cprintf (ent, PRINT_HIGH, "%4d EX:\"%s\"\n", disablecmd + 1, disablecmds[disablecmd].disablecmd);
					break;
					
				case DISABLE_RE:
					gi.cprintf (ent, PRINT_HIGH, "%4d RE:\"%s\"\n", disablecmd + 1, disablecmds[disablecmd].disablecmd);
					break;
				}
			disablecmd++;
			addCmdQueue(client, QCMD_DISPDISABLE, 0, disablecmd, 0);
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "End disbled-entities List\n");
		}
}



#define DISABLECMD     "[sv] !disablecmd [SW/EX/RE] \"command\"\n"

void disablecmdRun(int startarg, edict_t *ent, int client)
{
	char *cmd;
	int len;
	
	if(maxdisable_cmds >= DISABLE_MAXCMDS)
		{
			gi.cprintf (ent, PRINT_HIGH, "Sorry, maximum number of disbled-entitie commands has been reached.\n");
			return;
		}
		
	if (gi.argc() <= startarg + 1)
		{
			gi.cprintf (ent, PRINT_HIGH, DISABLECMD);
			return;
		}
		
	cmd = gi.argv(startarg);
	
	if(Q_stricmp(cmd, "SW") == 0)
		{
			disablecmds[maxdisable_cmds].type = DISABLE_SW;
		}
	else if(Q_stricmp(cmd, "EX") == 0)
		{
			disablecmds[maxdisable_cmds].type = DISABLE_EX;
		}
	else if(Q_stricmp(cmd, "RE") == 0)
		{
			disablecmds[maxdisable_cmds].type = DISABLE_RE;
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, DISABLECMD);
			return;
		}
		
	cmd = gi.argv(startarg + 1);
	
	if(isBlank(cmd))
		{
			gi.cprintf (ent, PRINT_HIGH, DISABLECMD);
			return;
		}
		
	len = q2a_strlen(cmd) + 20;
	
	disablecmds[maxdisable_cmds].disablecmd = gi.TagMalloc (len, TAG_LEVEL);
	processstring(disablecmds[maxdisable_cmds].disablecmd, cmd, len - 1, 0);
	//  q2a_strcpy(disablecmds[maxdisable_cmds].disablecmd, cmd);
	
	if(disablecmds[maxdisable_cmds].type == DISABLE_RE)
		{
			q_strupr(cmd);
			
			disablecmds[maxdisable_cmds].r = gi.TagMalloc (sizeof(*disablecmds[maxdisable_cmds].r), TAG_LEVEL);
			q2a_memset(disablecmds[maxdisable_cmds].r, 0x0, sizeof(*disablecmds[maxdisable_cmds].r));
			//        if(regcomp(disablecmds[maxdisable_cmds].r, cmd, REG_EXTENDED))
			if(regcomp(disablecmds[maxdisable_cmds].r, cmd, 0))
				{
					gi.TagFree(disablecmds[maxdisable_cmds].disablecmd);
					gi.TagFree(disablecmds[maxdisable_cmds].r);
					disablecmds[maxdisable_cmds].r = 0;
					
					// malformed re...
					gi.cprintf (ent, PRINT_HIGH, "Regular expression couldn't compile!\n");
					return;
				}
		}
	else
		{
			disablecmds[maxdisable_cmds].r = 0;
		}
		
	switch(disablecmds[maxdisable_cmds].type)
		{
		case DISABLE_SW:
			gi.cprintf (ent, PRINT_HIGH, "%4d SW:\"%s\" added\n", maxdisable_cmds + 1, disablecmds[maxdisable_cmds].disablecmd);
			break;
			
		case DISABLE_EX:
			gi.cprintf (ent, PRINT_HIGH, "%4d EX:\"%s\" added\n", maxdisable_cmds + 1, disablecmds[maxdisable_cmds].disablecmd);
			break;
			
		case DISABLE_RE:
			gi.cprintf (ent, PRINT_HIGH, "%4d RE:\"%s\" added\n", maxdisable_cmds + 1, disablecmds[maxdisable_cmds].disablecmd);
			break;
		}
		
	maxdisable_cmds++;
}



#define DISABLEDELCMD     "[sv] !disabledel disablenum\n"


void disableDelRun(int startarg, edict_t *ent, int client)
{
	int disable;
	
	if (gi.argc() <= startarg)
		{
			gi.cprintf (ent, PRINT_HIGH, DISABLEDELCMD);
			return;
		}
		
	disable = q2a_atoi(gi.argv(startarg));
	
	if(disable < 1 || disable > maxdisable_cmds)
		{
			gi.cprintf (ent, PRINT_HIGH, DISABLEDELCMD);
			return;
		}
		
	disable--;
	
	gi.TagFree(disablecmds[disable].disablecmd);
	if(disablecmds[disable].r)
		{
			regfree(disablecmds[disable].r);
			gi.TagFree(disablecmds[disable].r);
		}
		
	if(disable + 1 < maxdisable_cmds)
		{
			q2a_memmove((disablecmds + disable), (disablecmds + disable + 1), sizeof(disablecmd_t) * (maxdisable_cmds - disable));
		}
		
	maxdisable_cmds--;
	
	gi.cprintf (ent, PRINT_HIGH, "Disbled command deleted\n");
}



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
// zb_spawn.c
//
// copyright 2000 Shane Powell
//


#include "g_local.h"


#define SPAWNFILE             "q2adminspawn.txt"
#define SPAWN_MAXCMDS         50

typedef struct
	{
		char    *spawncmd;
		byte     type;
		qboolean onelevelflag;
		regex_t *r;
	}
spawncmd_t;

#define SPAWN_SW  0
#define SPAWN_EX  1
#define SPAWN_RE  2

spawncmd_t spawncmds[SPAWN_MAXCMDS];
int maxspawn_cmds = 0;



int entity_classname_offset = (sizeof(struct edict_s)) + 20;  // default byte offset to the classname variable.




qboolean ReadSpawnFile(char *spawnname, qboolean onelevelflag)
{
	FILE *spawnfile;
	unsigned int uptoLine = 0;
	
	if(maxspawn_cmds >= SPAWN_MAXCMDS)
		{
			return FALSE;
		}
		
	spawnfile = fopen(spawnname, "rt");
	if(!spawnfile)
		{
			return FALSE;
		}
		
	while(fgets(buffer, 256, spawnfile))
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
							spawncmds[maxspawn_cmds].type = SPAWN_SW;
							break;
							
						case 'E':
							spawncmds[maxspawn_cmds].type = SPAWN_EX;
							break;
							
						case 'R':
							spawncmds[maxspawn_cmds].type = SPAWN_RE;
							break;
						}
						
					spawncmds[maxspawn_cmds].onelevelflag = onelevelflag;
					
					cp += 3;
					SKIPBLANK(cp);
					
					len = q2a_strlen(cp) + 1;
					
					// zero length command
					if(!len)
						{
							gi.dprintf ("Error loading SPAWN from line %d in file %s\n", uptoLine, spawnname);
							continue;
						}
						
					spawncmds[maxspawn_cmds].spawncmd = gi.TagMalloc (len, TAG_GAME);
					q2a_strcpy(spawncmds[maxspawn_cmds].spawncmd, cp);
					
					if(spawncmds[maxspawn_cmds].type == SPAWN_RE)
						{
							q_strupr(cp);
							
							spawncmds[maxspawn_cmds].r = gi.TagMalloc (sizeof(*spawncmds[maxspawn_cmds].r), TAG_GAME);
							q2a_memset(spawncmds[maxspawn_cmds].r, 0x0, sizeof(*spawncmds[maxspawn_cmds].r));
							//        if(regcomp(spawncmds[maxspawn_cmds].r, strbuffer, REG_EXTENDED))
							if(regcomp(spawncmds[maxspawn_cmds].r, cp, 0))
								{
									gi.TagFree(spawncmds[maxspawn_cmds].r);
									spawncmds[maxspawn_cmds].r = 0;
									
									// malformed re... skip this spawn command
									gi.dprintf ("Error loading SPAWN from line %d in file %s\n", uptoLine, spawnname);
									continue;
								}
						}
					else
						{
							spawncmds[maxspawn_cmds].r = 0;
						}
						
					maxspawn_cmds++;
					
					if(maxspawn_cmds >= SPAWN_MAXCMDS)
						{
							break;
						}
				}
			else if(!(cp[0] == ';' || cp[0] == '\n' || isBlank (cp)))
				{
					gi.dprintf ("Error loading SPAWN from line %d in file %s\n", uptoLine, spawnname);
				}
		}
		
	fclose(spawnfile);
	
	return TRUE;
}



void freeSpawnLists(void)
{
	while(maxspawn_cmds)
		{
			maxspawn_cmds--;
			gi.TagFree(spawncmds[maxspawn_cmds].spawncmd);
			if(spawncmds[maxspawn_cmds].r)
				{
					regfree(spawncmds[maxspawn_cmds].r);
					gi.TagFree(spawncmds[maxspawn_cmds].r);
				}
		}
}

void freeOneLevelSpawnLists(void)
{
	int spawn = 0;
	
	while(spawn < maxspawn_cmds)
		{
			if(spawncmds[spawn].onelevelflag)
				{
					gi.TagFree(spawncmds[spawn].spawncmd);
					if(spawncmds[spawn].r)
						{
							regfree(spawncmds[spawn].r);
							gi.TagFree(spawncmds[spawn].r);
						}
						
					if(spawn + 1 < maxspawn_cmds)
						{
							q2a_memmove((spawncmds + spawn), (spawncmds + spawn + 1), sizeof(spawncmd_t) * (maxspawn_cmds - spawn));
						}
						
					maxspawn_cmds--;
				}
			else
				{
					spawn++;
				}
		}
}



void readSpawnLists(void)
{
	qboolean ret;
	
	freeSpawnLists();
	
	ret = ReadSpawnFile(SPAWNFILE, FALSE);
	
	sprintf(buffer, "%s/%s", moddir, SPAWNFILE);
	if(ReadSpawnFile(buffer, FALSE))
		{
			ret = TRUE;
		}
		
	if(!ret)
		{
			gi.dprintf ("WARNING: " SPAWNFILE " could not be found\n");
			logEvent(LT_INTERNALWARN, 0, NULL, SPAWNFILE " could not be found", IW_SPAWNSETUPLOAD, 0.0);
		}
}



void reloadSpawnFileRun(int startarg, edict_t *ent, int client)
{
	readSpawnLists();
	gi.cprintf (ent, PRINT_HIGH, "Disbled entities reloaded.\n");
}


qboolean checkforspawncmd(char *cp, int spawncmd)
{
	switch(spawncmds[spawncmd].type)
		{
		case SPAWN_SW:
			return startContains (cp, spawncmds[spawncmd].spawncmd);
			
		case SPAWN_EX:
			return !Q_stricmp(cp, spawncmds[spawncmd].spawncmd);
			
		case SPAWN_RE:
			return (regexec(spawncmds[spawncmd].r, cp, 0, 0, 0) != REG_NOMATCH);
		}
		
	return FALSE;
}


qboolean checkDisabledEntities(char *cp)
{
	unsigned int i;
	
	q2a_strcpy(buffer, cp);
	q_strupr(buffer);
	
	for(i = 0; i < maxspawn_cmds; i++)
		{
			if(checkforspawncmd(buffer, i))
				{
					return TRUE;
				}
		}
		
	return FALSE;
}

//===================================================================

void listspawnsRun(int startarg, edict_t *ent, int client)
{
	addCmdQueue(client, QCMD_DISPSPAWN, 0, 0, 0);
	
	gi.cprintf (ent, PRINT_HIGH, "Start disbled-entities List:\n");
}

void displayNextSpawn(edict_t *ent, int client, long spawncmd)
{
	if(spawncmd < maxspawn_cmds)
		{
			switch(spawncmds[spawncmd].type)
				{
				case SPAWN_SW:
					gi.cprintf (ent, PRINT_HIGH, "%4d SW:\"%s\"\n", spawncmd + 1, spawncmds[spawncmd].spawncmd);
					break;
					
				case SPAWN_EX:
					gi.cprintf (ent, PRINT_HIGH, "%4d EX:\"%s\"\n", spawncmd + 1, spawncmds[spawncmd].spawncmd);
					break;
					
				case SPAWN_RE:
					gi.cprintf (ent, PRINT_HIGH, "%4d RE:\"%s\"\n", spawncmd + 1, spawncmds[spawncmd].spawncmd);
					break;
				}
			spawncmd++;
			addCmdQueue(client, QCMD_DISPSPAWN, 0, spawncmd, 0);
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "End disbled-entities List\n");
		}
}

#define SPAWNCMD     "[sv] !spawncmd [SW/EX/RE] \"command\"\n"

void spawncmdRun(int startarg, edict_t *ent, int client)
{
	char *cmd;
	int len;
	
	if(maxspawn_cmds >= SPAWN_MAXCMDS)
		{
			gi.cprintf (ent, PRINT_HIGH, "Sorry, maximum number of disbled-entitie commands has been reached.\n");
			return;
		}
		
	if (gi.argc() <= startarg + 1)
		{
			gi.cprintf (ent, PRINT_HIGH, SPAWNCMD);
			return;
		}
		
	cmd = gi.argv(startarg);
	
	if(Q_stricmp(cmd, "SW") == 0)
		{
			spawncmds[maxspawn_cmds].type = SPAWN_SW;
		}
	else if(Q_stricmp(cmd, "EX") == 0)
		{
			spawncmds[maxspawn_cmds].type = SPAWN_EX;
		}
	else if(Q_stricmp(cmd, "RE") == 0)
		{
			spawncmds[maxspawn_cmds].type = SPAWN_RE;
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, SPAWNCMD);
			return;
		}
		
	spawncmds[maxspawn_cmds].onelevelflag = FALSE;
	
	cmd = gi.argv(startarg + 1);
	
	if(isBlank(cmd))
		{
			gi.cprintf (ent, PRINT_HIGH, SPAWNCMD);
			return;
		}
		
	len = q2a_strlen(cmd) + 20;
	
	spawncmds[maxspawn_cmds].spawncmd = gi.TagMalloc (len, TAG_GAME);
	processstring(spawncmds[maxspawn_cmds].spawncmd, cmd, len - 1, 0);
	//  q2a_strcpy(spawncmds[maxspawn_cmds].spawncmd, cmd);
	
	if(spawncmds[maxspawn_cmds].type == SPAWN_RE)
		{
			q_strupr(cmd);
			
			spawncmds[maxspawn_cmds].r = gi.TagMalloc (sizeof(*spawncmds[maxspawn_cmds].r), TAG_GAME);
			q2a_memset(spawncmds[maxspawn_cmds].r, 0x0, sizeof(*spawncmds[maxspawn_cmds].r));
			//        if(regcomp(spawncmds[maxspawn_cmds].r, cmd, REG_EXTENDED))
			if(regcomp(spawncmds[maxspawn_cmds].r, cmd, 0))
				{
					gi.TagFree(spawncmds[maxspawn_cmds].spawncmd);
					gi.TagFree(spawncmds[maxspawn_cmds].r);
					spawncmds[maxspawn_cmds].r = 0;
					
					// malformed re...
					gi.cprintf (ent, PRINT_HIGH, "Regular expression couldn't compile!\n");
					return;
				}
		}
	else
		{
			spawncmds[maxspawn_cmds].r = 0;
		}
		
	switch(spawncmds[maxspawn_cmds].type)
		{
		case SPAWN_SW:
			gi.cprintf (ent, PRINT_HIGH, "%4d SW:\"%s\" added\n", maxspawn_cmds + 1, spawncmds[maxspawn_cmds].spawncmd);
			break;
			
		case SPAWN_EX:
			gi.cprintf (ent, PRINT_HIGH, "%4d EX:\"%s\" added\n", maxspawn_cmds + 1, spawncmds[maxspawn_cmds].spawncmd);
			break;
			
		case SPAWN_RE:
			gi.cprintf (ent, PRINT_HIGH, "%4d RE:\"%s\" added\n", maxspawn_cmds + 1, spawncmds[maxspawn_cmds].spawncmd);
			break;
		}
		
	maxspawn_cmds++;
}

#define SPAWNDELCMD     "[sv] !spawndel spawnnum\n"

void spawnDelRun(int startarg, edict_t *ent, int client)
{
	int spawn;
	
	if (gi.argc() <= startarg)
		{
			gi.cprintf (ent, PRINT_HIGH, SPAWNDELCMD);
			return;
		}
		
	spawn = q2a_atoi(gi.argv(startarg));
	
	if(spawn < 1 || spawn > maxspawn_cmds)
		{
			gi.cprintf (ent, PRINT_HIGH, SPAWNDELCMD);
			return;
		}
		
	spawn--;
	
	gi.TagFree(spawncmds[spawn].spawncmd);
	if(spawncmds[spawn].r)
		{
			regfree(spawncmds[spawn].r);
			gi.TagFree(spawncmds[spawn].r);
		}
		
	if(spawn + 1 < maxspawn_cmds)
		{
			q2a_memmove((spawncmds + spawn), (spawncmds + spawn + 1), sizeof(spawncmd_t) * (maxspawn_cmds - spawn));
		}
		
	maxspawn_cmds--;
	
	gi.cprintf (ent, PRINT_HIGH, "Disbled-entities command deleted\n");
}

void linkentity_internal(edict_t *ent)
{
	if(spawnentities_internal_enable && spawnentities_enable)
		{
			if(checkDisabledEntities(*((char **)((unsigned long)ent + entity_classname_offset))))
				{
					char **classnameptr = ((char **)((unsigned long)ent + entity_classname_offset));
					
					*classnameptr = NULL;
					ent->inuse = FALSE;
					return;
				}
		}
		
	logEvent(LT_ENTITYCREATE, 0, NULL, *((char **)((unsigned long)ent + entity_classname_offset)), 0, 0.0);
	
	gi.linkentity(ent);
}

void unlinkentity_internal(edict_t *ent)
{
	logEvent(LT_ENTITYDELETE, 0, NULL, *((char **)((unsigned long)ent + entity_classname_offset)), 0, 0.0);
	
	gi.unlinkentity(ent);
}
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
// zb_lrcon.c
//
// copyright 2000 Shane Powell
//

#include "g_local.h"

#define LRCONFILE             "q2adminlrcon.txt"
#define LRCON_MAXCMDS         1024

typedef struct
	{
		char   *lrconcmd;
		char   *password;
		byte    type;
		regex_t *r;
	}
lrconcmd_t;

#define LRC_SW  0
#define LRC_EX  1
#define LRC_RE  2

lrconcmd_t lrconcmds[LRCON_MAXCMDS];
int maxlrcon_cmds = 0;

qboolean rcon_random_password = true;

//r1ch 2005-01-27 insecure lrcon fix BEGIN
qboolean	rcon_insecure = false;
//r1ch 2005-01-27 insecure lrcon fix END

int lrcon_timeout = 2;
char orginal_rcon_password[50];
float password_timeout = 0;

qboolean ReadLRconFile(char *lrcname)
{
	FILE *lrconfile;
	unsigned int uptoLine = 0;
	
	if(maxlrcon_cmds >= LRCON_MAXCMDS)
		{
			return FALSE;
		}
		
	lrconfile = fopen(lrcname, "rt");
	if(!lrconfile)
		{
			return FALSE;
		}
		
	while(fgets(buffer, 256, lrconfile))
		{
			char *cp = buffer, *pp;
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
							lrconcmds[maxlrcon_cmds].type = LRC_SW;
							break;
							
						case 'E':
							lrconcmds[maxlrcon_cmds].type = LRC_EX;
							break;
							
						case 'R':
							lrconcmds[maxlrcon_cmds].type = LRC_RE;
							break;
						}
						
					cp += 3;
					SKIPBLANK(cp);
					
					pp = cp;
					
					// find len of password
					len = 0;
					while(*pp && *pp != ' ')
						{
							pp++;
							len++;
						}
						
					if(!len || *pp == 0)
						{
							gi.dprintf ("Error loading LRCON from line %d in file %s\n", uptoLine, lrcname);
							// no command or zero length password
							continue;
						}
						
					// allocate memory for password and copy into buffer

					//r1ch 2005-01-26 fix insufficient memory allocation BEGIN
					lrconcmds[maxlrcon_cmds].password = gi.TagMalloc (len+1, TAG_LEVEL);
					//r1ch 2005-01-26 fix insufficient memory allocation END

					pp = lrconcmds[maxlrcon_cmds].password;
					while(*cp && *cp != ' ')
						{
							*pp++ = *cp++;
						}
					SKIPBLANK(cp);
					*pp = 0;
					
					len = q2a_strlen(cp) + 1;
					
					// zero length command
					if(!len)
						{
							gi.TagFree(lrconcmds[maxlrcon_cmds].password);
							gi.dprintf ("Error loading LRCON from line %d in file %s\n", uptoLine, lrcname);
							continue;
						}
						
					lrconcmds[maxlrcon_cmds].lrconcmd = gi.TagMalloc (len, TAG_LEVEL);
					q2a_strcpy(lrconcmds[maxlrcon_cmds].lrconcmd, cp);
					
					if(lrconcmds[maxlrcon_cmds].type == LRC_RE)
						{
							q_strupr(cp);
							
							lrconcmds[maxlrcon_cmds].r = gi.TagMalloc (sizeof(*lrconcmds[maxlrcon_cmds].r), TAG_LEVEL);
							q2a_memset(lrconcmds[maxlrcon_cmds].r, 0x0, sizeof(*lrconcmds[maxlrcon_cmds].r));
							//        if(regcomp(lrconcmds[maxlrcon_cmds].r, strbuffer, REG_EXTENDED))
							if(regcomp(lrconcmds[maxlrcon_cmds].r, cp, 0))
								{
									gi.TagFree(lrconcmds[maxlrcon_cmds].r);
									lrconcmds[maxlrcon_cmds].r = 0;
									
									// malformed re... skip this lrcon
									gi.dprintf ("Error loading LRCON from line %d in file %s\n", uptoLine, lrcname);
									continue;
								}
						}
					else
						{
							lrconcmds[maxlrcon_cmds].r = 0;
						}
						
					maxlrcon_cmds++;
					
					if(maxlrcon_cmds >= LRCON_MAXCMDS)
						{
							break;
						}
				}
			else if(!(cp[0] == ';' || cp[0] == '\n' || isBlank (cp)))
				{
					gi.dprintf ("Error loading LRCON from line %d in file %s\n", uptoLine, lrcname);
				}
		}
		
	fclose(lrconfile);
	
	return TRUE;
}

void freeLRconLists(void)
{
	while(maxlrcon_cmds)
		{
			maxlrcon_cmds--;
			gi.TagFree(lrconcmds[maxlrcon_cmds].password);
			gi.TagFree(lrconcmds[maxlrcon_cmds].lrconcmd);
			if(lrconcmds[maxlrcon_cmds].r)
				{
					regfree(lrconcmds[maxlrcon_cmds].r);
					gi.TagFree(lrconcmds[maxlrcon_cmds].r);
				}
		}
}

void readLRconLists(void)
{
	qboolean ret;
	
	freeLRconLists();
	
	ret = ReadLRconFile(LRCONFILE);
	
	sprintf(buffer, "%s/%s", moddir, LRCONFILE);
	if(ReadLRconFile(buffer))
		{
			ret = TRUE;
		}
		
	if(!ret)
		{
			gi.dprintf ("WARNING: " LRCONFILE " could not be found\n");
			logEvent(LT_INTERNALWARN, 0, NULL, LRCONFILE " could not be found", IW_LRCONSETUPLOAD, 0.0);
		}
}

void reloadlrconfileRun(int startarg, edict_t *ent, int client)
{
	readLRconLists();
	gi.cprintf (ent, PRINT_HIGH, "Lrcons reloaded.\n");
}

qboolean checklrcon(char *cp, int lrcon)
{
	char strbuffer[256];
	
	switch(lrconcmds[lrcon].type)
		{
		case LRC_SW:
			return startContains (cp, lrconcmds[lrcon].lrconcmd);
			
		case LRC_EX:
			return !Q_stricmp(cp, lrconcmds[lrcon].lrconcmd);
			
		case LRC_RE:
			//r1ch: overflow fix
			q2a_strncpy(strbuffer, cp, sizeof(strbuffer)-1);
			q_strupr(strbuffer);
			return (regexec(lrconcmds[lrcon].r, strbuffer, 0, 0, 0) != REG_NOMATCH);
		}
		
	return FALSE;
}


void run_lrcon(edict_t *ent, int client)
{
	unsigned int i;
	
	// check each command for a good password..
	for(i = 0; i < maxlrcon_cmds; i++)
		{
			// create command string to compare...
			char *cp = gi.args(), *pp, *orgcp;
			orgcp = cp;
			
			SKIPBLANK(cp);
			
			pp = lrconcmds[i].password;
			
			// check password...
			while(*cp && *cp != ' ')
				{
					if(*pp != *cp)
						{
							break;
						}
						
					cp++;
					pp++;
				}
				
			if ( *pp != 0 )
				{
					continue;
				}
				
			if(*cp != ' ')
				{
					continue;
				}
				
			SKIPBLANK(cp);
			
			if(*cp)
				{
					if(checklrcon(cp, i) && q2a_strchr(cp, ';') == NULL && q2a_strchr(cp, '\n') == NULL)
						{
							//r1ch 2005-01-27 insecure lrcon fix BEGIN
							if (rcon_insecure)
							{
								if ( rcon_random_password )
									{
										char cbuffer[RANDOM_STRING_LENGTH + 1];
										
										if ( password_timeout )
											{
												gi.cprintf(ent, PRINT_HIGH, "Sorry, another lrcon command is being processed, please try again later\n");
												return;
											}
											
										generateRandomString(cbuffer, RANDOM_STRING_LENGTH);
										
										//r1ch: fix for oversized rcon passwords
										q2a_strncpy (orginal_rcon_password, rconpassword->string, sizeof(orginal_rcon_password)-1);
										gi.cvar_set("rcon_password", cbuffer);
										
										password_timeout = ltime + lrcon_timeout;
										sprintf(buffer, "rcon %s %s\n", cbuffer, cp);
										stuffcmd(ent, buffer);
										
										sprintf(buffer, "rcon %s sv !resetrcon\n", cbuffer, cp);
										
									}
								else
									{
										sprintf(buffer, "rcon %s %s\n", rconpassword->string, cp);
									}
									
								// found a good command to run..
								stuffcmd(ent, buffer);
								}
							else
							{
								//we don't let the client execute arbitrary commands, we write the allowed
								//command directly to the server. downside is client never sees output of cmd.
								sprintf (buffer, "%s\n", cp);
								gi.AddCommandString (buffer);
							}
							//r1ch 2005-01-27 insecure lrcon fix END
							
							logEvent(LT_CLIENTLRCON, client, ent, orgcp, 0, 0.0);
							return;
						}
				}
		}
		
	gi.cprintf(ent, PRINT_HIGH, "Unknown lrcon command\n");
}



void check_lrcon_password ( void )
{
	if (password_timeout && (password_timeout < ltime))
		{
			lrcon_reset_rcon_password(0, 0, 0);
		}
}

void lrcon_reset_rcon_password(int startarg, edict_t *ent, int client)
{
	if ( password_timeout == 0 )
		{
			return;
		}
		
	password_timeout = 0;
	gi.cvar_set("rcon_password", orginal_rcon_password);
}


void listlrconsRun(int startarg, edict_t *ent, int client)
{
	addCmdQueue(client, QCMD_DISPLRCONS, 0, 0, 0);
	
	gi.cprintf (ent, PRINT_HIGH, "Start lrcon's List:\n");
}


void displayNextLRCon(edict_t *ent, int client, long lrconnum)
{
	if(lrconnum < maxlrcon_cmds)
		{
			switch(lrconcmds[lrconnum].type)
				{
				case LRC_SW:
					gi.cprintf (ent, PRINT_HIGH, "%4d SW:\"%s\" \"%s\"\n", lrconnum + 1, lrconcmds[lrconnum].password, lrconcmds[lrconnum].lrconcmd);
					break;
					
				case LRC_EX:
					gi.cprintf (ent, PRINT_HIGH, "%4d EX:\"%s\" \"%s\"\n", lrconnum + 1, lrconcmds[lrconnum].password, lrconcmds[lrconnum].lrconcmd);
					break;
					
				case LRC_RE:
					gi.cprintf (ent, PRINT_HIGH, "%4d RE:\"%s\" \"%s\"\n", lrconnum + 1, lrconcmds[lrconnum].password, lrconcmds[lrconnum].lrconcmd);
					break;
				}
			lrconnum++;
			addCmdQueue(client, QCMD_DISPLRCONS, 0, lrconnum, 0);
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "End lrcon's List\n");
		}
}



#define LRCONCMD     "[sv] !lrcon [SW/EX/RE] \"password\" \"command\"\n"

void lrconRun(int startarg, edict_t *ent, int client)
{
	char *cmd;
	int len;
	
	if(maxlrcon_cmds >= LRCON_MAXCMDS)
		{
			gi.cprintf (ent, PRINT_HIGH, "Sorry, maximum number of lrcon's has been reached.\n");
			return;
		}
		
	if (gi.argc() <= startarg + 2)
		{
			gi.cprintf (ent, PRINT_HIGH, LRCONCMD);
			return;
		}
		
	cmd = gi.argv(startarg);
	
	if(Q_stricmp(cmd, "SW") == 0)
		{
			lrconcmds[maxlrcon_cmds].type = LRC_SW;
		}
	else if(Q_stricmp(cmd, "EX") == 0)
		{
			lrconcmds[maxlrcon_cmds].type = LRC_EX;
		}
	else if(Q_stricmp(cmd, "RE") == 0)
		{
			lrconcmds[maxlrcon_cmds].type = LRC_RE;
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, LRCONCMD);
			return;
		}
		
	cmd = gi.argv(startarg + 1);
	
	if(isBlank(cmd))
		{
			gi.cprintf (ent, PRINT_HIGH, LRCONCMD);
			return;
		}
		
	len = q2a_strlen(cmd) + 1;
	
	lrconcmds[maxlrcon_cmds].password = gi.TagMalloc (len, TAG_LEVEL);
	
	q2a_strcpy(lrconcmds[maxlrcon_cmds].password, cmd);
	
	
	cmd = gi.argv(startarg + 2);
	
	if(isBlank(cmd))
		{
			gi.TagFree(lrconcmds[maxlrcon_cmds].password);
			gi.cprintf (ent, PRINT_HIGH, LRCONCMD);
			return;
		}
		
	len = q2a_strlen(cmd) + 20;
	
	lrconcmds[maxlrcon_cmds].lrconcmd = gi.TagMalloc (len, TAG_LEVEL);
	processstring(lrconcmds[maxlrcon_cmds].lrconcmd, cmd, len - 1, 0);
	//  q2a_strcpy(lrconcmds[maxlrcon_cmds].lrconcmd, cmd);
	
	if(lrconcmds[maxlrcon_cmds].type == LRC_RE)
		{
			q_strupr(cmd);
			
			lrconcmds[maxlrcon_cmds].r = gi.TagMalloc (sizeof(*lrconcmds[maxlrcon_cmds].r), TAG_LEVEL);
			q2a_memset(lrconcmds[maxlrcon_cmds].r, 0x0, sizeof(*lrconcmds[maxlrcon_cmds].r));
			//        if(regcomp(lrconcmds[maxlrcon_cmds].r, cmd, REG_EXTENDED))
			if(regcomp(lrconcmds[maxlrcon_cmds].r, cmd, 0))
				{
					gi.TagFree(lrconcmds[maxlrcon_cmds].password);
					gi.TagFree(lrconcmds[maxlrcon_cmds].lrconcmd);
					gi.TagFree(lrconcmds[maxlrcon_cmds].r);
					lrconcmds[maxlrcon_cmds].r = 0;
					
					// malformed re...
					gi.cprintf (ent, PRINT_HIGH, "Regular expression couldn't compile!\n");
					return;
				}
		}
	else
		{
			lrconcmds[maxlrcon_cmds].r = 0;
		}
		
	switch(lrconcmds[maxlrcon_cmds].type)
		{
		case LRC_SW:
			gi.cprintf (ent, PRINT_HIGH, "%4d SW:\"%s\" \"%s\" added\n", maxlrcon_cmds + 1, lrconcmds[maxlrcon_cmds].password, lrconcmds[maxlrcon_cmds].lrconcmd);
			break;
			
		case LRC_EX:
			gi.cprintf (ent, PRINT_HIGH, "%4d EX:\"%s\" \"%s\" added\n", maxlrcon_cmds + 1, lrconcmds[maxlrcon_cmds].password, lrconcmds[maxlrcon_cmds].lrconcmd);
			break;
			
		case LRC_RE:
			gi.cprintf (ent, PRINT_HIGH, "%4d RE:\"%s\" \"%s\" added\n", maxlrcon_cmds + 1, lrconcmds[maxlrcon_cmds].password, lrconcmds[maxlrcon_cmds].lrconcmd);
			break;
		}
		
	maxlrcon_cmds++;
}



#define LRCONDELCMD     "[sv] !lrcondel lrconnum\n"


void lrconDelRun(int startarg, edict_t *ent, int client)
{
	int lrcon;
	
	if (gi.argc() <= startarg)
		{
			gi.cprintf (ent, PRINT_HIGH, LRCONDELCMD);
			return;
		}
		
	lrcon = q2a_atoi(gi.argv(startarg));
	
	if(lrcon < 1 || lrcon > maxlrcon_cmds)
		{
			gi.cprintf (ent, PRINT_HIGH, LRCONDELCMD);
			return;
		}
		
	lrcon--;
	
	gi.TagFree(lrconcmds[lrcon].password);
	gi.TagFree(lrconcmds[lrcon].lrconcmd);
	if(lrconcmds[lrcon].r)
		{
			regfree(lrconcmds[lrcon].r);
			gi.TagFree(lrconcmds[lrcon].r);
		}
		
	if(lrcon + 1 < maxlrcon_cmds)
		{
			q2a_memmove((lrconcmds + lrcon), (lrconcmds + lrcon + 1), sizeof(lrconcmd_t) * (maxlrcon_cmds - lrcon));
		}
		
	maxlrcon_cmds--;
	
	gi.cprintf (ent, PRINT_HIGH, "lrcon deleted\n");
}



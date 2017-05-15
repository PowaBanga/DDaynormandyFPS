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
// zb_checkvar.c
//
// copyright 2000 Shane Powell
//

#include "g_local.h"

#define CHECKVARFILE             "q2admincheckvar.txt"
#define CHECKVAR_MAX             50

typedef struct
	{
		char    variablename[50];
		char    value[50];
		double  upper, lower;
		int     type;
	}
checkvar_t;

#define CV_CONSTANT     0
#define CV_RANGE        1

checkvar_t checkvarList[CHECKVAR_MAX];
int maxcheckvars = 0;

qboolean checkvarcmds_enable = FALSE;
int checkvar_poll_time = 60;

qboolean ReadCheckVarFile(char *checkvarname)
{
	FILE *checkvarfile;
	unsigned int uptoLine = 0;
	
	if(maxcheckvars >= CHECKVAR_MAX)
		{
			return FALSE;
		}
		
	checkvarfile = fopen(checkvarname, "rt");
	if(!checkvarfile)
		{
			return FALSE;
		}
		
	while(fgets(buffer, 256, checkvarfile))
		{
			char *cp = buffer;
			int len;
			
			// remove '\n'
			len = q2a_strlen(buffer);
			len -= 1;
			if(buffer[len] == '\n')
				{
					buffer[len] = 0x0;
				}
				
			SKIPBLANK(cp);
			
			uptoLine++;
			
			if(startContains (cp, "CT:") || startContains (cp, "RG:"))
				{
					// looks ok, add...
					if ( *cp == 'C')
					{
						checkvarList[maxcheckvars].type = CV_CONSTANT;
						continue;
					}
					else if ( *cp == 'R')
					{
						checkvarList[maxcheckvars].type = CV_RANGE;
						continue;
					}
						
					cp += 3;
					SKIPBLANK(cp);
					
					if ( *cp != '\"' )
						{
							gi.dprintf ("Error loading CHECKVAR from line %d in file %s, \" not found\n", uptoLine, checkvarname);
							continue;
						}
						
					cp++;
					cp = processstring(checkvarList[maxcheckvars].variablename, cp, sizeof(checkvarList[maxcheckvars].variablename) - 1, '\"');
					
					// make sure you are at the end quote
					while(*cp && *cp != '\"')
						{
							cp++;
						}
						
					if ( *cp == 0 )
						{
							gi.dprintf ("Error loading CHECKVAR from line %d in file %s, unexcepted end of line\n", uptoLine, checkvarname);
							continue;
						}
						
					cp++;
					
					if ( isBlank ( checkvarList[maxcheckvars].variablename ) )
						{
							gi.dprintf ("Error loading CHECKVAR from line %d in file %s, blank variable name\n", uptoLine, checkvarname);
							continue;
						}
						
						
					SKIPBLANK(cp);
					
					if (checkvarList[maxcheckvars].type == CV_CONSTANT)
						{
							if ( *cp != '\"' )
								{
									gi.dprintf ("Error loading CHECKVAR from line %d in file %s, \" not found\n", uptoLine, checkvarname);
									continue;
								}
								
							cp++;
							cp = processstring(checkvarList[maxcheckvars].value, cp, sizeof(checkvarList[maxcheckvars].value) - 1, '\"');
							continue;
						}	
					else if (checkvarList[maxcheckvars].type == CV_RANGE)
						{
							char rangevalue[50];
								
							if ( *cp != '\"' )
							{
								gi.dprintf ("Error loading CHECKVAR from line %d in file %s, \" not found\n", uptoLine, checkvarname);
								continue;
							}
									
							cp++;
							cp = processstring(rangevalue, cp, sizeof(rangevalue) - 1, '\"');
								
							checkvarList[maxcheckvars].lower = q2a_atof(rangevalue);
								
							while(*cp && *cp != '\"')
							{
								cp++;
							}
									
							if ( *cp == 0 )
							{
								gi.dprintf ("Error loading CHECKVAR from line %d in file %s, unexcepted end of line\n", uptoLine, checkvarname);
								continue;
							}
									
							cp++;
							SKIPBLANK(cp);
								
							if ( *cp != '\"' )
							{
								gi.dprintf ("Error loading CHECKVAR from line %d in file %s, \" not found\n", uptoLine, checkvarname);
								continue;
							}
									
							cp++;
							cp = processstring(rangevalue, cp, sizeof(rangevalue) - 1, '\"');
								
							checkvarList[maxcheckvars].upper = q2a_atof(rangevalue);
								
							continue;
						}
						
					maxcheckvars++;
					
					if(maxcheckvars >= CHECKVAR_MAX)
						{
							break;
						}
				}
			else if(!(cp[0] == ';' || cp[0] == '\n' || isBlank (cp) ))
				{
					gi.dprintf ("Error loading CHECKVAR from line %d in file %s\n", uptoLine, checkvarname);
				}
		}
		
	fclose(checkvarfile);
	
	return TRUE;
}

void readCheckVarLists(void)
{
	qboolean ret;
	
	maxcheckvars = 0;
	ret = ReadCheckVarFile(CHECKVARFILE);
	
	sprintf(buffer, "%s/%s", moddir, CHECKVARFILE);
	if(ReadCheckVarFile(buffer))
		{
			ret = TRUE;
		}
		
	if(!ret)
		{
			gi.dprintf ("WARNING: " CHECKVARFILE " could not be found\n");
			logEvent(LT_INTERNALWARN, 0, NULL, CHECKVARFILE " could not be found", IW_CHECKVARSETUPLOAD, 0.0);
		}
}

void reloadCheckVarFileRun(int startarg, edict_t *ent, int client)
{
	readCheckVarLists();
	gi.cprintf (ent, PRINT_HIGH, "Check-Variables reloaded.\n");
}

void checkVariableTest(edict_t *ent, int client, int idx)
{
	if (checkvarcmds_enable)
		{
			if ( idx >= maxcheckvars )
				{
					idx = 0;
				}
				
			if ( maxcheckvars )
				{
					proxyinfo[client].checkvar_idx = idx;
					generateRandomString(proxyinfo[client].hack_checkvar, RANDOM_STRING_LENGTH);
					sprintf(buffer, "%s $%s\n", proxyinfo[client].hack_checkvar, checkvarList[idx].variablename);
					stuffcmd(ent, buffer);
					
					idx++;
				}
				
		}
	else
		{
			idx = 0;
		}
		
	addCmdQueue(client, QCMD_CHECKVARTESTS, (float)checkvar_poll_time, idx, 0);
}

void checkVariableValid(edict_t *ent, int client, char *value)
{
	switch(checkvarList[proxyinfo[client].checkvar_idx].type)
		{
		case CV_CONSTANT:
			if ( q2a_strcmp(checkvarList[proxyinfo[client].checkvar_idx].value, value) != 0)
				{
					sprintf(buffer, "%s %s\n", checkvarList[proxyinfo[client].checkvar_idx].variablename, checkvarList[proxyinfo[client].checkvar_idx].value);
					stuffcmd(ent, buffer);
				}
			break;
			
		case CV_RANGE:
			{
				double fvalue = q2a_atof(value);
				
				if ( fvalue < checkvarList[proxyinfo[client].checkvar_idx].lower )
					{
						sprintf(buffer, "%s %g\n", checkvarList[proxyinfo[client].checkvar_idx].variablename, checkvarList[proxyinfo[client].checkvar_idx].lower);
						stuffcmd(ent, buffer);
					}
				else if ( fvalue > checkvarList[proxyinfo[client].checkvar_idx].upper )
					{
						sprintf(buffer, "%s %g\n", checkvarList[proxyinfo[client].checkvar_idx].variablename, checkvarList[proxyinfo[client].checkvar_idx].upper);
						stuffcmd(ent, buffer);
					}
				break;
			}
		}
}

//===================================================================

void listcheckvarsRun(int startarg, edict_t *ent, int client)
{
	addCmdQueue(client, QCMD_DISPCHECKVAR, 0, 0, 0);
	
	gi.cprintf (ent, PRINT_HIGH, "Start Check-Variables List:\n");
}

void displayNextCheckvar(edict_t *ent, int client, long checkvarcmd)
{
	if(checkvarcmd < maxcheckvars)
		{
			switch(checkvarList[checkvarcmd].type)
				{
				case CV_CONSTANT:
					gi.cprintf (ent, PRINT_HIGH, "%4d CT: %s = \"%s\"\n", checkvarcmd + 1, checkvarList[checkvarcmd].variablename, checkvarList[checkvarcmd].value);
					break;
					
				case CV_RANGE:
					gi.cprintf (ent, PRINT_HIGH, "%4d RG: %s = %g to %g\n", checkvarcmd + 1, checkvarList[checkvarcmd].variablename, checkvarList[checkvarcmd].lower, checkvarList[checkvarcmd].upper);
					break;
				}
			checkvarcmd++;
			addCmdQueue(client, QCMD_DISPCHECKVAR, 0, checkvarcmd, 0);
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "End Check-Variables List\n");
		}
}

#define CHECKVARCMD     "[sv] !checkvarcmd [CT/RG] \"variable\" [\"value\" | \"lower\" \"upper\"]\n"

void checkvarcmdRun(int startarg, edict_t *ent, int client)
{
	char *cmd;
	
	if(maxcheckvars >= CHECKVAR_MAX)
		{
			gi.cprintf (ent, PRINT_HIGH, "Sorry, maximum number of variables to check has been reached.\n");
			return;
		}
		
	if (gi.argc() <= startarg + 2)
		{
			gi.cprintf (ent, PRINT_HIGH, CHECKVARCMD);
			return;
		}
		
	cmd = gi.argv(startarg);
	startarg++;
	
	if(Q_stricmp(cmd, "CT") == 0)
		{
			checkvarList[maxcheckvars].type = CV_CONSTANT;
		}
	else if(Q_stricmp(cmd, "RG") == 0)
		{
			checkvarList[maxcheckvars].type = CV_RANGE;
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, CHECKVARCMD);
			return;
		}
		
	cmd = gi.argv(startarg);
	startarg++;
	
	if(isBlank(cmd))
		{
			gi.cprintf (ent, PRINT_HIGH, CHECKVARCMD);
			return;
		}
		
	processstring(checkvarList[maxcheckvars].variablename, cmd, sizeof(checkvarList[maxcheckvars].variablename) - 1, 0);
	
	switch(checkvarList[maxcheckvars].type)
		{
		case CV_CONSTANT:
			cmd = gi.argv(startarg);
			processstring(checkvarList[maxcheckvars].value, cmd, sizeof(checkvarList[maxcheckvars].value) - 1, 0);
			break;
			
		case CV_RANGE:
			cmd = gi.argv(startarg);
			startarg++;
			checkvarList[maxcheckvars].lower = q2a_atof(cmd);
			
			if(gi.argc() <= startarg)
				{
					gi.cprintf (ent, PRINT_HIGH, CHECKVARCMD);
					return;
				}
				
			cmd = gi.argv(startarg);
			checkvarList[maxcheckvars].upper = q2a_atof(cmd);
			break;
		}
		
	switch(checkvarList[maxcheckvars].type)
		{
		case CV_CONSTANT:
			gi.cprintf (ent, PRINT_HIGH, "%4d CT: %s = \"%s\"\n", maxcheckvars + 1, checkvarList[maxcheckvars].variablename, checkvarList[maxcheckvars].value);
			break;
			
		case CV_RANGE:
			gi.cprintf (ent, PRINT_HIGH, "%4d RG: %s = %g to %g\n", maxcheckvars + 1, checkvarList[maxcheckvars].variablename, checkvarList[maxcheckvars].lower, checkvarList[maxcheckvars].upper);
			break;
		}
		
	maxcheckvars++;
}

#define CHECKVARDELCMD     "[sv] !checkvardel checkvarnum\n"

void checkvarDelRun(int startarg, edict_t *ent, int client)
{
	int checkvar;
	
	if (gi.argc() <= startarg)
		{
			gi.cprintf (ent, PRINT_HIGH, CHECKVARDELCMD);
			return;
		}
		
	checkvar = q2a_atoi(gi.argv(startarg));
	
	if(checkvar < 1 || checkvar > maxcheckvars)
		{
			gi.cprintf (ent, PRINT_HIGH, CHECKVARDELCMD);
			return;
		}
		
	checkvar--;
	
	if(checkvar + 1 < maxcheckvars)
		{
			q2a_memmove((checkvarList + checkvar), (checkvarList + checkvar + 1), sizeof(checkvar_t) * (maxcheckvars - checkvar));
		}
		
	maxcheckvars--;
	
	gi.cprintf (ent, PRINT_HIGH, "Check-Variable command deleted\n");
}
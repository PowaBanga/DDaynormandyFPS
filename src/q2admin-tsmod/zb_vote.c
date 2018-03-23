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
// zb_vote.c
//
// copyright 2000 Shane Powell
//

#include "g_local.h"

#define VOTEFILE             "q2adminvote.txt"
#define VOTE_MAXCMDS         1024

typedef struct
	{
		char   *votecmd;
		byte    type;
		regex_t *r;
	}
votecmd_t;

#define VOTE_SW  0
#define VOTE_EX  1
#define VOTE_RE  2

votecmd_t votecmds[VOTE_MAXCMDS];
int maxvote_cmds = 0;

qboolean votecountnovotes = 1;
int votepasspercent = 50;
int voteminclients = 0;


static qboolean voteinprogress = 0;
static unsigned long votetimeout, voteremindtimeout;
char cmdvote[2048];

//*** UPDATE START ***
char cmdpassedvote[2048];
char votecaller[16];
//*** UPDATE END ***

int clientVoteTimeout = 60;
int clientRemindTimeout = 10;
int clientMaxVoteTimeout = 0;
int clientMaxVotes = 0;



qboolean ReadVoteFile(char *votename)
{
	FILE *votefile;
	unsigned int uptoLine = 0;
	
	if(maxvote_cmds >= VOTE_MAXCMDS)
		{
			return FALSE;
		}
		
	votefile = fopen(votename, "rt");
	if(!votefile)
		{
			return FALSE;
		}
		
	while(fgets(buffer, 256, votefile))
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
							votecmds[maxvote_cmds].type = VOTE_SW;
							break;
							
						case 'E':
							votecmds[maxvote_cmds].type = VOTE_EX;
							break;
							
						case 'R':
							votecmds[maxvote_cmds].type = VOTE_RE;
							break;
						}
						
					cp += 3;
					SKIPBLANK(cp);
					
					len = q2a_strlen(cp) + 1;
					
					// zero length command
					if(!len)
						{
							gi.dprintf ("Error loading VOTE from line %d in file %s\n", uptoLine, votename);
							continue;
						}
						
					votecmds[maxvote_cmds].votecmd = gi.TagMalloc (len, TAG_LEVEL);
					q2a_strcpy(votecmds[maxvote_cmds].votecmd, cp);
					
					if(votecmds[maxvote_cmds].type == VOTE_RE)
						{
							q_strupr(cp);
							
							votecmds[maxvote_cmds].r = gi.TagMalloc (sizeof(*votecmds[maxvote_cmds].r), TAG_LEVEL);
							q2a_memset(votecmds[maxvote_cmds].r, 0x0, sizeof(*votecmds[maxvote_cmds].r));
							//        if(regcomp(votecmds[maxvote_cmds].r, strbuffer, REG_EXTENDED))
							if(regcomp(votecmds[maxvote_cmds].r, cp, 0))
								{
									gi.TagFree(votecmds[maxvote_cmds].r);
									votecmds[maxvote_cmds].r = 0;
									
									// malformed re... skip this vote command
									gi.dprintf ("Error loading VOTE from line %d in file %s\n", uptoLine, votename);
									continue;
								}
						}
					else
						{
							votecmds[maxvote_cmds].r = 0;
						}
						
					maxvote_cmds++;
					
					if(maxvote_cmds >= VOTE_MAXCMDS)
						{
							break;
						}
				}
			else if(!(cp[0] == ';' || cp[0] == '\n' || isBlank (cp)))
				{
					gi.dprintf ("Error loading VOTE from line %d in file %s\n", uptoLine, votename);
				}
		}
		
	fclose(votefile);
	
	return TRUE;
}



void freeVoteLists(void)
{
	while(maxvote_cmds)
		{
			maxvote_cmds--;
			gi.TagFree(votecmds[maxvote_cmds].votecmd);
			if(votecmds[maxvote_cmds].r)
				{
					regfree(votecmds[maxvote_cmds].r);
					gi.TagFree(votecmds[maxvote_cmds].r);
				}
		}
}

void readVoteLists(void)
{
	qboolean ret;
	
	freeVoteLists();
	
	ret = ReadVoteFile(VOTEFILE);
	
	sprintf(buffer, "%s/%s", moddir, VOTEFILE);
	if(ReadVoteFile(buffer))
		{
			ret = TRUE;
		}
		
	if(!ret)
		{
			gi.dprintf ("WARNING: " VOTEFILE " could not be found\n");
			logEvent(LT_INTERNALWARN, 0, NULL, VOTEFILE " could not be found", IW_VOTESETUPLOAD, 0.0);
		}
}



void reloadVoteFileRun(int startarg, edict_t *ent, int client)
{
	readVoteLists();
	gi.cprintf (ent, PRINT_HIGH, "Disbled entities reloaded.\n");
}


qboolean checkforvotecmd(char *cp, int votecmd)
{
	switch(votecmds[votecmd].type)
		{
		case VOTE_SW:
			return startContains (cp, votecmds[votecmd].votecmd);
			
		case VOTE_EX:
			return !Q_stricmp(cp, votecmds[votecmd].votecmd);
			
		case VOTE_RE:
			return (regexec(votecmds[votecmd].r, cp, 0, 0, 0) != REG_NOMATCH);
		}
		
	return FALSE;
}


qboolean checkVoteCommand(char *cp)
{
	unsigned int i;
	
	q2a_strcpy(buffer, cp);
	q_strupr(buffer);
	
	for(i = 0; i < maxvote_cmds; i++)
		{
			if(checkforvotecmd(buffer, i))
				{
					return TRUE;
				}
		}
		
	return FALSE;
}



//===================================================================






void listvotesRun(int startarg, edict_t *ent, int client)
{
	addCmdQueue(client, QCMD_DISPVOTE, 0, 0, 0);
	
	gi.cprintf (ent, PRINT_HIGH, "Start Vote Command List:\n");
}


void displayNextVote(edict_t *ent, int client, long votecmd)
{
	if(votecmd < maxvote_cmds)
		{
			switch(votecmds[votecmd].type)
				{
				case VOTE_SW:
					gi.cprintf (ent, PRINT_HIGH, "%4d SW:\"%s\"\n", votecmd + 1, votecmds[votecmd].votecmd);
					break;
					
				case VOTE_EX:
					gi.cprintf (ent, PRINT_HIGH, "%4d EX:\"%s\"\n", votecmd + 1, votecmds[votecmd].votecmd);
					break;
					
				case VOTE_RE:
					gi.cprintf (ent, PRINT_HIGH, "%4d RE:\"%s\"\n", votecmd + 1, votecmds[votecmd].votecmd);
					break;
				}
			votecmd++;
			addCmdQueue(client, QCMD_DISPVOTE, 0, votecmd, 0);
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "End Vote Command List\n");
		}
}



#define VOTECMD     "[sv] !votecmd [SW/EX/RE] \"command\"\n"

void votecmdRun(int startarg, edict_t *ent, int client)
{
	char *cmd;
	int len;
	
	if(maxvote_cmds >= VOTE_MAXCMDS)
		{
			gi.cprintf (ent, PRINT_HIGH, "Sorry, maximum number of vote commands has been reached.\n");
			return;
		}
		
	if (gi.argc() <= startarg + 1)
		{
			gi.cprintf (ent, PRINT_HIGH, VOTECMD);
			return;
		}
		
	cmd = gi.argv(startarg);
	
	if(Q_stricmp(cmd, "SW") == 0)
		{
			votecmds[maxvote_cmds].type = VOTE_SW;
		}
	else if(Q_stricmp(cmd, "EX") == 0)
		{
			votecmds[maxvote_cmds].type = VOTE_EX;
		}
	else if(Q_stricmp(cmd, "RE") == 0)
		{
			votecmds[maxvote_cmds].type = VOTE_RE;
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, VOTECMD);
			return;
		}
		
	cmd = gi.argv(startarg + 1);
	
	if(isBlank(cmd))
		{
			gi.cprintf (ent, PRINT_HIGH, VOTECMD);
			return;
		}
		
	len = q2a_strlen(cmd) + 20;
	
	votecmds[maxvote_cmds].votecmd = gi.TagMalloc (len, TAG_LEVEL);
	processstring(votecmds[maxvote_cmds].votecmd, cmd, len - 1, 0);
	//  q2a_strcpy(votecmds[maxvote_cmds].votecmd, cmd);
	
	if(votecmds[maxvote_cmds].type == VOTE_RE)
		{
			q_strupr(cmd);
			
			votecmds[maxvote_cmds].r = gi.TagMalloc (sizeof(*votecmds[maxvote_cmds].r), TAG_LEVEL);
			q2a_memset(votecmds[maxvote_cmds].r, 0x0, sizeof(*votecmds[maxvote_cmds].r));
			//        if(regcomp(votecmds[maxvote_cmds].r, cmd, REG_EXTENDED))
			if(regcomp(votecmds[maxvote_cmds].r, cmd, 0))
				{
					gi.TagFree(votecmds[maxvote_cmds].votecmd);
					gi.TagFree(votecmds[maxvote_cmds].r);
					votecmds[maxvote_cmds].r = 0;
					
					// malformed re...
					gi.cprintf (ent, PRINT_HIGH, "Regular expression couldn't compile!\n");
					return;
				}
		}
	else
		{
			votecmds[maxvote_cmds].r = 0;
		}
		
	switch(votecmds[maxvote_cmds].type)
		{
		case VOTE_SW:
			gi.cprintf (ent, PRINT_HIGH, "%4d SW:\"%s\" added\n", maxvote_cmds + 1, votecmds[maxvote_cmds].votecmd);
			break;
			
		case VOTE_EX:
			gi.cprintf (ent, PRINT_HIGH, "%4d EX:\"%s\" added\n", maxvote_cmds + 1, votecmds[maxvote_cmds].votecmd);
			break;
			
		case VOTE_RE:
			gi.cprintf (ent, PRINT_HIGH, "%4d RE:\"%s\" added\n", maxvote_cmds + 1, votecmds[maxvote_cmds].votecmd);
			break;
		}
		
	maxvote_cmds++;
}



#define VOTEDELCMD     "[sv] !votedel votenum\n"


void voteDelRun(int startarg, edict_t *ent, int client)
{
	int vote;
	
	if (gi.argc() <= startarg)
		{
			gi.cprintf (ent, PRINT_HIGH, VOTEDELCMD);
			return;
		}
		
	vote = q2a_atoi(gi.argv(startarg));
	
	if(vote < 1 || vote > maxvote_cmds)
		{
			gi.cprintf (ent, PRINT_HIGH, VOTEDELCMD);
			return;
		}
		
	vote--;
	
	gi.TagFree(votecmds[vote].votecmd);
	if(votecmds[vote].r)
		{
			regfree(votecmds[vote].r);
			gi.TagFree(votecmds[vote].r);
		}
		
	if(vote + 1 < maxvote_cmds)
		{
			q2a_memmove((votecmds + vote), (votecmds + vote + 1), sizeof(votecmd_t) * (maxvote_cmds - vote));
		}
		
	maxvote_cmds--;
	
	gi.cprintf (ent, PRINT_HIGH, "Vote command deleted\n");
}



void displayVote(void)
{
	int client;
	unsigned int maxclientsused = 0, voteyes = 0, voteno = 0, novote = 0;
	
	// count votes
	for(client = 0; client < maxclients->value; client++)
		{
			if(proxyinfo[client].inuse)
				{
					maxclientsused++;
					
					if(proxyinfo[client].clientcommand & CCMD_VOTED)
						{
							if(proxyinfo[client].clientcommand & CCMD_VOTEYES)
								{
									voteyes++;
								}
							else
								{
									voteno++;
								}
						}
					else
						{
							novote++;
						}
				}
		}
		
	for(client = 0; client < maxclients->value; client++)
		{
			if(proxyinfo[client].inuse)
				{
					if(proxyinfo[client].clientcommand & CCMD_VOTED)
						{
							// just display the stat's
							gi.centerprintf(getEnt((client + 1)), "Vote Summary So Far:\n"
											"Proposed Vote: %s by %s\n"	//UPDATE
											"Voted Yes: %d    Voted No: %d\n"
											"Haven't Voted Yet: %d\n", cmdvote, votecaller, voteyes, voteno, novote);	//UPDATE
						}
					else
						{
							// format the remember and the stat's
							gi.centerprintf(getEnt((client + 1)), "You haven't voted yet. To vote at the \n"
											"console type '%s yes' or '%s no'\n"
											"\n"
											"Vote Summary So Far:\n"
											"Proposed Vote: %s by %s\n"	//UPDATE
											"Voted Yes: %d    Voted No: %d\n"
											"Haven't Voted Yet: %d\n", clientVoteCommand, clientVoteCommand,
											cmdvote, votecaller, voteyes, voteno, novote);	//UPDATE
						}
				}
		}
}




void run_vote(edict_t *ent, int client)
{
	char *votecmd;
	
	if(gi.argc() <= 1)
		{
			// menu driven interface...
			if(voteinprogress)
				{
					int clienti;
					int maxclientsused = 0, voteyes = 0, voteno = 0, novote = 0;
					
					// count votes
					for(clienti = 0; clienti < maxclients->value; clienti++)
						{
							if(proxyinfo[clienti].inuse)
								{
									maxclientsused++;
									
									if(proxyinfo[clienti].clientcommand & CCMD_VOTED)
										{
											if(proxyinfo[clienti].clientcommand & CCMD_VOTEYES)
												{
													voteyes++;
												}
											else
												{
													voteno++;
												}
										}
									else
										{
											novote++;
										}
								}
						}
						
					if(proxyinfo[client].clientcommand & CCMD_VOTED)
						{
							// just display the stat's
							gi.centerprintf(ent, "Vote Summary So Far:\n"
											"Proposed Vote: %s\n"
											"Voted Yes: %d    Voted No: %d\n"
											"Haven't Voted Yet: %d\n", cmdvote, voteyes, voteno, novote);
						}
					else
						{
							// format the remember and the stat's
							gi.centerprintf(ent, "You haven't voted yet. To vote at the \n"
											"console type '%s yes' or '%s no'\n"
											"\n"
											"Vote Summary So Far:\n"
											"Proposed Vote: %s\n"
											"Voted Yes: %d    Voted No: %d\n"
											"Haven't Voted Yet: %d\n", clientVoteCommand, clientVoteCommand,
											cmdvote, voteyes, voteno, novote);
						}
				}
			else
				{
					gi.centerprintf(ent, "To propose a vote type in \n%s <cmd>\n at the console.\n", clientVoteCommand);
				}
			return;
		}
		
	votecmd = gi.args();
	
	if(q2a_strchr(votecmd, ';'))
		{
			gi.cprintf(ent, PRINT_HIGH, "Invalid vote command!\n");
			return;
		}
		
	SKIPBLANK(votecmd);
	
	if(startContains (votecmd, "YES"))
		{
			if(voteinprogress)
				{
					proxyinfo[client].clientcommand |= (CCMD_VOTEYES | CCMD_VOTED);
					gi.cprintf(ent, PRINT_HIGH, "You have voted: YES\n");
				}
			else
				{
					gi.cprintf(ent, PRINT_HIGH, "There is no vote in progress!\n");
				}
			return;
		}
	else if(startContains (votecmd, "NO"))
		{
			if(voteinprogress)
				{
					proxyinfo[client].clientcommand |= CCMD_VOTED;
					proxyinfo[client].clientcommand &= ~CCMD_VOTEYES;
					gi.cprintf(ent, PRINT_HIGH, "You have voted: NO\n");
				}
			else
				{
					gi.cprintf(ent, PRINT_HIGH, "There is no vote in progress!\n");
				}
			return;
		}
		
	if(voteinprogress)
		{
			gi.cprintf(ent, PRINT_HIGH, "There is already a vote in progress!\n");
			return;
		}
		
	if(checkVoteCommand(votecmd))
		{
			if(voteminclients)
				{
					int client;
					int maxclientsingame = 0;
					
					// count number of clients
					for(client = 0; client < maxclients->value; client++)
						{
							if(proxyinfo[client].inuse)
								{
									maxclientsingame++;
								}
						}
						
					if(voteminclients > maxclientsingame)
						{
							gi.cprintf(ent, PRINT_HIGH, "Not enough people to vote.\n");
							return;
						}
				}
				
				
			// check if allowed to vote at this time...
			if(clientMaxVotes)
				{
					//*** UPDATE START *** - Nick
					if (ltime <= 45)
					{
						gi.cprintf(ent, PRINT_HIGH, "Recent map change - too soon to vote (please wait).\n");
						return;
					}
					//*** UPDATE END ***
					if(proxyinfo[client].votescast == -1)
						{
							// not allowed to vote again..
							gi.cprintf(ent, PRINT_HIGH, "You can't propose any more votes until the next level.\n");
							return;
						}
						
					// started counting votes?
					if(proxyinfo[client].votetimeout > ltime || clientMaxVoteTimeout == 0)
						{
							// exceeded maximum votes allowed?
							if(proxyinfo[client].votescast >= clientMaxVotes)
								{
									int secleft = (int)(proxyinfo[client].votetimeout - ltime) + 1;
									
									gi.cprintf (ent, PRINT_HIGH, "You can't propose any more votes for %d seconds.\n", secleft);
									return;
								}
							else
								{
									proxyinfo[client].votescast++;
									
									// if they proposed the last vote for the level?
									if(clientMaxVoteTimeout == 0 && proxyinfo[client].votescast >= clientMaxVotes)
										{
											proxyinfo[client].votescast = -1;
										}
								}
						}
					else
						{
							// first vote for the timeout period.
							proxyinfo[client].votescast = 1;
							proxyinfo[client].votetimeout = ltime + clientMaxVoteTimeout;
						}
				}
				
			voteinprogress = 1;
			votetimeout = ltime + clientVoteTimeout;
			voteremindtimeout = ltime + clientRemindTimeout;
			proxyinfo[client].clientcommand |= (CCMD_VOTEYES | CCMD_VOTED);
			q2a_strcpy(cmdvote, votecmd);
			q2a_strcat(cmdvote, "\n");
			//*** UPDATE START ***
			q2a_strcpy(votecaller,proxyinfo[client].name);
			q2a_strcat(votecaller, "\n");
			//*** UPDATE END ***
			
			displayVote();
		}
	else
		{
			gi.cprintf(ent, PRINT_HIGH, "Invalid vote command specified.\n");
		}
}







void checkOnVoting(void)
{
	int client;
	unsigned int maxclientsused = 0, voteyes = 0, voteno = 0, novote = 0;
	double percent;
	char printstr[100];
	
	if(voteinprogress)
		{
			// count votes and run vote command if successful
			for(client = 0; client < maxclients->value; client++)
				{
					if(proxyinfo[client].inuse)
						{
							if(!(proxyinfo[client].clientcommand & CCMD_VOTED))
								{
									break;
								}
						}
				}
				
			if(votetimeout < ltime || client >= maxclients->value)
				{
					voteinprogress = 0;
					
					// count votes and run vote command if successful
					for(client = 0; client < maxclients->value; client++)
						{
							if(proxyinfo[client].inuse)
								{
									maxclientsused++;
									
									if(proxyinfo[client].clientcommand & CCMD_VOTED)
										{
											if(proxyinfo[client].clientcommand & CCMD_VOTEYES)
												{
													voteyes++;
												}
											else
												{
													voteno++;
												}
										}
									else
										{
											novote++;
										}
								}
								
							proxyinfo[client].clientcommand &= ~(CCMD_VOTEYES | CCMD_VOTED);
						}
						
					percent = ((double)voteyes / ((double)maxclientsused - ((double)votecountnovotes ? 0.0 : novote)));
					
					if(percent >= ((double)votepasspercent / 100))
						{
							q2a_strcpy(printstr, "Vote PASSED!");
							q2a_strcpy(cmdpassedvote, cmdvote); //UPDATE
							addCmdQueue(-1, QCMD_RUNVOTECMD, 5, 0, 0);
						}
					else
						{
							q2a_strcpy(printstr, "Vote FAILED!");
						}
						
					for(client = 0; client < maxclients->value; client++)
						{
							if(proxyinfo[client].inuse)
								{
									gi.centerprintf(getEnt((client + 1)), "%s\n"
													"\n"
													"Vote Summary:\n"
													"Proposed Vote: %s\n"
													"Voted Yes: %d    Voted No: %d\n"
													"Didn't Vote: %d\n", printstr, cmdvote, voteyes, voteno, novote);
								}
						}
				}
			else if(voteremindtimeout < ltime)
				{
					voteremindtimeout = ltime + clientRemindTimeout;
					displayVote();
				}
		}
}

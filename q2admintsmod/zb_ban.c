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
// zb_ban.c
//
// copyright 2000 Shane Powell
//

#include "g_local.h"



baninfo_t *banhead;
chatbaninfo_t *chatbanhead;


qboolean IPBanning_Enable = FALSE;
qboolean NickBanning_Enable = FALSE;
qboolean ChatBanning_Enable = TRUE;  // %%quadz -- was: FALSE
qboolean kickOnNameChange = FALSE;


char defaultBanMsg[256];
char *currentBanMsg;


long banNumUpto = 0;
long chatBanNumUpto = 0;
char defaultChatBanMsg[256];

qboolean ReadBanFile(char *bfname)
{
	FILE *banfile;
	baninfo_t *newentry;
	chatbaninfo_t *cnewentry;
	char strbuffer[256];
	unsigned int uptoLine = 0;
	
	banfile = fopen(bfname, "rt");
	if(!banfile)
		{
			return FALSE;
		}
		
	while(fgets(buffer, 256, banfile))
		{
			char *cp = buffer;
			int num;
			unsigned int i;
			qboolean like, re, all;
			
			SKIPBLANK(cp);
			
			uptoLine++;
			
			if(!(cp[0] == ';' || cp[0] == '\n' || isBlank (cp)))
				{
					if(startContains(cp, "BAN:"))
						{
							// create include / exclude ban.
							// BAN: [+/-(-)] [ALL/[NAME [LIKE/RE] "name"/BLANK/ALL(ALL)] [IP xxx[.xxx(0)[.xxx(0)[.xxx(0)]]][/yy(32)]] [PASSWORD "xxx"] [MAX 0-xxx(0)] [FLOOD xxx xxx xxx] [MSG "xxx"]
							
							// allocate memory for ban record
							newentry = gi.TagMalloc (sizeof(baninfo_t), TAG_LEVEL);
							
							newentry->loadType = LT_PERM;
							newentry->timeout = 0.0;
							newentry->r = 0;
							
							cp += 4;
							
							SKIPBLANK(cp);
							
							// get +/-
							if(*cp == '-')
								{
									cp++;
									SKIPBLANK(cp);
									newentry->exclude = TRUE;
								}
							else if(*cp == '+')
								{
									cp++;
									SKIPBLANK(cp);
									newentry->exclude = FALSE;
								}
							else
								{
									newentry->exclude = TRUE;
								}
								
							if(startContains(cp, "ALL"))
								{
									newentry->type = NICKALL;
									newentry->ip[0] = 0;
									newentry->ip[1] = 0;
									newentry->ip[2] = 0;
									newentry->ip[3] = 0;
									newentry->subnetmask = 0;
									newentry->maxnumberofconnects = 0;
									newentry->numberofconnects = 0;
									newentry->msg = NULL;
									all = TRUE;
									
									cp += 3;
									SKIPBLANK(cp);
								}
							else
								{
									all = FALSE;
									
									// Name:
									if(startContains(cp, "NAME"))
										{
											cp += 4;
											SKIPBLANK(cp);
											
											// Like?
											if(startContains(cp, "LIKE"))
												{
													cp += 4;
													like = TRUE;
													re = FALSE;
													SKIPBLANK(cp);
													
												} //re?
											else if(startContains(cp, "RE"))
												{
													cp += 2;
													like = FALSE;
													re = TRUE;
													SKIPBLANK(cp);
												}
											else
												{
													like = FALSE;
													re = FALSE;
												}
												
											// BLANK or ALL or name
											if(startContains(cp, "BLANK"))
												{
													cp += 5;
													newentry->type = NICKBLANK;
												}
											else if(startContains(cp, "ALL"))
												{
													cp += 3;
													newentry->type = NICKALL;
												}
											else if(*cp == '\"')
												{
													if(like)
														{
															newentry->type = NICKLIKE;
														}
													else if(re)
														{
															newentry->type = NICKRE;
														}
													else
														{
															newentry->type = NICKEQ;
														}
														
													// copy name
													cp++;
													cp = processstring(newentry->nick, cp, sizeof(newentry->nick) - 1, '\"');
													
													// make sure you are at the end quote
													while(*cp && *cp != '\"')
														{
															cp++;
														}
														
													cp++;
												}
											else
												{
													newentry->type = NOTUSED;
												}
												
												
											if(newentry->type == NICKRE)
												{ // compile RE
													q2a_strcpy(strbuffer, newentry->nick);
													q_strupr(strbuffer);
													newentry->r = gi.TagMalloc (sizeof(*newentry->r), TAG_LEVEL);
													q2a_memset(newentry->r, 0x0, sizeof(*newentry->r));
													//              if(regcomp(newentry->r, strbuffer, REG_EXTENDED))
													if(regcomp(newentry->r, strbuffer, 0))
														{
															gi.TagFree(newentry->r);
															newentry->r = 0;
														}
												}
												
												
											SKIPBLANK(cp);
										}
									else
										{
											newentry->type = NICKALL;
										}
										
									// get ip address
									newentry->ip[0] = 0;
									newentry->ip[1] = 0;
									newentry->ip[2] = 0;
									newentry->ip[3] = 0;
									
									if(startContains(cp, "IP"))
										{
											cp += 2;
											
											SKIPBLANK(cp);
											
											if(isdigit(*cp))
												{
													for(i = 0; i < 4; i++)
														{
															num = q2a_atoi(cp);
															
															if(num > 255)
																{
																	num = 255;
																}
																
															newentry->ip[i] = num;
															
															while(isdigit(*cp))
																{
																	cp++;
																}
																
															if(*cp == '.')
																{
																	cp++;
																}
															else
																{
																	break;
																}
														}
														
													if(*cp == '/')
														{
															cp++;
															newentry->subnetmask = q2a_atoi(cp);
															
															while(isdigit(*cp))
																{
																	cp++;
																}
														}
													else
														{
															newentry->subnetmask = 32;
														}
												}
												
											SKIPBLANK(cp);
											/*
											// get MASK
											if(startContains(cp, "MASK"))
											{
											cp += 4;
											 
											SKIPBLANK(cp);
											 
											newentry->subnetmask = q2a_atoi(cp);
											 
											if(newentry->subnetmask > 32)
											{
											newentry->subnetmask = 32;
											}
											 
											while(isdigit(*cp))
											{
											cp++;
											}
											 
											SKIPBLANK(cp);
											}
											else
											{
											newentry->subnetmask = 32;
											}
											*/
										}
									else
										{
											newentry->subnetmask= 0;
										}
								}
								
								
							// get PASSWORD
							if(!newentry->exclude && startContains(cp, "PASSWORD"))
								{
									cp += 8;
									
									SKIPBLANK(cp);
									
									if(*cp == '\"')
										{
											// copy password
											
											cp++;
											
											cp = processstring(newentry->password, cp, sizeof(newentry->password) - 1, '\"');
											
											// make sure you are at the end quote
											while(*cp && *cp != '\"')
												{
													cp++;
												}
												
											cp++;
										}
									else
										{
											newentry->type = NOTUSED;
										}
										
									SKIPBLANK(cp);
								}
							else
								{
									newentry->password[0] = 0;
								}
								
							// get MAX
							if(!newentry->exclude && startContains(cp, "MAX"))
								{
									cp += 3;
									
									SKIPBLANK(cp);
									
									newentry->maxnumberofconnects = q2a_atoi(cp);
									
									while(isdigit(*cp))
										{
											cp++;
										}
										
									SKIPBLANK(cp);
								}
							else
								{
									newentry->maxnumberofconnects = 0;
								}
								
							newentry->numberofconnects = 0;
							
							// get FLOOD
							if(!newentry->exclude && startContains(cp, "FLOOD"))
								{
									cp += 5;
									
									SKIPBLANK(cp);
									
									newentry->floodinfo.chatFloodProtectNum = q2a_atoi(cp);
									
									while(isdigit(*cp))
										{
											cp++;
										}
										
									SKIPBLANK(cp);
									
									newentry->floodinfo.chatFloodProtectSec = q2a_atoi(cp);
									
									while(isdigit(*cp))
										{
											cp++;
										}
										
									SKIPBLANK(cp);
									
									newentry->floodinfo.chatFloodProtectSilence = q2a_atoi(cp);
									
									if(*cp == '-')
										{
											cp++;
										}
										
									while(isdigit(*cp))
										{
											cp++;
										}
										
									SKIPBLANK(cp);
									
									if(newentry->floodinfo.chatFloodProtectNum && newentry->floodinfo.chatFloodProtectSec)
										{
											newentry->floodinfo.chatFloodProtect = TRUE;
										}
									else
										{
											newentry->floodinfo.chatFloodProtect = FALSE;
										}
								}
							else
								{
									newentry->floodinfo.chatFloodProtect = FALSE;
								}
								
							// get MSG
							if(startContains(cp, "MSG"))
								{
									cp += 3;
									
									SKIPBLANK(cp);
									
									// copy MSG
									
									cp++;
									
									cp = processstring(buffer2, cp, sizeof(buffer2) - 1, '\"');
									
									// make sure you are at the end quote
									while(*cp && *cp != '\"')
										{
											cp++;
										}
										
									cp++;
									
									SKIPBLANK(cp);
									
									
									num = q2a_strlen(buffer2);
									
									if(num)
										{
											newentry->msg = gi.TagMalloc (num + 1, TAG_LEVEL);
											q2a_strcpy(newentry->msg, buffer2);
										}
									else
										{
											newentry->msg = NULL;
										}
								}
							else
								{
									newentry->msg = NULL;
								}
								
							// do you have a valid ban record?
							if(newentry->type == NOTUSED ||
								(!all && newentry->type == NICKALL && newentry->subnetmask == 0 && newentry->maxnumberofconnects == 0) ||
								(newentry->type == NICKRE && !newentry->r))
								{
									// no, abort
									if(newentry->msg)
										{
											gi.TagFree(newentry->msg);
										}
										
									if(newentry->r)
										{
											regfree(newentry->r);
											gi.TagFree(newentry->r);
										}
									gi.TagFree(newentry);
									
									gi.dprintf ("Error loading BAN from line %d in file %s\n", uptoLine, bfname);
								}
							else
								{
									// we have the ban record...
									// insert at the head of the correct list.
									newentry->bannum = banNumUpto;
									banNumUpto++;
									
									newentry->next = banhead;
									banhead = newentry;
								}
						}
					else if(startContains(cp, "CHATBAN:"))
						{
							// create chat ban.
							// CHATBAN: [LIKE/RE(LIKE)] "xxx" [MSG "xxx"]
							
							// allocate memory for chat ban record
							cnewentry = gi.TagMalloc (sizeof(chatbaninfo_t), TAG_LEVEL);
							
							cnewentry->loadType = LT_PERM;
							cnewentry->r = 0;
							
							cp += 8;
							
							SKIPBLANK(cp);
							
							if(startContains(cp, "LIKE"))
								{
									cp += 4;
									SKIPBLANK(cp);
									
									cnewentry->type = CHATLIKE;
									
								} //re?
							else if(startContains(cp, "RE"))
								{
									cp += 2;
									SKIPBLANK(cp);
									
									cnewentry->type = CHATRE;
								}
							else
								{
									cnewentry->type = CHATLIKE;
								}
								
							if(*cp == '\"')
								{
									// copy chat
									cp++;
									cp = processstring(cnewentry->chat, cp, sizeof(cnewentry->chat) - 1, '\"');
									
									// make sure you are at the end quote
									while(*cp && *cp != '\"')
										{
											cp++;
										}
										
									cp++;
									
									SKIPBLANK(cp);
									
									if(cnewentry->type == CHATRE)
										{ // compile RE
											q2a_strcpy(strbuffer, cnewentry->chat);
											q_strupr(strbuffer);
											cnewentry->r = gi.TagMalloc (sizeof(*cnewentry->r), TAG_LEVEL);
											q2a_memset(cnewentry->r, 0x0, sizeof(*cnewentry->r));
											//            if(regcomp(cnewentry->r, strbuffer, REG_EXTENDED))
											if(regcomp(cnewentry->r, strbuffer, 0))
												{
													gi.TagFree(cnewentry->r);
													cnewentry->r = 0;
												}
										}
								}
							else
								{
									cnewentry->type = CNOTUSED;
								}
								
							// get MSG
							if(startContains(cp, "MSG"))
								{
									cp += 3;
									
									SKIPBLANK(cp);
									
									// copy MSG
									
									cp++;
									
									cp = processstring(buffer2, cp, sizeof(buffer2) - 1, '\"');
									
									// make sure you are at the end quote
									while(*cp && *cp != '\"')
										{
											cp++;
										}
										
									cp++;
									
									SKIPBLANK(cp);
									
									
									num = q2a_strlen(buffer2);
									
									if(num)
										{
											cnewentry->msg = gi.TagMalloc (num + 1, TAG_LEVEL);
											q2a_strcpy(cnewentry->msg, buffer2);
										}
									else
										{
											cnewentry->msg = NULL;
										}
								}
							else
								{
									cnewentry->msg = NULL;
								}
								
								
							// do you have a valid ban record?
							if(cnewentry->type == CNOTUSED || (cnewentry->type == CHATRE && !cnewentry->r))
								{
									// no, abort
									if(cnewentry->msg)
										{
											gi.TagFree(cnewentry->msg);
										}
										
									if(cnewentry->r)
										{
											regfree(cnewentry->r);
											gi.TagFree(cnewentry->r);
										}
									gi.TagFree(cnewentry);
									
									gi.dprintf ("Error loading CHATBAN from line %d in file %s\n", uptoLine, bfname);
								}
							else
								{
									// we have the ban record...
									// insert at the head of the correct list.
									cnewentry->bannum = chatBanNumUpto;
									chatBanNumUpto++;
									
									cnewentry->next = chatbanhead;
									chatbanhead = cnewentry;
								}
						}
					else if(startContains(cp, "INCLUDE:"))
						{
							// include another ban file..
							// INCLUDE: "banfile"
							
							cp += 8;
							
							SKIPBLANK(cp);
							
							if(*cp == '\"')
								{
									cp++;
									cp = processstring(strbuffer, cp, sizeof(strbuffer) - 1, '\"');
									
									if(strbuffer[0])
										{
											ReadBanFile(strbuffer);
										}
									else
										{
											gi.dprintf ("Error with INCLUDE in line %d in file %s\n", uptoLine, bfname);
										}
								}
							else
								{
									gi.dprintf ("Error with INCLUDE in line %d in file %s\n", uptoLine, bfname);
								}
						}
					else
						{
							gi.dprintf ("Unknown ban line from line %d in file %s\n", uptoLine, bfname);
						}
				}
		}
		
	fclose(banfile);
	
	return TRUE;
}



void freeBanLists(void)
{
	while(banhead)
		{
			baninfo_t *freeentry = banhead;
			banhead = banhead->next;
			
			if(freeentry->msg)
				{
					gi.TagFree(freeentry->msg);
				}
				
			if(freeentry->r)
				{
					regfree(freeentry->r);
					gi.TagFree(freeentry->r);
				}
			gi.TagFree(freeentry);
		}
		
	while(chatbanhead)
		{
			chatbaninfo_t *freeentry = chatbanhead;
			chatbanhead = chatbanhead->next;
			
			if(freeentry->msg)
				{
					gi.TagFree(freeentry->msg);
				}
				
			if(freeentry->r)
				{
					regfree(freeentry->r);
					gi.TagFree(freeentry->r);
				}
			gi.TagFree(freeentry);
		}
		
	banNumUpto = 0;
	chatBanNumUpto = 0;
}



void readBanLists(void)
{
	char cfgFile[100];
	qboolean ret;
	
	if(!q2adminbantxt || isBlank(q2adminbantxt->string))
		{
			q2a_strcpy(cfgFile, BANLISTFILE);
		}
	else
		{
			q2a_strcpy(cfgFile, q2adminbantxt->string);
		}
		
	freeBanLists();
	
	ret = ReadBanFile(cfgFile);
	
	sprintf(buffer, "%s/%s", moddir, cfgFile);
	if(ReadBanFile(buffer))
		{
			ret = TRUE;
		}
		
	if(!ret)
		{
			gi.dprintf ("WARNING: " BANLISTFILE " could not be found\n");
			logEvent(LT_INTERNALWARN, 0, NULL, BANLISTFILE " could not be found", IW_BANSETUPLOAD, 0.0);
		}
}



#define BANCMD_LAYOUT  "[sv] !BAN [+/-(-)] [ALL/[NAME [LIKE/RE] name/%%p x/BLANK/ALL(ALL)] [IP [xxx[.xxx(0)[.xxx(0)[.xxx(0)]]]/%%p x][/yy(32)]] [PASSWORD xxx] [MAX 0-xxx(0)] [FLOOD xxx(num) xxx(sec) xxx(silence] [MSG xxx] [TIME 1-xxx(mins)] [SAVE [MOD]] [NOCHECK]\n"


void banRun(int startarg, edict_t *ent, int client)
{
	char *cp;
	int clienti, num;
	unsigned int i, save;
	qboolean like, all, re;
	baninfo_t *newentry;
	char savecmd[256];
	char strbuffer[256];
	qboolean nocheck = FALSE;
	
	// [sv] !BAN [+/-(-)] [ALL/[NAME [LIKE/RE] name/%p x/BLANK/ALL(ALL)] [IP [xxx[.xxx(0)[.xxx(0)[.xxx(0)]]]/%p x][/yy(32)]] [PASSWORD xxx] [MAX 0-xxx(0)]] [FLOOD xxx xxx xxx] [MSG xxx] [TIME 1-xxx(mins)] [SAVE [MOD]] [NOCHECK]
	
	if(gi.argc() <= startarg)
		{
			gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
			return;
		}
		
	cp = gi.argv(startarg);
	startarg++;
	
	// allocate memory for ban record
	newentry = gi.TagMalloc (sizeof(baninfo_t), TAG_LEVEL);
	newentry->r = 0;
	
	q2a_strcpy(savecmd, "BAN: ");
	
	
	// get +/-
	if(*cp == '-')
		{
			newentry->exclude = TRUE;
			
			if(gi.argc() <= startarg)
				{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
					gi.TagFree(newentry);
					return;
				}
				
			cp = gi.argv(startarg);
			startarg++;
			
		}
	else if(*cp == '+')
		{
			newentry->exclude = FALSE;
			
			if(gi.argc() <= startarg)
				{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
					gi.TagFree(newentry);
					return;
				}
				
			cp = gi.argv(startarg);
			startarg++;
			
			q2a_strcat(savecmd, "+ ");
		}
	else
		{
			newentry->exclude = TRUE;
		}
		
	if(startContains(cp, "ALL"))
		{
			newentry->type = NICKALL;
			newentry->ip[0] = 0;
			newentry->ip[1] = 0;
			newentry->ip[2] = 0;
			newentry->ip[3] = 0;
			newentry->subnetmask = 0;
			newentry->maxnumberofconnects = 0;
			newentry->numberofconnects = 0;
			newentry->msg = NULL;
			newentry->floodinfo.chatFloodProtect = FALSE;
			all = TRUE;
			
			
			if(gi.argc() <= startarg)
				{
					cp = "";
				}
			else
				{
					cp = gi.argv(startarg);
					startarg++;
				}
		}
	else
		{
			all = FALSE;
			
			// Name:
			if(startContains(cp, "NAME"))
				{
					if(gi.argc() <= startarg)
						{
							gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
							gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
							gi.TagFree(newentry);
							return;
						}
						
					cp = gi.argv(startarg);
					startarg++;
					
					q2a_strcat(savecmd, "NAME ");
					
					// Like?
					if(startContains(cp, "LIKE"))
						{
							like = TRUE;
							
							if(gi.argc() <= startarg)
								{
									gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
									gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
									gi.TagFree(newentry);
									return;
								}
								
							cp = gi.argv(startarg);
							startarg++;
							
							q2a_strcat(savecmd, "LIKE ");
							
						} //re?
					else if(startContains(cp, "RE"))
						{
							like = FALSE;
							re = TRUE;
							
							if(gi.argc() <= startarg)
								{
									gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
									gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
									gi.TagFree(newentry);
									return;
								}
								
							cp = gi.argv(startarg);
							startarg++;
							
							q2a_strcat(savecmd, "RE ");
						}
					else
						{
							like = FALSE;
						}
						
					// BLANK or ALL or name
					if(startContains(cp, "BLANK"))
						{
							newentry->type = NICKBLANK;
							q2a_strcat(savecmd, "BLANK ");
						}
					else if(startContains(cp, "ALL"))
						{
							newentry->type = NICKALL;
							q2a_strcat(savecmd, "ALL ");
						}
					else if(startContains(cp, "%P"))
						{
							if(gi.argc() <= startarg)
								{
									gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
									gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
									gi.TagFree(newentry);
									return;
								}
								
							cp = gi.argv(startarg);
							startarg++;
							
							if(!isdigit(*cp))
								{
									gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
									gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
									gi.TagFree(newentry);
									return;
								}
								
							if(like)
								{
									newentry->type = NICKLIKE;
								}
							else if(re)
								{
									newentry->type = NICKRE;
								}
							else
								{
									newentry->type = NICKEQ;
								}
								
							clienti = q2a_atoi(cp);
							
							if(clienti < 0 || clienti > maxclients->value || !proxyinfo[clienti].inuse)
								{
									gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
									gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
									gi.TagFree(newentry);
									return;
								}
								
							q2a_strcpy(newentry->nick, proxyinfo[clienti].name);
							
							q2a_strcat(savecmd, "\"");
							q2a_strcat(savecmd, proxyinfo[clienti].name);
							q2a_strcat(savecmd, "\" ");
							
							if(newentry->type == NICKRE)
								{ // compile RE
									q2a_strcpy(strbuffer, newentry->nick);
									q_strupr(strbuffer);
									
									newentry->r = gi.TagMalloc (sizeof(*newentry->r), TAG_LEVEL);
									q2a_memset(newentry->r, 0x0, sizeof(*newentry->r));
									//          if(regcomp(newentry->r, strbuffer, REG_EXTENDED))
									if(regcomp(newentry->r, strbuffer, 0))
										{
											gi.TagFree(newentry->r);
											gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
											gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
											gi.TagFree(newentry);
											return;
										}
								}
						}
					else
						{
							if(like)
								{
									newentry->type = NICKLIKE;
								}
							else if(re)
								{
									newentry->type = NICKRE;
								}
							else
								{
									newentry->type = NICKEQ;
								}
								
							q2a_strcat(savecmd, "\"");
							q2a_strcat(savecmd, cp);
							q2a_strcat(savecmd, "\" ");
							
							// copy name
							processstring(newentry->nick, cp, sizeof(newentry->nick) - 1, 0);
						}
						
					if(newentry->type == NICKRE)
						{ // compile RE
							q2a_strcpy(strbuffer, newentry->nick);
							q_strupr(strbuffer);
							
							newentry->r = gi.TagMalloc (sizeof(*newentry->r), TAG_LEVEL);
							q2a_memset(newentry->r, 0x0, sizeof(*newentry->r));
							//        if(regcomp(newentry->r, strbuffer, REG_EXTENDED))
							if(regcomp(newentry->r, strbuffer, 0))
								{
									gi.TagFree(newentry->r);
									gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
									gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
									gi.TagFree(newentry);
									return;
								}
						}
						
					if(gi.argc() <= startarg)
						{
							cp = "";
						}
					else
						{
							cp = gi.argv(startarg);
							startarg++;
						}
				}
			else
				{
					newentry->type = NICKALL;
				}
				
			// get ip address
			newentry->ip[0] = 0;
			newentry->ip[1] = 0;
			newentry->ip[2] = 0;
			newentry->ip[3] = 0;
			
			if(startContains(cp, "IP"))
				{
					if(gi.argc() <= startarg)
						{
							gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
							gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
							
							if(newentry->r)
								{
									regfree(newentry->r);
									gi.TagFree(newentry->r);
								}
							gi.TagFree(newentry);
							return;
						}
						
					cp = gi.argv(startarg);
					startarg++;
					
					q2a_strcat(savecmd, "IP ");
					
					if(startContains(cp, "%P"))
						{
							if(gi.argc() <= startarg)
								{
									gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
									gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
									
									if(newentry->r)
										{
											regfree(newentry->r);
											gi.TagFree(newentry->r);
										}
									gi.TagFree(newentry);
									return;
								}
								
							cp = gi.argv(startarg);
							startarg++;
							
							if(!isdigit(*cp))
								{
									gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
									gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
									
									if(newentry->r)
										{
											regfree(newentry->r);
											gi.TagFree(newentry->r);
										}
									gi.TagFree(newentry);
									return;
								}
								
							clienti = q2a_atoi(cp);
							
							if(clienti < 0 || clienti > maxclients->value || !proxyinfo[clienti].inuse)
								{
									gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
									gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
									
									if(newentry->r)
										{
											regfree(newentry->r);
											gi.TagFree(newentry->r);
										}
									gi.TagFree(newentry);
									return;
								}
								
							newentry->ip[0] = proxyinfo[clienti].ipaddressBinary[0];
							newentry->ip[1] = proxyinfo[clienti].ipaddressBinary[1];
							newentry->ip[2] = proxyinfo[clienti].ipaddressBinary[2];
							newentry->ip[3] = proxyinfo[clienti].ipaddressBinary[3];
								
							sprintf(savecmd + q2a_strlen(savecmd), "%d.%d.%d.%d ", newentry->ip[0], newentry->ip[1], newentry->ip[2], newentry->ip[3]);
							
							while(isdigit(*cp))
								{
									cp++;
								}
								
							if(*cp == '/')
								{
									cp++;
									
									newentry->subnetmask = q2a_atoi(cp);
									
									while(isdigit(*cp))
										{
											cp++;
										}
								}
							else
								{
									newentry->subnetmask = 32;
								}
								
							if(*cp != 0)
								{
									gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
									gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
									
									if(newentry->r)
										{
											regfree(newentry->r);
											gi.TagFree(newentry->r);
										}
									gi.TagFree(newentry);
									return;
								}
						}
					else
						{
							q2a_strcat(savecmd, cp);
							q2a_strcat(savecmd, " ");
							
							if(isdigit(*cp))
								{
									for(i = 0; i < 4; i++)
										{
											num = q2a_atoi(cp);
											
											if(num > 255)
												{
													num = 255;
												}
												
											newentry->ip[i] = num;
											
											while(isdigit(*cp))
												{
													cp++;
												}
												
											if(*cp == '.')
												{
													cp++;
												}
											else
												{
													break;
												}
										}
										
									if(*cp == '/')
										{
											cp++;
											
											newentry->subnetmask = q2a_atoi(cp);
											
											while(isdigit(*cp))
												{
													cp++;
												}
										}
									else
										{
											newentry->subnetmask = 32;
										}
										
									if(*cp != 0)
										{
											gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
											gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
											
											if(newentry->r)
												{
													regfree(newentry->r);
													gi.TagFree(newentry->r);
												}
											gi.TagFree(newentry);
											return;
										}
								}
						}
						
					if(gi.argc() <= startarg)
						{
							cp = "";
						}
					else
						{
							cp = gi.argv(startarg);
							startarg++;
						}
						
					// get MASK
					/*      if(startContains(cp, "MASK"))
					{
					if(gi.argc() <= startarg)
					{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
					 
					if(newentry->r)
					{
					regfree(newentry->r);
					gi.TagFree(newentry->r);
					}
					gi.TagFree(newentry);
					return;
					}
					 
					cp = gi.argv(startarg);
					startarg++;
					 
					q2a_strcat(savecmd, "MASK ");
					q2a_strcat(savecmd, cp);
					q2a_strcat(savecmd, " ");
					 
					newentry->subnetmask = q2a_atoi(cp);
					 
					if(newentry->subnetmask > 32)
					{
					newentry->subnetmask = 32;
					}
					 
					if(gi.argc() <= startarg)
					{
					cp = "";
					}
					else
					{
					cp = gi.argv(startarg);
					startarg++;
					}
					}
					else
					{
					newentry->subnetmask = 32;
					}
					*/
				}
			else
				{
					newentry->subnetmask= 0;
				}
		}
		
	// get PASSWORD
	if(!newentry->exclude && startContains(cp, "PASSWORD"))
		{
			if(gi.argc() <= startarg)
				{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
					
					if(newentry->r)
						{
							regfree(newentry->r);
							gi.TagFree(newentry->r);
						}
					gi.TagFree(newentry);
					return;
				}
				
			cp = gi.argv(startarg);
			startarg++;
			
			q2a_strcat(savecmd, "PASSWORD ");
			
			q2a_strcat(savecmd, "\"");
			q2a_strcat(savecmd, cp);
			q2a_strcat(savecmd, "\" ");
			
			// copy password
			
			processstring(newentry->password, cp, sizeof(newentry->password) - 1, 0);
			
			if(gi.argc() <= startarg)
				{
					cp = "";
				}
			else
				{
					cp = gi.argv(startarg);
					startarg++;
				}
		}
	else
		{
			newentry->password[0] = 0;
		}
		
		
	// get MAX
	if(!newentry->exclude && startContains(cp, "MAX"))
		{
			if(gi.argc() <= startarg)
				{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
					
					if(newentry->r)
						{
							regfree(newentry->r);
							gi.TagFree(newentry->r);
						}
					gi.TagFree(newentry);
					return;
				}
				
			cp = gi.argv(startarg);
			startarg++;
			
			q2a_strcat(savecmd, "MAX ");
			q2a_strcat(savecmd, cp);
			q2a_strcat(savecmd, " ");
			
			newentry->maxnumberofconnects = q2a_atoi(cp);
			
			if(gi.argc() <= startarg)
				{
					cp = "";
				}
			else
				{
					cp = gi.argv(startarg);
					startarg++;
				}
		}
	else
		{
			newentry->maxnumberofconnects = 0;
		}
		
	newentry->numberofconnects = 0;
	
	// get FLOOD
	if(!newentry->exclude && startContains(cp, "FLOOD"))
		{
			if(gi.argc() <= startarg + 2)
				{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
					
					if(newentry->r)
						{
							regfree(newentry->r);
							gi.TagFree(newentry->r);
						}
					gi.TagFree(newentry);
					return;
				}
				
			cp = gi.argv(startarg);
			startarg++;
			
			newentry->floodinfo.chatFloodProtectNum = q2a_atoi(cp);
			
			cp = gi.argv(startarg);
			startarg++;
			
			newentry->floodinfo.chatFloodProtectSec = q2a_atoi(cp);
			
			cp = gi.argv(startarg);
			startarg++;
			
			newentry->floodinfo.chatFloodProtectSilence = q2a_atoi(cp);
			
			
			if(gi.argc() <= startarg)
				{
					cp = "";
				}
			else
				{
					cp = gi.argv(startarg);
					startarg++;
				}
				
			if(newentry->floodinfo.chatFloodProtectNum && newentry->floodinfo.chatFloodProtectSec)
				{
					sprintf(savecmd + q2a_strlen(savecmd), "FLOOD %d %d %d ", newentry->floodinfo.chatFloodProtectNum, newentry->floodinfo.chatFloodProtectSec, newentry->floodinfo.chatFloodProtectSilence);
					newentry->floodinfo.chatFloodProtect = TRUE;
				}
			else
				{
					newentry->floodinfo.chatFloodProtect = FALSE;
				}
		}
	else
		{
			newentry->floodinfo.chatFloodProtect = FALSE;
		}
		
		
	// get MSG
	if(startContains(cp, "MSG"))
		{
			if(gi.argc() <= startarg)
				{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
					
					if(newentry->r)
						{
							regfree(newentry->r);
							gi.TagFree(newentry->r);
						}
					gi.TagFree(newentry);
					return;
				}
				
			cp = gi.argv(startarg);
			startarg++;
			
			q2a_strcat(savecmd, "MSG ");
			q2a_strcat(savecmd, "\"");
			q2a_strcat(savecmd, cp);
			q2a_strcat(savecmd, "\" ");
			
			// copy MSG
			processstring(buffer2, cp, sizeof(buffer2) - 1, '\"');
			
			num = q2a_strlen(buffer2);
			
			if(num)
				{
					newentry->msg = gi.TagMalloc (num + 1, TAG_LEVEL);
					q2a_strcpy(newentry->msg, buffer2);
				}
			else
				{
					newentry->msg = NULL;
				}
				
			if(gi.argc() <= startarg)
				{
					cp = "";
				}
			else
				{
					cp = gi.argv(startarg);
					startarg++;
				}
		}
	else
		{
			newentry->msg = NULL;
		}
		
	// get Timeout
	if(startContains(cp, "TIME"))
		{
			if(gi.argc() <= startarg)
				{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
					if(newentry->msg)
						{
							gi.TagFree(newentry->msg);
						}
						
					if(newentry->r)
						{
							regfree(newentry->r);
							gi.TagFree(newentry->r);
						}
					gi.TagFree(newentry);
					return;
				}
				
			cp = gi.argv(startarg);
			startarg++;
			
			newentry->timeout = q2a_atof(cp);
			if(newentry->timeout < 1.0)
				{
					newentry->timeout = 1.0;
				}
				
			newentry->timeout *= 60.0;
			newentry->timeout += ltime;
			
			if(gi.argc() <= startarg)
				{
					cp = "";
				}
			else
				{
					cp = gi.argv(startarg);
					startarg++;
				}
		}
	else
		{
			newentry->timeout = 0.0;
		}
		
	// get Save?
	if(startContains(cp, "SAVE"))
		{
			if(newentry->timeout >= 1.0)
				{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
					if(newentry->msg)
						{
							gi.TagFree(newentry->msg);
						}
						
					if(newentry->r)
						{
							regfree(newentry->r);
							gi.TagFree(newentry->r);
						}
					gi.TagFree(newentry);
					return;
				}
				
			if(gi.argc() <= startarg)
				{
					cp = "";
				}
			else
				{
					cp = gi.argv(startarg);
					startarg++;
				}
				
			newentry->loadType = LT_PERM;
			
			if(startContains(cp, "MOD"))
				{
					if(gi.argc() <= startarg)
						{
							cp = "";
						}
					else
						{
							cp = gi.argv(startarg);
							startarg++;
						}
						
					save = 2;
				}
			else
				{
					save = 1;
				}
		}
	else
		{
			newentry->loadType = LT_TEMP;
			save = 0;
		}
		
		
	// get nocheck
	if(startContains(cp, "NOCHECK"))
		{
			if(gi.argc() <= startarg)
				{
					cp = "";
				}
			else
				{
					cp = gi.argv(startarg);
					startarg++;
				}
				
			nocheck = TRUE;
		}
		
	if(*cp != 0)
		{
			// something is wrong...
			if(newentry->msg)
				{
					gi.TagFree(newentry->msg);
				}
				
			if(newentry->r)
				{
					regfree(newentry->r);
					gi.TagFree(newentry->r);
				}
			gi.TagFree(newentry);
			gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
			gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
			return;
		}
		
	// do you have a valid ban record?
	if(!all && newentry->type == NICKALL && newentry->subnetmask == 0 && newentry->maxnumberofconnects == 0)
		{
			// no, abort
			if(newentry->msg)
				{
					gi.TagFree(newentry->msg);
				}
				
			if(newentry->r)
				{
					regfree(newentry->r);
					gi.TagFree(newentry->r);
				}
			gi.TagFree(newentry);
			gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
			gi.cprintf(ent, PRINT_HIGH, BANCMD_LAYOUT);
			return;
		}
	else
		{
			// we have the ban record...
			// insert at the head of the correct list.
			newentry->bannum = banNumUpto;
			banNumUpto++;
			
			newentry->next = banhead;
			banhead = newentry;
			
			gi.cprintf(ent, PRINT_HIGH, "Ban Added!!\n");
			
			if(save)
				{
					FILE *banlistfptr;
					
					if(save == 1)
						{
							q2a_strcpy(buffer, BANLISTFILE);
						}
					else
						{
							sprintf(buffer, "%s/%s", moddir, BANLISTFILE);
						}
						
					banlistfptr = fopen(buffer, "at");
					if(!banlistfptr)
						{
							gi.cprintf(ent, PRINT_HIGH, "Error opening banfile!\n");
						}
					else
						{
							fprintf(banlistfptr, "%s\n", savecmd);
							fclose(banlistfptr);
							
							gi.cprintf(ent, PRINT_HIGH, "Ban stored.\n");
						}
				}
				
			if(!nocheck)
				{
					for(clienti = 0; clienti < maxclients->value; clienti++)
						{
							if(proxyinfo[clienti].inuse)
								{
									edict_t *enti = getEnt((clienti + 1));
									if(checkCheckIfBanned(enti, clienti))
										{
											logEvent(LT_BAN, clienti, enti, currentBanMsg, 0, 0.0);
											gi.cprintf (NULL, PRINT_HIGH, "%s: %s (IP = %s)\n", proxyinfo[clienti].name, currentBanMsg, proxyinfo[clienti].ipaddress);
											gi.cprintf (enti, PRINT_HIGH, "%s: %s\n", proxyinfo[clienti].name, currentBanMsg);
											addCmdQueue(clienti, QCMD_DISCONNECT, 1, 0, currentBanMsg);
										}
								}
						}
				}
		}
}





void reloadbanfileRun(int startarg, edict_t *ent, int client)
{
	readBanLists();
	gi.cprintf (ent, PRINT_HIGH, "Bans reloaded.\n");
}




int checkBanList(edict_t *ent, int client)
{
	baninfo_t *checkentry = banhead, *prevcheckentry = NULL;
	char strbuffer[256];
	
	while(checkentry)
		{
			if(checkentry->type != NOTUSED)
				{
					if(checkentry->timeout && checkentry->timeout < ltime)
						{
							unsigned int clienti;
							
							// found dead ban, delete...
							for(clienti = 0; clienti < maxclients->value; clienti++)
								{
									if(proxyinfo[clienti].baninfo == checkentry)
										{
											proxyinfo[clienti].baninfo = NULL;
										}
								}
								
							if(prevcheckentry)
								{
									prevcheckentry->next = checkentry->next;
								}
							else
								{
									banhead = checkentry->next;
								}
								
							if(checkentry->msg)
								{
									gi.TagFree(checkentry->msg);
								}
								
							if(checkentry->r)
								{
									regfree(checkentry->r);
									gi.TagFree(checkentry->r);
								}
							gi.TagFree(checkentry);
							
							
							if(prevcheckentry)
								{
									checkentry = prevcheckentry->next;
								}
							else
								{
									checkentry = banhead;
								}
								
							continue;
						}
						
					// check name...
					if(checkentry->type != NICKALL)
						{
							if(!NickBanning_Enable)
								{
									prevcheckentry = checkentry;
									checkentry = checkentry->next;
									continue;
								}
								
							switch(checkentry->type)
								{
								case NICKEQ:
									if(Q_stricmp(proxyinfo[client].name, checkentry->nick))
										{
											prevcheckentry = checkentry;
											checkentry = checkentry->next;
											continue;
										}
									break;
									
								case NICKLIKE:
									if(!stringContains(proxyinfo[client].name, checkentry->nick))
										{
											prevcheckentry = checkentry;
											checkentry = checkentry->next;
											continue;
										}
									break;
									
								case NICKRE:
									q2a_strcpy(strbuffer, proxyinfo[client].name);
									q_strupr(strbuffer);
									
									if(regexec(checkentry->r, strbuffer, 0, 0, 0) == REG_NOMATCH)
										{
											prevcheckentry = checkentry;
											checkentry = checkentry->next;
											continue;
										}
									break;
									
								case NICKBLANK:
									if(!isBlank(proxyinfo[client].name))
										{
											prevcheckentry = checkentry;
											checkentry = checkentry->next;
											continue;
										}
									break;
								}
						}
						
					// check IP
					if(IPBanning_Enable)
						{
							int snm = checkentry->subnetmask;
							unsigned int i;
							
							for(i = 0; i < 4 && snm; i++)
								{
									if(snm < 8)
										{
											byte mask = 0xFF << (8 - snm);
											
											if((checkentry->ip[i] & mask) != (proxyinfo[client].ipaddressBinary[i] & mask))
												{
													break;
												}
										}
									else
										{
											if(checkentry->ip[i] != proxyinfo[client].ipaddressBinary[i])
												{
													break;
												}
										}
										
									snm -= 8;
								}
								
							if(snm > 0)
								{
									prevcheckentry = checkentry;
									checkentry = checkentry->next;
									continue;
								}
						}
					else if(checkentry->subnetmask)
						{
							prevcheckentry = checkentry;
							checkentry = checkentry->next;
							continue;
						}
						
					if(checkentry->exclude)
						{
							// ok, a ban situation..
							
							if(checkentry->msg)
								{
									currentBanMsg = checkentry->msg;
								}
								
							return 1;
						}

					if(checkentry->password[0])
						{
							char *s = Info_ValueForKey (proxyinfo[client].userinfo, "pw");
							
//*** UPDATE START ***
							sprintf(strbuffer,"INCLUDE - %s", s);
							logEvent(LT_ADMINLOG, client, ent, strbuffer, 0, 0.0);
							gi.dprintf("%s\n", strbuffer);
//*** UPDATE END ***
							
							if(q2a_strcmp(checkentry->password, s))
								{
									if(checkentry->msg)
										{
											currentBanMsg = checkentry->msg;
										}
										
									return 1;
								}
						}
						
					// check max connections..
					if(checkentry->maxnumberofconnects)
						{
							if(checkentry->numberofconnects >= checkentry->maxnumberofconnects)
								{
									if(checkentry->msg)
										{
											currentBanMsg = checkentry->msg;
										}
										
									return 1;
								}
								
							proxyinfo[client].baninfo = checkentry;
							checkentry->numberofconnects++;
						}
						
					// user included...  set user settings for this include ban
					
					if(checkentry->floodinfo.chatFloodProtect)
						{
							proxyinfo[client].floodinfo = checkentry->floodinfo;
						}
						
					return 0;
				}
				
			prevcheckentry = checkentry;
			checkentry = checkentry->next;
		}
		
	return 0;
}


int checkCheckIfBanned(edict_t *ent, int client)
{
	if(proxyinfo[client].baninfo)
		{
			if(proxyinfo[client].baninfo->numberofconnects)
				{
					proxyinfo[client].baninfo->numberofconnects--;
				}
				
			proxyinfo[client].baninfo = NULL;
		}
		
	if(!IPBanning_Enable && !NickBanning_Enable)
		{
			return 0;
		}
		
	currentBanMsg = defaultBanMsg;
	return checkBanList(ent, client);
}



void listbansRun(int startarg, edict_t *ent, int client)
{
	addCmdQueue(client, QCMD_DISPBANS, 0, 0, 0);
}


void displayNextBan(edict_t *ent, int client, long bannum)
{
	long upto = bannum;
	baninfo_t *findentry = banhead;
	
	bannum++;

	if (bannum == 1)
		gi.cprintf (ent, PRINT_HIGH, "Current ban list:\n");	//UPDATE
		//gi.cprintf (ent, PRINT_HIGH, "%ld\n", bannum);
	
	while(findentry && upto)
		{
			findentry = findentry->next;
			
			if(findentry && findentry->type != NOTUSED)
				{
					if(!findentry->timeout || findentry->timeout > ltime)
						{
							upto--;
						}
				}
		}
		
	if(findentry)
		{
			if(findentry->loadType == LT_TEMP)
				{
					sprintf(buffer, " (%ld, Temporary) BAN:", findentry->bannum);
				}
			else
				{
					sprintf(buffer, " (%ld, Permanent) BAN:", findentry->bannum);
				}
				
			if(!findentry->exclude)
				{
					q2a_strcat(buffer, " +");
				}
				
			if(findentry->type == NICKALL && findentry->subnetmask == 0)
				{
					q2a_strcat(buffer, " ALL");
				}
			else
				{
					if(findentry->type != NICKALL)
						{
							q2a_strcat(buffer, " NAME");
							
							if(findentry->type == NICKBLANK)
								{
									q2a_strcat(buffer, " BLANK");
								}
							else
								{
									if(findentry->type == NICKLIKE)
										{
											q2a_strcat(buffer, " LIKE");
										}
									else if(findentry->type == NICKRE)
										{
											q2a_strcat(buffer, " RE");
										}
										
									q2a_strcat(buffer, " \"");
									q2a_strcat(buffer, findentry->nick);
									q2a_strcat(buffer, "\"");
								}
						}
						
					if(findentry->subnetmask != 0)
						{
							sprintf(buffer + q2a_strlen(buffer), " IP %d.%d.%d.%d/%d", findentry->ip[0], findentry->ip[1], findentry->ip[2], findentry->ip[3], findentry->subnetmask);
						}
				}
				
			if(!findentry->exclude && findentry->password[0])
				{
					q2a_strcat(buffer, " PASSWORD \"");
					q2a_strcat(buffer, findentry->password);
					q2a_strcat(buffer, "\"");
				}
				
			if(!findentry->exclude && findentry->maxnumberofconnects)
				{
					sprintf(buffer + q2a_strlen(buffer), " MAX %d", findentry->maxnumberofconnects);
				}
				
			if(!findentry->exclude && findentry->floodinfo.chatFloodProtect)
				{
					sprintf(buffer + q2a_strlen(buffer), " FLOOD %d %d %d", findentry->floodinfo.chatFloodProtectNum, findentry->floodinfo.chatFloodProtectSec, findentry->floodinfo.chatFloodProtectSilence);
				}
				
			if(findentry->msg)
				{
					q2a_strcat(buffer, " MSG \"");
					q2a_strcat(buffer, findentry->msg);
					q2a_strcat(buffer, "\"");
				}
				
			if(findentry->timeout)
				{
					sprintf(buffer + q2a_strlen(buffer), " TIME %g", (findentry->timeout - ltime) / 60.0);
				}
				
			gi.cprintf (ent, PRINT_HIGH, "%s\n", buffer);
			addCmdQueue(client, QCMD_DISPBANS, 0, bannum, 0);
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "End of ban list.\n");
		}
}





void delbanRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg)
		{
			int banToDelete = q2a_atoi(gi.argv(startarg));
			baninfo_t *findentry = banhead, *prevban = NULL;
			
			while(findentry)
				{
					if(findentry->bannum == banToDelete)
						{
							break;
						}
						
					prevban = findentry;
					findentry = findentry->next;
				}
				
			if(findentry)
				{
					unsigned int clienti;
					
					for(clienti = 0; clienti < maxclients->value; clienti++)
						{
							if(proxyinfo[clienti].baninfo == findentry)
								{
									proxyinfo[clienti].baninfo = NULL;
								}
						}
						
					if(prevban)
						{
							prevban->next = findentry->next;
						}
					else
						{
							banhead = findentry->next;
						}
						
					if(findentry->msg)
						{
							gi.TagFree(findentry->msg);
						}
						
					if(findentry->r)
						{
							regfree(findentry->r);
							gi.TagFree(findentry->r);
						}
					gi.TagFree(findentry);
					
					gi.cprintf (ent, PRINT_HIGH, "Ban deleted.\n");
				}
			else
				{
					gi.cprintf (ent, PRINT_HIGH, "Ban not found.\n");
				}
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "No ban number supplied to delete.\n");
		}
}

#define CHATBANCMD_LAYOUT  "[sv] !CHATBAN [LIKE/RE(LIKE)] xxx [MSG xxx] [SAVE [MOD]]\n"


void chatbanRun(int startarg, edict_t *ent, int client)
{
	char *cp;
	unsigned int num, save;
	chatbaninfo_t *cnewentry;
	char savecmd[256];
	char strbuffer[256];
	
	// [sv] !CHATBAN [LIKE/RE(LIKE)] xxx [MSG xxx] [SAVE [MOD]]
	
	if(gi.argc() <= startarg)
		{
			gi.cprintf(ent, PRINT_HIGH, CHATBANCMD_LAYOUT);
			return;
		}
		
	cp = gi.argv(startarg);
	startarg++;
	
	// allocate memory for ban record
	cnewentry = gi.TagMalloc (sizeof(chatbaninfo_t), TAG_LEVEL);
	cnewentry->r = 0;
	
	q2a_strcpy(savecmd, "CHATBAN: ");
	
	if(startContains(cp, "LIKE"))
		{
			if(gi.argc() <= startarg)
				{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, CHATBANCMD_LAYOUT);
					gi.TagFree(cnewentry);
					return;
				}
				
			cp = gi.argv(startarg);
			startarg++;
			
			q2a_strcat(savecmd, "LIKE ");
			
			cnewentry->type = CHATLIKE;
			
		} //re?
	else if(startContains(cp, "RE"))
		{
			if(gi.argc() <= startarg)
				{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, CHATBANCMD_LAYOUT);
					gi.TagFree(cnewentry);
					return;
				}
				
			cp = gi.argv(startarg);
			startarg++;
			
			
			q2a_strcat(savecmd, "RE ");
			
			cnewentry->type = CHATRE;
		}
	else
		{
			cnewentry->type = CHATLIKE;
			q2a_strcat(savecmd, "LIKE ");
		}
		
	q2a_strcat(savecmd, "\"");
	q2a_strcat(savecmd, cp);
	q2a_strcat(savecmd, "\" ");
	
	// copy chat
	cp = processstring(cnewentry->chat, cp, sizeof(cnewentry->chat) - 1, 0);
	
	if(cnewentry->type == CHATRE)
		{ // compile RE
			q2a_strcpy(strbuffer, cnewentry->chat);
			q_strupr(strbuffer);
			
			cnewentry->r = gi.TagMalloc (sizeof(*cnewentry->r), TAG_LEVEL);
			q2a_memset(cnewentry->r, 0x0, sizeof(*cnewentry->r));
			//    if(regcomp(cnewentry->r, strbuffer, REG_EXTENDED))
			if(regcomp(cnewentry->r, strbuffer, 0))
				{
					gi.TagFree(cnewentry->r);
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, CHATBANCMD_LAYOUT);
					gi.TagFree(cnewentry);
					return;
				}
		}
		
	if(gi.argc() <= startarg)
		{
			cp = "";
		}
	else
		{
			cp = gi.argv(startarg);
			startarg++;
		}
		
	// get MSG
	if(startContains(cp, "MSG"))
		{
			if(gi.argc() <= startarg)
				{
					gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
					gi.cprintf(ent, PRINT_HIGH, CHATBANCMD_LAYOUT);
					
					if(cnewentry->r)
						{
							regfree(cnewentry->r);
							gi.TagFree(cnewentry->r);
						}
					gi.TagFree(cnewentry);
					return;
				}
				
			cp = gi.argv(startarg);
			startarg++;
			
			q2a_strcat(savecmd, "MSG ");
			q2a_strcat(savecmd, "\"");
			q2a_strcat(savecmd, cp);
			q2a_strcat(savecmd, "\" ");
			
			// copy MSG
			processstring(buffer2, cp, sizeof(buffer2) - 1, '\"');
			
			num = q2a_strlen(buffer2);
			
			if(num)
				{
					cnewentry->msg = gi.TagMalloc (num + 1, TAG_LEVEL);
					q2a_strcpy(cnewentry->msg, buffer2);
				}
			else
				{
					cnewentry->msg = NULL;
				}
				
			if(gi.argc() <= startarg)
				{
					cp = "";
				}
			else
				{
					cp = gi.argv(startarg);
					startarg++;
				}
		}
	else
		{
			cnewentry->msg = NULL;
		}
		
	// get Save?
	if(startContains(cp, "SAVE"))
		{
			if(gi.argc() <= startarg)
				{
					cp = "";
				}
			else
				{
					cp = gi.argv(startarg);
					startarg++;
				}
				
			cnewentry->loadType = LT_PERM;
			
			if(startContains(cp, "MOD"))
				{
					if(gi.argc() <= startarg)
						{
							cp = "";
						}
					else
						{
							cp = gi.argv(startarg);
							startarg++;
						}
						
					save = 2;
				}
			else
				{
					save = 1;
				}
		}
	else
		{
			cnewentry->loadType = LT_TEMP;
			save = 0;
		}
		
	if(*cp != 0)
		{
			// something is wrong...
			if(cnewentry->msg)
				{
					gi.TagFree(cnewentry->msg);
				}
				
			if(cnewentry->r)
				{
					regfree(cnewentry->r);
					gi.TagFree(cnewentry->r);
				}
			gi.TagFree(cnewentry);
			gi.cprintf(ent, PRINT_HIGH, "UpTo: %s\n", savecmd);
			gi.cprintf(ent, PRINT_HIGH, CHATBANCMD_LAYOUT);
			return;
		}
		
	// we have the chat ban record...
	// insert at the head of the correct list.
	cnewentry->bannum = chatBanNumUpto;
	chatBanNumUpto++;
	
	cnewentry->next = chatbanhead;
	chatbanhead = cnewentry;
	
	gi.cprintf(ent, PRINT_HIGH, "Chatban added.\n");
	
	if(save)
		{
			FILE *banlistfptr;
			
			if(save == 1)
				{
					q2a_strcpy(buffer, BANLISTFILE);
				}
			else
				{
					sprintf(buffer, "%s/%s", moddir, BANLISTFILE);
				}
				
			banlistfptr = fopen(buffer, "at");
			if(!banlistfptr)
				{
					gi.cprintf(ent, PRINT_HIGH, "Error opening banfile!\n");
				}
			else
				{
					fprintf(banlistfptr, "%s\n", savecmd);
					fclose(banlistfptr);
					
					gi.cprintf(ent, PRINT_HIGH, "Chatban stored.\n");
				}
		}
}





int checkCheckIfChatBanned(char *txt)
{
	chatbaninfo_t *checkentry = chatbanhead;
	char strbuffer[4096];
	
	// filter out characters that are disallowed.
	if(filternonprintabletext)
		{
			char *cp = txt;
			
			while(*cp)
				{
					if(!isprint(*cp))
						{
							*cp = ' ';
						}
						
					*cp++;
				}
		}
		
		
	if(!ChatBanning_Enable)
		{
			return 0;
		}
		
	currentBanMsg = defaultChatBanMsg;
	
	while(checkentry)
		{
			switch(checkentry->type)
				{
				case CHATLIKE:
					if(!stringContains(txt, checkentry->chat))
						{
							checkentry = checkentry->next;
							continue;
						}
					break;
					
				case CHATRE:
					q2a_strcpy(strbuffer, txt);
					q_strupr(strbuffer);
					if(regexec(checkentry->r, strbuffer, 0, 0, 0) == REG_NOMATCH)
						{
							checkentry = checkentry->next;
							continue;
						}
					break;
				}
				
				
			// ok, a ban situation..
			if(checkentry->msg)
				{
					currentBanMsg = checkentry->msg;
				}
				
			return 1;
		}
		
	return 0;
}



void listchatbansRun(int startarg, edict_t *ent, int client)
{
	addCmdQueue(client, QCMD_DISPCHATBANS, 0, 0, 0);
	
	gi.cprintf (ent, PRINT_HIGH, "Start Chat Ban List:\n");
}



void displayNextChatBan(edict_t *ent, int client, long chatbannum)
{
	long upto = chatbannum;
	chatbaninfo_t *findentry = chatbanhead;
	
	chatbannum++;
	
	while(findentry && upto)
		{
			findentry = findentry->next;
			upto--;
		}
		
	if(findentry)
		{
			if(findentry->loadType == LT_TEMP)
				{
					sprintf(buffer, " (%ld, Temporary) CHATBAN:", findentry->bannum);
				}
			else
				{
					sprintf(buffer, " (%ld, Permanent) CHATBAN:", findentry->bannum);
				}
				
			if(findentry->type == CHATLIKE)
				{
					q2a_strcat(buffer, " LIKE");
				}
			else if(findentry->type == CHATRE)
				{
					q2a_strcat(buffer, " RE");
				}
				
			q2a_strcat(buffer, " \"");
			q2a_strcat(buffer, findentry->chat);
			q2a_strcat(buffer, "\"");
			
			if(findentry->msg)
				{
					q2a_strcat(buffer, " MSG \"");
					q2a_strcat(buffer, findentry->msg);
					q2a_strcat(buffer, "\"");
				}
				
			gi.cprintf (ent, PRINT_HIGH, "%s\n", buffer);
			addCmdQueue(client, QCMD_DISPCHATBANS, 0, chatbannum, 0);
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "End Chat Ban List\n");
		}
}





void delchatbanRun(int startarg, edict_t *ent, int client)
{
	if (gi.argc() > startarg)
		{
			int banToDelete = q2a_atoi(gi.argv(startarg));
			chatbaninfo_t *findentry = chatbanhead, *prevban = NULL;
			
			while(findentry)
				{
					if(findentry->bannum == banToDelete)
						{
							break;
						}
						
					prevban = findentry;
					findentry = findentry->next;
				}
				
			if(findentry)
				{
					if(prevban)
						{
							prevban->next = findentry->next;
						}
					else
						{
							chatbanhead = findentry->next;
						}
						
					if(findentry->msg)
						{
							gi.TagFree(findentry->msg);
						}
						
					if(findentry->r)
						{
							regfree(findentry->r);
							gi.TagFree(findentry->r);
						}
					gi.TagFree(findentry);
					
					gi.cprintf (ent, PRINT_HIGH, "Chat Ban deleted.\n");
				}
			else
				{
					gi.cprintf (ent, PRINT_HIGH, "Chat Ban not found.\n");
				}
		}
	else
		{
			gi.cprintf (ent, PRINT_HIGH, "No chat ban number supplied to delete.\n");
		}
}


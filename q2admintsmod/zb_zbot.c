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
// zb_zbot.c
//
// copyright 2000 Shane Powell
//

#include "g_local.h"

//*** UPDATE START ***
admin_type admin_pass[MAX_ADMINS];
admin_type q2a_bypass_pass[MAX_ADMINS];
int num_admins = 0;
int num_q2a_admins = 0;
//*** UPDATE END ***

int clientsidetimeout = 30;  // 30 seconds should be good for internet play
int zbotdetectactivetimeout = 0;  // -1 == random

// char testchars[] = "!@#%^&*()_=|?.>,<[{]}\':1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
char testchars[] = "!@#%^&*()_=|?.>,<[{]}\':";
//                  012345678901234567890 1234567890123456789012345678901234567890123456789012345678901234
//                            1         2          3         4         5         6         7         8
int testcharslength = sizeof(testchars) - 1;


qboolean zbc_enable = TRUE;
qboolean timescaledetect = TRUE;
qboolean swap_attack_use = FALSE;
qboolean dopversion = TRUE;


byte impulsesToKickOn[MAXIMPULSESTOTEST];
byte maxImpulses = 0;

char *impulsemessages[] =
	{
		"169 (zbot toggle menu command)",
		"170 (zbot toggle menu command)",
		"171 (zbot toggle menu command)",
		"172 (zbot toggle menu command)",
		"173 (zbot toggle menu command)",
		"174 (zbot toggles bot on/off command)",
		"175 (zbot toggles scanner display command)"
	};
    
    
    
//===================================================================

void generateRandomString(char *buffer, int length)
{
	unsigned int index;
	for(index = 0; index < length; index++)
		{
			buffer[index] = RANDCHAR();
		}
	buffer[index] = 0;
}

qboolean checkImpulse(byte impulse)
{
	unsigned int i;
	
	if(!maxImpulses)
		{
			return TRUE;
		}
		
	for(i = 0; i < maxImpulses; i++)
		{
			if(impulsesToKickOn[i] == impulse)
				{
					return TRUE;
				}
		}
		
	return FALSE;
}





void readIpFromLog(int client, edict_t *ent)
{
	FILE *dumpfile;
	long fpos;
	
	if(proxyinfo[client].ipaddress[0])
		{
			return;
		}
		
	sprintf(buffer, "%s/qconsole.log", moddir);
	dumpfile = fopen(buffer, "rt");
	if(!dumpfile)
		{
			return;
		}
		
	fseek(dumpfile, 0, SEEK_END);
	fpos = ftell(dumpfile) - 1;
	
	while(getLastLine(buffer, dumpfile, &fpos))
		{
			if(startContains(buffer, "ip"))
				{
					char *cp = buffer + 3;
					char *dp = proxyinfo[client].ipaddress;
					
					SKIPBLANK(cp);
					
					while(*cp && *cp != ' ' && *cp != '\n')
						{
							*dp++ = *cp++;
						}
						
					*dp = 0;
					break;
				}
			else if(startContains(buffer, "userinfo"))
				{
					break;
				}
		}
		
	fclose(dumpfile);
}




int checkForOverflows(edict_t *ent, int client)
{
	FILE *q2logfile;
	char checkmask1[100], checkmask2[100];
	unsigned int ret = 0;
	
	sprintf(buffer, "%s/qconsole.log", moddir);
	q2logfile = fopen(buffer, "rt");
	if(!q2logfile)
		{
			return 0;  // assume ok
		}
		
	fseek(q2logfile, proxyinfo[client].logfilecheckpos, SEEK_SET);
	
	sprintf(checkmask1, "WARNING: msg overflowed for %s", proxyinfo[client].name);
	sprintf(checkmask2, "%s overflowed", proxyinfo[client].name);
	
	while(fgets(buffer, 256, q2logfile))
		{
			if(startContains(buffer, checkmask1) || startContains(buffer, checkmask2))
				{ // we have a problem...
					ret = 1;
					proxyinfo[client].clientcommand &= ~CCMD_ZPROXYCHECK2;
					removeClientCommand(client, QCMD_ZPROXYCHECK1);
					removeClientCommand(client, QCMD_ZPROXYCHECK2);
					addCmdQueue(client, QCMD_RESTART, 2 + (5 * random()), 0, 0);
					
					sprintf(checkmask1, "I(%d) Exp(%s) (%s) (overflow detected)", proxyinfo[client].charindex, proxyinfo[client].teststr, buffer);
					logEvent(LT_INTERNALWARN, client, ent, checkmask1, IW_OVERFLOWDETECT, 0.0);
					break;
				}
		}
		
	fseek(q2logfile, 0, SEEK_END);
	proxyinfo[client].logfilecheckpos = ftell(q2logfile);
	fclose(q2logfile);
	
	return ret;
}




void serverLogZBot(edict_t *ent, int client)
{
	addCmdQueue(client, QCMD_LOGTOFILE1, 0, 0, 0);
	
	if(customServerCmd[0])
		{
			// copy string across to buffer, replacing %c with client number
			char *cp = customServerCmd;
			char *dp = buffer;
			
			while(*cp)
				{
					if(*cp == '%' && tolower(*(cp + 1)) == 'c')
						{
							sprintf(dp, "%d", client);
							dp += q2a_strlen(dp);
							cp += 2;
						}
					else
						{
							*dp++ = *cp++;
						}
				}
				
			*dp = 0x0;
			
			gi.AddCommandString(buffer);
		}
}



//===================================================================


/*
static pmove_done = 0;
static vec3_t pmove_origin, pmove_old, pmove_orgvel, pmove_newvel;
static short pmove_orggrav;
*/


void Pmove_internal(pmove_t *pmove)
{
	if(q2adminrunmode== 0)
		{
			gi.Pmove(pmove);
			return;
		}
		
	//  pmove_orggrav = pmove->s.gravity;
	//  VectorCopy(pmove->s.velocity, pmove_orgvel);
	//  VectorCopy(pmove->s.origin, pmove_old);
	
	gi.Pmove(pmove);
	
	//  VectorCopy(pmove->s.origin, pmove_origin);
	//  VectorCopy(pmove->s.velocity, pmove_newvel);
	
	//  pmove_done = 1;
}


int VectorCompare (vec3_t v1, vec3_t v2)
{
	if (v1[0] != v2[0] || v1[1] != v2[1] || v1[2] != v2[2])
		return 0;
		
	return 1;
}


void ClientThink (edict_t *ent, usercmd_t *ucmd)
{
	int client;
	char *msg = 0;
	INITPERFORMANCE_2(1);
	INITPERFORMANCE_2(2);
	
	if(!dllloaded) return;
	
	if(q2adminrunmode == 0)
		{
			dllglobals->ClientThink(ent, ucmd);
			copyDllInfo();
			return;
		}
		
	client = getEntOffset(ent);
	client -= 1;
	
	STARTPERFORMANCE(1);

//*** UPDATE START ***
	proxyinfo[client].frames_count++;

	if (lframenum > proxyinfo[client].msec_start)
	{ 
		if (proxyinfo[client].show_fps)
		{
			if (proxyinfo[client].msec_count == 500)
			{
				gi.cprintf(ent, PRINT_HIGH, "%3.2f fps\n", (float)proxyinfo[client].frames_count * 2);
			}
		}

		if (proxyinfo[client].msec_count > msec_max)
		{
			if (msec_kick_on_bad)
			{
				proxyinfo[client].msec_bad++;
				if (proxyinfo[client].msec_bad >= msec_kick_on_bad)
				{
					//kick
	  				gi.bprintf(PRINT_HIGH, PRV_KICK_MSG, proxyinfo[client].name);
		   			addCmdQueue(client, QCMD_DISCONNECT, 1, 0, "Speed hack.");
				}
			}
			else
			{
				if (proxyinfo[client].enteredgame + 5 < ltime)			
				{
					proxyinfo[client].speedfreeze = ltime;
					proxyinfo[client].speedfreeze += 3;
				}
			}
		}

		proxyinfo[client].msec_start = lframenum;
		proxyinfo[client].msec_start += msec_int * 10;
		proxyinfo[client].msec_last = proxyinfo[client].msec_count;
		proxyinfo[client].msec_count = 0;
		proxyinfo[client].frames_count = 0;
	}

	proxyinfo[client].msec_count += ucmd->msec;

	if (proxyinfo[client].speedfreeze)
	{
		if (proxyinfo[client].speedfreeze > ltime)
		{
			ucmd->msec = 0;
		}
		else
		{
			if (speedbot_check_type & 2)
			{
				// gi.bprintf(PRINT_HIGH, "%s has been frozen for exceeding the speed limit.\n", proxyinfo[client].name);
			}
			proxyinfo[client].speedfreeze = 0;
		}
		
	}
//*** UPDATE END ***
	
	if(ucmd->impulse)
		{
			if(client >= maxclients->value) return;
			
			if(displayimpulses)
				{
					if(ucmd->impulse >= 169 && ucmd->impulse <= 175)
						{
							msg = impulsemessages[ucmd->impulse - 169];
							gi.bprintf (PRINT_HIGH, "%s generated an impulse %s\n", proxyinfo[client].name, msg);
						}
					else
						{
							msg = "generated an impulse";
							gi.bprintf (PRINT_HIGH, "%s generated an impulse %d\n", proxyinfo[client].name, ucmd->impulse);
						}
				}
				
			if(ucmd->impulse >= 169 && ucmd->impulse <= 175)
				{
					proxyinfo[client].impulse = ucmd->impulse;
					addCmdQueue(client, QCMD_LOGTOFILE2, 0, 0, 0);
				}
			else
				{
					proxyinfo[client].impulse = ucmd->impulse;
					addCmdQueue(client, QCMD_LOGTOFILE3, 0, 0, 0);
				}
				
			if(disconnectuserimpulse && checkImpulse(ucmd->impulse))
				{
					proxyinfo[client].impulsesgenerated++;
					
					if(proxyinfo[client].impulsesgenerated >= maximpulses)
						{
							addCmdQueue(client, QCMD_DISCONNECT, 1, 0, msg);
						}
				}
		}
		
	if(swap_attack_use)
		{
			byte temp = (ucmd->buttons & BUTTON_ATTACK);
			
			if(ucmd->buttons & BUTTON_USE)
				{
					ucmd->buttons |= BUTTON_ATTACK;
				}
			else
				{
					ucmd->buttons &= ~BUTTON_ATTACK;
				}
				
			if(temp)
				{
					ucmd->buttons |= BUTTON_USE;
				}
			else
				{
					ucmd->buttons &= ~BUTTON_USE;
				}
		}
		
	if(!(proxyinfo[client].clientcommand & BANCHECK))
		{
			if(zbc_enable && !(proxyinfo[client].clientcommand & CCMD_ZBOTDETECTED))
				{
					if(zbc_ZbotCheck(client, ucmd))
						{
							proxyinfo[client].clientcommand |= (CCMD_ZBOTDETECTED | CCMD_ZPROXYCHECK2);
							removeClientCommand(client, QCMD_ZPROXYCHECK1);
							addCmdQueue(client, QCMD_ZPROXYCHECK2, 1, IW_ZBCHECK, 0);
							addCmdQueue(client, QCMD_RESTART, 1, IW_ZBCHECK, 0);
						}
				}
				
			STARTPERFORMANCE(2);
			dllglobals->ClientThink(ent, ucmd);
			STOPPERFORMANCE_2(2, "mod->ClientThink", 0, NULL);
			
			copyDllInfo();
		}
		
	STOPPERFORMANCE_2(1, "q2admin->ClientThink", 0, NULL);
}

//*** UPDATE START ***
void PMOD_TimerCheck(int client)
{
	edict_t *ent;	
	ent = getEnt((client + 1));

	proxyinfo[client].pmodver = ltime + 10;
	proxyinfo[client].pmod = 0;
	proxyinfo[client].pver = 0;
	addCmdQueue(client, QCMD_PMODVERTIMEOUT_INGAME, 10, 0, 0);
	gi.cprintf(ent, PRINT_HIGH, "q2admin: p_modified Standard Proxy Test\r\n");

	if (gl_driver_check & 1)
		stuffcmd(ent,"say Q2ADMIN_GL_DRIVER_CHECK $gl_driver / $vid_ref / $gl_mode\n");

	if (q2a_command_check)
	{
		addCmdQueue(client, QCMD_GETCMDQUEUE, 5, 0, 0);
	}
}
//*** UPDATE END ***

void G_RunFrame(void)
{
	unsigned int j, required_cmdlist;	//UPDATE

	int maxdoclients;
	static int client = -1;
	edict_t *ent;
	byte command;
	unsigned long data;
	char *str;
	char checkConnectProxy[RANDOM_STRING_LENGTH+1];
	char ReconnectString[RANDOM_STRING_LENGTH+1];
	char rndConnectString[RANDOM_STRING_LENGTH+1];	//UPDATE
	
	INITPERFORMANCE_2(1);
	INITPERFORMANCE_2(2);
	
	if(!dllloaded) return;
	
	if(q2adminrunmode == 0)
		{
			dllglobals->RunFrame();
			copyDllInfo();
			return;
		}
		
	STARTPERFORMANCE(1);
	
	lframenum++;
	ltime = lframenum * FRAMETIME;
	
	if(serverinfoenable && (lframenum > 10))
		{
			//    sprintf(buffer, "logfile 2;set Bot \"No Bots\" s\n");
			sprintf(buffer, "set Q2Admin \"" Q2ADMINVERSION "\" s\n");
			gi.AddCommandString(buffer);
			serverinfoenable = 0;
		}
		
	// check if a lrcon password has timed out
	check_lrcon_password();
	
	if(maxReconnectList)
		{
			unsigned int i;
			
			for(i = 0; i < maxReconnectList; i++)
				{
					if(reconnectlist[i].reconnecttimeout < ltime)
						{
							unsigned int j;
							
							// remove the retry list entry if needed...
							for(j = 0; j < maxReconnectList; j++)
								{
									if((j != i) && (reconnectlist[j].retrylistidx == reconnectlist[i].retrylistidx))
										{
											break;
										}
								}
								
							if(j >= maxReconnectList)
								{
									if((reconnectlist[i].retrylistidx + 1) < maxretryList)
										{
											q2a_memmove(&(retrylist[reconnectlist[i].retrylistidx]), &(retrylist[reconnectlist[i].retrylistidx + 1]), (maxretryList - (reconnectlist[i].retrylistidx + 1)) * sizeof(retrylist_info));
										}
									maxretryList--;
								}
								
							if((i + 1) < maxReconnectList)
								{
									q2a_memmove(&(reconnectlist[i]), &(reconnectlist[i + 1]), (maxReconnectList - (i + 1)) * sizeof(reconnect_info));
									i--;
								}
							maxReconnectList--;
						}
				}
		}
		
	if(framesperprocess && ((lframenum % framesperprocess) != 0))
		{
			dllglobals->RunFrame();
			copyDllInfo();
			return;
		}
		
	maxdoclients = client;
	maxdoclients += maxclientsperframe;

	if(maxdoclients > maxclients->value)
		{
			maxdoclients = maxclients->value;
		}
		
	for(; client < maxdoclients; client++)
		{
//*** UPDATE START ***
			if(client < 0)
			{
				ent = NULL;
			}
			else
			{
				ent = getEnt((client + 1));
			}

			if (timers_active)
				timer_action(client,ent);
//*** UPDATE END ***

			if(getCommandFromQueue(client, &command, &data, &str))
				{		
					if(!proxyinfo[client].inuse)
					{
						if(command == QCMD_STARTUP)
						{
							addCmdQueue(client, QCMD_STARTUPTEST, 2, 0, 0);
							proxyinfo[client].clientcommand |= CCMD_STARTUPTEST;
						}
						else if(command == QCMD_STARTUPTEST)
						{
//*** UPDATE START ***
							addCmdQueue(client, QCMD_EXECMAPCFG, 5, 0, 0);
							if (do_franck_check)
							{
								stuffcmd(ent,"riconnect; roconnect; connect; set frkq2 disconnect; set quake2frk disconnect; set q2frk disconnect\n");
							}

							if (do_vid_restart)
							{
								if (!proxyinfo[client].vid_restart)
								{
									proxyinfo[client].vid_restart = true;
									stuffcmd(ent,"vid_restart\n");
								}
							}

							addCmdQueue(client, QCMD_SHOWMOTD, 0, 0, 0);
//*** UPDATE END***
							if(proxyinfo[client].clientcommand & CCMD_ZBOTDETECTED)
								{
									break;
								}

							proxyinfo[client].inuse = 1;

							if(proxyinfo[client].retries > MAXSTARTTRY)
								{
									if(zbotdetect)
										{
											serverLogZBot(ent, client);
											proxyinfo[client].clientcommand &= ~CCMD_STARTUPTEST;
											proxyinfo[client].clientcommand |= (CCMD_ZBOTDETECTED | CCMD_ZPROXYCHECK2);
											addCmdQueue(client, QCMD_ZPROXYCHECK2, 1, IW_STARTUP, 0);
											proxyinfo[client].charindex = -1;
											logEvent(LT_INTERNALWARN, client, ent, "Startup Init Fail", IW_STARTUPFAIL, 0.0);
										}
									break;
								}

							stuffcmd(ent, zbot_teststring1);
							addCmdQueue(client, QCMD_STARTUPTEST, 5, 0, 0);
							proxyinfo[client].retries++;
						}
						else if((command == QCMD_DISCONNECT) || (command == QCMD_KICK))
						{
							//stuffcmd(ent, "disconnect\n");
							proxyinfo[client].clientcommand |= CCMD_KICKED;
							logEvent(LT_CLIENTKICK, client, ent, str, 0, 0.0);
							gi.cprintf (ent, PRINT_HIGH, "You have been kicked %s\n", proxyinfo[client].name);
							sprintf(buffer, "\nkick %d\n", client);
							gi.AddCommandString(buffer);
						}
						else if(command == QCMD_RECONNECT)
						{
							unsigned int i;
							char ipbuffer[40];
							char *ip = ipbuffer;
							char *bp = ip;

							strcpy(ipbuffer, proxyinfo[client].ipaddress);

							while(*bp && (*bp != ':'))
								{
									bp++;
								}

							*bp = 0;

							if ( *ip )
								{
									q2a_strcpy(reconnectlist[maxReconnectList].userinfo, proxyinfo[client].userinfo);
									reconnectlist[maxReconnectList].reconnecttimeout = ltime;
									reconnectlist[maxReconnectList].reconnecttimeout += reconnect_time;

									// check for retry entry..
									for(i = 0; i < maxretryList; i++)
										{
											if(q2a_strcmp(retrylist[i].ip, ip) == 0)
												{
													break;
												}
										}

									if(i < maxretryList)
										{
											if(retrylist[i].retry >= 5)
												{
													unsigned int j;

													// remove the retry list entry if needed...
													for(j = 0; j < maxReconnectList; j++)
														{
															if(reconnectlist[j].retrylistidx == i)
																{
																	break;
																}
														}
													if(j >= maxReconnectList)
														{
															if((i + 1) < maxretryList)
																{
																	q2a_memmove(&(retrylist[i]), &(retrylist[i + 1]), (maxretryList - (i + 1)) * sizeof(retrylist_info));
																}
															maxretryList--;
														}

													// cut off here...
													sprintf(buffer, "\ndisconnect\n");
													stuffcmd(ent, buffer);
													break;
												}
														
											retrylist[i].retry++;
											reconnectlist[maxReconnectList].retrylistidx = i;
										}
									else
										{
											q2a_strcpy(retrylist[maxretryList].ip, ip);
											retrylist[maxretryList].retry = 0;
											maxretryList++;
										}

									maxReconnectList++;
								}

							q2a_strcpy(buffer, ("%s\n", defaultreconnectmessage));
							gi.cprintf (ent, PRINT_HIGH, buffer);

							generateRandomString(ReconnectString, 5);
							generateRandomString(rndConnectString, 5);      //UPDATE
							sprintf(buffer, "\nset %s %s\nset %s connect\n",ReconnectString, reconnect_address, rndConnectString);  //UPDATE
							stuffcmd(ent,buffer);

							generateRandomString(proxyinfo[client].hack_teststring3, RANDOM_STRING_LENGTH);
							generateRandomString(checkConnectProxy, RANDOM_STRING_LENGTH);

							sprintf(buffer, "\nalias connect %s\nalias %s $%s $%s\n%s\n", proxyinfo[client].hack_teststring3, checkConnectProxy, rndConnectString, ReconnectString, checkConnectProxy);	//UPDATE

							proxyinfo[client].clientcommand |= CCMD_WAITFORCONNECTREPLY;
							stuffcmd(ent, buffer);
							//addCmdQueue(client, QCMD_KICK, 0, 0, NULL);
						}
						else
						{
							// add command to back of line for processing later..
							addCmdQueue(client, command, 0, data, str);
						}
					}
				else if(command == QCMD_STARTUPTEST)
				{
					if(proxyinfo[client].clientcommand & CCMD_ZBOTDETECTED)
						{
							break;
						}

					if(proxyinfo[client].retries > MAXSTARTTRY)
						{
							if(zbotdetect)
								{
									serverLogZBot(ent, client);
									proxyinfo[client].clientcommand &= ~CCMD_STARTUPTEST;
									proxyinfo[client].clientcommand |= (CCMD_ZBOTDETECTED | CCMD_ZPROXYCHECK2);
									addCmdQueue(client, QCMD_ZPROXYCHECK2, 1, IW_STARTUPTEST, 0);
									proxyinfo[client].charindex = -2;
									logEvent(LT_INTERNALWARN, client, ent, "Startup Init Fail 2", IW_STARTUPFAIL, 0.0);
									break;
								}
						}

					stuffcmd(ent, zbot_teststring1);
					addCmdQueue(client, QCMD_STARTUPTEST, 5, 0, 0);
					proxyinfo[client].retries++;
				}
				else if(command == QCMD_LETRATBOTQUIT)
				{
					if(zbotdetect)
						{
							sprintf(buffer, "\n%s\n", zbot_teststring_test3);
							stuffcmd(ent, buffer);
							stuffcmd(ent, buffer);
							stuffcmd(ent, buffer);
						}
				}
				else if(command == QCMD_RESTART)
				{
					if(zbotdetect)
						{
							if(!(proxyinfo[client].clientcommand & CCMD_ZBOTCLEAR))
								{
									addCmdQueue(client, QCMD_ZPROXYCHECK1, 0, 0, 0); // retry check for proxy
								}
						}
				}
				else if(command == QCMD_ZPROXYCHECK1)
				{
					// are we at the end?
					if(proxyinfo[client].charindex >= testcharslength)
						{
							break;
						}

					// begin test for proxies
					proxyinfo[client].teststr[0] = testchars[proxyinfo[client].charindex];
					proxyinfo[client].teststr[1] = BOTDETECT_CHAR1;
					proxyinfo[client].teststr[2] = BOTDETECT_CHAR2;
					proxyinfo[client].teststr[3] = zbot_testchar1;
					proxyinfo[client].teststr[4] = zbot_testchar2;
					proxyinfo[client].teststr[5] = RANDCHAR();
					proxyinfo[client].teststr[6] = RANDCHAR();
					proxyinfo[client].teststr[7] = 0;
					proxyinfo[client].teststr[8] = 0;

					sprintf(buffer, "\n%s\n%s\n", proxyinfo[client].teststr, zbot_teststring_test2);
					stuffcmd(ent, buffer);

					proxyinfo[client].clientcommand |= CCMD_ZPROXYCHECK2;
					addCmdQueue(client, QCMD_ZPROXYCHECK2, clientsidetimeout, IW_ZBOTTEST, 0);
				}
				else if(command == QCMD_ZPROXYCHECK2) // are we dealing with a proxy???
				{
					char text[35];

					if(!(proxyinfo[client].clientcommand & CCMD_ZPROXYCHECK2))
						{
							sprintf(text, "I(%d) Exp(%s)", proxyinfo[client].charindex, proxyinfo[client].teststr);
							logEvent(LT_INTERNALWARN, client, ent, text, data, 0.0);
							break;
						}

					if(proxyinfo[client].charindex >= testcharslength)
						{
							sprintf(text, "I(%d >= end) Exp(%s)", proxyinfo[client].charindex, proxyinfo[client].teststr);
							logEvent(LT_INTERNALWARN, client, ent, text, data, 0.0);
							break;
						}

					// yes...  detected by long timeout or the normal timeout on detect
					if(!(proxyinfo[client].clientcommand & CCMD_ZBOTDETECTED))
						{
							if(checkForOverflows(ent, client))
								{
									break;
								}

							if(proxyinfo[client].retries < MAXDETECTRETRIES)
								{
									// try and get "unknown command" off the screen as fast as possible
									proxyinfo[client].clientcommand &= ~CCMD_ZPROXYCHECK2;
									addCmdQueue(client, QCMD_CLEAR, 0, 0, 0);
									addCmdQueue(client, QCMD_RESTART, 2 + (3 * random()), 0, 0);
									proxyinfo[client].retries++;
									break;
								}

							serverLogZBot(ent, client);
						}

					proxyinfo[client].clientcommand &= ~CCMD_ZPROXYCHECK2;
					proxyinfo[client].clientcommand |= CCMD_ZBOTDETECTED;

					if(displayzbotuser)
						{
							unsigned int i;

							q2a_strcpy(buffer, ("%s\n", zbotuserdisplay));

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
//*** UPDATE START ***							
				else if(command == QCMD_TESTSTANDARDPROXY)
				{
					if (private_commands[0].command[0])
					{
						addCmdQueue(client, QCMD_PRIVATECOMMAND, 10, 0, 0);
						stuff_private_commands(client,ent);	
					}

					if (!proxyinfo[client].q2a_bypass)
					{
						if(dopversion)
						{
/*							if (client_check > 0)
							{
								addCmdQueue(client, QCMD_PMODVERTIMEOUT, 0, 0, 0);
								gi.cprintf(ent, PRINT_HIGH, "%s: p_version Standard Proxy Test\r\n", proxyinfo[client].name);
							}*/
						}
					}
				}
//*** UPDATE END ***							
				else if(command == QCMD_TESTRATBOT)
				{
					gi.cprintf(ent, PRINT_HIGH, "ratbot Detect Test ( %s )\r\n", "rbkck &%trf .disconnect");
					addCmdQueue(client, QCMD_TESTRATBOT2, clientsidetimeout, 0, 0);
					proxyinfo[client].clientcommand |= CCMD_RATBOTDETECT;
					addCmdQueue(client, QCMD_TESTRATBOT3, 2, 0, 0);
				}
				else if(command == QCMD_TESTRATBOT2)
				{
					if(!(proxyinfo[client].clientcommand & CCMD_RATBOTDETECT))
						{
							logEvent(LT_INTERNALWARN, client, ent, "RatBot detect problem", 0, 0.0);
							break;
						}
					//proxyinfo[client].clientcommand &= ~CCMD_RATBOTDETECT;
				}
				else if(command == QCMD_TESTRATBOT3)
				{
					proxyinfo[client].clientcommand |= CCMD_RATBOTDETECTNAME;
					addCmdQueue(client, QCMD_TESTRATBOT4, clientsidetimeout, 0, 0);
					sprintf(buffer, "\nname " RATBOT_CHANGENAMETEST ";wait;wait;name \"%s\"\n", proxyinfo[client].name);
					stuffcmd(ent, buffer);
				}
				else if(command == QCMD_TESTRATBOT4)
				{
					if(!(proxyinfo[client].clientcommand & CCMD_RATBOTDETECTNAME))
						{
							logEvent(LT_INTERNALWARN, client, ent, "RatBot Detect 2 problem", 0, 0.0);
							break;
						}

					// yes...  detected by long timeout or the normal timeout on detect
					if(!(proxyinfo[client].clientcommand & CCMD_ZBOTDETECTED))
						{
							if(checkForOverflows(ent, client))
								{
									addCmdQueue(client, QCMD_TESTRATBOT3, 2, 0, 0);
									break;
								}

							if(proxyinfo[client].rbotretries < MAXDETECTRETRIES)
								{
									//            proxyinfo[client].clientcommand &= ~CCMD_RATBOTDETECTNAME;
									addCmdQueue(client, QCMD_TESTRATBOT3, 2 + (3 * random()), 0, 0);
									proxyinfo[client].rbotretries++;
									break;
								}

							proxyinfo[client].charindex = -4;
							serverLogZBot(ent, client);
						}

					proxyinfo[client].clientcommand &= ~CCMD_RATBOTDETECTNAME;
					proxyinfo[client].clientcommand |= CCMD_ZBOTDETECTED;

					if(displayzbotuser)
						{
							unsigned int i;

							q2a_strcpy(buffer, ("%s\n", zbotuserdisplay));

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
				else if(command == QCMD_TESTALIASCMD1)
				{
					generateRandomString(proxyinfo[client].hack_teststring1, RANDOM_STRING_LENGTH);
					generateRandomString(proxyinfo[client].hack_teststring2, RANDOM_STRING_LENGTH);
					sprintf(buffer, "\nalias %s %s\n", proxyinfo[client].hack_teststring1, proxyinfo[client].hack_teststring2);
					stuffcmd(ent, buffer);
					proxyinfo[client].clientcommand |= CCMD_WAITFORALIASREPLY1;
					addCmdQueue(client, QCMD_TESTALIASCMD2, 1, 0, NULL);
				}
				else if(command == QCMD_TESTALIASCMD2)
				{
					sprintf(buffer, "\n%s\n", proxyinfo[client].hack_teststring1);
					stuffcmd(ent, buffer);
					proxyinfo[client].clientcommand |= CCMD_WAITFORALIASREPLY2;
				}
				else if(command == QCMD_DISPLOGFILE)
				{
					displayLogFileCont(ent, client, data);
				}
				else if(command == QCMD_DISPLOGFILELIST)
				{
					displayLogFileListCont(ent, client, data);
				}
				else if(command == QCMD_DISPLOGEVENTLIST)
				{
					displayLogEventListCont(ent, client, data, FALSE);
				}
				else if(command == QCMD_GETIPALT)
				{
					// open logfile and read IP address from log
					readIpFromLog(client, ent);
					addCmdQueue(client, QCMD_GETIPALT, 0, 0, 0);
				}
				else if(command == QCMD_LOGTOFILE1)
				{
					logEvent(LT_ZBOT, client, ent, NULL, proxyinfo[client].charindex, 0.0);
				}
				else if(command == QCMD_LOGTOFILE2)
				{
					logEvent(LT_ZBOTIMPULSES, client, ent, impulsemessages[proxyinfo[client].impulse - 169], proxyinfo[client].impulse, 0.0);
				}
				else if(command == QCMD_LOGTOFILE3)
				{
					logEvent(LT_IMPULSES, client, ent, NULL, proxyinfo[client].impulse, 0.0);
				}
				else if(command == QCMD_CONNECTCMD)
				{
					if(customClientCmdConnect[0])
						{
							sprintf(buffer, "%s\n", customClientCmdConnect);
							stuffcmd(ent, buffer);
						}

					if(customServerCmdConnect[0])
						{
							// copy string across to buffer, replacing %c with client number
							char *cp = customServerCmdConnect;
							char *dp = buffer;

							while(*cp)
								{
									if((*cp == '%') && (tolower(*(cp + 1)) == 'c'))
										{
											sprintf(dp, "%d", client);
											dp += q2a_strlen(dp);
											cp += 2;
										}
									else
										{
											*dp++ = *cp++;
										}
								}

							*dp = 0x0;

							gi.AddCommandString(buffer);
						}
				}
				else if(command == QCMD_CLEAR)
				{
					stuffcmd(ent, "clear\n");
				}
				else if(command == QCMD_CUSTOM)
				{
					if(customClientCmd[0])
						{
							sprintf(buffer, "%s\n", customClientCmd);
							stuffcmd(ent, buffer);
						}
				}
				else if((command == QCMD_DISCONNECT) || (command == QCMD_KICK))
				{
					//stuffcmd(ent, "disconnect\n");
					proxyinfo[client].clientcommand |= CCMD_KICKED;
					logEvent(LT_CLIENTKICK, client, ent, str, 0, 0.0);
					gi.cprintf (ent, PRINT_HIGH, "You have been kicked %s\n", proxyinfo[client].name);
					sprintf(buffer, "\nkick %d\n", client);
					gi.AddCommandString(buffer);
				}
				else if(command == QCMD_RECONNECT)
				{
					sprintf(buffer, "\nconnect %s\n", reconnect_address);
					stuffcmd(ent, buffer);
					//        addCmdQueue(client, QCMD_KICK, 0, 0, NULL);
				}
				else if(command == QCMD_CLIPTOMAXRATE)
				{
					sprintf(buffer, "rate %d\n", maxrateallowed);
					stuffcmd(ent, buffer);
				}
				else if(command == QCMD_CLIPTOMINRATE)
				{
					sprintf(buffer, "rate %d\n", minrateallowed);
					stuffcmd(ent, buffer);
				}
				else if(command == QCMD_SETUPMAXFPS)
				{
					stuffcmd(ent, "set cl_maxfps $cl_maxfps u\n");
					addCmdQueue(client, QCMD_FORCEUDATAUPDATE, 0, 0, 0);
				}
				else if(command == QCMD_FORCEUDATAUPDATE)
				{
					if(proxyinfo[client].rate)
						{
							sprintf(buffer, "set rate %d\nset rate %d\n", proxyinfo[client].rate + 1, proxyinfo[client].rate);
							stuffcmd(ent, buffer);
						}
				}
				else if(command == QCMD_SETMAXFPS)
				{
					if(maxfpsallowed)
						{
							sprintf(buffer, "cl_maxfps %d\n", maxfpsallowed);
							stuffcmd(ent, buffer);
						}
				}
				else if(command == QCMD_SETMINFPS)
				{
					if(minfpsallowed)
						{
							sprintf(buffer, "cl_maxfps %d\n", minfpsallowed);
							stuffcmd(ent, buffer);
						}
				}
				else if(command == QCMD_DISPBANS)
				{
					displayNextBan(ent, client, data);
				}
				else if(command == QCMD_DISPLRCONS)
				{
					displayNextLRCon(ent, client, data);
				}
				else if(command == QCMD_DISPFLOOD)
				{
					displayNextFlood(ent, client, data);
				}
				else if(command == QCMD_DISPSPAWN)
				{
					displayNextSpawn(ent, client, data);
				}
							
				else if(command == QCMD_DISPVOTE)
				{
					displayNextVote(ent, client, data);
				}
				else if(command == QCMD_DISPDISABLE)
				{
					displayNextDisable(ent, client, data);
				}
				else if(command == QCMD_DISPCHECKVAR)
				{
					displayNextCheckvar(ent, client, data);
				}
				else if(command == QCMD_CHECKVARTESTS)
				{
					checkVariableTest(ent, client, data);
				}
				else if(command == QCMD_CHANGENAME)
				{
					sprintf(buffer, "name \"%s\"\n", proxyinfo[client].name);
					stuffcmd(ent, buffer);
				}
				else if(command == QCMD_CHANGESKIN)
				{
					sprintf(buffer, "skin \"%s\"\n", proxyinfo[client].skin);
					stuffcmd(ent, buffer);
				}
				else if(command == QCMD_BAN)
				{
					gi.cprintf (NULL, PRINT_HIGH, "%s: %s\n", proxyinfo[client].name, proxyinfo[client].buffer);
					gi.cprintf (ent, PRINT_HIGH, "%s: %s\n", proxyinfo[client].name, proxyinfo[client].buffer);
					addCmdQueue(client, QCMD_DISCONNECT, 1, 0, proxyinfo[client].buffer);
				}
				else if(command == QCMD_DISPCHATBANS)
				{
					displayNextChatBan(ent, client, data);
				}
				else if(command == QCMD_STUFFCLIENT)
				{
					stuffNextLine(ent, client);
				}
				else if(command == QCMD_TESTADMIN)
				{
					stuffcmd(ent, "!setadmin $q2adminpassword\n");
				}
//*** UPDATE START ***
				else if(command == QCMD_TESTADMIN2)
				{
					stuffcmd(ent, "!admin $q2adminuser $q2adminpass\n");
				}
				else if(command == QCMD_TESTADMIN3)
				{
					stuffcmd(ent, "!bypass $clientuser $clientpass\n");
				}
				else if(command == QCMD_PMODVERTIMEOUT)
				{
					//no reply? kick the bastard
					if (!proxyinfo[client].q2a_bypass)
					{
						/*if (client_check > 0)
						{
							if (proxyinfo[client].pmod != 1)
							{
								gi.bprintf(PRINT_HIGH,NOMATCH_KICK_MSG,proxyinfo[client].name);
								sprintf(buffer,client_msg,version_check);
								gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",buffer);
								addCmdQueue(client, QCMD_DISCONNECT, 1, 0, Q2A_NOMATCH_KICK_MSG);
			  				}
							else if (proxyinfo[client].cmdlist == 7) //Kick false NoCheat clients
							{
								gi.bprintf(PRINT_HIGH, PRV_KICK_MSG, proxyinfo[client].name);
								sprintf(buffer,client_msg,version_check);
								gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",buffer);
								addCmdQueue(client, QCMD_DISCONNECT, 1, 0, FRKQ2_KICK_MSG);
							}
		  				}*/
					}
				}
				else if(command == QCMD_PRIVATECOMMAND)
				{
					for (j=0;j<PRIVATE_COMMANDS;j++)
					{
						//check each command, if we didnt get a response log it
						if (private_commands[j].command[0])
						{
							if ( ((!proxyinfo[client].private_command_got[j]) && (j<4)) || ((proxyinfo[client].private_command_got[j]) && (j>3)) )
							{
								//log
								logEvent(LT_PRIVATELOG, client, ent, private_commands[j].command, 0,0.0);

								//kick on private_command
								if (private_command_kick)
								{
									gi.bprintf(PRINT_HIGH, PRV_KICK_MSG, proxyinfo[client].name);
									// %%quadz 17nov06 -- stop bogus 'modified client' kicks that are just lag spikes
									// addCmdQueue(client, QCMD_DISCONNECT, 1, 0, Q2A_PRV_KICK_MSG);
									//dont want this printed
									//return FALSE;
								}
							}
						}
					}
				}
				else if(command == QCMD_PMODVERTIMEOUT_INGAME)
				{
					if (!proxyinfo[client].q2a_bypass)
					{
						/*if (client_check > 0)
						{
						//no reply? increase no reply count
							if (!proxyinfo[client].pmod)
							{
								proxyinfo[client].pmod_noreply_count++;
								if (proxyinfo[client].pmod_noreply_count > max_pmod_noreply)
								{
									gi.bprintf(PRINT_HIGH,MOD_KICK_MSG,proxyinfo[client].name,proxyinfo[client].pmod);	          
									sprintf(buffer,client_msg,version_check);
									gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",buffer);
									addCmdQueue(client, QCMD_DISCONNECT, 1, 0, Q2A_MOD_KICK_MSG);
								}
							}
							else if (proxyinfo[client].pmod != 1)
							{
								gi.bprintf(PRINT_HIGH,MOD_KICK_MSG,proxyinfo[client].name,proxyinfo[client].pmod);	          
								sprintf(buffer,client_msg,version_check);
								gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",buffer);
								addCmdQueue(client, QCMD_DISCONNECT, 1, 0, Q2A_MOD_KICK_MSG);
							}
						}*/
					}
				}
				else if(command == QCMD_GL_CHECK)
				{
				}
				else if(command == QCMD_SETUPTIMESCALE)
				{
					stuffcmd(ent, "set timescale $timescale u\n");
				}
				else if(command == QCMD_SETTIMESCALE)
				{
					if(timescaledetect)
					{
						strcpy(buffer, "set timescale 1\n");
						stuffcmd(ent, buffer);
					}
				}
				else if(command == QCMD_SPAMBYPASS)
				{
					if(proxyinfo[client].q2a_bypass)
					{
						gi.bprintf(PRINT_HIGH, "ƒ  %s has logged on without an anti-cheat client because of an arrangement\nƒ  with the server admin.  This is most likely because %s is using a linux\nƒ  or mac client - contact the server admin if you have issues with %s.\n", proxyinfo[client].name, proxyinfo[client].name, proxyinfo[client].name);
					}
				}
				else if(command == QCMD_GETCMDQUEUE)
				{
					addCmdQueue(client, QCMD_TESTCMDQUEUE, 5, 0, 0);
					proxyinfo[client].cmdlist_timeout = ltime;
					proxyinfo[client].cmdlist_timeout += 5;
					proxyinfo[client].cmdlist = 1;
  					//1.20
					if (!proxyinfo[client].done_server_and_blocklist)
					{
						proxyinfo[client].blocklist = random()*(MAX_BLOCK_MODELS-1);
						sprintf(buffer,"p_blocklist %i\n",proxyinfo[client].blocklist);
						stuffcmd(ent,buffer); 
						generateRandomString(proxyinfo[client].serverip, 15);
						sprintf(buffer,"p_server %s\n",proxyinfo[client].serverip);
						stuffcmd(ent,buffer); 
						//q2ace responds with blahblah %i %s
					}
				}
				else if(command == QCMD_TESTCMDQUEUE)
				{
					if (proxyinfo[client].done_server_and_blocklist)
						required_cmdlist = 1;
					else
						required_cmdlist = 7;

					if (!proxyinfo[client].cmdlist)
					{
						proxyinfo[client].pcmd_noreply_count++;
						if (proxyinfo[client].pcmd_noreply_count > max_pmod_noreply)
						{
							gi.bprintf(PRINT_HIGH,MOD_KICK_MSG,proxyinfo[client].name,16);
							//sprintf(buffer,client_msg,version_check);
							//gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",buffer);
							addCmdQueue(client, QCMD_DISCONNECT, 1, 0, Q2A_MOD_KICK_MSG);
						}
					}
					else if (proxyinfo[client].cmdlist == required_cmdlist)
					{
						//all 3 checks came thru fine
						proxyinfo[client].done_server_and_blocklist = 1;
					}
					else
					{
						//just kick anyway
						//gi.bprintf(PRINT_HIGH,MOD_KICK_MSG,proxyinfo[client].name,proxyinfo[client].cmdlist);
						//sprintf(buffer,client_msg,version_check);
						//gi.cprintf(getEnt((client + 1)),PRINT_HIGH,"%s\n",buffer);
						addCmdQueue(client, QCMD_DISCONNECT, 1, 0, Q2A_MOD_KICK_MSG);
					}
				}
				else if(command == QCMD_EXECMAPCFG)
				{
					if (client_map_cfg & 1)
					{
						sprintf(buffer,"set map_name %s\n",gmapname);
						stuffcmd(ent,buffer);
					}
					else if (client_map_cfg & 2)
					{
						sprintf(buffer,"exec cfg/%s.cfg\n",gmapname);
						stuffcmd(ent,buffer);
					}
					else if (client_map_cfg & 4)
					{
						sprintf(buffer,"exec cfg/all.cfg\n");
						stuffcmd(ent,buffer);
					}
				}
				else if(command == QCMD_SHOWMOTD)
				{
					if(zbotmotd[0])
					{
						gi.centerprintf(ent, motd);
					}
				}
//*** UPDATE END ***							
				else if(command == QCMD_RUNVOTECMD)
				{
					gi.AddCommandString(cmdpassedvote);
				}
				else if(command == QCMD_TESTTIMESCALE)
				{
					if(timescaledetect)
						{
							generateRandomString(proxyinfo[client].hack_timescale, RANDOM_STRING_LENGTH);
							sprintf(buffer, "%s $timescale\n", proxyinfo[client].hack_timescale);
							stuffcmd(ent, buffer);
							addCmdQueue(client, QCMD_TESTTIMESCALE, 15, 0, 0);
						}
				}
				else if(command == QCMD_SETUPCL_PITCHSPEED)
				{
					stuffcmd(ent, "set cl_pitchspeed $cl_pitchspeed u\n");
					addCmdQueue(client, QCMD_FORCEUDATAUPDATEPS, 0, 0, 0);
				}
				else if(command == QCMD_FORCEUDATAUPDATEPS)
				{
					if(proxyinfo[client].cl_pitchspeed)
						{
							sprintf(buffer, "set cl_pitchspeed %d\nset cl_pitchspeed %d\n", proxyinfo[client].cl_pitchspeed + 1, proxyinfo[client].cl_pitchspeed);
							stuffcmd(ent, buffer);
						}
				}
				else if(command == QCMD_SETUPCL_ANGLESPEEDKEY)
				{
					stuffcmd(ent, "set cl_anglespeedkey $cl_anglespeedkey u\n");
					addCmdQueue(client, QCMD_FORCEUDATAUPDATEAS, 0, 0, 0);
				}
				else if(command == QCMD_FORCEUDATAUPDATEAS)
				{
					if(proxyinfo[client].cl_anglespeedkey)
						{
							sprintf(buffer, "set cl_anglespeedkey %g\nset cl_anglespeedkey %g\n", proxyinfo[client].cl_anglespeedkey + 1.0, proxyinfo[client].cl_anglespeedkey);
							stuffcmd(ent, buffer);
						}
				}
				else if(command == QCMD_MSGDISCONNECT)
				{
					sprintf(buffer, "Client 'msg' mode has to be set to less than %d on this server!\n", maxMsgLevel + 1);
					gi.cprintf (ent, PRINT_HIGH, buffer);
					addCmdQueue(client, QCMD_DISCONNECT, 1, 0, buffer);
				}
			}
			else
			{
				if(maxdoclients < maxclients->value)
				{
					maxdoclients++;
				}
			}
		}
		
	//*** UPDATE START ***	
	if(client >= maxclients->value)
	{
		client = -1;
	}

	checkOnVoting();

	STARTPERFORMANCE(2);
	dllglobals->RunFrame();
	STOPPERFORMANCE_2(2, "mod->G_RunFrame", 0, NULL);

	copyDllInfo();

	STOPPERFORMANCE_2(1, "q2admin->G_RunFrame", 0, NULL);
}

int get_admin_level(char *givenpass,char *givenname)
{
	int got_level = 0;
	unsigned int i;

	for (i = 0; i < num_admins; i++)
	{
		if (!admin_pass[i].level)
			break;
		if ((strcmp(givenpass,admin_pass[i].password) == 0) && (strcmp(givenname,admin_pass[i].name) == 0))
		{
			got_level = admin_pass[i].level;
			break;
		}
	}
	return got_level;
}

int get_bypass_level(char *givenpass,char *givenname)
{
	int got_level = 0;
	unsigned int i;

	for (i = 0; i < num_q2a_admins; i++)
	{
		if (!q2a_bypass_pass[i].level)
			break;
		if ((strcmp(givenpass,q2a_bypass_pass[i].password) == 0) && (strcmp(givenname,q2a_bypass_pass[i].name) == 0))
		{
			got_level = q2a_bypass_pass[i].level;
			break;
		}
	}
	return got_level;
}

void List_Admin_Commands(edict_t *ent,int client)
{
	
	if (proxyinfo[client].q2a_admin & 1)
	{
		gi.cprintf(ent,PRINT_HIGH,"    - !boot <number>\n");
	}
	if (proxyinfo[client].q2a_admin & 2)
	{
		gi.cprintf(ent,PRINT_HIGH,"    - !dumpmsec\n");
	}
	if (proxyinfo[client].q2a_admin & 4)
	{
		gi.cprintf(ent,PRINT_HIGH,"    - !changemap <mapname>\n");
	}
	if (proxyinfo[client].q2a_admin & 8)
	{
		gi.cprintf(ent,PRINT_HIGH,"    - !dumpuser <num>\n");
	}
	if (proxyinfo[client].q2a_admin & 16)
	{
		gi.cprintf(ent,PRINT_HIGH,"    - !auth\n");
		gi.cprintf(ent,PRINT_HIGH,"    - !gfx\n");
	}
	if (proxyinfo[client].q2a_admin & 32)
	{
		gi.cprintf(ent,PRINT_HIGH,"    - !dostuff <num> <commands>\n");
	}
	if (proxyinfo[client].q2a_admin & 128)
	{
		if (whois_active)
			gi.cprintf(ent,PRINT_HIGH,"    - !writewhois\n");
	}
	gi.cprintf(ent,PRINT_HIGH,"\n");
}

void Read_Admin_cfg(void)
{
	FILE	*f;
	char	name[256];
	int i,i2;

	sprintf(name, "%s/q2adminlogin.txt", moddir);

	f = fopen (name, "rb");
	if (!f)
	{
		gi.dprintf ("WARNING: %s could not be found\n", name);
		goto file2;
		return;
	}	
	
	i = 0;
	while ((!feof(f)) && (i<MAX_ADMINS))
	{
		fscanf(f,"%s %s %d",&admin_pass[i].name,&admin_pass[i].password,&admin_pass[i].level);
		i++;
	}
	if (!admin_pass[i].level)
		i--;
	num_admins = i;
	if (i<MAX_ADMINS)
		for (i2=i; i2<MAX_ADMINS; i2++)
			admin_pass[i2].level = 0;

	//read em in
	fclose(f);

file2:;
	sprintf(name, "%s/q2adminbypass.txt", moddir);

	f = fopen (name, "rb");
	if (!f)
	{
		gi.dprintf ("WARNING: %s could not be found\n", name);
		return;
	}
	
	i = 0;
	while ((!feof(f)) && (i<MAX_ADMINS))
	{
		fscanf(f,"%s %s %d",&q2a_bypass_pass[i].name,&q2a_bypass_pass[i].password,&q2a_bypass_pass[i].level);
		i++;
	}
	if (!q2a_bypass_pass[i].level)
		i--;
	num_q2a_admins = i;
	if (i<MAX_ADMINS)
		for (i2=i; i2<MAX_ADMINS; i2++)
			q2a_bypass_pass[i2].level = 0;

	//read em in
	fclose(f);
}

void ADMIN_players(edict_t *ent,int client)
{
	unsigned int i;
	gi.cprintf(ent,PRINT_HIGH,"Players\n");
	for (i = 0; i < maxclients->value; i++)
	{
	if (proxyinfo[i].inuse)
	{
			gi.cprintf(ent,PRINT_HIGH,"  %2i : %s\n",i,proxyinfo[i].name);
	}
	}
	gi.cprintf(ent,PRINT_HIGH,"*******************************\n");
}

void ADMIN_dumpmsec(edict_t *ent,int client)
{
	unsigned int i;
	gi.cprintf(ent,PRINT_HIGH,"MSEC\n");
	for (i = 0; i < maxclients->value; i++)
	{
	if (proxyinfo[i].inuse)
	{
			gi.cprintf(ent,PRINT_HIGH,"  %2i : %-16s %d\n",i,proxyinfo[i].name,proxyinfo[i].msec_last);
	}
	}
	gi.cprintf(ent,PRINT_HIGH,"*******************************\n");
}

void ADMIN_dumpuser(edict_t *ent,int client,int user,qboolean check)
{
	char *cp1;

	if (gi.argc() <2)
	{
		ADMIN_players(ent,client);
		return;
	}
	
	if (check)
		if (!proxyinfo[user].inuse)
			return;
	//if (proxyinfo[user].inuse)
	{
		gi.cprintf(ent,PRINT_HIGH,"User Info for client %d\n",user);

		cp1 = Info_ValueForKey(proxyinfo[user].userinfo, "msg");
		gi.cprintf(ent,PRINT_HIGH,"msg          %s\n",cp1);

		cp1 = Info_ValueForKey(proxyinfo[user].userinfo, "spectator");
		gi.cprintf(ent,PRINT_HIGH,"spectator    %s\n",cp1);

		cp1 = Info_ValueForKey(proxyinfo[user].userinfo, "cl_maxfps");
		gi.cprintf(ent,PRINT_HIGH,"cl_maxfps    %s\n",cp1);

		cp1 = Info_ValueForKey(proxyinfo[user].userinfo, "gender");
		gi.cprintf(ent,PRINT_HIGH,"gender       %s\n",cp1);

		cp1 = Info_ValueForKey(proxyinfo[user].userinfo, "fov");
		gi.cprintf(ent,PRINT_HIGH,"fov          %s\n",cp1);

		cp1 = Info_ValueForKey(proxyinfo[user].userinfo, "rate");
		gi.cprintf(ent,PRINT_HIGH,"rate         %s\n",cp1);

		cp1 = Info_ValueForKey(proxyinfo[user].userinfo, "skin");
		gi.cprintf(ent,PRINT_HIGH,"skin         %s\n",cp1);

		cp1 = Info_ValueForKey(proxyinfo[user].userinfo, "hand");
		gi.cprintf(ent,PRINT_HIGH,"hand         %s\n",cp1);

		if (strlen(proxyinfo[user].gl_driver))
		{
				gi.cprintf(ent,PRINT_HIGH,"gl_driver    %s\n",proxyinfo[user].gl_driver);
		}

		if (proxyinfo[client].q2a_admin & 16)
		{
			gi.cprintf(ent,PRINT_HIGH,"ip           %s\n",proxyinfo[user].ipaddress);
		}

		cp1 = Info_ValueForKey(proxyinfo[user].userinfo, "name");
		gi.cprintf(ent,PRINT_HIGH,"name         %s\n",cp1);

		if (proxyinfo[client].q2a_admin & 128)
		{
			gi.cprintf(ent,PRINT_HIGH,"full         %s\n",proxyinfo[user].userinfo);
		}
	}
}

void ADMIN_auth(edict_t *ent)
{
	unsigned int i;
	for (i = 0; i < maxclients->value; i++)
	{
		if (proxyinfo[i].inuse)
		{
			stuffcmd(getEnt((i+1)),"say I'm using $version\n");
			//if (client_check)
			//	stuffcmd(getEnt((i+1)),"nc_say\n");
		}
	}
}

void ADMIN_gfx(edict_t *ent)
{
	unsigned int i;
	for (i = 0; i < maxclients->value; i++)
	{
		if (proxyinfo[i].inuse)
		{
			stuffcmd(getEnt((i+1)),"say I'm using $gl_driver ( $vid_ref ) / $gl_mode\n");
		}
	}
}

void ADMIN_boot(edict_t *ent,int client,int user)
{
	char  tmptext[100];
	if (gi.argc() <2)
	{
		ADMIN_players(ent,client);
		return;
	}
	if ((user>=0) && (user<maxclients->value))
	{
		if (proxyinfo[user].inuse)		
		{
			gi.bprintf(PRINT_HIGH,"%s was kicked by %s.\n",proxyinfo[user].name,proxyinfo[client].name);
			sprintf(tmptext, "\nkick %d\n", user);
			gi.AddCommandString(tmptext);
		}
	}
}

void ADMIN_changemap(edict_t *ent,int client,char *mname)
{
	char  tmptext[100];
	if (gi.argc() <2)
	{
		ADMIN_players(ent,client);
		return;
	}
	if (q2a_strstr (mname, "\""))
		return ;
	if (q2a_strstr (mname, ";"))
		return ;

	gi.bprintf(PRINT_HIGH,"%s is changing map to %s.\n",proxyinfo[client].name,mname);
	sprintf(tmptext, "\nmap %s\n", mname);
	gi.AddCommandString(tmptext);
}

int ADMIN_process_command(edict_t *ent,int client)
{
	unsigned int i, done = 0;
	int send_to_client;
	edict_t *send_to_ent;
	char send_string[512];
	char abuffer[256];

	if (strlen(gi.args()))
	{
		sprintf(abuffer,"COMMAND - %s %s",gi.argv(0),gi.args());
		logEvent(LT_ADMINLOG, client, ent, abuffer, 0, 0.0);
		gi.dprintf("%s\n",abuffer);
	}

	if (proxyinfo[client].q2a_admin & 1)
	{
		//Level 1 commands
		if (strcmp(gi.argv(0),"!boot")==0)
		{			
			ADMIN_boot(ent,client,atoi(gi.argv(1)));
			done = 1;
		}
	}
	
	if (proxyinfo[client].q2a_admin & 2)
	{
		//Level 2 commands
		if (strcmp(gi.argv(0),"!dumpmsec")==0)
		{			
			ADMIN_dumpmsec(ent,client);
			done = 1;
		}
	}
	if (proxyinfo[client].q2a_admin & 4)
	{
		//Level 3 commands
		if (strcmp(gi.argv(0),"!changemap")==0)
		{			
			ADMIN_changemap(ent,client,gi.argv(1));
			done = 1;
		}
	}
	if (proxyinfo[client].q2a_admin & 8)
	{
		//Level 4 commands
		if (strcmp(gi.argv(0),"!dumpuser")==0)
		{
			ADMIN_dumpuser(ent,client,atoi(gi.argv(1)),true);
			done = 1;
		}
		else if (strcmp(gi.argv(0),"!dumpuser_any")==0)
		{
			ADMIN_dumpuser(ent,client,atoi(gi.argv(1)),false);
			done = 1;
		}
	}
	if (proxyinfo[client].q2a_admin & 16)
	{
		//Level 5 commands
		if (strcmp(gi.argv(0),"!auth")==0)
		{
			ADMIN_auth(ent);
			done = 1;
			gi.cprintf(ent,PRINT_HIGH,"A new auth command has been issued.\n");
		}
		else if (strcmp(gi.argv(0),"!gfx")==0)
		{
			ADMIN_gfx(ent);
			done = 1;
			gi.cprintf(ent,PRINT_HIGH,"Graphics command issued.\n");
		}
	}

	if (proxyinfo[client].q2a_admin & 32)
	{
		//Level 7 commands

		if (strcmp(gi.argv(0),"!dostuff")==0)
		{
			if (gi.argc()>2)
			{
				send_to_client = atoi(gi.argv(1));
				if (strcmp(gi.argv(1),"all")==0)
				{
					for (send_to_client = 0; send_to_client < maxclients->value; send_to_client++)
					if (proxyinfo[send_to_client].inuse)
					{
						strcpy(send_string,gi.argv(2));
						if (gi.argc()>3)
							for (i = 3; i < gi.argc(); i++)
							{
								strcat(send_string," ");
								strcat(send_string,gi.argv(i));
							}
						send_to_ent = getEnt((send_to_client + 1));
						stuffcmd(send_to_ent,send_string);
						gi.cprintf(ent,PRINT_HIGH,"Client %d (%s) has been stuffed!\n",send_to_client,proxyinfo[send_to_client].name);
					}
				}
				else
				if (proxyinfo[send_to_client].inuse)
				{
					strcpy(send_string,gi.argv(2));
					if (gi.argc()>3)
						for (i = 3; i < gi.argc(); i++)
						{
							strcat(send_string," ");
							strcat(send_string,gi.argv(i));
						}
					send_to_ent = getEnt((send_to_client + 1));
					stuffcmd(send_to_ent,send_string);
					gi.cprintf(ent,PRINT_HIGH,"Client %d (%s) has been stuffed!\n",send_to_client,proxyinfo[send_to_client].name);
				}
			}
			done = 2;
		}
	}

	if (proxyinfo[client].q2a_admin & 128)
	{
		if ((strcmp(gi.argv(0),"!writewhois")==0) && (whois_active))
		{
			whois_write_file();
			done = 1;
			gi.cprintf(ent,PRINT_HIGH,"Whois file written.\n");
		}
	}
	return done;
}

priv_t private_commands[PRIVATE_COMMANDS];
int private_command_count;

void stuff_private_commands(int client,edict_t *ent)
{
	unsigned int i;
	char temp[256];

	proxyinfo[client].private_command = ltime+10;

	for(i = 0; i < PRIVATE_COMMANDS; i++)
	{
		if (private_commands[i].command[0])
		{
			//stuff this
			sprintf(temp,"%s\r\n",private_commands[i].command);
			stuffcmd(ent,temp);
		}
		proxyinfo[client].private_command_got[i] = false;
	}
}



qboolean can_do_new_cmds(int client)
{
	if (proxyinfo[client].newcmd_timeout<=ltime)
	{
		proxyinfo[client].newcmd_timeout = ltime + 3;
		return TRUE;
	}
	else
	{		
		return FALSE;
	}

}

//new whois code
void whois_getid(int client,edict_t *ent);
void whois(int client,edict_t *ent);
void whois_adduser(int client,edict_t *ent);
void whois_newname(int client,edict_t *ent);
void whois_update_seen(int client,edict_t *ent);
void whois_dumpdetails(int client,edict_t *ent,int userid);
user_details* whois_details;
int WHOIS_COUNT = 0;
int whois_active = 0;
qboolean timers_active = false;
int timers_min_seconds = 10;
int timers_max_seconds = 180;

void whois(int client,edict_t *ent)
{
	char a1[256];
	unsigned int i;
	int temp;
	
//	if (!can_do_new_cmds(client))
//		return;

	if (gi.argc() <2)
	{
		gi.cprintf(ent,PRINT_HIGH,"\nIncorrect syntax, use: 'whois <name>' or 'whois <id>'\n");
		ADMIN_players(ent,client);
		return;
	}

	//r1ch another overflow fix here.... 
	strncpy (a1, gi.argv(1), sizeof(a1));
	a1[sizeof(a1)-1] = 0;

	temp = q2a_atoi(a1);
	if ((temp==0) && (strcmp(a1,"0")))
	{
		temp = -1;
	}

	//do numbers first
	if ((temp<maxclients->value) && (temp>=0))
	{
		if ((proxyinfo[temp].inuse) && (proxyinfo[temp].userid >= 0))
		{
			//got match, dump details except if admin has proper flag
			if (proxyinfo[temp].q2a_admin & 64)
			{
				gi.cprintf(ent,PRINT_HIGH,"  Unable to fetch info for %i\n",temp);
				return;
			}
			gi.cprintf(ent,PRINT_HIGH,"\n  Whois details for client %i\n",temp);
			whois_dumpdetails(client,ent,proxyinfo[temp].userid);
			return;
		}
		
	}
	//then process all connected clients
	for (i = 0; i < maxclients->value; i++)
	{
		if ((proxyinfo[i].inuse) && (proxyinfo[i].userid >= 0))
		{
			//only do partial match on these, dump all that apply
			if (q2a_strcmp(proxyinfo[i].name,a1)==0)
			{
				if (proxyinfo[i].q2a_admin & 64)
				{
					gi.cprintf(ent,PRINT_HIGH,"  Unable to fetch info for %s\n",a1);
					return;
				}
				gi.cprintf(ent,PRINT_HIGH,"\n  Whois details for %s\n",proxyinfo[i].name);
				whois_dumpdetails(client,ent,proxyinfo[i].userid);
				//got match, dump details
				return;
			}
		}
	}
	//then if still no match process our stored list
	for (i = 0; i < WHOIS_COUNT; i++)
	{
		if ((whois_details[i].dyn[0].name[0]) || (whois_details[i].dyn[1].name[0]) || (whois_details[i].dyn[2].name[0]) ||
			(whois_details[i].dyn[3].name[0]) || (whois_details[i].dyn[4].name[0]) || (whois_details[i].dyn[5].name[0]) ||
			(whois_details[i].dyn[6].name[0]) || (whois_details[i].dyn[7].name[0]) || (whois_details[i].dyn[8].name[0]) ||
			(whois_details[i].dyn[9].name[0]))
		{
			//r1ch: wtf?
			if (((q2a_strcmp(whois_details[i].dyn[0].name,a1)==0)) ||
				((q2a_strcmp(whois_details[i].dyn[1].name,a1)==0)) ||
				((q2a_strcmp(whois_details[i].dyn[2].name,a1)==0)) ||
				((q2a_strcmp(whois_details[i].dyn[3].name,a1)==0)) ||
				((q2a_strcmp(whois_details[i].dyn[4].name,a1)==0)) ||
				((q2a_strcmp(whois_details[i].dyn[5].name,a1)==0)) ||
				((q2a_strcmp(whois_details[i].dyn[6].name,a1)==0)) ||
				((q2a_strcmp(whois_details[i].dyn[7].name,a1)==0)) ||
				((q2a_strcmp(whois_details[i].dyn[8].name,a1)==0)) ||
				((q2a_strcmp(whois_details[i].dyn[9].name,a1)==0)))
			{
				gi.cprintf(ent,PRINT_HIGH,"\n  Whois details for %s\n",a1);
				whois_dumpdetails(client,ent,i);
				//got a match, dump details
				return;
			}

		}
	}
	gi.cprintf(ent,PRINT_HIGH,"  No entry found for %s\n",a1);
}

void whois_dumpdetails(int client,edict_t *ent,int userid)
{
	unsigned int i;
	for (i = 0; i < 10; i++)
	{
		if (whois_details[userid].dyn[i].name[0])
		{
			if (!proxyinfo[client].q2a_admin)
				gi.cprintf(ent,PRINT_HIGH,"    %02i. %s\n",i+1,whois_details[userid].dyn[i].name);
			else
				gi.cprintf(ent,PRINT_HIGH,"    %02i. %s %s\n",i+1,whois_details[userid].dyn[i].name,whois_details[userid].ip);
		}
	}
	gi.cprintf(ent,PRINT_HIGH,"  Last seen: %s\n\n",whois_details[userid].seen);
}

void whois_adduser(int client,edict_t *ent)
{
	if (WHOIS_COUNT >= whois_active)
	{
		WHOIS_COUNT = WHOIS_COUNT - 1;	//If max reached, replace latest entry with new client
//		return;
	}

	whois_details[WHOIS_COUNT].id = WHOIS_COUNT;
	strcpy(whois_details[WHOIS_COUNT].ip, strtok(proxyinfo[client].ipaddress,":"));
	strcpy(whois_details[WHOIS_COUNT].dyn[0].name,proxyinfo[client].name);
	proxyinfo[client].userid = WHOIS_COUNT;
	WHOIS_COUNT++;
}

void whois_newname(int client,edict_t *ent)
{
	//called when a client changes name
	unsigned int i;

	if (proxyinfo[client].userid==-1)
	{
		whois_getid(client,ent);
		return;
	}
	else
	{
		for (i = 0; i < 10; i++)
		{
			if (!whois_details[proxyinfo[client].userid].dyn[i].name[0])
			{
				//this is empty, so add here
				strcpy(whois_details[proxyinfo[client].userid].dyn[i].name,proxyinfo[client].name);
				return;
			}
			if (q2a_strcmp(whois_details[proxyinfo[client].userid].dyn[i].name,proxyinfo[client].name)==0)
			{
				//the name already exists, return
				return;
			}
		}
	}
	//if we got here we have a new name but no free slots, so remove 1 for insertion
	for (i = 0; i < 9; i++)
	{
		strcpy(whois_details[proxyinfo[client].userid].dyn[i].name,whois_details[proxyinfo[client].userid].dyn[i+1].name);
	}
	strcpy(whois_details[proxyinfo[client].userid].dyn[9].name,proxyinfo[client].name);
}

void whois_getid(int client,edict_t *ent)
{
	//called when a client connects
	unsigned int i;	
	for (i = 0; i < WHOIS_COUNT; i++)
	{
		if (q2a_strcmp(whois_details[i].ip,strtok(proxyinfo[client].ipaddress,":"))==0)
		{
			//got a match, store new id
			proxyinfo[client].userid = i;
			whois_newname(client, ent);
			return;
		}
	}
	whois_adduser(client,ent);
}

void whois_update_seen(int client,edict_t *ent)
{
	//to be called on client connect and disconnect
	time_t ltimetemp;
	time( &ltimetemp );
	if (proxyinfo[client].userid>=0)
	{
		q2a_strcpy(whois_details[proxyinfo[client].userid].seen, ctime( &ltimetemp ));
		whois_details[proxyinfo[client].userid].seen[strlen(whois_details[proxyinfo[client].userid].seen)-1] = 0;
	}
}

void whois_write_file(void)
{
	//file format...?
	//id ip seen names
	//when do we want to write this file? it might be processor hungry
	//maybe create a timer that is checked on each spawnentities
	//if 1 day has elapsed then write file
	FILE	*f;
	char	name[256];
	char	temp[256];
	int temp_len;
	unsigned int i, j, k;

	sprintf(name, "%s/q2adminwhois.txt", moddir);

	f = fopen (name, "wb");
	if (!f)
	{
		return;
	}	

	for (i = 0; i < WHOIS_COUNT; i++)
	{
		if(whois_details[i].ip[0] == 0)
			continue;

		strcpy(temp,whois_details[i].ip);
		temp_len = strlen(temp);

		//convert spaces to ÿ
		for (j=0; j<temp_len; j++)
		{
			if (temp[j] == ' ')
				temp[j] = 'ÿ';
		}
		fprintf(f,"%i %s ",whois_details[i].id,temp);

		strcpy(temp,whois_details[i].seen);
		temp_len = strlen(temp);

		for (j=0; j<temp_len; j++)
		{
			if (temp[j] == ' ')
				temp[j] = 'ÿ';
		}
		fprintf(f,"%s ",temp);

		for (j=0;j<10;j++)
		{
			if (whois_details[i].dyn[j].name[0])
			{
				strcpy(temp,whois_details[i].dyn[j].name);
				temp_len = strlen(temp);

				for (k=0;k<temp_len;k++)
				{
					if (temp[k] == ' ')
						temp[k] = 'ÿ';
				}
				fprintf(f,"%s ",temp);
			}
			else
			{
				fprintf(f,"ÿ ");
			}
		}
		fprintf(f,"\n");
	}
	fclose(f);
}

void whois_read_file(void)
{
	FILE	*f;
	char	name[256];
	unsigned int i,j;
	int temp_len,name_len;

	sprintf(name, "%s/q2adminwhois.txt", moddir);

	f = fopen (name, "rb");
	if (!f)
	{
		gi.dprintf ("WARNING: %s could not be found\n", name);
		return;
	}	

	WHOIS_COUNT = 0;
	while ((!feof(f)) && (WHOIS_COUNT < whois_active))
	{
		fscanf(f, "%i %s %s %s %s %s %s %s %s %s %s %s %s",
			&whois_details[WHOIS_COUNT].id,
			&whois_details[WHOIS_COUNT].ip,
			&whois_details[WHOIS_COUNT].seen,
			&whois_details[WHOIS_COUNT].dyn[0].name,
			&whois_details[WHOIS_COUNT].dyn[1].name,
			&whois_details[WHOIS_COUNT].dyn[2].name,
			&whois_details[WHOIS_COUNT].dyn[3].name,
			&whois_details[WHOIS_COUNT].dyn[4].name,
			&whois_details[WHOIS_COUNT].dyn[5].name,
			&whois_details[WHOIS_COUNT].dyn[6].name,
			&whois_details[WHOIS_COUNT].dyn[7].name,
			&whois_details[WHOIS_COUNT].dyn[8].name,
			&whois_details[WHOIS_COUNT].dyn[9].name);		
		
		//convert all ÿ back to spaces
		temp_len = strlen(whois_details[WHOIS_COUNT].ip);
		for(i=0; i<temp_len; i++)
		{
			if(whois_details[WHOIS_COUNT].ip[i] == 'ÿ')
			{
				whois_details[WHOIS_COUNT].ip[i] = ' ';
			}
		}

		temp_len = strlen(whois_details[WHOIS_COUNT].seen);
		for(i=0; i<temp_len; i++)
		{
			if(whois_details[WHOIS_COUNT].seen[i] == 'ÿ')
			{
				whois_details[WHOIS_COUNT].seen[i] = ' ';
			}
		}
		
		for (i=0; i<10; i++)
		{
			if ((whois_details[WHOIS_COUNT].dyn[i].name[0]==255)
				|| (whois_details[WHOIS_COUNT].dyn[i].name[0]== -1) 
				|| (whois_details[WHOIS_COUNT].dyn[i].name[0] == 'ÿ'))
			{
				whois_details[WHOIS_COUNT].dyn[i].name[0] = 0;
			}
			else
			{
				name_len = strlen(whois_details[WHOIS_COUNT].dyn[i].name);
				for(j=0; j<name_len; j++)
				{
					if(whois_details[WHOIS_COUNT].dyn[i].name[j] == 'ÿ')
					{
						whois_details[WHOIS_COUNT].dyn[i].name[j] = ' ';
					}
				}
			}
		}
		WHOIS_COUNT++;
	}
	fclose(f);
}

void reloadWhoisFileRun(int startarg, edict_t *ent, int client)
{
	whois_read_file();
	gi.cprintf (ent, PRINT_HIGH, "Whois file reloaded.\n");
}

void reloadLoginFileRun(int startarg, edict_t *ent, int client)
{
	Read_Admin_cfg();
	gi.cprintf (ent, PRINT_HIGH, "Login file reloaded.\n");
}

//quad/ps etc timer code
void timer_start(int client,edict_t *ent)
{
	int seconds;
	int num;
	
	if (gi.argc() <4)
	{
		gi.cprintf(ent,PRINT_HIGH,"Incorrect syntax, use: 'timer_start <number> <seconds> <action>'\n");
		//ADMIN_players(ent,client);
		return;
	}

	if (!can_do_new_cmds(client))
	{
		gi.cprintf(ent,PRINT_HIGH,"Please wait 5 seconds\n");
		return;//wait 5 secs before starting the timer again
	}

	num = q2a_atoi(gi.argv(1));
	seconds =  q2a_atoi(gi.argv(2));
	if ((seconds<timers_min_seconds) || (seconds>timers_max_seconds))
	{
		gi.cprintf(ent,PRINT_HIGH,"Timer seconds falls outside acceptable range of %i to %i.\n",timers_min_seconds,timers_max_seconds);
		return;
	}
	if ((num<1) || (num>TIMERS_MAX))
	{
		gi.cprintf(ent,PRINT_HIGH,"Invalid timer number\n");
		return;
	}
	proxyinfo[client].timers[num].start = ltime + seconds;
	strcpy(proxyinfo[client].timers[num].action,gi.argv(3));

}

void timer_stop(int client,edict_t *ent)
{
	int num;

	if (gi.argc() <2)
	{
		gi.cprintf(ent,PRINT_HIGH,"Invalid Timer\n");
		return;
	}
		num = q2a_atoi(gi.argv(1));
	if ((num<1) || (num>TIMERS_MAX))
	{
		gi.cprintf(ent,PRINT_HIGH,"Invalid timer number\n");
		return;
	}

	proxyinfo[client].timers[num].start = 0;
}

void timer_action(int client,edict_t *ent)
{
	int num;
	for (num=0; num<TIMERS_MAX; num++)
	if (proxyinfo[client].timers[num].start)
	{
		if (proxyinfo[client].timers[num].start <= ltime)
		{
			proxyinfo[client].timers[num].start = 0;
			stuffcmd(ent,proxyinfo[client].timers[num].action);
		}
	}
}
//*** UPDATE END ***

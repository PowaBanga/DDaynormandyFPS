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
// zb_msgqueue.c
//
// copyright 2000 Shane Powell
//
#include "g_local.h"

void addCmdQueue(int  client, byte command, float timeout, unsigned long data, char *str)
{
	char tmptext[128];	//UPDATE

	proxyinfo[client].cmdQueue[proxyinfo[client].maxCmds].command = command;
	proxyinfo[client].cmdQueue[proxyinfo[client].maxCmds].timeout = ltime + timeout;
	proxyinfo[client].cmdQueue[proxyinfo[client].maxCmds].data = data;
	proxyinfo[client].cmdQueue[proxyinfo[client].maxCmds].str = str;
	proxyinfo[client].maxCmds++;

	if (command == QCMD_DISCONNECT)
	{
		gi.cprintf (NULL, PRINT_HIGH, "%s is being disconnected for %s.\n", proxyinfo[client].name, str);
	}

//*** UPDATE START ***
	if ( proxyinfo[client].maxCmds >= ALLOWED_MAXCMDS_SAFETY)
	{
		proxyinfo[client].clientcommand |= CCMD_KICKED;
		gi.bprintf (PRINT_HIGH, "%s tried to flood the server.\n", proxyinfo[client].name);
		sprintf(tmptext, "kick %d\n", client);
		//need to log
		gi.AddCommandString(tmptext);
    }
//*** UPDATE END ***
}

qboolean getCommandFromQueue(int client, byte *command, unsigned long *data, char **str)
{
	unsigned int  i;
	
	for(i = 0; i < proxyinfo[client].maxCmds; i++)
		{
			if(proxyinfo[client].cmdQueue[i].timeout < ltime)
				{
					// found good command..
					// get info to return
					*command = proxyinfo[client].cmdQueue[i].command;
					*data = proxyinfo[client].cmdQueue[i].data;
					
					if(str)
						{
							*str = proxyinfo[client].cmdQueue[i].str;
						}
						
					// remove command
					proxyinfo[client].maxCmds--;
					
					if(i < proxyinfo[client].maxCmds)
						{
							q2a_memmove(proxyinfo[client].cmdQueue + i, proxyinfo[client].cmdQueue + i + 1, (proxyinfo[client].maxCmds - i) * sizeof(CMDQUEUE));
						}
						
					return TRUE;
				}
		}
		
	return FALSE;
}

void removeClientCommand(int client, byte command)
{
	unsigned int i = 0;
	
	while(i < proxyinfo[client].maxCmds)
		{
			if(proxyinfo[client].cmdQueue[i].command == command)
				{
					// remove command
					proxyinfo[client].maxCmds--;
					if(i < proxyinfo[client].maxCmds)
						{
							q2a_memmove(proxyinfo[client].cmdQueue + i, proxyinfo[client].cmdQueue + i + 1, (proxyinfo[client].maxCmds - i) * sizeof(CMDQUEUE));
						}
				}
			else
				{
					i++;
				}
		}
}

void removeClientCommands(int client)
{
	proxyinfo[client].maxCmds = 0;
}

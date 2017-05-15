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

/*============================================================================
ZbotCheck v1.01 for Quake 2 by Matt "WhiteFang" Ayres (matt@lithium.com)
 
This is provided for mod authors to implement Zbot detection, nothing more.
The code has so far proven to be reliable at detecting Zbot auto-aim clients
(cheaters).  However, no guarantees of any kind are made.  This is provided
as-is.  You must be familiar with Quake 2 mod coding to make use of this.
 
In g_local.h, add to struct client_respawn_t:
 
short angles[2][2];
int tog;
int jitter;
float jitter_time;
float jitter_last;
 
Next, in p_client.c, add a simple forward declaration:
 
qboolean ZbotCheck(edict_t *ent, usercmd_t *ucmd);
 
Then in p_client.c, anywhere in the ClientThink function, call the
ZbotCheck function.  Pass it the same parameters you get from ClientThink.
It will return true if the client is using a Zbot.  Simple example:
 
if(ZbotCheck(ent, ucmd))
gi.bprintf(PRINT_HIGH, ">>> Zbot detected: %s\n",
ent->client->pers.netname);
 
From here you can do as you please with the cheater.  ZbotCheck will only
return true once, following returns will be false.
============================================================================*/
/*
* Modified for q2admin by Shane Powell.
*/
#include "g_local.h"

int zbc_jittermax= 4;
int zbc_jittertime= 10;
int zbc_jittermove= 500;

qboolean zbc_ZbotCheck(int client, usercmd_t *ucmd)
{
	int tog0, tog1;
	
	tog0 = proxyinfo[client].zbc_tog;
	proxyinfo[client].zbc_tog ^= 1;
	tog1 = proxyinfo[client].zbc_tog;
	
	if(ucmd->angles[0]== proxyinfo[client].zbc_angles[tog1][0]&&
		ucmd->angles[1]== proxyinfo[client].zbc_angles[tog1][1]&&
		ucmd->angles[0]!= proxyinfo[client].zbc_angles[tog0][0]&&
		ucmd->angles[1]!= proxyinfo[client].zbc_angles[tog0][1]&&
		abs(ucmd->angles[0]- proxyinfo[client].zbc_angles[tog0][0])+
		abs(ucmd->angles[1]- proxyinfo[client].zbc_angles[tog0][1])>= zbc_jittermove)
		{
			if(ltime <= proxyinfo[client].zbc_jitter_last + 0.1)
				{
					if(!proxyinfo[client].zbc_jitter)
						proxyinfo[client].zbc_jitter_time = ltime;
					if(proxyinfo[client].zbc_jitter++ >= zbc_jittermax)
						return true;
				}
			proxyinfo[client].zbc_jitter_last = ltime;
		}
	proxyinfo[client].zbc_angles[tog1][0] = ucmd->angles[0];
	proxyinfo[client].zbc_angles[tog1][1] = ucmd->angles[1];
	
	if(ltime > (proxyinfo[client].zbc_jitter_time + zbc_jittertime))
		proxyinfo[client].zbc_jitter = 0;
		
	return false;
}

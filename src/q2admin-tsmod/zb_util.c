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
// zb_util.c
//
// copyright 2000 Shane Powell
//

#include "g_local.h"

// required for proxy testing
void stuffcmd(edict_t *e, char *s)
{
	gi.WriteByte (11);
	gi.WriteString (s);
	gi.unicast (e, true);
}


int Q_stricmp (char *string1, char *string2)
{
	while(*string1 && *string2)
		{
			char s1c = tolower(*string1);
			char s2c = tolower(*string2);
			if(s1c != s2c)
				{
					if(s1c < s2c)
						{
							return -1;
						}
					else
						{
							return 1;
						}
				}
				
			string1++;
			string2++;
		}
		
	if(*string2)
		{
			return -1;
		}
		
	if(*string1)
		{
			return 1;
		}
		
	return 0;
	
	/*
	#if defined(WIN32)
	return q2a_stricmp (s1, s2);
	#else
	return q2a_strcasecmp (s1, s2);
	#endif
	*/
}


// required for the RE code

char *q2admin_malloc (int size)
{
	char *mem = gi.TagMalloc(size + sizeof(int), TAG_GAME);
	
	*(int *)mem = size;
	
	return mem + sizeof(int);
}

char *q2admin_realloc (char *oldmem, int newsize)
{
	int oldsize;
	int *start = (int *)(oldmem - sizeof(int));
	char *newmem;
	
	oldsize = *start;
	
	if(oldsize >= newsize)
		{
			return oldmem;
		}
		
	newmem = gi.TagMalloc(newsize + sizeof(int), TAG_GAME);
	*(int *)newmem = newsize;
	newmem += sizeof(int);
	
	q2a_memcpy(newmem, oldmem, newsize - oldsize);
	
	gi.TagFree(start);
	
	return newmem;
}

void q2admin_free (char *mem)
{
	gi.TagFree(mem - sizeof(int));
}


/*
===============
Info_ValueForKey
 
Searches the string for the given
key and returns the associated value, or an empty string.
===============
*/
char *Info_ValueForKey(char *s, char *key)
{
	char pkey[512];
	static char value[2][512];// use two buffers so compares
	// work without stomping on each other
	static int valueindex;
	char *o;
	
	valueindex ^= 1;
	if (*s == '\\')
		s++;
	while (1)
		{
			o = pkey;
			while (*s != '\\')
				{
					if (!*s)
						return "";
					*o++ = *s++;
				}
			*o = 0;
			s++;
			
			o = value[valueindex];
			
			while (*s != '\\' && *s)
				{
					if (!*s)
						return "";
					*o++ = *s++;
				}
			*o = 0;
			
			if (!q2a_strcmp (key, pkey) )
				return value[valueindex];
				
			if (!*s)
				return "";
			s++;
		}
}


/*
==================
Info_Validate
 
Some characters are illegal in info strings because they
can mess up the server's parsing
==================
*/
qboolean Info_Validate(char *s)
{
	if(q2a_strstr(s,"\""))
		return false;
	if(q2a_strstr(s,";"))
		return false;
	return true;
}


void copyDllInfo(void)
{
	globals.apiversion = dllglobals->apiversion;
	globals.edict_size = dllglobals->edict_size;
	globals.edicts = dllglobals->edicts;
	globals.num_edicts = dllglobals->num_edicts;
	globals.max_edicts = dllglobals->max_edicts;
}


int breakLine(char *buffer, char *buff1, char *buff2, int buff2size)
{
	char *cp,*dp;
	
	cp= buffer;
	dp= buff1;
	
	while( *cp && *cp!=' ' && *cp!='\t')
		{
			*dp++ = *cp++;
		}
	*dp= 0x0;
	
	if(dp== buff1 || !*cp)
		{
			return 0;
		}
		
	dp = buff2;
	
	SKIPBLANK(cp);
	
	if( *cp!='\"')
		{
			return 0;
		}
	cp++;
	
	cp = processstring(buff2, cp, buff2size,'\"');
	
	if(!buff2[0]||*cp!='\"')
		{
			return 0;
		}
		
	return 1;
}

int startContains(char *src, char *cmp)
{
	while( *cmp)
		{
			if(!( *src)|| toupper( *src)!= toupper(*cmp))
				{
					return 0;
				}
				
			src++;
			cmp++;
		}
		
	return 1;
}

int stringContains(char *buff1, char *buff2)
{
	char strbuffer1[4096];
	char strbuffer2[4096];
	
	q2a_strcpy(strbuffer1, buff1);
	q_strupr(strbuffer1);
	q2a_strcpy(strbuffer2, buff2);
	q_strupr(strbuffer2);
	return(q2a_strstr(strbuffer1, strbuffer2)!= NULL);
}

int isBlank(char *buff1)
{
	while( *buff1 == ' ')
		{
			buff1++;
		}
		
	return!( *buff1);
}


char *processstring(char *output, char *input, int max, char end)
{

	while( *input && *input != end && max)
		{
			if( *input == '\\')
				{
					*input++;
					
					if((*input == 'n') || (*input == 'N'))
					{
						*output++ = '\n';
						input++;
					}
					else if((*input == 'd') || (*input == 'D'))
					{
						*output++ = '$';
						input++;
					}
					else if((*input == 'q') || (*input == 'Q'))
					{
						*output++ = '\"';
						input++;
					}
					else if((*input == 's') || (*input == 'S'))
					{
						*output++ = ' ';
						input++;
					}
					else if((*input == 'm') || (*input == 'M'))
					{
						int modlen = strlen(moddir);
						if(max >= modlen && modlen)
							{
								q2a_strcpy(output, moddir);
								output += modlen;
								max -=(modlen- 1);
							}
						input++;
					}
					else if((*input == 't') || (*input == 'T'))
					{
						struct tm*timestamptm;
						time_t timestampsec;
						char *timestampcp;
						int timestamplen;

						time(&timestampsec);/* Get time in seconds */
						timestamptm= localtime(&timestampsec);/* Convert time to struct */
						/* tm form */

						timestampcp= asctime( timestamptm);/* get string version of date / time */
						timestamplen= strlen( timestampcp)- 1;/* length minus the '\n' */

						if(timestamplen && max>= timestamplen)
						{
							q2a_strncpy(output, timestampcp, timestamplen);
							output += timestamplen;
							max -=(timestamplen- 1);
						}
						input++;
					}
					else
					{
						*output++ = *input++;
					}
						
					max--;
				}
			else
				{
					*output++ = *input++;
					max--;
				}
		}
		
	*output= 0x0;
	
	return input;
}


qboolean getLogicalValue(char *arg)
{
	if(Q_stricmp(arg,"Yes")== 0||
		Q_stricmp(arg,"1")== 0||
		Q_stricmp(arg,"Y")== 0)
		{
			return TRUE;
		}
		
	return FALSE;
}


int getLastLine(char *buffer, FILE*dumpfile, long*fpos)
{
	// char buffer2[256];
	char *bp = buffer2;
	int length = 255;
	
	if(*fpos < 0)
		{
			return 0;
		}
		
	while(length && *fpos >= 0)
		{
			fseek(dumpfile, *fpos, SEEK_SET);
			(*fpos)--;
			
			if(fread(bp, 1, 1, dumpfile) != 1)
				{
					break;
				}
				
			if(*bp == '\n')
				{
					break;
				}
				
			bp++;
			length--;
		}
		
	if(bp != buffer2)
		{
			bp--;
			
			// reverse string
			while(bp >= buffer2)
				{
					*buffer++ = *bp--;
				}
		}
		
	*buffer = 0;
	return 1;
}


void q_strupr(char *c)
{
	while(*c)
		{
			if(islower((*c)))
				{
					*c = toupper((*c));
				}
				
			c++;
		}
}

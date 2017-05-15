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
// zb_clib.c
//
// copyright 2000 Shane Powell
//

/*
* q2admin clib functions 
*
*/

#include "g_local.h"

#ifdef Q2ADMINCLIB


char *q2a_strcpy( char *strDestination, const char *strSource)
{
	char *ret= strDestination;
	
	while( *strSource)
		{
			*strDestination++ = *strSource++;
		}
		
	*strDestination = *strSource;
	
	return ret;
}



char *q2a_strncpy( char *strDest, const char *strSource, size_t count)
{
	char *cp= strDest;
	
	if(!count)
		{
			return strDest;
		}
		
	count--;
	
	while( *strSource && count)
		{
			*cp++ = *strSource++;
			if(count)
				{
					count--;
				}
		}
		
	*cp= 0x0;
	
	return strDest;
}





char *q2a_strcat( char *strDestination, const char *strSource)
{
	char *cp= strDestination;
	
	while( *cp)
		{
			cp++;
		}
		
	q2a_strcpy(cp, strSource);
	
	return strDestination;
}




int q2a_strcmp( const char *string1, const char *string2)
{
	while( *string1 && *string2)
		{
			if( *string1 != *string2)
				{
					if( *string1 < *string2)
						{
							return-1;
						}
					else
						{
							return 1;
						}
				}
				
			string1++;
			string2++;
		}
		
	if( *string2)
		{
			return-1;
		}
		
	if( *string1)
		{
			return 1;
		}
		
	return 0;
}




char *q2a_strstr( const char *string, const char *strCharSet)
{
	const char *cp;
	const char *ts;
	
	while( *string)
		{
			if( *string == *strCharSet )
				{
					cp = string;
					cp += 1;
					ts = strCharSet;
					ts += 1;
					
					while( *cp && *ts )
						{
							if( *cp!= *ts )
								{
									break;
								}
							cp++;
							ts++;
						}
						
					if(!( *ts))
						{
							return(char *)string;
						}
				}
				
			string++;
		}
		
	return 0x0;
}




char *q2a_strchr( const char *string, int c)
{
	while( *string)
		{
			if( *string== c)
				{
					return(char *)string;
				}
			string++;
		}
		
	return 0x0;
}





size_t q2a_strlen( const char *string)
{
	size_t len= 0;
	
	while( *string)
		{
			len++;
			string++;
		}
		
	return len;
}









int q2a_memcmp( const void *buf1, const void *buf2, size_t count)
{
	unsigned long *dwbuf1=(unsigned long *)buf1;
	unsigned long *dwbuf2=(unsigned long *)buf2;
	unsigned char *bbuf1, *bbuf2;
	
	size_t dwcount= count/ sizeof(unsigned long);
	count = count % sizeof(unsigned long);
	
	
	while(dwcount)
		{
			if( *dwbuf1!= *dwbuf2)
				{
					return 1;
				}
				
			dwbuf1++;
			dwbuf2++;
			dwcount--;
		}
		
	if(count)
		{
			bbuf1=(unsigned char *)dwbuf1;
			bbuf2=(unsigned char *)dwbuf2;
			
			while(count)
				{
					if( *bbuf1 != *bbuf2)
						{
							return 1;
						}
						
					bbuf1++;
					bbuf2++;
					count--;
				}
		}
		
	return 0;
}



void *q2a_memcpy( void *dest, const void *src, size_t count)
{
	unsigned long *dwbuf1=(unsigned long *)dest;
	unsigned long *dwbuf2=(unsigned long *)src;
	unsigned char *bbuf1, *bbuf2;
	
	size_t dwcount= count/ sizeof(unsigned long);
	count = count % sizeof(unsigned long);
	
	
	while(dwcount)
		{
			*dwbuf1++ = *dwbuf2++;
			dwcount--;
		}
		
	if(count)
		{
			bbuf1=(unsigned char *)dwbuf1;
			bbuf2=(unsigned char *)dwbuf2;
			
			while(count)
				{
					*bbuf1++ = *bbuf2++;
					count--;
				}
		}
		
	return dest;
}



void *q2a_memmove( void *dest, const void *src, size_t count)
{
	if((unsigned char *)dest>(unsigned char *)src && (unsigned char *)dest<(unsigned char *)src + count)
		{/* overlap... */
			char *buf= gi.TagMalloc(count, TAG_LEVEL);
			void *ret;
			q2a_memcpy(buf, src, count);
			ret= q2a_memcpy( dest, buf, count);
			gi.TagFree(buf);
			return ret;
		}
		
	return q2a_memcpy( dest, src, count);
}




void *q2a_memset( void *dest, int c, size_t count)
{
	unsigned char *bbuf1=(unsigned char *)dest;
	
	while(count)
		{
			*bbuf1++ = c;
			count--;
		}
		
	return dest;
}











int q2a_atoi( const char *string)
{
	int retvalue = 0;
	
	switch( *string)
		{
		case'-':
			retvalue = -retvalue;
			string++;
			break;
			
		case'+':
			retvalue = +retvalue;
			string++;
			break;
		}
		
	while(isdigit( *string))
		{
			retvalue=(retvalue* 10)+(*string-'0');
			string++;
		}
		
	return retvalue;
}





double q2a_atof( const char *string)
{
	double retvalue = 0;
	unsigned int divide = 10;
	
	switch( *string)
		{
		case'-':
			retvalue = -retvalue;
			string++;
			break;
			
		case'+':
			retvalue = +retvalue;
			string++;
			break;
		}
		
	while(isdigit( *string))
		{
			retvalue=(retvalue* 10.0)+(*string-'0');
			string++;
		}
		
	if( *string=='.')
		{
			string++;
			
			while(isdigit( *string))
				{
					retvalue += (*string-'0') / divide;
					divide *= 10;
					string++;
				}
		}
		
	return retvalue;
}


#endif

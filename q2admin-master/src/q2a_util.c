//
// q2admin
//
// q2a_util.c
//
// copyright 2000 Shane Powell
// copyright 2009 Toni Spets
//

#include "g_local.h"

#if defined(WIN32)
#define Q_stricmp stricmp
#else
#define Q_stricmp strcasecmp
#endif

/* why the fuck int size is added to every alloc?! -hifi */
char *q2a_malloc (int size)
{
	char *mem = gi.TagMalloc(size + sizeof(int), TAG_GAME);

	if(!mem)
		return NULL;
	
	*(int *)mem = size;
	
	return mem + sizeof(int);
}

char *q2a_realloc (char *oldmem, int newsize)
{
	int oldsize;
	int *start = (int *)(oldmem - sizeof(int));
	char *newmem;

	/* ANSI C */
	if(oldmem == NULL)
		return q2a_malloc(newsize);

	oldsize = *start;

	if(oldsize >= newsize)
		return oldmem;

	newmem = gi.TagMalloc(newsize + sizeof(int), TAG_GAME);
	*(int *)newmem = newsize;
	newmem += sizeof(int);

	q2a_memcpy(newmem, oldmem, newsize - oldsize);

	gi.TagFree(start);

	return newmem;
}


void q2a_free (char *mem)
{
	/* ANSI C */
	if(mem == NULL)
		return;

	gi.TagFree(mem - sizeof(int));
}

void copyDllInfo(void)
{
	globals.edict_size = dllglobals->edict_size;
	globals.edicts = dllglobals->edicts;
	globals.num_edicts = dllglobals->num_edicts;
	globals.max_edicts = dllglobals->max_edicts;
}

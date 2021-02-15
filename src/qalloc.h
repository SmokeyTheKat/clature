#ifndef __clature_qalloc_h__
#define __clature_qalloc_h__

#include "./ddcLib/ddcDef.h"

#define qmake(t,s) ((t*)(qalloc(s*sizeof(t))))

static sizet maxSize = 0;
static void* qtop;
static void* qbot;

void init_qalloc(sizet _maxSize)
{
	maxSize = _maxSize;
	qtop = malloc(maxSize);
	qbot = qtop;
}

void* qalloc(sizet size)
{
	//printf("qalloc size: %f\n", (float)((long)qtop-(long)qbot)/((float)maxSize));
	void* ret = qtop;
	qtop += size;
	return ret;
}

#endif

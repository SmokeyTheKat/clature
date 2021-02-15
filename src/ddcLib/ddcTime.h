#ifndef __ddcTime__
#define __ddcTime__

#include <time.h>

#define ddTimer_time_statement(x,y,v) ddTimer_start();for (int i = 0; i < y; i++){x};v=ddTimer_stop();

void ddTimer_start(void);
double ddTimer_stop(void);
double ddTimer_get_time(void);
bool ddTimer_is_past(double _v);

static clock_t __dt_begin;
static clock_t __dt_end;

void ddTimer_start(void)
{
	__dt_begin = clock();
}
double ddTimer_stop(void)
{
	__dt_end = clock();
	return ((double)(__dt_end - __dt_begin) / CLOCKS_PER_SEC);
}
double ddTimer_get_time(void)
{
	return ((double)(clock() - __dt_begin) / CLOCKS_PER_SEC);
}
bool ddTimer_is_past(double _v)
{
	return (_v < ((double)(clock() - __dt_begin) / CLOCKS_PER_SEC));
}


#endif

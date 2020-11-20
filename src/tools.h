#ifndef __ddcomp_tools_h__
#define __ddcomp_tools_h__

static bool is_number(ddString ds)
{
	for (sizet i = 0; i < ds.length; i++)
		if (ds.cstr[i] < 48 || ds.cstr[i] > 57) return false;
	return true;
}

#endif

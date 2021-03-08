#ifndef __clature_data_h__
#define __clature_data_h__

struct stVariable;
struct stackTracker;

struct stVariable* stackt_get_var(ddString name);
struct stVariable* stackt_set_var(ddString name, sizet size);

struct stackTracker stackt;

struct stVariable
{
	ddString name;
	sizet spos;
	sizet size;
};
struct stackTracker
{
	int size;
	sizet top;
	struct stVariable vars[500];
};

struct stVariable* stackt_get_var(ddString name)
{
	for (sizet i = 0; i < stackt.top; i++)
	{
		if (ddString_compare(name, stackt.vars[i].name))
		{
			return &(stackt.vars[i]);
		}
	}
	return (struct stVariable*)nullptr;
}
struct stVariable* stackt_set_var(ddString name, sizet size)
{
	stackt.vars[stackt.top].name = name;
	stackt.vars[stackt.top].size = size;
	stackt.top++;
	stackt.size += size;
	stackt.vars[stackt.top-1].spos = stackt.size;
	return &(stackt.vars[stackt.top-1]);
}

#endif

#ifndef __clature_data_h__
#define __clature_data_h__

#include "./generate.h"

const char* DEFINE_SIZES[9] = {
	"ERROR_SIZE",
	"DB",
	"DW",
	"ERROR_SIZE",
	"DD",
	"ERROR_SIZE",
	"ERROR_SIZE",
	"ERROR_SIZE",
	"DQ"
};
const char* DATA_SIZES[9] = {
	"ERROR_SIZE",
	"BYTE",
	"WORD",
	"ERROR_SIZE",
	"DWORD",
	"ERROR_SIZE",
	"ERROR_SIZE",
	"ERROR_SIZE",
	"QWORD"
};

struct stVariable;
struct stackTracker;
struct dtVariable;
struct dataTracker;

struct stVariable* stackt_get_var(ddString name);
struct stVariable* stackt_set_var(ddString name, sizet size);
struct dtVariable datat_get_data(ddString name);
struct dtVariable datat_add_data(ddString name, ddString value, sizet size);
struct dtVariable datat_add_string(ddString value);
void pop_stack_var(struct stVariable var);
void pop_ref(ddString value, sizet size);
void push_ref(ddString value, sizet size);
void push_stack_var(struct stVariable var);
void pop_nsize(ddString value, sizet size);
void push_nsize(ddString value, sizet size);
void push_param_nsize(struct stVariable var);

struct stackTracker stackt;
struct stackTracker fstackt;
struct dataTracker datat;
sizet stringCount = 0;

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

struct dtVariable
{
	ddString name;
	ddString data;
	ddString defineSize;
	sizet size;
};
struct dataTracker
{
	sizet top;
	struct dtVariable data[500];
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
void stackt_set_param_var(ddString name, int size, sizet tsize)
{
	stackt.vars[stackt.top].name = name;
	stackt.vars[stackt.top].size = size;
	stackt.top++;
	stackt.vars[stackt.top-1].spos = -(16 + tsize);
}


struct dtVariable datat_get_data(ddString name)
{
	for (sizet i = 0; i < datat.top; i++)
	{
		if (ddString_compare(name, datat.data[i].name)) return datat.data[i];
	}
	struct dtVariable errorout;
	errorout.size = -6969;
	return errorout;
}
struct dtVariable datat_add_data(ddString name, ddString value, sizet size)
{
	datat.data[datat.top].name = name;
	datat.data[datat.top].data = value;
	datat.data[datat.top].size = size;
	datat.data[datat.top].defineSize = make_constant_ddString(DEFINE_SIZES[size]);
	return datat.data[datat.top++];
}
struct dtVariable datat_add_string(ddString value)
{
	datat.data[datat.top].data = value;
	ddString_push_cstring_back(&(datat.data[datat.top].data), ",0");
	datat.data[datat.top].data.cstr[datat.data[datat.top].data.length] = '\0';
	datat.data[datat.top].name = make_format_ddString("_69_str%d", stringCount++);
	datat.data[datat.top].defineSize = make_constant_ddString("DB");
	return datat.data[datat.top++];
}



void pop_stack_var(struct stVariable var)
{
	switch (var.size)
	{
		case 1:
		{
			gen_pop(REG_RAX);
			btc_set(BTC_MOV, make_format_ddString("BYTE[RBP-%d]", var.spos), REG_AL);
			break;
		}
		case 2:
		{
			gen_pop(REG_RAX);
			btc_set(BTC_MOV, make_format_ddString("WORD[RBP-%d]", var.spos), REG_AX);
			break;
		}
		case 4:
		{
			gen_pop(REG_RAX);
			btc_set(BTC_MOV, make_format_ddString("DWORD[RBP-%d]", var.spos), REG_EAX);
			break;
		}
		case 8:
		{
			btc_set(BTC_POP, make_format_ddString("QWORD[RBP-%d]", var.spos), REG_NONE);
			break;
		}
		default:
			pop_nsize(make_format_ddString("RBP-%d", var.spos), var.size);
	}
}
void pop_ref(ddString value, sizet size)
{
	switch (size)
	{
		case 1:
		{
			gen_pop(REG_RAX);
			btc_set(BTC_MOV, make_format_ddString("BYTE[%s]", value.cstr), REG_AL);
			break;
		}
		case 2:
		{
			gen_pop(REG_RAX);
			btc_set(BTC_MOV, make_format_ddString("WORD[%s]", value.cstr), REG_AX);
			break;
		}
		case 4:
		{
			gen_pop(REG_RAX);
			btc_set(BTC_MOV, make_format_ddString("DWORD[%s]", value.cstr), REG_EAX);
			break;
		}
		case 8:
		{
			btc_set(BTC_POP, make_format_ddString("QWORD[%s]", value.cstr), REG_NONE);
			break;
		}
		default:
			pop_nsize(value, size);
	}
}
void push_ref(ddString value, sizet size)
{
	if (addSize && size >= 0) addSizeVal += size;
	switch (size)
	{
		case 1:
		{
			btc_set(BTC_MOVSX, REG_RAX, make_format_ddString("BYTE[%s]", value.cstr));
			gen_push(REG_RAX);
			break;
		}
		case 2:
		{
			btc_set(BTC_MOVSX, REG_RAX, make_format_ddString("WORD[%s]", value.cstr));
			gen_push(REG_RAX);
			break;
		}
		case 4:
		{
			btc_set(BTC_MOV, REG_EAX, make_format_ddString("DWORD[%s]", value.cstr));
			gen_push(REG_RAX);
			break;
		}
		case 8:
		{
			btc_set(BTC_PUSH, make_format_ddString("QWORD[%s]", value.cstr), REG_NONE);
			break;
		}
		default:
			push_nsize(value, size);
	}
}
void push_stack_var(struct stVariable var)
{
	if (addSize && var.size >= 0) addSizeVal += var.size;
	switch (var.size)
	{
		case 1:
		{
			btc_set(BTC_MOVSX, REG_RAX, make_format_ddString("BYTE[RBP-%d]", var.spos));
			gen_push(REG_RAX);
			break;
		}
		case 2:
		{
			btc_set(BTC_MOVSX, REG_RAX, make_format_ddString("WORD[RBP-%d]", var.spos));
			gen_push(REG_RAX);
			break;
		}
		case 4:
		{
			btc_set(BTC_MOV, REG_EAX, make_format_ddString("DWORD[RBP-%d]", var.spos));
			gen_push(REG_RAX);
			break;
		}
		case 8:
		{
			btc_set(BTC_PUSH, make_format_ddString("QWORD[RBP-%d]", var.spos), REG_NONE);
			break;
		}
		default:
			push_nsize(make_format_ddString("RBP-%d", var.spos), var.size);
	}
}
void pop_nsize(ddString value, sizet size)
{
	int remain = size;
	int done = 0;
	int com[100] = {0};
	int cpos = 0;
	while (remain != 0)
	{
		if (remain >= 8)
		{
			com[cpos++] = 8;
			remain -= 8;
		}
		else if (remain >= 4)
		{
			com[cpos++] = 4;
			remain -= 4;
		}
		else if (remain >= 2)
		{
			com[cpos++] = 2;
			remain -= 2;
		}
		else if (remain >= 1)
		{
			com[cpos++] = 1;
			remain -= 1;
		}

	}
	cpos--;
	while (cpos >= 0)
	{
		if (com[cpos] == 8)
		{
			btc_set(BTC_POP, make_format_ddString("QWORD[%s+%d]", value.cstr, done), REG_NONE);
			done += 8;
		}
		else if (com[cpos] == 4)
		{
			gen_pop(REG_RAX);
			btc_set(BTC_MOV, make_format_ddString("DWORD[%s+%d]", value.cstr, done), REG_EAX);
			done += 4;
		}
		else if (com[cpos] == 2)
		{
			gen_pop(REG_RAX);
			btc_set(BTC_MOV, make_format_ddString("WORD[%s+%d]", value.cstr, done), REG_AX);
			done += 2;
		}
		else if (com[cpos] == 1)
		{
			gen_pop(REG_RAX);
			btc_set(BTC_MOV, make_format_ddString("BYTE[%s+%d]", value.cstr, done), REG_AL);
			done += 1;
		}
		cpos--;
	}
}
void push_nsize(ddString value, sizet size)
{
	int remain = size;
	int done = 0;
	while (remain != 0)
	{
		if (remain >= 8)
		{
			btc_set(BTC_PUSH, make_format_ddString("QWORD[%s+%d]", value.cstr, size-done-8), REG_NONE);
			remain -= 8;
			done += 8;
		}
		else if (remain >= 4)
		{
			btc_set(BTC_MOV, REG_EAX, make_format_ddString("DWORD[%s+%d]", value.cstr, size-done-4));
			gen_push(REG_RAX);
			remain -= 4;
			done += 4;
		}
		else if (remain >= 2)
		{
			btc_set(BTC_MOVSX, REG_RAX, make_format_ddString("WORD[%s+%d]", value.cstr, size-done-2));
			gen_push(REG_RAX);
			remain -= 2;
			done += 2;
		}
		else if (remain >= 1)
		{
			btc_set(BTC_MOVSX, REG_RAX, make_format_ddString("BYTE[%s+%d]", value.cstr, size-done-1));
			gen_push(REG_RAX);
			remain -= 1;
			done += 1;
		}
	}
}

#endif

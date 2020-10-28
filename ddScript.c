#include <ddcString.h>
#include <ddcGArray.h>
#include <ddcDef.h>
#include <ddcArguments.h>
#include <stdio.h>
#include <ddcCursor.h>
#include <time.h>
#include <ddcTime.h>

typedef enum dstypes dstypes;

enum dstypes {
	dtint=0,
	dtstring=1
};

typedef struct Stack Stack;
struct Stack
{
	ddGArray vals;
	ddString* names;
	dstypes* typs;
	ddsize cap;
	ddsize len;
};
Stack make_stack(ddsize _cap)
{
	Stack _o;
	_o.vals = make_ddGArray(_cap);
	_o.names = make(ddString, _cap);
	_o.typs = make(dstypes, _cap);
	_o.cap = _cap;
	_o.len = 0;
	return _o;
}
void stack_add(Stack* stack, ddString name, void* value, dstypes type)
{
	stack->typs[stack->len] = type;
	stack->names[stack->len] = name;
	__ddGArray_push(&(stack->vals), value);
	stack->len++;
}

typedef struct FNStack FNStack;
struct FNStack
{
	ddString* names;
	void (**funs)(void);
	ddsize len;
	ddsize cap;
};

FNStack make_FNStack(ddsize _cap)
{
	FNStack _o;
	_o.names = make(ddString, _cap);
	_o.funs = calloc(_cap, sizeof(void(*)(void)));
	_o.cap = _cap;
	_o.len = 0;
	return _o;
}
void FNStack_add(FNStack* fn, ddString name, void(*f)(void))
{
	fn->names[fn->len] = name;
	fn->funs[fn->len] = f;
	(fn->len)++;
}
void FNStack_call(FNStack fn, ddString fu)
{
	for (int i = 0; i < fn.len; i++)
	{
		if (ddString_compare(fn.names[i], fu))
		{
			(*(fn.funs[i]))();
		}
	}
}

void parse_comms(ddString line, ddString* comms)
{
	for (int i = 0; i < 6; i++)
	{
		ddString_empty(&(comms[i]));
	}
	int comc = 0;
	for (int i = 0; i < line.length; i++)
	{
		if (line.cstr[i] == ' ')
		{
			comc++;	
			continue;
		}
		ddString_push_char_back(&(comms[comc]), line.cstr[i]);
	}
}

int stack_get_index(const Stack stack, const ddString name)
{
	for (int i = 0; i < stack.len; i++)
	{
		if (ddString_compare(name, stack.names[i]))
		{
			return i;
		}
	}
}
ddString file[20];
ddsize flen;
ddString* comms;

//FNStack_add(&fnstack, make_constant_ddString("print"), 

void compute(void)
{
	Stack stack = make_stack(20);
	FNStack fnstack = make_FNStack(10);
	for (int i = 0; i < flen; i++)
	{
		parse_comms(file[i], comms);
		if (ddString_compare_cstring(comms[0], "add"))
		{
			int indx = stack_get_index(stack, comms[1]);
			switch (stack.typs[indx])
			{
				case dtint: 
				{
					int* v = ((int*)__ddGArray_get(stack.vals, indx));
					int t = ddString_to_int(make_constant_ddString(comms[2].cstr));
					(*v) += t;
				}
			}
		}
		if (ddString_compare_cstring(comms[0], "if"))
		{// if a == 1 then
			int indx = stack_get_index(stack, comms[1]);
			int val = ddGArray_get(stack.vals, indx, int);
			int comp = ddString_to_int(make_constant_ddString(comms[3].cstr));
			if (val == comp)
			{
				if (ddString_compare_cstring(comms[5], "goto"))
				{
					int g = ddString_to_int(make_constant_ddString(comms[6].cstr));
					i = g-1;
					continue;
				}
				if (ddString_compare_cstring(comms[5], "end"))
				{
					return;
				}

			}
		}
		if (ddString_compare_cstring(comms[0], "goto"))
		{
			int g = ddString_to_int(make_constant_ddString(comms[1].cstr));
			i = g-1;
			continue;
		}
		if (ddString_compare_cstring(comms[0], "usleep"))
		{
			int g = ddString_to_int(make_constant_ddString(comms[1].cstr));
			usleep(g);
		}
		if (ddString_compare_cstring(comms[0], "sleep"))
		{
			int g = ddString_to_int(make_constant_ddString(comms[1].cstr));
			sleep(g);
		}
		if (ddString_compare_cstring(comms[0], "let"))
		{
			if (ddString_compare_cstring(comms[1], "int"))
			{
				int* v = make(int, 1);
				(*v) = ddString_to_int(make_constant_ddString(comms[4].cstr));
				stack_add(&stack, make_ddString((comms[2].cstr)),
						(void*)v, dtint);
			}
			if (ddString_compare_cstring(comms[1], "str"))
			{
				ddString v = make_ddString(comms[4].cstr);
				stack_add(&stack, make_ddString((comms[2].cstr)),
						(void*)&v, dtstring);
			}
		}
		if (ddString_compare_cstring(comms[0], "print"))
		{
			int indx = stack_get_index(stack, comms[1]) - 1;
			switch (stack.typs[indx])
			{
				case dtint: ddPrint_int_nl(ddGArray_get(stack.vals, indx, int));
				case dtstring: ddPrint_ddString(ddGArray_get(stack.vals, indx, ddString));
			}
		}
	}
}

int main(ddsize agsc, char** ags)
{
	ddArgs da = init_ddArgs(ags, agsc);

	comms = make(ddString, 22);
	flen = 0;


	for (int i = 0; i < 20; i++)
	{
		file[i] = make_ddString_length("", 0);
	}

	for (int i = 0; i < 22; i++)
	{
		comms[i] = make_ddString_length("", 0);
	}


	for(;;)
	{
		char* cinp = make(char, 50);
		ddPrint_int(flen);
		ddPrint_cstring(": ");
		fgets(cinp,50,stdin);
		ddString ds = make_ddString(cinp);
		if (cinp[0] == 'r' &&
			cinp[1] == 'u' &&
			cinp[2] == 'n')
		{
			compute();
		}
		else if (cinp[0] == 't' &&
			cinp[1] == 'i' &&
			cinp[2] == 'm' &&
			cinp[3] == 'e')
		{
			ddTimer_start();
			compute();
			double ti = ddTimer_stop();
			ddPrint_nl();
			ddPrint_cstring("RunTime: ");
			ddPrint_double_nl(ti);
		}
		else if (cinp[0] == 'c' &&
			cinp[1] == 'l' &&
			cinp[2] == 's')
		{
			cursor_clear();
		}
		else if (cinp[0] == 'l' &&
			cinp[1] == 'i' &&
			cinp[2] == 's' &&
			cinp[3] == 't')
		{
			ddPrint_nl();
			for (int i = 0; i < flen; i++)
			{
				ddPrint_int(i);
				ddPrint_cstring(": ");
				ddPrint_ddString(file[i]);
			}
		}
		else if (cinp[0] == 'c' &&
			cinp[1] == 'l' &&
			cinp[2] == 'e' &&
			cinp[3] == 'a' &&
			cinp[4] == 'r')
		{
			for (int i = 0; i < 20; i++)
			{
				file[i] = make_ddString_length("", 0);
			}
			flen = 0;
		}
		else
		{
			file[flen] = ds;
			flen++;
		}
	}




	return 0;
}

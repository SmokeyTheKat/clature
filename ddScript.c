#include <ddcString.h>
#include <ddcGArray.h>
#include <ddcDef.h>
#include <ddcArguments.h>

typedef enum dstypes dstypes;
enum dstypes {
	dtint=0,
	dtstring=0
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



int main(ddsize agsc, char** ags)
{
	ddArgs da = init_ddArgs(ags, agsc);


	ddString file[4] = {
		make_constant_ddString("let int a = 5"),
		make_constant_ddString("set a = a + 2"),
		make_constant_ddString("set a = a * 2"),
		make_constant_ddString("fn print a")
	};

	Stack stack = make_stack(20);
	FNStack fnstack = make_FNStack(10);
	//FNStack_add(&fnstack, make_constant_ddString("print"), 
	

	for (int i = 0; i < 4; i++)
	{
		if (file[i].cstr[0] == 'l' &&
			file[i].cstr[1] == 'e' &&
			file[i].cstr[2] == 't')
		{
			if (file[i].cstr[4] == 'i' &&
				file[i].cstr[5] == 'n' &&
				file[i].cstr[6] == 't')
			{
				int v = 5;
				stack_add(&stack, make_ddString((char*)&(file[i].cstr[8])),
						(void*)&v, dtint);
			}
		}
		if (file[i].cstr[0] == 'f' &&
			file[i].cstr[1] == 'n')
		{
			if (file[i].cstr[3] == 'p' &&
				file[i].cstr[4] == 'r' &&
				file[i].cstr[5] == 'i' &&
				file[i].cstr[6] == 'n' &&
				file[i].cstr[7] == 't')

		}
	}



	return 0;
}

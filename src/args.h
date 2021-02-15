#ifndef __clature_args_h__
#define __clature_args_h__

#include "./qalloc.h"
#include "./ddcLib/ddcPrint.h"

struct compilerArgs
{
	ddString name;
	ddString value;
};

struct compilerArgs* cargs;
sizet cargsCount = 0;

void read_args(int argc, char** argv)
{
	cargs = make(struct compilerArgs, 1000);
	for (int i = 1; i < argc; i++)
	{
		ddString dsarg = make_ddString(argv[i]);
		if (ddString_compare_cstring(dsarg, "--help") || ddString_compare_cstring(dsarg, "-help"))
		{
			ddPrint_cstring("clature\n");
			ddPrint_cstring("usage: ccl [OPTIONS...] file\n");
			ddPrint_cstring("options:\n");
			ddPrint_cstring("	-o			output elf executable file\n");
			ddPrint_cstring("	-s			output asm file\n");
			ddPrint_cstring("	--no-clean-up		lets the .s and .o files stick around\n");
			ddPrint_cstring("	--debug			debug logs\n");
			ddPrint_cstring("	--parse-tree		debug logs\n");
			ddPrint_cstring("	--no-stack		will not use push's and pop's and will try to use registers for variables\n");
			ddPrint_cstring("	--no-main		no main function\n");
			ddPrint_cstring("	-O0			btc generation goes directly to asm\n");
			ddPrint_cstring("	-O1	(DEFAULT)	stack optimizations\n");
			ddPrint_cstring("	-O2			optimzation level 2\n");
			ddPrint_cstring("	-O3			optimzation level 3\n");
			ddPrint_cstring("all paramaters are passed as macros to the compiler.\n");
			ddPrint_cstring("docs: https://ddmo.rf.gd/clature/\n");
			exit(0);
		}
		else if (ddString_compare_cstring(dsarg, "-o"))
		{
			cargs[cargsCount].name = dsarg;
			cargs[cargsCount++].value = make_ddString(argv[++i]);
		}
		else if (dsarg.cstr[0] == '-')
		{
			cargs[cargsCount++].name = dsarg;
		}
		else if (dsarg.cstr[0] != '-')
		{
			cargs[cargsCount].name = make_constant_ddString("__INPUT_FILE");
			cargs[cargsCount++].value = dsarg;
		}
	}
}
ddString args_get_value(ddString name)
{
	for (sizet i = 0; i < cargsCount; i++)
	{
		if (ddString_compare(name, cargs[i].name)) return cargs[i].value;
	}
	return make_constant_ddString("ERROR");
}
bool args_if_def(ddString name)
{
	for (sizet i = 0; i < cargsCount; i++)
	{
		if (ddString_compare(name, cargs[i].name)) return true;
	}
	return false;
}

#endif

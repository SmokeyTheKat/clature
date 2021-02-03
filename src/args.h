#ifndef __clature_args_h__
#define __clature_args_h__

#include "./qalloc.h"
#include <ddcPrint.h>

struct compilerArgs
{
	ddString name;
	ddString value;
};

struct compilerArgs* cargs;
sizet cargsCount = 0;

void read_args(int argc, char** argv)
{
	cargs = qmake(struct compilerArgs, 1000);
	for (int i = 1; i < argc; i++)
	{
		ddString dsarg = make_ddString(argv[i]);
		if (ddString_compare_cstring(dsarg, "--help") || ddString_compare_cstring(dsarg, "-help"))
		{
			ddPrint_cstring_nl("clature");
			ddPrint_cstring_nl("usage: ccl [OPTIONS...] file");
			ddPrint_cstring_nl("options:");
			ddPrint_cstring_nl("	-o			output elf executable file");
			ddPrint_cstring_nl("	-s			output asm file");
			ddPrint_cstring_nl("	--no-clean-up		lets the .s and .o files stick around");
			ddPrint_cstring_nl("	--debug			debug logs");
			ddPrint_cstring_nl("	--parse-tree		debug logs");
			ddPrint_cstring_nl("	--no-stack		will not use push's and pop's and will try to use registers for variables");
			ddPrint_cstring_nl("	--no-main		no main function");
			ddPrint_cstring_nl("	-O0			btc generation goes directly to asm");
			ddPrint_cstring_nl("	-O1	(DEFAULT)	stack optimizations");
			ddPrint_cstring_nl("	-O2			optimzation level 2");
			ddPrint_cstring_nl("	-O3			optimzation level 3");
			ddPrint_cstring_nl("all paramaters are passed as macros to the compiler.");
			ddPrint_cstring_nl("docs: https://ddmo.rf.gd/clature/");
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

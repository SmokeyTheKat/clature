#ifndef __clature_system_h__
#define __clature_system_h__

#include "./utils.h"

void system_compile_nasm(void)
{
	ddString output = make_ddString(args_get_value(make_constant_ddString("-o")).cstr);
	ddString_push_cstring_back(&output, ".o");
	ddString input = make_ddString(args_get_value(make_constant_ddString("-o")).cstr);
	ddString_push_cstring_back(&input, ".s");

	ddString nasmcommand = make_ddString("nasm -f elf64 ");
	ddString_push_back(&nasmcommand, input);
	ddString_push_cstring_back(&nasmcommand, " -o ");
	ddString_push_back(&nasmcommand, output);
	nasmcommand.cstr[nasmcommand.length] = '\0';

	system(nasmcommand.cstr);
	compile_message(nasmcommand.cstr);

	raze_ddString(&nasmcommand);
	raze_ddString(&output);
	raze_ddString(&input);
}
void system_compile_ld(void)
{
	ddString output = make_ddString(args_get_value(make_constant_ddString("-o")).cstr);
	ddString input = make_ddString(args_get_value(make_constant_ddString("-o")).cstr);
	ddString_push_cstring_back(&input, ".o");

	ddString ldcommand = make_ddString("gcc -static -v ");
	ddString_push_back(&ldcommand, input);
	ddString_push_cstring_back(&ldcommand, " -o ");
	ddString_push_back(&ldcommand, output);
	ddString_push_cstring_back(&ldcommand, " > /dev/null 2>&1");

	ldcommand.cstr[ldcommand.length] = '\0';

	system(ldcommand.cstr);
	compile_message(ldcommand.cstr);

	raze_ddString(&ldcommand);
	raze_ddString(&output);
	raze_ddString(&input);
}
void system_compile_clear(void)
{
	ddString rmout = make_ddString(args_get_value(make_constant_ddString("-o")).cstr);//name
	ddString_push_cstring_back(&rmout, ".o");//name.o
	rmout.cstr[rmout.length] = '\0';//name

	ddString rmcommand = make_ddString("rm ");
	ddString_push_back(&rmcommand, rmout);
	compile_message(rmcommand.cstr);
	system(rmcommand.cstr);

	rmout.cstr[rmout.length-1] = 's';

	remake_ddString(&rmcommand, "rm ");
	ddString_push_back(&rmcommand, rmout);
	compile_message(rmcommand.cstr);
	system(rmcommand.cstr);

	raze_ddString(&rmout);
	raze_ddString(&rmcommand);
}

#endif

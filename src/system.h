#ifndef __ddScript_system_h__
#define __ddScript_system_h__

void system_compile_nasm(int agsc, char** ags)
{
	ddString output = make_ddString(ags[2]);
	ddString_push_cstring_back(&output, ".o");
	ddString input = make_ddString(ags[2]);
	ddString_push_cstring_back(&input, ".s");

	ddString nasmcommand = make_ddString("nasm -f elf64 ");
	ddString_push_back(&nasmcommand, input);
	ddString_push_cstring_back(&nasmcommand, " -o ");
	ddString_push_back(&nasmcommand, output);
	nasmcommand.cstr[nasmcommand.length] = '\0';

	system(nasmcommand.cstr);
	ddPrint_ddString_nl(nasmcommand);

	raze_ddString(&nasmcommand);
	raze_ddString(&output);
	raze_ddString(&input);
}
void system_compile_ld(int agsc, char** ags)
{
	ddString output = make_ddString(ags[2]);
	ddString input = make_ddString(ags[2]);
	ddString_push_cstring_back(&input, ".o");

	ddString ldcommand = make_ddString("ld ");
	ddString_push_back(&ldcommand, input);
	ddString_push_cstring_back(&ldcommand, " -o ");
	ddString_push_back(&ldcommand, output);

	ldcommand.cstr[ldcommand.length] = '\0';

	system(ldcommand.cstr);
	ddPrint_ddString_nl(ldcommand);

	raze_ddString(&ldcommand);
	raze_ddString(&output);
	raze_ddString(&input);
}
void system_compile_clear(int agsc, char** ags)
{
	ddString rmout = make_ddString(ags[2]);//name
	ddString_push_cstring_back(&rmout, ".o");//name.o
	rmout.cstr[rmout.length] = '\0';//name

	ddString rmcommand = make_ddString("rm ");
	ddString_push_back(&rmcommand, rmout);
	ddPrint_ddString_nl(rmcommand);
	system(rmcommand.cstr);

	rmout.cstr[rmout.length-1] = 's';

	remake_ddString(&rmcommand, "rm ");
	ddString_push_back(&rmcommand, rmout);
	ddPrint_ddString_nl(rmcommand);
	system(rmcommand.cstr);

	raze_ddString(&rmout);
	raze_ddString(&rmcommand);
}

#endif

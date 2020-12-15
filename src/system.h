#ifndef __ddScript_system_h__
#define __ddScript_system_h__

void system_compile_nasm(int agsc, char** ags)
{
	ddString nasmout = make_ddString(ags[2]);
	ddString nasmcommand = make_ddString("nasm -f elf64 ");
	ddString_push_back(&nasmcommand, nasmout);
	ddString_push_cstring_back(&nasmcommand, " -o ");
	nasmout.cstr[nasmout.length-1] = 'o';
	ddString_push_back(&nasmcommand, nasmout);
	ddPrint_ddString_nl(nasmcommand);
	system(nasmcommand.cstr);
	raze_ddString(&nasmout);
	raze_ddString(&nasmcommand);
}
void system_compile_ld(int agsc, char** ags)
{
	ddString nasmout = make_ddString(ags[2]);
	nasmout.cstr[nasmout.length-1] = 'o';
	ddString ldcommand = make_ddString("ld ");
	ddString_push_back(&ldcommand, nasmout);
	ddString_push_cstring_back(&ldcommand, " -o ");
	nasmout.cstr[nasmout.length-2] = '\0';
	nasmout.length -= 2;
	ddString_push_back(&ldcommand, nasmout);
	ddPrint_ddString_nl(ldcommand);
	system(ldcommand.cstr);
	raze_ddString(&nasmout);
	raze_ddString(&ldcommand);
}

#endif

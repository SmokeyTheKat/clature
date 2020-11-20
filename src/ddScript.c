#include <ddcString.h>
#include <ddcGArray.h>
#include <ddcDef.h>
#include <ddcArguments.h>
#include <stdio.h>
#include <ddcCursor.h>
#include <time.h>
#include <ddcTime.h>
#include <math.h>
#include "./parse.h"
#include "./compile.h"
#include "./tools.h"
#include "./commands.h"


ddString generate_assembly(struct bitCode code)
{
	ddString afile = make_ddString("");
	ddString_push_cstring_back(&afile, "section .text\n");
	ddString_push_cstring_back(&afile, "global _start\n");
	ddString_push_cstring_back(&afile, "_start:\n");
	ddString_push_cstring_back(&afile, "	push rbp;\n");
	ddString_push_cstring_back(&afile, "	mov rbp, rsp;\n");
	ddString_push_cstring_back(&afile, "	add rsp, ");

	ddString spsize = make_ddString_from_int(code.stackSize);
	ddString_push_cstring_back(&afile, spsize.cstr);
	raze_ddString(&spsize);

	ddString_push_cstring_back(&afile, ";\n");

	for (sizet i = 0; i < code.length; i++)
	{
		switch (code.bcommands[i].command)
		{
			case DC_DEF1:
			{
				ddString_push_cstring_back(&afile, "	mov [rbp");

				ddString soffset = make_ddString_from_int(code.bcommands[i].r1o);
				ddString_push_back(&afile, soffset);
				raze_ddString(&soffset);

				ddString_push_cstring_back(&afile, "], byte ");

				ddString sval = make_ddString_from_int(code.bcommands[i].r2.vint);
				ddString_push_cstring_back(&afile, sval.cstr);
				raze_ddString(&sval);

				ddString_push_cstring_back(&afile, ";\n");
				break;
			}
			case DC_DEF2:
			{
				ddString_push_cstring_back(&afile, "	mov [rbp");

				ddString soffset = make_ddString_from_int(code.bcommands[i].r1o);
				ddString_push_back(&afile, soffset);
				raze_ddString(&soffset);

				ddString_push_cstring_back(&afile, "], word ");

				ddString sval = make_ddString_from_int(code.bcommands[i].r2.vint);
				ddString_push_cstring_back(&afile, sval.cstr);
				raze_ddString(&sval);

				ddString_push_cstring_back(&afile, ";\n");
				break;
			}
			case DC_DEF4:
			{
				ddString_push_cstring_back(&afile, "	mov [rbp");

				ddString soffset = make_ddString_from_int(code.bcommands[i].r1o);
				ddString_push_back(&afile, soffset);
				raze_ddString(&soffset);

				ddString_push_cstring_back(&afile, "], dword ");

				ddString sval = make_ddString_from_int(code.bcommands[i].r2.vint);
				ddString_push_cstring_back(&afile, sval.cstr);
				raze_ddString(&sval);

				ddString_push_cstring_back(&afile, ";\n");
				break;
			}

		}
	}
	ddString_push_cstring_back(&afile, "	pop rbp;\n");
	ddString_push_cstring_back(&afile, "	mov eax, 1;\n");
	ddString_push_cstring_back(&afile, "	int 0x80;\n");
	ddString_push_cstring_back(&afile, "section .data\n");
	return afile;
}

int main(int agsc, char** ags)
{
	if (agsc != 2) compile_error(0);
	ddPrint_cstring_nl(ags[1]);
	ddPrint_nl();
	struct codeLine* lines = format_code(make_constant_ddString(ags[1]));
	struct bitCode bcode = generate_bitcode(lines, 1);
	ddString assemblyFile = generate_assembly(bcode);
#include <stdio.h>
	printf("%s", assemblyFile.cstr);
	return 0;
}

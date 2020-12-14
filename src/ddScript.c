#include <ddcDef.h>
#include <ddcString.h>
#include <ddcTime.h>

#include <stdio.h>

#include "./utils.h"
#include "./tokens.h"
#include "./parser.h"
#include "./generate.h"
#include "./file.h"
#include "./compile.h"

#include <stdio.h>
#include <stdlib.h>

bool debug;

int main(int agsc, char** ags)
{
	double runtime;
	double before;
	double tknt;
	double gent;
	double btcwt;
	double fot;
	ddTimer_start();
	ddPrint_cstring("\x1b[38;2;255;255;255m");
	if (agsc < 2) compile_error("NO INPUT FILES");
	debug = false;
	if (agsc > 3 && ddString_compare_cstring(make_constant_ddString("-debug"), ags[3])) debug = true;
	sizet tokenCount = 0;
	ddString file = read_file(ags[1]);

	before = ddTimer_get_time();
	struct token* tokens = tokenize_file(file, &tokenCount);
	runtime = ddTimer_get_time();
	tknt = runtime-before;

	if (debug) ddPrintf("TokenCount: %d\n", tokenCount);
	raze_ddString(&file);
	for (sizet i = 0; i < tokenCount; i++)
	{
		tokens[i].value.cstr[tokens[i].value.length] = '\0';
		if (debug) ddPrintf("%d: %s: %s\n", i, TKN_STRS[tokens[i].type], tokens[i].value.cstr);
	}
	struct bitcode* btcCode = make(struct bitcode, 1);

	before = ddTimer_get_time();
	struct bitcode* btcCodeEnd = generate_bitcode(btcCode, tokens, tokenCount);
	runtime = ddTimer_get_time();
	gent = runtime-before;

	ddString fileOut = make_ddString("");

	before = ddTimer_get_time();
	write_bitcode(btcCodeEnd, &fileOut);
	runtime = ddTimer_get_time();
	btcwt = runtime-before;

	if (debug) ddPrintf("TokenCount: %d\n", tokenCount);
	before = ddTimer_get_time();
	if (agsc < 3)
		ddPrint_ddString(fileOut);
	else if (!debug)
		write_file(ags[2], fileOut);
	else ddPrint_ddString(fileOut);
	runtime = ddTimer_get_time();
	fot = runtime-before;

	if (debug) ddPrint_nl();
	raze(tokens);
	runtime = ddTimer_stop();
	printf("TOKENIZE TIME: %f\n", tknt);
	printf("GENERATION TIME: %f\n", gent);
	printf("BITCODE WRITING TIME: %f\n", btcwt);
	printf("FILEOUT TIME: %f\n", fot);
	printf("TOTAL RUNTIME: %f\n", runtime);

	ddString nasmout = make_ddString(ags[2]);
	ddString nasmcommand = make_ddString("nasm -f elf64 ");
	ddString_push_back(&nasmcommand, nasmout);
	ddString_push_cstring_back(&nasmcommand, " -o ");
	nasmout.cstr[nasmout.length-1] = 'o';
	ddString_push_back(&nasmcommand, nasmout);
	ddPrint_ddString_nl(nasmcommand);
	system(nasmcommand.cstr);

	ddString ldcommand = make_ddString("ld ");
	ddString_push_back(&ldcommand, nasmout);
	ddString_push_cstring_back(&ldcommand, " -o ");
	nasmout.cstr[nasmout.length-2] = '\0';
	nasmout.length -= 2;
	ddString_push_back(&ldcommand, nasmout);
	ddPrint_ddString_nl(ldcommand);
	system(ldcommand.cstr);
	return 0;
}

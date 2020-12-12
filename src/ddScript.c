#include <ddcDef.h>
#include <ddcString.h>

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
	ddPrint_cstring("\x1b[38;2;255;255;255m");
	if (agsc < 2) compile_error("NO INPUT FILES");
	debug = false;
	if (agsc > 3 && ddString_compare_cstring(make_constant_ddString("-debug"), ags[3])) debug = true;
	sizet tokenCount = 0;
	ddString file = read_file(ags[1]);
	struct token* tokens = tokenize_file(file, &tokenCount);
	if (debug) ddPrintf("TokenCount: %d\n", tokenCount);
	raze_ddString(&file);
	for (sizet i = 0; i < tokenCount; i++)
	{
		tokens[i].value.cstr[tokens[i].value.length] = '\0';
		if (debug) ddPrintf("%d: %s: %s\n", i, TKN_STRS[tokens[i].type], tokens[i].value.cstr);
	}
	struct bitcode* btcCode = make(struct bitcode, 1);
	struct bitcode* btcCodeEnd = generate_bitcode(btcCode, tokens, tokenCount);
	ddString fileOut = make_ddString("");
	write_bitcode(btcCodeEnd, &fileOut);
	if (agsc < 3)
		ddPrint_ddString(fileOut);
	else if (!debug)
		write_file(ags[2], fileOut);
	else ddPrint_ddString(fileOut);
	if (debug) ddPrint_nl();
	raze(tokens);
	return 0;
}

#include <ddcDef.h>
#include <ddcString.h>

#include <stdio.h>

#include "./utils.h"
#include "./tokens.h"
#include "./parser.h"
#include "./generate.h"
#include "./file.h"

#include <stdio.h>
#include <stdlib.h>

sizet tokens_get_command_count(struct token* tokens, sizet tokenCount)
{
	sizet semiColons = 0;
	for (sizet i = 0; i < tokenCount; i++)
		if (tokens[i].value.cstr[0] == ';') semiColons++;
	return semiColons;
}
int tokens_get_next_command(struct token* tokens, int start, sizet tokenCount)
{
	for (sizet i = start; i >= 0; i--)
		if (tokens[i].value.cstr[0] == ';') return i;
	return 0;
}

int main(int agsc, char** ags)
{
	ddPrint_cstring("\x1b[38;2;255;255;255m");
	if (agsc < 2) compile_error("NO INPUT FILES");

	sizet tokenCount = 0;
	ddString file = read_file(ags[1]);
	struct token* tokens = tokenize_file(file, &tokenCount);
	ddPrint_int_nl(tokenCount);
	raze_ddString(&file);
	for (sizet i = 0; i < tokenCount; i++)
	{
		tokens[i].value.cstr[tokens[i].value.length] = '\0';
		ddPrintf("%d: %s: %s\n", i, TKN_STRS[tokens[i].type], tokens[i].value.cstr);
	}
	ddPrint_nl();
	ddPrint_nl();
	ddPrint_nl();
	ddPrint_nl();
	ddPrintf("global _start\n_start:\n	push rbp;\n	mov rbp, rsp;\n");
	sizet commandPos = tokenCount-1;
	sizet nextCommandPos = tokens_get_next_command(tokens, commandPos-1, tokenCount)+1;
	while (commandPos > 0)
	{
		struct tokenNode* commandHead = make(struct tokenNode, 1);
		(*commandHead) = make_tokenNode(nullptr, nullptr, nullptr, nullptr);
		parser(tokens, commandHead, nextCommandPos, commandPos, tokenCount);
		generate_asm(commandHead);
		commandPos = nextCommandPos-1;
		nextCommandPos = tokens_get_next_command(tokens, commandPos-1, tokenCount)+1;
		if (nextCommandPos == 1) nextCommandPos = 0;
	}
	ddPrintf("	mov eax, 0;\n	pop rbp;\n	ret;");
	ddPrint_nl();
	ddPrint_nl();
	raze(tokens);
	return 0;
}

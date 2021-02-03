#ifndef __clature_compile_h__
#define __clature_compile_h__

#include <ddcTime.h>

#include "./generate.h"
#include "./lexer.h"
#include "./macros.h"
#include "./qalloc.h"
#include "regs.h"

struct bitcode* generate_bitcode(struct tokenNode** tokenTrees, sizet treeCount);
void write_bitcode(struct bitcode* code, ddString* outp);

bool inFunction;
struct bitcode* functionCode;
extern bool debug;
extern struct stackTracker stackt;

void compile_main(int agsc, char** ags)
{
	compile_reset_color();
	debug = false;
	if (args_if_def(make_constant_ddString("-debug"))) debug = true;
	ddString file;
	if (args_if_def(make_constant_ddString("__INPUT_FILE")))
		file = read_file(args_get_value(make_constant_ddString("__INPUT_FILE")).cstr);
	else compile_error("NO INPUT FILES. USE \"ccl --help\" FOR MORE INFOMATION\n");
		
	ddString fileOut = make_ddString("");
	if (!args_if_def(make_constant_ddString("--no-macros")))
		read_macros(&file);

	init_compiler();

	sizet tokenCount = 0;
	ddTimer_start();
	struct token* tokens = lexer_main(file, &tokenCount);
	compile_message(make_format_ddString("LEXER: %f", ddTimer_stop()).cstr);
	if (debug)
	{
		ddPrintf("TokenCount: %d\n", tokenCount);
		for (sizet i = 0; i < tokenCount; i++)
			ddPrintf("%d: %s: %s\n", i, TKN_STRS[tokens[i].type], tokens[i].value.cstr);
	}

	sizet treeCount = 0;
	ddTimer_start();
	struct tokenNode** parseTrees = parser_main(tokens, tokenCount, &treeCount);
	compile_message(make_format_ddString("PARSER: %f", ddTimer_stop()).cstr);

	ddTimer_start();
	inFunction = false;
	init_regs();
	struct bitcode* bitcodeHead = generate_bitcode_main(parseTrees, treeCount);
	compile_message(make_format_ddString("BTC GEN: %f", ddTimer_stop()).cstr);

	ddTimer_start();
	write_bitcode(bitcodeHead, &fileOut);
	compile_message(make_format_ddString("WRITE BTC: %f", ddTimer_stop()).cstr);

	ddTimer_start();
	if (args_if_def(make_constant_ddString("-o")))
	{
		if (!debug)
			write_file(args_get_value(make_constant_ddString("-o")).cstr, fileOut);
		else
			ddPrint_ddString(fileOut);
	}
	compile_message(make_format_ddString("OUTPUT ASM: %f", ddTimer_stop()).cstr);

	if (debug) ddPrint_nl();

	compile_message(make_format_ddString("RUNTIME: %f", ddTimer_stop()).cstr);
}

/*
sizet bitcode_get_length(struct bitcode* code)
{
	sizet output = 0;
	while (code->next != nullptr)
	{
		code = code->next;
		output++;
	}
	return output;
}
*/

void write_bitcode(struct bitcode* code, ddString* outp)
{
	ddString btcstrs[] = {
		make_constant_ddString("PUSH"),
		make_constant_ddString("POP"),
		make_constant_ddString("MOV"),
		make_constant_ddString("ADD"),
		make_constant_ddString("SUB"),
		make_constant_ddString("MUL"),
		make_constant_ddString("DIV"),
		make_constant_ddString("MOVZX"),
		make_constant_ddString("CMP"),
		make_constant_ddString("SETE"),
		make_constant_ddString("SETNE"),
		make_constant_ddString("SETG"),
		make_constant_ddString("SETGE"),
		make_constant_ddString("SETL"),
		make_constant_ddString("SETLE"),
		make_constant_ddString("JE"),
		make_constant_ddString("JNE"),
		make_constant_ddString("JG"),
		make_constant_ddString("JGE"),
		make_constant_ddString("JL"),
		make_constant_ddString("JLE"),
		make_constant_ddString(""),
		make_constant_ddString("GLOBAL"),
		make_constant_ddString("SYSCALL"),
		make_constant_ddString(""),
		make_constant_ddString("RET"),
		make_constant_ddString("CDQE"),
		make_constant_ddString("MOVSX"),
		make_constant_ddString("CALL"),
		make_constant_ddString("JMP"),
		make_constant_ddString("INC"),
		make_constant_ddString("DEC"),
		make_constant_ddString(""),
		make_constant_ddString("XOR"),
		make_constant_ddString("EXTERN")
	};
	ddString tb = make_constant_ddString("	");
	ddString cn = make_constant_ddString(":");
	ddString cs = make_constant_ddString(", ");
	ddString ed = make_constant_ddString(";\n");
	ddString nl = make_constant_ddString("\n");
	ddString gb = make_constant_ddString("global ");
	outp->cstr = qmake(char, MAX_OFILE_SIZE);
	outp->capacity = MAX_OFILE_SIZE;
	while (code->next!= nullptr)
	{
		switch (code->opc)
		{
			case BTC_LABEL:
				ddString_push_back(outp, code->lhs);
				ddString_push_back(outp, cn);
				ddString_push_back(outp, nl);
				goto wcont;
			case BTC_GLOBAL:
				ddString_push_back(outp, gb);
				ddString_push_back(outp, code->lhs);
				ddString_push_back(outp, nl);
				goto wcont;
			case BTC_ILA:
				ddString_push_back(outp, code->lhs);
				ddString_push_back(outp, ed);
				goto wcont;
			case BTC_TAG:
				ddString_push_back(outp, tb);
				ddString_push_back(outp, code->lhs);
				ddString_push_back(outp, tb);
				ddString_push_back(outp, code->rhs);
				ddString_push_back(outp, ed);
				goto wcont;
		}
		ddString_push_back(outp, tb);
		ddString_push_back(outp, btcstrs[code->opc]);
		if (code->lhs.length != 0)
		{
			ddString_push_back(outp, tb);
			ddString_push_back(outp, code->lhs);
		}
		if (code->rhs.length != 0)
		{
			ddString_push_back(outp, cs);
			ddString_push_back(outp, code->rhs);
		}
		ddString_push_back(outp, ed);
wcont:
		code = code->next;
	}
}

#endif

#ifndef __ddScript_compile_h__
#define __ddScript_compile_h__

#include <ddcTime.h>

#include "./generate.h"
#include "./lexer.h"
#include "./macros.h"
#include "regs.h"

struct bitcode* generate_bitcode(struct tokenNode** tokenTrees, sizet treeCount);
void write_bitcode(struct bitcode* code, ddString* outp);

bool inFunction;
struct bitcode* functionCode;
extern bool debug;
extern struct stackTracker stackt;

void print_tokentree(struct tokenNode* node)
{
	if (node == nullptr) return;
	if (node->value == nullptr) return;
	ddPrintf("TREE VALUE: %s\n", node->value->value.cstr);
	if (node->nodes[1] != nullptr) print_tokentree(node->nodes[1]);
	if (node->nodes[0] != nullptr) print_tokentree(node->nodes[0]);
}

void compile_main(int agsc, char** ags)
{
	compile_reset_color();


	ddTimer_start();

	debug = false;
	sizet tokenCount = 0;
	struct token* tokens;
	struct tokenNode** parseTrees;
	sizet treeCount = 0;
	struct bitcode* bitcodeHead;

	ddString file;
	if (args_if_def(make_constant_ddString("__INPUT_FILE")))
		file = read_file(args_get_value(make_constant_ddString("__INPUT_FILE")).cstr);
	else compile_error("NO INPUT FILES\n");
		
	ddString fileOut = make_ddString("");


	if (args_if_def(make_constant_ddString("-debug"))) debug = true;
	if (!args_if_def(make_constant_ddString("--no-macros")))
		read_macros(&file);
	//execute_macros(&file);
/*
	ddPrint_nl();
	ddPrint_ddString_nl(file);
*/

	init_lexer();
	tokens = tokenize_file(file, &tokenCount);

	if (debug) ddPrintf("TokenCount: %d\n", tokenCount);

	for (sizet i = 0; i < tokenCount; i++)
	{
		tokens[i].value.cstr[tokens[i].value.length] = '\0';
		if (debug) ddPrintf("%d: %s: %s\n", i, TKN_STRS[tokens[i].type], tokens[i].value.cstr);
	}

	parseTrees = parser_main(tokens, tokenCount, &treeCount);
/*
	for (sizet i = 0; i < treeCount; i++)
		print_tokentree(parseTrees[i]);
*/

	bitcodeHead = generate_bitcode(parseTrees, treeCount);

	write_bitcode(bitcodeHead, &fileOut);

	if (args_if_def(make_constant_ddString("-o")))
	{
		if (!debug)
			write_file(args_get_value(make_constant_ddString("-o")).cstr, fileOut);
		else
			ddPrint_ddString(fileOut);
	}

	if (debug) ddPrint_nl();

	printf("RUNTIME: %f\n", ddTimer_stop());

	raze(tokens);
}

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

struct bitcode* generate_bitcode(struct tokenNode** tokenTrees, sizet treeCount)
{
	inFunction = false;
	init_regs();

	struct bitcode* bitcode = generate_bitcode_main(tokenTrees, treeCount);

	//stackSizeNode->rhs = make_ddString_from_int(stackt.size);
	return bitcode;
}

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
	outp->cstr = make(char, 1000000000);
	outp->capacity = 1000000000;
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

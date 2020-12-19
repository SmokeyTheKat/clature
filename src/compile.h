#ifndef __ddScript_compile_h__
#define __ddScript_compile_h__

#include <ddcTime.h>

#include "./generate.h"
#include "./lexer.h"
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
	if (node->right != nullptr) print_tokentree(node->right);
	if (node->left != nullptr) print_tokentree(node->left);
}

void compile_main(int agsc, char** ags)
{
	compile_reset_color();

	if (agsc < 2) compile_error("NO INPUT FILES");

	ddTimer_start();

	debug = false;
	sizet tokenCount = 0;
	struct token* tokens;
	struct tokenNode** parseTrees;
	sizet treeCount = 0;
	struct bitcode* bitcodeHead;

	ddString file = read_file(ags[1]);
	ddString fileOut = make_ddString("");

	if (agsc > 3 && ddString_compare_cstring(make_constant_ddString("-debug"), ags[3])) debug = true;

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

	if (agsc < 3)
		ddPrint_ddString(fileOut);
	else if (!debug)
		write_file(ags[2], fileOut);
	else ddPrint_ddString(fileOut);

	if (debug) ddPrint_nl();

	printf("RUNTIME: %f\n", ddTimer_stop());

	raze_ddString(&file);
	raze_ddString(&fileOut);
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
		make_constant_ddString("push"),
		make_constant_ddString("pop"),
		make_constant_ddString("mov"),
		make_constant_ddString("add"),
		make_constant_ddString("sub"),
		make_constant_ddString("mul"),
		make_constant_ddString("div"),
		make_constant_ddString("movzx"),
		make_constant_ddString("cmp"),
		make_constant_ddString("sete"),
		make_constant_ddString("setne"),
		make_constant_ddString("setg"),
		make_constant_ddString("setge"),
		make_constant_ddString("setl"),
		make_constant_ddString("setle"),
		make_constant_ddString("je"),
		make_constant_ddString("jne"),
		make_constant_ddString("jg"),
		make_constant_ddString("jge"),
		make_constant_ddString("jl"),
		make_constant_ddString("jle"),
		make_constant_ddString(""),
		make_constant_ddString("global"),
		make_constant_ddString("syscall"),
		make_constant_ddString(""),
		make_constant_ddString("ret"),
		make_constant_ddString("cdqe"),
		make_constant_ddString("movsx"),
		make_constant_ddString("call"),
		make_constant_ddString("jmp"),
		make_constant_ddString("inc"),
		make_constant_ddString("dec"),
		make_constant_ddString("")
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

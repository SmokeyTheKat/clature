#ifndef __ddScript_compile_h__
#define __ddScript_compile_h__

#include <ddcTime.h>

#include "./generate.h"
#include "./tokens.h"
#include "regs.h"

struct bitcode* generate_bitcode(struct bitcode* code, struct token* tokens, sizet tokenCount);
void write_bitcode(struct bitcode* code, ddString* outp);

bool inFunction;
struct bitcode* functionCode;
extern bool debug;
extern struct stackTracker stackt;

void compile_main(int agsc, char** ags)
{
	compile_reset_color();

	if (agsc < 2) compile_error("NO INPUT FILES");

	ddTimer_start();

	debug = false;
	sizet tokenCount = 0;
	struct token* tokens;
	struct tokenNode** parseTrees;
	struct bitcode* bitcodeHead = make(struct bitcode, 1);

	ddString file = read_file(ags[1]);
	ddString fileOut = make_ddString("");

	if (agsc > 3 && ddString_compare_cstring(make_constant_ddString("-debug"), ags[3])) debug = true;

	tokens = tokenize_file(file, &tokenCount);

	if (debug) ddPrintf("TokenCount: %d\n", tokenCount);

	for (sizet i = 0; i < tokenCount; i++)
	{
		tokens[i].value.cstr[tokens[i].value.length] = '\0';
		if (debug) ddPrintf("%d: %s: %s\n", i, TKN_STRS[tokens[i].type], tokens[i].value.cstr);
	}

	parseTrees = parser_main(tokens, tokenCount);

	bitcodeHead = generate_bitcode(btcCode, tokens, tokenCount);

	write_bitcode(btcCodeEnd, &fileOut);

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

struct bitcode* generate_bitcode(struct bitcode* code, struct token* tokens, sizet tokenCount)
{
	struct bitcode* codeHead = code;
	inFunction = false;
	functionCode = make(struct bitcode, 1);
	struct bitcode* functionCodeHead = functionCode;
	init_regs();
	generate_write_btc(&code, BTC_GLOBAL, make_constant_ddString("_start"), REG_NONE);
	generate_write_btc(&code, BTC_LABEL, make_constant_ddString("_start"), REG_NONE);
	generate_write_btc(&code, BTC_PUSH, REG_RBP, REG_NONE);
	generate_write_btc(&code, BTC_MOV, REG_RBP, REG_RSP);
	struct bitcode* stackSizeNode = code;
	generate_write_btc(&code, BTC_SUB, REG_RSP, REG_NONE);
	init_generation();
	sizet commandPos = 0;
	sizet nextCommandPos = tokens_get_next_command(tokens, commandPos, tokenCount);
	while (commandPos < tokenCount)
	{
		if (debug) printf("c: %lld    nc: %lld\n", commandPos, nextCommandPos);
		struct tokenNode* commandHead = make(struct tokenNode, 1);
		(*commandHead) = make_tokenNode(nullptr, nullptr, nullptr, nullptr);
		parser(tokens, commandHead, commandPos, nextCommandPos, tokenCount);
		sizet lineCount = 0;
		if (inFunction)
			generate_asm(commandHead, &functionCode);
		else
			generate_asm(commandHead, &code);
		commandPos = nextCommandPos+1;
		nextCommandPos = tokens_get_next_command(tokens, commandPos, tokenCount);
	}
	generate_write_btc(&code, BTC_MOV, REG_EAX, make_constant_ddString("0"));
	generate_write_btc(&code, BTC_POP, REG_RBP, REG_NONE);
	generate_write_btc(&code, BTC_MOV, REG_RAX, make_constant_ddString("60"));
	generate_write_btc(&code, BTC_MOV, REG_RDI, make_constant_ddString("0"));
	generate_write_btc(&code, BTC_SYSCALL, REG_NONE, REG_NONE);
	if (functionCode->prev != nullptr)
	{
		codeHead->prev = functionCode->prev;
		functionCode->prev->next = codeHead;
	}
	else functionCodeHead = codeHead;
	stackSizeNode->rhs = make_ddString_from_int(stackt.size);
	return functionCodeHead;
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
		make_constant_ddString("jmp")
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

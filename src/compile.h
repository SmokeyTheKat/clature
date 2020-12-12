#ifndef __ddScript_compile_h__
#define __ddScript_compile_h__

#include <ddcTime.h>

#include "./generate.h"
#include "./tokens.h"
#include "regs.h"

bool inFunction;
struct bitcode* functionCode;
extern bool debug;
extern struct stackTracker stackt;

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
		make_constant_ddString("call")
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

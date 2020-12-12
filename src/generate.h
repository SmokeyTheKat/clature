#ifndef __ddScript_generate_h__
#define __ddScript_generate_h__

#include "./regs.h"

#define BTC_PUSH	0x00
#define BTC_POP		0x01
#define BTC_MOV		0x02
#define BTC_ADD		0x03
#define BTC_SUB		0x04
#define BTC_MUL		0x05
#define BTC_DIV		0x06
#define BTC_MOVZX	0x07
#define BTC_CMP 	0x08
#define BTC_SETE 	0x09
#define BTC_SETNE 	0x0A
#define BTC_SETG 	0x0B
#define BTC_SETGE 	0x0C
#define BTC_SETL 	0x0D
#define BTC_SETLE	0x0E
#define BTC_JE		0x0F
#define BTC_JNE		0x10
#define BTC_JG		0x11
#define BTC_JGE		0x12
#define BTC_JL		0x13
#define BTC_JLE		0x14
#define BTC_LABEL	0x15
#define BTC_GLOBAL	0x16
#define BTC_SYSCALL	0x17
#define BTC_ILA		0x18
#define BTC_RET		0x19

struct variable;
struct stackTracker;
struct function;
struct functionTracker;
struct bitcode;

void generate_asm(struct tokenNode* node, struct bitcode** code);
void generate_write_btc(struct bitcode** codePtr, int opc, ddString lhs, ddString rhs);
void generate_asm_2reg(struct tokenNode* node, int opc, struct bitcode** code);
void generate_asm_1reg(struct tokenNode* node, int opc, struct bitcode** code);
void init_generation(void);
void generate_add_var(ddString name, int size);
void generate_add_function(ddString name, sizet retSize);
void generate_print_var(ddString name, struct bitcode* code);

struct stackTracker stackt;
struct functionTracker functiont;
struct functionTracker functiont;
sizet scope = 0;
sizet lscope = 0;
extern bool inFunction;
extern struct bitcode* functionCode;

const char* ASM_SIZES[5] = {
	"",
	"byte",
	"word",
	"",
	"dword"
};

struct variable
{
	ddString name;
	sizet spos;
	sizet size;
};

struct stackTracker
{
	sizet size;
	sizet top;
	struct variable vars[500];
	
};

struct function
{
	ddString name;
	sizet retSize;
};

struct functionTracker
{
	sizet size;
	struct function funs[500];
};

struct bitcode
{
	int opc;
	ddString lhs;
	ddString rhs;
	struct bitcode* next;
	struct bitcode* prev;
};


void generate_write_btc(struct bitcode** codePtr, int opc, ddString lhs, ddString rhs)
{
/*
	if (rhs.length == 0 && lhs.length != 0)
		ddPrintf("opc: %d    lhs: %s\n", opc, lhs.cstr, rhs.cstr);
	else if (rhs.length == 0 && lhs.length == 0)
		ddPrintf("opc: %d\n", opc, lhs.cstr, rhs.cstr);
	else
		ddPrintf("opc: %d    lhs: %s    rhs: %s\n", opc, lhs.cstr, rhs.cstr);
*/
	(*codePtr)->opc = opc;
	(*codePtr)->lhs = lhs;
	(*codePtr)->rhs = rhs;
	struct bitcode* nbtc = make(struct bitcode, 1);
	(*codePtr)->next = nbtc;
	(*codePtr)->next->prev = (*codePtr);
	(*codePtr) = (*codePtr)->next;
}

void generate_asm_2reg(struct tokenNode* node, int opc, struct bitcode** code)
{
	generate_asm(node->left, code);
	generate_asm(node->right, code);
	generate_write_btc(code, BTC_POP, REG_R14, REG_NONE);
	generate_write_btc(code, BTC_POP, REG_R15, REG_NONE);
	generate_write_btc(code, opc, REG_R15, REG_R14);
	generate_write_btc(code, BTC_PUSH, REG_R15, REG_NONE);
	return;
}

void generate_asm_1reg(struct tokenNode* node, int opc, struct bitcode** code)
{
	generate_asm(node->left, code);
	generate_asm(node->right, code);
	generate_write_btc(code, BTC_POP, REG_R15, REG_NONE);
	generate_write_btc(code, BTC_POP, REG_RAX, REG_NONE);
	generate_write_btc(code, opc, REG_R15, REG_NONE);
	generate_write_btc(code, BTC_PUSH, REG_RAX, REG_NONE);
	return;
}

void init_generation(void)
{
	init_regs();
	stackt.top = 0;
	stackt.size = 0;
	functiont.size = 0;
}

void generate_add_var(ddString name, int size)
{
	stackt.vars[stackt.top].name = name;
	stackt.vars[stackt.top].size = size;
	stackt.top++;
	stackt.size += size;
	stackt.vars[stackt.top-1].spos = stackt.size;
};

void generate_add_function(ddString name, sizet retSize)
{
	functiont.funs[functiont.size].name = name;
	functiont.funs[functiont.size].retSize = retSize;
	functiont.size++;
}

void generate_print_var(ddString name, struct bitcode* code)
{
	for (sizet i = 0; i < stackt.top; i++)
	{
		name.cstr[name.length] = '\0';
		stackt.vars[i].name.cstr[stackt.vars[i].name.length] = '\0';
		if (ddString_compare(name, stackt.vars[i].name))
		{
			code->lhs = make_format_ddString("qword [rbp-%d]", stackt.vars[i].spos);
			return;
		}
	}
	code->lhs = name;
}

void generate_asm(struct tokenNode* node, struct bitcode** code)
{
	if (node == nullptr) return;
	if (node->value->type == TKN_ASSEMBLY)
	{
		node->value->value.cstr[0] = '	';
		generate_write_btc(code, BTC_ILA, node->value->value, REG_NONE);
		return;
	}
	if (node->value->type == TKN_KEYWORD)
	{
		if (ddString_compare_cstring(node->value->value, "if"))
		{
			generate_asm(node->right, code);
			generate_write_btc(code, BTC_POP, REG_R15, REG_NONE);
			generate_write_btc(code, BTC_CMP, REG_R15, make_constant_ddString("0"));
			generate_write_btc(code, BTC_JE, make_format_ddString("SC%d", scope+1), REG_NONE);
			return;
		}
		if (ddString_compare_cstring(node->value->value, "sub"))
		{
			if (!inFunction)
			{
				inFunction = true;
				generate_asm(node, &functionCode);
				return;
			}
			generate_add_function(node->right->right->right->value->value, ddString_to_int(node->right->right->value->value));
			generate_asm(node->right, code);
			generate_write_btc(code, BTC_LABEL, make_format_ddString("%s", functiont.funs[functiont.size-1].name.cstr), REG_NONE);
			generate_write_btc(code, BTC_PUSH, REG_RBP, REG_NONE);
			generate_write_btc(code, BTC_MOV, REG_RBP, REG_RSP);
		}
		return;
	}
	switch (node->value->value.cstr[0])
	{
		case ';': return;
		case '{':
		{
			generate_asm(node->right, code);
			scope++;
			lscope++;
			break;
		}
		case '}':
		{
			generate_write_btc(code, BTC_LABEL, make_format_ddString("SC%d", scope), REG_NONE);
			//scope--;
			lscope--;
			if (inFunction && lscope == 0)
			{
				inFunction = false;
				generate_write_btc(code, BTC_POP, REG_RBP, REG_NONE);
				generate_write_btc(code, BTC_RET, REG_NONE, REG_NONE);
			}
			break;
		}
		case '>':
		{
			if (node->value->value.cstr[1] == '=')
			{
				generate_write_btc(code, BTC_PUSH, REG_R15, REG_NONE);
				generate_write_btc(code, BTC_MOVZX, REG_R15, REG_AL);
				generate_write_btc(code, BTC_SETG, REG_AL, REG_NONE);
				generate_write_btc(code, BTC_CMP, REG_R14, REG_R15);
				generate_write_btc(code, BTC_POP, REG_R14, REG_NONE);
				generate_write_btc(code, BTC_POP, REG_R15, REG_NONE);
				generate_asm(node->left, code);
				generate_asm(node->right, code);
				return;
			}
			else
			{
				generate_write_btc(code, BTC_PUSH, REG_R15, REG_NONE);
				generate_write_btc(code, BTC_MOVZX, REG_R15, REG_AL);
				generate_write_btc(code, BTC_SETGE, REG_AL, REG_NONE);
				generate_write_btc(code, BTC_CMP, REG_R14, REG_R15);
				generate_write_btc(code, BTC_POP, REG_R14, REG_NONE);
				generate_write_btc(code, BTC_POP, REG_R15, REG_NONE);
				generate_asm(node->left, code);
				generate_asm(node->right, code);
				return;
			}
		}
		case '<':
		{
			if (node->value->value.cstr[1] == '=')
			{
				generate_write_btc(code, BTC_PUSH, REG_R15, REG_NONE);
				generate_write_btc(code, BTC_MOVZX, REG_R15, REG_AL);
				generate_write_btc(code, BTC_SETL, REG_AL, REG_NONE);
				generate_write_btc(code, BTC_CMP, REG_R14, REG_R15);
				generate_write_btc(code, BTC_POP, REG_R14, REG_NONE);
				generate_write_btc(code, BTC_POP, REG_R15, REG_NONE);
				generate_asm(node->left, code);
				generate_asm(node->right, code);
				return;
			}
			else
			{
				generate_write_btc(code, BTC_PUSH, REG_R15, REG_NONE);
				generate_write_btc(code, BTC_MOVZX, REG_R15, REG_AL);
				generate_write_btc(code, BTC_SETLE, REG_AL, REG_NONE);
				generate_write_btc(code, BTC_CMP, REG_R14, REG_R15);
				generate_write_btc(code, BTC_POP, REG_R14, REG_NONE);
				generate_write_btc(code, BTC_POP, REG_R15, REG_NONE);
				generate_asm(node->left, code);
				generate_asm(node->right, code);
				return;
			}
		}
		case '!':
		{
			if (node->value->value.cstr[1] == '=')
			{
				generate_write_btc(code, BTC_PUSH, REG_R15, REG_NONE);
				generate_write_btc(code, BTC_MOVZX, REG_R15, REG_AL);
				generate_write_btc(code, BTC_SETNE, REG_AL, REG_NONE);
				generate_write_btc(code, BTC_CMP, REG_R14, REG_R15);
				generate_write_btc(code, BTC_POP, REG_R14, REG_NONE);
				generate_write_btc(code, BTC_POP, REG_R15, REG_NONE);
				generate_asm(node->left, code);
				generate_asm(node->right, code);
				return;
			}
		}
		case '=':
		{
			if (node->value->value.cstr[1] == '=')
			{
				generate_asm(node->left, code);
				generate_asm(node->right, code);
				generate_write_btc(code, BTC_POP, REG_R15, REG_NONE);
				generate_write_btc(code, BTC_POP, REG_R14, REG_NONE);
				generate_write_btc(code, BTC_CMP, REG_R14, REG_R15);
				generate_write_btc(code, BTC_SETE, REG_AL, REG_NONE);
				generate_write_btc(code, BTC_MOVZX, REG_R15, REG_AL);
				generate_write_btc(code, BTC_PUSH, REG_R15, REG_NONE);
				return;
			}
			if (node->left->value->value.cstr[0] == '@')
			{
				generate_add_var(node->left->right->right->value->value, ddString_to_int(node->left->right->value->value));
				generate_asm(node->right, code);
				generate_write_btc(code, BTC_POP, make_format_ddString("qword[rbp-%d]", stackt.vars[stackt.top-1].spos), REG_NONE);
				return;
			}
			else
			{
				generate_asm(node->right, code);
				generate_write_btc(code, BTC_POP, make_format_ddString("qword[rbp-%d]", stackt.vars[stackt.top-1].spos, node->left->value->value.cstr), REG_NONE);
			}
			break;
		}
		case '@':
		{
			//ddPrintf("	FLKJDSFL:\n");
			break;
		}
		case '*':
			generate_asm_1reg(node, BTC_MUL, code);
			return;
		case '/':
			generate_asm_1reg(node, BTC_DIV, code);
			return;
		case '+':
			generate_asm_2reg(node, BTC_ADD, code);
			return;
		case '-':
			generate_asm_2reg(node, BTC_SUB, code);
			return;
		default:
		{
			(*code)->opc = BTC_PUSH;
			generate_print_var(node->value->value, (*code));
			(*code)->rhs = REG_NONE;
			(*code)->next = make(struct bitcode, 1);
			(*code)->next->prev = (*code);
			(*code )= (*code)->next;
			break;
		}
	}
}

#endif

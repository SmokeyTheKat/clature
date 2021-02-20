#ifndef __clature_generate_h__
#define __clature_generate_h__

#include "./regs.h"
#include "./qalloc.h"

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
#define BTC_CDQE	0x1A
#define BTC_MOVSX	0x1B
#define BTC_CALL	0x1C
#define BTC_JMP		0x1D
#define BTC_INC		0x1E
#define BTC_DEC		0x1F
#define BTC_TAG		0x20
#define BTC_XOR		0x21
#define BTC_EXTERN	0x22

struct bitcode* generate_bitcode_main(struct tokenNode** parseTrees, sizet _treeCount);
void generate_write_btc(int opc, ddString lhs, ddString rhs);
void generate_trees_asm(void);

static sizet treePosition = 0;
static sizet treeCount = 0;
static struct tokenNode** tokenTrees;
static struct tokenNode* cnode;
static struct bitcode* bitcode;
static struct bitcode* bitcodeHead;

#define MAX_SCOPES 40

sizet scope = 0;
sizet scopeCounts[MAX_SCOPES];
sizet scopeStackPos = 0;
sizet scopeStack[MAX_SCOPES];


struct tac_value
{
	union
	{
		ddString reg;
		ddString str;
		long num;
		int tmp;
	} val;
	int type;
};
struct tac
{
	int opatr;
	struct tac_value lhs;
	struct tac_value rhs;
	struct tac_value set;
};
struct bitcode
{
	int opc;
	ddString lhs;
	ddString rhs;
	struct bitcode* next;
	struct bitcode* prev;
};

struct tac_value generate_tac(struct tac* tc, int* tcpos, struct tokenNode* node)
{
	tc[*tcpos].opatr = node->nodes[1]->value->symbol;
	tc[*tcpos].lhs = generate_tac(tc, tcpos+1, node->nodes[1]);
	tc[*tcpos].rhs = generate_tac(tc, tcpos+1, node->nodes[1]);
	struct tac_value output;
	output.val.num = 0;
	output.type = 0;
	return output;
}

struct bitcode* generate_bitcode_main(struct tokenNode** parseTrees, sizet _treeCount)
{
	struct tac tc[200];
	int tcptr = 0;

	generate_tac(tc, &tcptr, *parseTrees);

	ddPrintf("%d - %d\n", tc[0].lhs.val.num, tc[0].rhs.val.num);
	
	return bitcodeHead;
}

void generate_trees_asm(void)
{
}

void generate_asm_step(struct tokenNode* node)
{
}

void generate_write_btc(int opc, ddString lhs, ddString rhs)
{
	bitcode->opc = opc;
	bitcode->lhs = lhs;
	bitcode->rhs = rhs;
	struct bitcode* nbtc = make(struct bitcode, 1);
	bitcode->next = nbtc;
	bitcode->next->prev = bitcode;
	bitcode = bitcode->next;
}
#endif

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

enum
{
	NTYPE_THO=0,
	NTYPE_MOD,
	NTYPE_VALUE,
	NTYPE_DEREF,
	NTYPE_DEREF_OFFSET,
};

struct bitcode;

static struct bitcode* bitcode_get_first(void);

struct bitcode
{
	int opc;
	ddString r1, r2;
	struct bitcode* next;
	struct bitcode* prev;
};

static struct tokenNode** trees;
static sizet treeCount;
static struct bitcode* btcptr;

static void btc_set(int opc, ddString r1, ddString r2)
{
	btcptr->opc = opc;
	btcptr->r1 = r1;
	btcptr->r2 = r2;
	btcptr->next = malloc(sizeof(struct bitcode));
	btcptr->next->prev = btcptr;
	btcptr = btcptr->next;
}

static struct bitcode* bitcode_get_first(void)
{
	struct bitcode* b = btcptr;
	while (b->prev) b = b->prev;
	return b;
}

static int toh_get_opc(struct tokenNode* node)
{
	int type = node->value->type;
	switch (type)
	{
		case TKN_ADD: return BTC_ADD;
		case TKN_SUB: return BTC_SUB;
		case TKN_MUL: return BTC_MUL;
		case TKN_DIV: return BTC_DIV;
	}
}

static void gen_push(ddString val)
{
	btc_set(BTC_PUSH, val, REG_NONE);
}
static void gen_pop(ddString val)
{
	btc_set(BTC_POP, val, REG_NONE);
}

static void gen_split(struct tokenNode* node, int idx)
{
	generate_tree(node->nodes[idx]);
}

static int node_classify(struct tokenNode* node)
{
	switch (node->nodeCount)
	{
		case 0:
		{
			return NTYPE_VALUE;
		} break;
		case 2:
		{
			return NTYPE_MOD;
		} break;
		case 3:
		{
			if (node->nodes[2]->value->type == TKN_AT)
				return NTYPE_DEREF;
			else return NTYPE_THO;
		} break;
		case 5:
		{
			if (node->nodes[4]->value->type == TKN_AT)
				return NTYPE_DEREF;
		} break;
		case 7:
		{
			if (node->nodes[6]->value->type == TKN_AT)
				return NTYPE_DEREF_OFFSET;
		} break;
	}
}

void generate_tree(struct tokenNode* node)
{
	int type = node_classify(node);
	switch (type)
	{
		case NTYPE_VALUE:
		{
			gen_push(node->value->value);
		} break;
		case NTYPE_THO:
		{
			if (node->value->type == TKN_EQUALS)
			{
				
			}
			gen_split(node, 0);
			gen_split(node, 2);
			gen_pop(REG_R8);
			gen_pop(REG_R9);
			int opc = toh_get_opc(node);
			btc_set(opc, REG_R8, REG_R9);
			gen_push(REG_R8);

		} break;
	}
}

struct bitcode* generate_bitcode_main(struct tokenNode** parseTrees, sizet _treeCount)
{
	trees = parseTrees;
	treeCount = _treeCount;
	generate_tree(parseTrees[0]);
	exit(1);
	return bitcode_get_first();
}



#endif

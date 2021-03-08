#ifndef __clature_generate_h__
#define __clature_generate_h__

#include "./regs.h"
#include "./qalloc.h"
#include "./data.h"

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

enum
{
	TAC_ID,
	TAC_REG,
	TAC_NUM,
	TAC_TMP,
	TAC_NONE,
};

struct tac_value
{
	union
	{
		ddString reg;
		struct stVariable* id;
		ddString num;
		int tmp;
		int none;
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

enum
{
	STMT_VALUE,
	STMT_LR_OPATR,
	STMT_MODIFYER,
	STMT_ASSIGN_AT,
	STMT_DEREF,
	STMT_DEREF_COLON,
};

struct tac_value tac_number(ddString num)
{
	struct tac_value output;
	output.type = TAC_NUM;
	output.val.num = num;
	return output;
}
struct tac_value tac_temp(int tmp)
{
	struct tac_value output;
	output.type = TAC_TMP;
	output.val.tmp = tmp;
	return output;
}
struct tac_value tac_none(void)
{
	struct tac_value output;
	output.type = TAC_NONE;
	output.val.none = 0;
	return output;
}

int identify(struct tokenNode* node)
{
	switch (node->nodeCount)
	{
		case 0:
		{
			return  STMT_VALUE;
		} break;
		case 2:
		{
			return  STMT_MODIFYER;
		} break;
		case 3:
		{
			if (node->nodes[2]->value->type == TKN_AT &&
			    node->nodes[1]->value->type == TKN_NUMBER)
				return STMT_ASSIGN_AT;
			return STMT_LR_OPATR;
		} break;
		case 5:
		{
			if (node->nodes[4]->value->type == TKN_AT &&
			    node->nodes[3]->value->type == TKN_NUMBER)
				return STMT_DEREF;
		} break;
		case 7:
		{
			if (node->nodes[6]->value->type == TKN_AT &&
			    node->nodes[5]->value->type == TKN_NUMBER &&
			    node->nodes[2]->value->type == TKN_COLON)
				return STMT_DEREF_COLON;
		} break;
	}
}

struct tac_value generate_tac(struct tac* tc, int* tcpos, int* tmp, struct tokenNode* node)
{
	int otmp = *tmp;
	struct tac_value output;
	int statement_type = identify(node);
	switch (statement_type)
	{
		case STMT_VALUE:
		{
			if (node->value->type == TKN_ID)
			{
				output.type = TAC_ID;
				output.val.id = stackt_get_var(node->value->value);
			}
			else if (node->value->type == TKN_NUMBER)
			{
				output.type = TAC_NUM;
				output.val.num = node->value->value;
			}
			return output;
		} break;
		case STMT_ASSIGN_AT:
		{
			output.type = TAC_ID;
			struct stVariable* var = stackt_set_var(node->nodes[0]->value->value,
					ddString_to_int(node->nodes[1]->value->value));
			output.val.id = var;
			return output;
			
		} break;
		case STMT_DEREF:
		{
			(*tcpos)++;
			int stcpos = *tcpos;
			tc[stcpos].opatr = OPATR_DEREF + ddString_to_int(node->nodes[3]->value->value);
			tc[stcpos].set.type = TAC_TMP;
			tc[stcpos].set.val.tmp = (*tmp)++;
			tc[stcpos].lhs = generate_tac(tc, tcpos, tmp, node->nodes[1]);
			tc[stcpos].rhs = tac_none();
		} break;
		case STMT_DEREF_COLON:
		{
			(*tcpos)++;
			int stcpos = *tcpos;
			tc[stcpos].opatr = OPATR_DEREF + ddString_to_int(node->nodes[5]->value->value);
			tc[stcpos].set.type = TAC_TMP;
			tc[stcpos].set.val.tmp = (*tmp)++;
			tc[stcpos].lhs = tac_temp((*tmp));
			tc[stcpos].rhs = tac_none();

			(*tcpos)++;
			stcpos++;
			tc[stcpos].opatr = TKN_ADD;
			tc[stcpos].set.type = TAC_TMP;
			tc[stcpos].set.val.tmp = (*tmp)++;
			tc[stcpos].lhs = tac_temp((*tmp));
			tc[stcpos].rhs = generate_tac(tc, tcpos, tmp, node->nodes[3]);

			(*tcpos)++;
			stcpos++;
			tc[stcpos].opatr = TKN_MUL;
			tc[stcpos].set.type = TAC_TMP;
			tc[stcpos].set.val.tmp = (*tmp)++;
			tc[stcpos].lhs = tac_number(node->nodes[5]->value->value);
			tc[stcpos].rhs = generate_tac(tc, tcpos, tmp, node->nodes[1]);
		} break;
		case STMT_LR_OPATR:
		{
			(*tcpos)++;
			int stcpos = *tcpos;
			tc[stcpos].opatr = node->nodes[1]->value->type;
			tc[stcpos].set.type = TAC_TMP;
			tc[stcpos].set.val.tmp = (*tmp)++;
			tc[stcpos].lhs = generate_tac(tc, tcpos, tmp, node->nodes[0]);
			tc[stcpos].rhs = generate_tac(tc, tcpos, tmp, node->nodes[2]);
		} break;
		case STMT_MODIFYER:
		{
			(*tcpos)++;
			int stcpos = *tcpos;
			tc[stcpos].opatr = node->nodes[1]->value->type;
			tc[stcpos].set.type = TAC_TMP;
			tc[stcpos].set.val.tmp = (*tmp)++;
			tc[stcpos].lhs = generate_tac(tc, tcpos, tmp, node->nodes[0]);
			tc[stcpos].rhs = tac_none();
		} break;
	}
	(*tmp)++;
	output.type = TAC_TMP;
	output.val.tmp = otmp;
	return output;
}

void tac_print(struct tac_value tv)
{
	switch (tv.type)
	{
		case TAC_ID:
		{
			ddPrintf("!%s", tv.val.id->name.cstr);
		} break;
		case TAC_NUM:
		{
			ddPrintf("#%s", tv.val.num.cstr);
		} break;
		case TAC_TMP:
		{
			ddPrintf("T%d", tv.val.tmp);
		} break;
	}
}

struct bitcode* generate_bitcode_main(struct tokenNode** parseTrees, sizet _treeCount)
{
	for (int i = 0; i < _treeCount; i++)
	{
		struct tac tc[200];
		int tcptr = -1;
		int tmp = 0;

		generate_tac(tc, &tcptr, &tmp, parseTrees[i]);

		tcptr++;

		ddPrintf("%d    -      \n", tcptr);

		for (int i = 0; i < tcptr; i++)
		{
			tac_print(tc[i].set);
			ddPrintf(" = ");
			tac_print(tc[i].lhs);
			ddPrintf(" %s ", tkn_strs[tc[i].opatr]);
			tac_print(tc[i].rhs);
			ddPrint_nl();
		}
	}

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

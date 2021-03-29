#ifndef __clature_generate_h__
#define __clature_generate_h__
sizet addSizeVal = 0;
bool addSize = false;

#include "./regs.h"

const ddString* reg_stack[] = {
	&REG_R8,
	&REG_R9,
	&REG_R10,
	&REG_R11,
	&REG_R12,
};

int reg_stack_pos = -1;

#include "./utils.h"
#include "./data.h"
#include "./qalloc.h"

#define MAX_SCOPES 40

enum
{
	NTYPE_THO=0,
	NTYPE_MOD,
	NTYPE_VALUE,
	NTYPE_DEREF,
	NTYPE_DEREF_OFFSET,
	NTYPE_VAR_DEF,
	NTYPE_ASM,
	NTYPE_SUB,
	NTYPE_SUB_CALL,
	NTYPE_IF,
	NTYPE_WHILE,
	NTYPE_RETURN,
	NTYPE_FOR,
	NTYPE_OB,
	NTYPE_CB,
};

struct bitcode;

struct bitcode* bitcode_get_first(void);
static inline void switch_stacks(void);
static inline struct bitcode* gen_sub_headder(ddString name);
static inline void gen_sub_footer(void);
struct tokenNode* next_tree(void);
struct tokenNode* peek_tree(void);
void btc_set(int opc, ddString r1, ddString r2);
struct bitcode* bitcode_get_first(void);
int toh_get_opc(struct tokenNode* node);
void gen_push(ddString val);
void gen_pop(ddString val);
void gen_split(struct tokenNode* node, int idx);
void gen_cmp(struct tokenNode* node, void(*cmpfun)(void));
static void compare_lessthan_equal_flag(void);
static void compare_lessthan_flag(void);
static void compare_greaterthan_flag(void);
static void compare_greaterthan_equal_flag(void);
static void compare_equal_flag(void);
static void compare_not_equal_flag(void);
int node_classify(struct tokenNode* node);
void generate_tree(struct tokenNode* node);
struct bitcode* generate_bitcode_main(struct tokenNode** parseTrees, sizet _treeCount);
void gen_mod(struct tokenNode* node);
void gen_tho(struct tokenNode* node);
void gen_mod_equals(struct tokenNode* node);
void gen_div_equals(struct tokenNode* node);
void gen_mul_equals(struct tokenNode* node);
void gen_sub_equals(struct tokenNode* node);
void gen_add_equals(struct tokenNode* node);
void gen_plus_plus(struct tokenNode* node);
void gen_mod_operator(struct tokenNode* node);
void gen_equals(struct tokenNode* node);
void gen_rax_toh(struct tokenNode* node);
void gen_if(struct tokenNode* node);
void gen_while(struct tokenNode* node);
void gen_return(struct tokenNode* node);
void gen_sub_call(struct tokenNode* node);
void gen_var_def(struct tokenNode* node);
void gen_deref(struct tokenNode* node);
void gen_value(struct tokenNode* node);
void gen_asm(struct tokenNode* node);
void gen_scope_down(struct tokenNode* node);
void gen_scope_up(struct tokenNode* node);
struct bitcode* generate_bitcode_main(struct tokenNode** parseTrees, sizet _treeCount);

struct bitcode
{
	int opc;
	ddString r1, r2;
	struct bitcode* next;
	struct bitcode* prev;
};

static struct tokenNode** trees;
static long treePos;
static sizet treeCount;
static struct bitcode* btcptr;
static bool statementIsEquality;

sizet scopeCounts[MAX_SCOPES];
sizet scopeStack[MAX_SCOPES];
sizet scopeStackPos = 0;
sizet scope = 0;

void gen_set_value(ddString reg, sizet size)
{
	ddString r = *reg_stack[reg_stack_pos--];
	switch (size)
	{
		case 8:
		{
		} break;
		case 4:
		{
		} break;
		case 2:
		{
		} break;
		case 1:
		{
		} break;
	}
}

static inline void switch_stacks(void)
{
	struct stackTracker temp;
	temp = stackt;
	stackt = fstackt;
	fstackt = temp;
}

static inline struct bitcode* gen_sub_headder(ddString name)
{
	btc_set(BTC_LABEL, name, REG_NONE);
	btc_set(BTC_PUSH, REG_RBP, REG_NONE);
	btc_set(BTC_MOV, REG_RBP, REG_RSP);
	struct bitcode* bitcodeMoveStack = btcptr;
	btc_set(BTC_SUB, REG_RSP, REG_NONE);
	return bitcodeMoveStack;
}

static inline void gen_sub_footer(void)
{
	//push_result(REG_R8);
	gen_pop(REG_RBP);
	btc_set(BTC_RET, REG_NONE, REG_NONE);
}

struct tokenNode* next_tree(void)
{
	return trees[treePos++];
}
struct tokenNode* peek_tree(void)
{
	return trees[treePos];
}

void btc_set(int opc, ddString r1, ddString r2)
{
	btcptr->opc = opc;
	btcptr->r1 = r1;
	btcptr->r2 = r2;
	btcptr->next = malloc(sizeof(struct bitcode));
	btcptr->next->prev = btcptr;
	btcptr = btcptr->next;
}

struct bitcode* bitcode_get_first(void)
{
	struct bitcode* b = btcptr;
	while (b->prev) b = b->prev;
	return b;
}

int toh_get_opc(struct tokenNode* node)
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

void gen_push(ddString val)
{
	if (addSize) addSizeVal += 8;
	btc_set(BTC_PUSH, val, REG_NONE);
}
void gen_pop(ddString val)
{
	btc_set(BTC_POP, val, REG_NONE);
}

void gen_split(struct tokenNode* node, int idx)
{
	generate_tree(node->nodes[idx]);
}
void gen_cmp(struct tokenNode* node, void(*cmpfun)(void))
{
	gen_split(node, 0);
	gen_split(node, 2);
	gen_pop(REG_R8);
	gen_pop(REG_R9);
	btc_set(BTC_CMP, REG_R8, REG_R9);
	cmpfun();
	gen_push(REG_R8);
}


static void compare_lessthan_equal_flag(void)
{
	btc_set(BTC_SETLE, REG_AL, REG_NONE);
	btc_set(BTC_MOVZX, REG_R8, REG_AL);
}
static void compare_lessthan_flag(void)
{
	btc_set(BTC_SETL, REG_AL, REG_NONE);
	btc_set(BTC_MOVZX, REG_R8, REG_AL);
}
static void compare_greaterthan_flag(void)
{
	btc_set(BTC_SETG, REG_AL, REG_NONE);
	btc_set(BTC_MOVZX, REG_R8, REG_AL);
}
static void compare_greaterthan_equal_flag(void)
{
	btc_set(BTC_SETGE, REG_AL, REG_NONE);
	btc_set(BTC_MOVZX, REG_R8, REG_AL);
}
static void compare_equal_flag(void)
{
	btc_set(BTC_SETE, REG_AL, REG_NONE);
	btc_set(BTC_MOVZX, REG_R8, REG_AL);
}
static void compare_not_equal_flag(void)
{
	btc_set(BTC_SETNE, REG_AL, REG_NONE);
	btc_set(BTC_MOVZX, REG_R8, REG_AL);
}

int node_classify(struct tokenNode* node)
{
	if (node->nodeCount > 0 && ddString_compare_cstring(node->nodes[node->nodeCount-1]->value->value, "sub"))
		return NTYPE_SUB;

	switch (node->nodeCount)
	{
		case 0:
		{
			return NTYPE_VALUE;
		} break;
		case 1:
		{
			if (node->nodes[0]->value->type == TKN_ASM)
				return NTYPE_ASM;
			else if (node->nodes[0]->value->type == TKN_OBU)
				return NTYPE_OB;
			else if (node->nodes[0]->value->type == TKN_CBU)
				return NTYPE_CB;
		} break;
		case 2:
		{
			if (node->nodes[1]->value->type == TKN_KEYWORD &&
			ddString_compare_cstring(node->nodes[1]->value->value, "if"))
				return NTYPE_IF;
			else if (node->nodes[1]->value->type == TKN_KEYWORD &&
			ddString_compare_cstring(node->nodes[1]->value->value, "while"))
				return NTYPE_WHILE;
			else if (node->nodes[1]->value->type == TKN_KEYWORD &&
			ddString_compare_cstring(node->nodes[1]->value->value, "for"))
				return NTYPE_FOR;
			else if (node->nodes[1]->value->type == TKN_KEYWORD &&
			ddString_compare_cstring(node->nodes[1]->value->value, "return"))
				return NTYPE_RETURN;
			else if (node->nodes[1]->value->type == TKN_ID &&
			node->nodes[0]->value->symbol == G_FP && node->nodes[1]->value->symbol == G_I)
				return NTYPE_SUB_CALL;
			return NTYPE_MOD;
		} break;
		case 3:
		{
			if (node->nodes[2]->value->type == TKN_AT)
				return NTYPE_VAR_DEF;
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
		case NTYPE_OB:
			gen_scope_up(node);
			break;
		case NTYPE_CB:
			gen_scope_down(node);
			break;
		case NTYPE_ASM:
			gen_asm(node);
			break;
		case NTYPE_VALUE:
			gen_value(node);
			break;
		case NTYPE_DEREF:
			gen_deref(node);
			break;
		case NTYPE_VAR_DEF:
			gen_var_def(node);
			break;
		case NTYPE_MOD:
			gen_mod(node);
			break;
		case NTYPE_SUB_CALL:
			gen_sub_call(node);
			break;
		case NTYPE_RETURN:
			gen_return(node);
			break;
		case NTYPE_SUB:
			gen_sub(node);
			break;
		case NTYPE_FOR:
			gen_for(node);
			break;
		case NTYPE_WHILE:
			gen_while(node);
			break;
		case NTYPE_IF:
			gen_if(node);
			break;
		case NTYPE_THO:
			gen_tho(node);
			break;
	}
}

struct bitcode* generate_bitcode_main(struct tokenNode** parseTrees, sizet _treeCount)
{
	scope = 0;
	trees = parseTrees;
	treeCount = _treeCount;
	treePos = 0;
	btcptr = malloc(sizeof(struct bitcode));
	btcptr->prev = nullptr;

	btc_set(BTC_GLOBAL, make_constant_ddString("main"), REG_NONE);
	btc_set(BTC_LABEL, make_constant_ddString("main"), REG_NONE);
	btc_set(BTC_EXTERN, make_constant_ddString("malloc"), REG_NONE);
	btc_set(BTC_EXTERN, make_constant_ddString("free"), REG_NONE);
	btc_set(BTC_PUSH, REG_RBP, REG_NONE);
	btc_set(BTC_MOV, REG_RBP, REG_RSP);
	struct bitcode* bitcodeMoveStack = btcptr;
	btc_set(BTC_SUB, REG_RSP, REG_NONE);


	datat_add_data(make_constant_ddString("argv"), make_constant_ddString("0"), 8);
	datat_add_data(make_constant_ddString("argc"), make_constant_ddString("0"), 4);
	btc_set(BTC_MOV, make_constant_ddString("QWORD[argv]"), REG_RSI);
	btc_set(BTC_MOV, make_constant_ddString("DWORD[argc]"), REG_EDI);

	while (treePos < treeCount)
	{
		generate_tree(parseTrees[treePos++]);
	}

	btc_set(BTC_MOV, REG_EAX, make_constant_ddString("0"));
	btc_set(BTC_POP, REG_RBP, REG_NONE);
	btc_set(BTC_MOV, REG_RAX, make_constant_ddString("60"));
	btc_set(BTC_MOV, REG_RDI, make_constant_ddString("0"));
	btc_set(BTC_SYSCALL, REG_NONE, REG_NONE);
	bitcodeMoveStack->r2 = make_ddString_from_int(stackt.size);

	struct bitcode* bitcodeHead = bitcode_get_first();
	struct bitcode* temp = btcptr;
	struct bitcode* datacode = make(struct bitcode, 1);
	btcptr = datacode;
	btc_set(BTC_ILA, make_constant_ddString("section .data"), REG_NONE);
	for (sizet i = 0; i < datat.top; i++)
	{
		btc_set(BTC_LABEL, datat.data[i].name, REG_NONE);
		btc_set(BTC_TAG, datat.data[i].defineSize, datat.data[i].data);
	}
	btc_set(BTC_ILA, make_constant_ddString("section .text"), REG_NONE);
	bitcodeHead->prev = btcptr->prev;
	btcptr->prev->next = bitcodeHead;
	bitcodeHead = datacode;
	btcptr = temp;

	return bitcodeHead;
}

void gen_scope_up(struct tokenNode* node)
{
	scopeStack[scopeStackPos++] = stackt.top;
	scope++;
}
void gen_scope_down(struct tokenNode* node)
{
	stackt.top = scopeStack[--scopeStackPos];
	scope--;
	btc_set(BTC_LABEL, make_format_ddString(".SC%d%d", scope, scopeCounts[scope]), REG_NONE);
	scopeCounts[scope]++;
}

void gen_asm(struct tokenNode* node)
{
	node->nodes[0]->value->value.cstr[0] = '	';
	btc_set(BTC_ILA, node->nodes[0]->value->value, REG_NONE);
}

void gen_value(struct tokenNode* node)
{
	if (ddString_is_number(node->value->value))
	{
		push_value(node->value->value, 8);
	}
	else
	{
		push_stack_var(*stackt_get_var(node->value->value));
	}
}

void gen_deref(struct tokenNode* node)
{
	gen_split(node, 1);
	gen_pop(REG_R8);
	push_ref(REG_R8, ddString_to_int(node->nodes[3]->value->value));
}

void gen_var_def(struct tokenNode* node)
{
	stackt_set_var(node->nodes[0]->value->value,
			ddString_to_int(node->nodes[1]->value->value));
}

void gen_sub_call(struct tokenNode* node)
{
	addSize = true;
	sizet paramCount = (node->nodes[0]->nodeCount)/2;
	ddPrintf("init i: %d\n", paramCount);
	for (sizet i = (paramCount*2)-1; i >= 0; i -= 2)
	{
		ddPrintf("--------------- i: %d\n", i-1);
		generate_tree(node->nodes[0]->nodes[i-1]);
	}
	addSize = false;
	btc_set(BTC_CALL, node->nodes[1]->value->value, REG_NONE);
	btc_set(BTC_ADD, REG_RSP, make_ddString_from_int(addSizeVal));
	ddPrintf("call: %s\n", node->nodes[1]->value->value.cstr);
	addSizeVal = 0;
	if (statementIsEquality)
		gen_push(REG_R8);
}

void gen_return(struct tokenNode* node)
{
	gen_split(node, 0);
	gen_pop(REG_R8);
	btc_set(BTC_JMP, make_format_ddString(".SC%d%d", 0, scopeCounts[0]), REG_NONE);
}

void gen_sub(struct tokenNode* node)
{
	struct bitcode* bitcodeHead = bitcode_get_first();
	struct bitcode* temp = btcptr;
	struct bitcode* functionCodeHead = make(struct bitcode, 1);
	btcptr = functionCodeHead;
	struct bitcode* btcMoveStack = gen_sub_headder(node->nodes[1]->value->value);
	fstackt.size = 0;
	fstackt.top = 0;
	switch_stacks();
	sizet paramCount = (node->nodes[0]->nodeCount)/2;
	struct tokenNode* pnode = node->nodes[0]->nodes[node->nodes[0]->nodeCount-1];
	sizet tsize = 0;
	for (sizet i = 0; i < (paramCount*2)-1; i += 2)
	{
		int csize = ddString_to_int(node->nodes[0]->nodes[i]->nodes[1]->value->value);
		stackt_set_param_var(node->nodes[0]->nodes[i]->nodes[0]->value->value, csize, tsize);
		tsize += csize;
	}
	sizet tscope = scope;
	struct tokenNode* cnode = next_tree();
	generate_tree(cnode);
	while (scope != tscope)
	{
		cnode = next_tree();
		generate_tree(cnode);
	}
	btc_set(BTC_ADD, REG_RSP, make_ddString_from_int(stackt.size));
	gen_sub_footer();
	btcMoveStack->r2 = make_ddString_from_int(stackt.size);
	bitcodeHead->prev = btcptr->prev;
	btcptr->prev->next = bitcodeHead;
	bitcodeHead = functionCodeHead;
	btcptr = temp;
	switch_stacks();
}

void gen_for(struct tokenNode* node)
{
	sizet stotop = stackt.top;
	gen_split(node->nodes[0], 4);
	btc_set(BTC_LABEL, make_format_ddString(".WL%d%d", scope, scopeCounts[scope]), REG_NONE);
	sizet tscope = scope;
	struct tokenNode* cnode = next_tree();
	generate_tree(cnode);
	while (scope != tscope)
	{
		cnode = next_tree();
		generate_tree(cnode);
	}
	gen_split(node->nodes[0], 0);
	statementIsEquality = true;
	gen_split(node->nodes[0], 2);
	gen_pop(REG_R8);
	btc_set(BTC_CMP, REG_R8, make_constant_ddString("1"));
	btc_set(BTC_JE, make_format_ddString(".WL%d%d", scope, scopeCounts[scope]-1), REG_NONE);
	statementIsEquality = false;
	stackt.top = stotop;
}

void gen_while(struct tokenNode* node)
{
	btc_set(BTC_LABEL, make_format_ddString(".WL%d%d", scope, scopeCounts[scope]), REG_NONE);
	sizet tscope = scope;
	struct tokenNode* cnode = next_tree();
	generate_tree(cnode);
	while (scope != tscope)
	{
		cnode = next_tree();
		generate_tree(cnode);
	}
	statementIsEquality = true;
	gen_split(node, 0);
	gen_pop(REG_R8);
	btc_set(BTC_CMP, REG_R8, make_constant_ddString("1"));
	btc_set(BTC_JE, make_format_ddString(".WL%d%d", scope, scopeCounts[scope]-1), REG_NONE);
	statementIsEquality = false;
}

void gen_if(struct tokenNode* node)
{
	statementIsEquality = true;
	gen_split(node, 0);
	gen_pop(REG_R8);
	btc_set(BTC_CMP, REG_R8, make_constant_ddString("0"));
	btc_set(BTC_JE, make_format_ddString(".SC%d%d", scope, scopeCounts[scope]), REG_NONE);
	bool iselse = false;
	sizet tscope = scope;
	statementIsEquality = false;
	generate_tree(trees[treePos++]);
	struct tokenNode* cnode;
	while (scope != tscope)
	{
		cnode = next_tree();
		if (cnode->nodeCount == 1 && cnode->nodes[0]->value->value.cstr[0] == '}')
		{
			struct tokenNode* nextTree = peek_tree();
			if (nextTree && nextTree->nodeCount == 1 && nextTree->nodes[0]->value->symbol == G_KW_ELSE)
			{
				iselse = true;
				btc_set(BTC_JMP, make_format_ddString(".SC%d%d", scope-1, scopeCounts[scope-1]+1), REG_NONE);
			}
		}
		generate_tree(cnode);
	}
	if (iselse)
	{
		next_tree();
		tscope = scope;
		do
		{
			cnode = next_tree();
			generate_tree(cnode);
		} while (scope != tscope);
	}
}

void gen_toh(struct tokenNode* node)
{
	int opc = toh_get_opc(node->nodes[1]);
	gen_split(node, 0);
	ddString* reg1 = reg_stack[reg_stack_pos];
	gen_split(node, 2);
	ddString* reg2 = reg_stack[reg_stack_pos];
	btc_set(opc, *reg1, *reg2);
	reg_stack_pos--;
}
void gen_rax_toh(struct tokenNode* node)
{
	int opc = toh_get_opc(node->nodes[1]);
	gen_split(node, 0);
	ddString* reg1 = reg_stack[reg_stack_pos];
	gen_split(node, 2);
	ddString* reg2 = reg_stack[reg_stack_pos];
	btc_set(BTC_MOV, REG_RAX, *reg1);
	btc_set(opc, *reg2, REG_NONE);
	btc_set(BTC_MOV, *reg1, REG_RAX);
	reg_stack_pos--;
}
void gen_equals(struct tokenNode* node)
{
	statementIsEquality = true;
	struct stVariable* var;
	int n2class = node_classify(node->nodes[2]);
	if (n2class == NTYPE_VAR_DEF)
	{
		var = stackt_set_var(node->nodes[2]->nodes[0]->value->value,
			ddString_to_int(node->nodes[2]->nodes[1]->value->value));
	}
	else if (n2class == NTYPE_DEREF)
	{
		gen_split(node, 0);
		ddString reg1 = *reg_stack[reg_stack_pos];
		gen_split(node->nodes[2], 1);
		ddString reg2 = *reg_stack[reg_stack_pos];
		gen_pop(REG_R8);
		pop_ref(REG_R8, ddString_to_int(node->nodes[2]->nodes[3]->value->value));
		mov_stack_var(*var, *reg_stack[reg_stack_pos--]);
		reg_stack_pos -= 2;
		return;
	}
	else var = stackt_get_var(node->nodes[2]->value->value);

	gen_split(node, 0);
	mov_stack_var(*var, *reg_stack[reg_stack_pos--]);
	statementIsEquality = false;
}

void gen_mod_operator(struct tokenNode* node)
{
	struct stVariable* var = stackt_get_var(node->nodes[0]->value->value);
	btc_set(BTC_MOV, REG_R8, REG_RBP);
	btc_set(BTC_SUB, REG_R8, make_ddString_from_int(var->spos));
	gen_push(REG_R8);
}

void gen_plus_plus(struct tokenNode* node)
{
	struct stVariable* var = stackt_get_var(node->nodes[1]->value->value);
	btc_set(BTC_ADD, make_format_ddString("QWORD[RBP-%d]", var->spos), make_constant_ddString("1"));
	if (statementIsEquality)
		push_stack_var(*var);
}

void gen_add_equals(struct tokenNode* node)
{
	struct stVariable* var = stackt_get_var(node->nodes[2]->value->value);
	gen_split(node, 0);
	gen_split(node, 2);
	gen_pop(REG_R8);
	gen_pop(REG_R9);
	btc_set(BTC_ADD, REG_R8, REG_R9);
	gen_push(REG_R8);
	pop_stack_var(*var);
}
void gen_sub_equals(struct tokenNode* node)
{
	struct stVariable* var = stackt_get_var(node->nodes[2]->value->value);
	gen_split(node, 0);
	ddString* reg1 = reg_stack[reg_stack_pos];
	gen_split(node, 2);
	ddString* reg2 = reg_stack[reg_stack_pos];
	btc_set(BTC_SUB, *reg2, *reg1);
	pop_stack_var(*var);
	reg_stack_pos--;
}
void gen_mul_equals(struct tokenNode* node)
{
	struct stVariable* var = stackt_get_var(node->nodes[2]->value->value);
	gen_split(node, 0);
	gen_split(node, 2);
	gen_pop(REG_RAX);
	gen_pop(REG_R8);
	btc_set(BTC_SUB, REG_R8, REG_NONE);
	gen_push(REG_RAX);
	pop_stack_var(*var);
}
void gen_div_equals(struct tokenNode* node)
{
	struct stVariable* var = stackt_get_var(node->nodes[2]->value->value);
	gen_split(node, 0);
	gen_split(node, 2);
	gen_pop(REG_RAX);
	gen_pop(REG_R8);
	btc_set(BTC_SUB, REG_R8, REG_NONE);
	gen_push(REG_RAX);
	pop_stack_var(*var);
}
void gen_mod_equals(struct tokenNode* node)
{
	struct stVariable* var = stackt_get_var(node->nodes[2]->value->value);
	gen_split(node, 0);
	gen_split(node, 2);
	gen_pop(REG_RAX);
	gen_pop(REG_R8);
	btc_set(BTC_SUB, REG_R8, REG_NONE);
	gen_push(REG_RDX);
	pop_stack_var(*var);
}

void gen_tho(struct tokenNode* node)
{
	switch (node->nodes[1]->value->type)
	{
		case TKN_EQUALS:
			gen_equals(node);
			break;
		case TKN_EQUALS_EQUALS:
			gen_cmp(node, compare_equal_flag);
			break;
		case TKN_NOT_EQUALS:
			gen_cmp(node, compare_not_equal_flag);
			break;
		case TKN_LESS_THAN_EQUALS:
			gen_cmp(node, compare_lessthan_equal_flag);
			break;
		case TKN_GREATER_THAN_EQUALS:
			gen_cmp(node, compare_greaterthan_equal_flag);
			break;
		case TKN_LESS_THAN:
			gen_cmp(node, compare_lessthan_flag);
			break;
		case TKN_GREATER_THAN:
			gen_cmp(node, compare_greaterthan_flag);
			break;
		case TKN_ADD_EQUALS:
			gen_add_equals(node);
			break;
		case TKN_SUB_EQUALS:
			gen_sub_equals(node);
			break;
		case TKN_MUL_EQUALS:
			gen_mul_equals(node);
			break;
		case TKN_DIV_EQUALS:
			gen_div_equals(node);
			break;
		case TKN_MOD_EQUALS:
			gen_mod_equals(node);
			break;
		case TKN_MOD:
			gen_mod_operator(node);
			break;
		case TKN_MUL:
		case TKN_DIV:
			gen_rax_toh(node);
			break;
		default:
			gen_toh(node);
			break;
	}
}

void gen_ref(struct tokenNode* node)
{
	struct stVariable* var = stackt_get_var(node->nodes[0]->value->value);
	btc_set(BTC_MOV, REG_R8, REG_RBP);
	btc_set(BTC_SUB, REG_R8, make_ddString_from_int(var->spos));
	gen_push(REG_R8);
}

void gen_mod(struct tokenNode* node)
{
	switch (node->nodes[1]->value->type)
	{
		case TKN_QUEST:
			gen_ref(node);
			break;
	}
	switch (node->nodes[0]->value->type)
	{
		case TKN_ADD_ADD:
			gen_plus_plus(node);
			break;
		case TKN_SUB_SUB:
			//gen_minus_minus(node);
			break;
	}
}

#endif

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
#define BTC_CDQE	0x1A
#define BTC_MOVSX	0x1B
#define BTC_CALL	0x1C
#define BTC_JMP		0x1D
#define BTC_INC		0x1E
#define BTC_DEC		0x1F

struct stVariable;
struct stackTracker;
struct function;
struct functionTracker;
struct bitcode;

struct bitcode* generate_bitcode_main(struct tokenNode** parseTrees, sizet _treeCount);
static inline struct tokenNode* next_tree(void);
void generate_write_btc(int opc, ddString lhs, ddString rhs);
static inline bool is_equals_new_assignemnt(struct tokenNode* node);
static void push_stack_var(struct stVariable var);
static void pop_stack_var(struct stVariable var);
static inline void push_result(ddString reg);
static inline void compare_equal_flag(void);
static inline void compare_not_equal_flag(void);
static inline void compare_sides(void);
static inline void pop_input(ddString reg);
static inline void pop_both_sides(void);
static inline void generate_split_left(struct tokenNode* node);
static inline void generate_split_right(struct tokenNode* node);
static inline void generate_bisplit(struct tokenNode* node);
static inline void generate_equality(struct tokenNode* node);//2 == 3-1
static inline void generate_inequality(struct tokenNode* node);//2 != 3
static inline void generate_times_equals(struct tokenNode* node);
static inline void generate_divide_equals(struct tokenNode* node);
static inline void generate_plus_equals(struct tokenNode* node);
static inline void generate_minus_equals(struct tokenNode* node);
static inline void generate_plus_plus(struct tokenNode* node);
static inline void generate_minus_minus(struct tokenNode* node);
static inline struct bitcode* generate_write_function_headder(ddString name);// returns the code line of the sub rsp
static inline void generate_write_function_footer(void);
static void generate_if_statement(struct tokenNode* node);
static void generate_while_statement(struct tokenNode* node);
static void generate_sub_statement(struct tokenNode* node);
static void generate_equels_make_set_asm(struct tokenNode* node);//@8 i = 9-3;
static void generate_equels_set_asm(struct tokenNode* node);//i = 2*3;
static void generate_1reg_operation(int opc, struct tokenNode* node);
static void generate_2reg_operation(int opc, struct tokenNode* node);
void generate_asm_step(struct tokenNode* node);
void generate_trees_asm(void);
struct stVariable stackt_get_var(ddString name);
struct stVariable stackt_set_var(ddString name, sizet size);
static inline void switch_stacks(void);

static sizet treePosition = 0;
static sizet treeCount = 0;
static struct tokenNode** tokenTrees;
static struct tokenNode* cnode;
static struct bitcode* bitcode;
static struct bitcode* bitcodeHead;

#define MAX_SCOPES 40

struct stackTracker stackt;
struct stackTracker fstackt;
struct functionTracker functiont;
struct functionTracker functiont;
sizet scope = 0;
sizet scopeCounts[MAX_SCOPES];
extern bool inFunction;
extern struct bitcode* functionCode;
bool whileLoop = false;

struct stVariable
{
	ddString name;
	sizet spos;
	sizet size;
};

struct stackTracker
{
	sizet size;
	sizet top;
	struct stVariable vars[500];
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

struct bitcode* generate_bitcode_main(struct tokenNode** parseTrees, sizet _treeCount)
{
 	for (sizet i = 0; i < MAX_SCOPES; i++) scopeCounts[i] = 0;

	tokenTrees = parseTrees;
	treeCount = _treeCount;

	bitcode = make(struct bitcode, 1);
	struct bitcode* btcHead = bitcode;
	bitcodeHead = bitcode;

	generate_write_btc(BTC_GLOBAL, make_constant_ddString("_start"), REG_NONE);
	generate_write_btc(BTC_LABEL, make_constant_ddString("_start"), REG_NONE);
	generate_write_btc(BTC_PUSH, REG_RBP, REG_NONE);
	generate_write_btc(BTC_MOV, REG_RBP, REG_RSP);
	struct bitcode* bitcodeMoveStack = bitcode;
	generate_write_btc(BTC_SUB, REG_RSP, REG_NONE);

	generate_trees_asm();

	generate_write_btc(BTC_MOV, REG_EAX, make_constant_ddString("0"));
	generate_write_btc(BTC_POP, REG_RBP, REG_NONE);
	generate_write_btc(BTC_MOV, REG_RAX, make_constant_ddString("60"));
	generate_write_btc(BTC_MOV, REG_RDI, make_constant_ddString("0"));
	generate_write_btc(BTC_SYSCALL, REG_NONE, REG_NONE);

	bitcodeMoveStack->rhs = make_ddString_from_int(stackt.size);
	return bitcodeHead;
}

void generate_trees_asm(void)
{
	while ((cnode = next_tree()) != nullptr)
	{
		generate_asm_step(cnode);
	}
}
void generate_asm_step(struct tokenNode* node)
{
	if (node->value->type == TKN_SYNTAX || node->value->type == TKN_OPERATOR)
	{
		switch (node->value->value.cstr[0])
		{
			case '{':
				scope++;
				break;
			case '}':
				scope--;
				generate_write_btc(BTC_LABEL, make_format_ddString("SC%d%d", scope, scopeCounts[scope]), REG_NONE);
				scopeCounts[scope]++;
				break;
			case '=':
				if (node->value->value.cstr[1] == '=')
					generate_equality(node);
				else if (is_equals_new_assignemnt(node))
					generate_equels_make_set_asm(node);
				else
					generate_equels_set_asm(node);
				break;
			case '!':
				if (node->value->value.cstr[1] == '=')
					generate_inequality(node);
			case '@':
				break;
			case '*':
				if (node->value->value.cstr[1] == '=')
					generate_times_equals(node);
				else
					generate_1reg_operation(BTC_MUL, node);
				break;
			case '/':
				if (node->value->value.cstr[1] == '=')
					generate_divide_equals(node);
				else
					generate_1reg_operation(BTC_DIV, node);
				break;
			case '+':
				if (node->value->value.cstr[1] == '=')
					generate_plus_equals(node);
				if (node->value->value.cstr[1] == '+')
					generate_plus_plus(node);
				else
					generate_2reg_operation(BTC_ADD, node);
				break;
			case '-':
				if (node->value->value.cstr[1] == '=')
					generate_minus_equals(node);
				if (node->value->value.cstr[1] == '-')
					generate_minus_minus(node);
				else
					generate_2reg_operation(BTC_SUB, node);
				break;
			default:
				compile_error(make_format_ddString("UNDEFINED OPERATOR/SYNTAX/SYMBOL \"%s\"\n", node->value->value.cstr).cstr);
		}
	}
	else if (node->value->type == TKN_LITERAL)
	{
		struct stVariable var = stackt_get_var(node->value->value);
		if (var.size != -6969)
		{
			push_stack_var(var);
		}
		else 
		{
			generate_write_btc(BTC_PUSH, node->value->value, REG_NONE);
		}
	}
	else if (node->value->type == TKN_KEYWORD)
	{
		if (ddString_compare_cstring(node->value->value, "if"))
			generate_if_statement(node);
		else if (ddString_compare_cstring(node->value->value, "while"))
			generate_while_statement(node);
		else if (ddString_compare_cstring(node->value->value, "sub"))
			generate_sub_statement(node);
	}
	else if (node->value->type == TKN_ASSEMBLY)
	{
		node->value->value.cstr[0] = '	';
		generate_write_btc(BTC_ILA, node->value->value, REG_NONE);
	}
}

static void generate_2reg_operation(int opc, struct tokenNode* node)
{
	generate_bisplit(node);
	pop_both_sides();
	generate_write_btc(opc, REG_R9, REG_R8);
	push_result(REG_R9);
}
static void generate_1reg_operation(int opc, struct tokenNode* node)
{
	generate_bisplit(node);
	pop_input(REG_R8);
	pop_input(REG_RAX);
	generate_write_btc(opc, REG_R8, REG_NONE);
	push_result(REG_RAX);
}
static inline struct bitcode* generate_write_function_headder(ddString name)// returns the code line of the sub rsp
{
	generate_write_btc(BTC_LABEL, name, REG_NONE);
	generate_write_btc(BTC_PUSH, REG_RBP, REG_NONE);
	generate_write_btc(BTC_MOV, REG_RBP, REG_RSP);
	struct bitcode* bitcodeMoveStack = bitcode;
	generate_write_btc(BTC_SUB, REG_RSP, REG_NONE);
	return bitcodeMoveStack;
}
static inline void generate_write_function_footer(void)
{
	//push_result(REG_R8);
	pop_input(REG_RBP);
	generate_write_btc(BTC_RET, REG_NONE, REG_NONE);
}
static void generate_sub_statement(struct tokenNode* node)
{
	struct bitcode* temp = bitcode;
	struct bitcode* functionCodeHead= make(struct bitcode, 1);
	bitcode = functionCodeHead;
	struct bitcode* btcMoveStack = generate_write_function_headder(node->right->right->right->value->value);
	fstackt.size = 0;
	fstackt.top = 0;
	switch_stacks();
	while ((cnode = next_tree())->value->value.cstr[0] != '}')
		generate_asm_step(cnode);
	generate_asm_step(cnode);//also generate code for '}'
	generate_write_btc(BTC_ADD, REG_RSP, make_ddString_from_int(stackt.size));
	generate_write_function_footer();
	btcMoveStack->rhs = make_ddString_from_int(stackt.size);
	bitcodeHead->prev = bitcode->prev;
	bitcode->prev->next = bitcodeHead;
	bitcodeHead = functionCodeHead;
	bitcode = temp;
	switch_stacks();
}
static void generate_while_statement(struct tokenNode* node)
{
	generate_write_btc(BTC_LABEL, make_format_ddString("WL%d%d", scope, scopeCounts[scope]), REG_NONE);

	sizet tscope = scope;
	cnode = next_tree();
	generate_asm_step(cnode);
	while (scope != tscope)
	{
		cnode = next_tree();
		generate_asm_step(cnode);
	}
	generate_split_right(node);
	pop_input(REG_R8);
	generate_write_btc(BTC_CMP, REG_R8, make_constant_ddString("1"));
	generate_write_btc(BTC_JE, make_format_ddString("WL%d%d", scope, scopeCounts[scope]-1), REG_NONE);
}
static void generate_if_statement(struct tokenNode* node)
{
	generate_split_right(node);
	pop_input(REG_R8);
	generate_write_btc(BTC_CMP, REG_R8, make_constant_ddString("0"));
	generate_write_btc(BTC_JE, make_format_ddString("SC%d%d", scope, scopeCounts[scope]), REG_NONE);
}
static void generate_equels_set_asm(struct tokenNode* node)//i = 2*3;
{
	ddPrintf("it here\n");
	struct stVariable var = stackt_get_var(node->left->value->value);
	generate_split_right(node);
	pop_stack_var(var);
}
static void generate_equels_make_set_asm(struct tokenNode* node)//@8 i = 9-3;
{
	struct stVariable var = stackt_set_var(node->left->right->right->value->value, ddString_to_int(node->left->right->value->value));
	generate_split_right(node);
	pop_stack_var(var);
}
static inline void generate_equality(struct tokenNode* node)//2 == 3-1
{
	generate_bisplit(node);
	pop_both_sides();
	compare_sides();
	compare_equal_flag();
	push_result(REG_R8);
}
static inline void generate_inequality(struct tokenNode* node)//2 != 3
{
	generate_bisplit(node);
	pop_both_sides();
	compare_sides();
	compare_not_equal_flag();
	push_result(REG_R8);
}
static inline void generate_times_equals(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->left->value->value);
	generate_1reg_operation(BTC_MUL, node);
	pop_stack_var(var);
}
static inline void generate_divide_equals(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->left->value->value);
	generate_1reg_operation(BTC_DIV, node);
	pop_stack_var(var);
}
static inline void generate_plus_equals(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->left->value->value);
	generate_2reg_operation(BTC_ADD, node);
	pop_stack_var(var);
}
static inline void generate_minus_equals(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->left->value->value);
	generate_2reg_operation(BTC_SUB, node);
	pop_stack_var(var);
}
static inline void generate_plus_plus(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->left->value->value);
	generate_split_left(node);
	pop_input(REG_R8);
	generate_write_btc(BTC_INC, REG_R8, REG_NONE);
	push_result(REG_R8);
	pop_stack_var(var);
}
static inline void generate_minus_minus(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->left->value->value);
	generate_split_left(node);
	pop_input(REG_R8);
	generate_write_btc(BTC_DEC, REG_R8, REG_NONE);
	push_result(REG_R8);
	pop_stack_var(var);
}
static inline void generate_bisplit(struct tokenNode* node)
{
	generate_asm_step(node->left);
	generate_asm_step(node->right);
}
static inline void generate_split_right(struct tokenNode* node)
{
	generate_asm_step(node->right);
}
static inline void generate_split_left(struct tokenNode* node)
{
	generate_asm_step(node->left);
}
static inline void pop_both_sides(void)
{
	generate_write_btc(BTC_POP, REG_R8, REG_NONE);
	generate_write_btc(BTC_POP, REG_R9, REG_NONE);
}
static inline void pop_input(ddString reg)
{
	generate_write_btc(BTC_POP, reg, REG_NONE);
}
static inline void compare_sides(void)
{
	generate_write_btc(BTC_CMP, REG_R9, REG_R8);
}
static inline void compare_equal_flag(void)
{
	generate_write_btc(BTC_SETE, REG_AL, REG_NONE);
	generate_write_btc(BTC_MOVZX, REG_R8, REG_AL);
}
static inline void compare_not_equal_flag(void)
{
	generate_write_btc(BTC_SETNE, REG_AL, REG_NONE);
	generate_write_btc(BTC_MOVZX, REG_R8, REG_AL);
}
static inline void push_result(ddString reg)
{
	generate_write_btc(BTC_PUSH, reg, REG_NONE);
}
static inline bool is_equals_new_assignemnt(struct tokenNode* node)
{
	if (node->left != nullptr && node->left->value->value.cstr[0] == '@')
		return true;
	return false;
}
static inline struct tokenNode* next_tree(void)
{
	return (treePosition < treeCount) ? tokenTrees[treePosition++] : (struct tokenNode*)nullptr;
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
static void pop_stack_var(struct stVariable var)
{
	switch (var.size)
	{
		case 1:
		{
			pop_input(REG_RAX);
			generate_write_btc(BTC_MOV, make_format_ddString("byte[rbp-%d]", var.spos), REG_AL);
			break;
		}
		case 2:
		{
			pop_input(REG_RAX);
			generate_write_btc(BTC_MOV, make_format_ddString("word[rbp-%d]", var.spos), REG_AX);
			break;
		}
		case 4:
		{
			pop_input(REG_RAX);
			generate_write_btc(BTC_MOV, make_format_ddString("dword[rbp-%d]", var.spos), REG_EAX);
			break;
		}
		case 8:
		{
			generate_write_btc(BTC_POP, make_format_ddString("qword[rbp-%d]", var.spos), REG_NONE);
			break;
		}
		default:
			compile_error("HOW THE FUCK DID THIS HAPPEN (POP)\n");
	}
}
static void push_stack_var(struct stVariable var)
{
	switch (var.size)
	{
		case 1:
		{
			generate_write_btc(BTC_MOVSX, REG_RAX, make_format_ddString("byte[rbp-%d]", var.spos));
			push_result(REG_RAX);
			break;
		}
		case 2:
		{
			generate_write_btc(BTC_MOVSX, REG_RAX, make_format_ddString("word[rbp-%d]", var.spos));
			push_result(REG_RAX);
			break;
		}
		case 4:
		{
			generate_write_btc(BTC_MOV, REG_EAX, make_format_ddString("dword[rbp-%d]", var.spos));
			push_result(REG_RAX);
			break;
		}
		case 8:
		{
			generate_write_btc(BTC_PUSH, make_format_ddString("qword[rbp-%d]", var.spos), REG_NONE);
			break;
		}
		default:
			compile_error("HOW THE FUCK DID THIS HAPPEN (PUSH)\n");
	}
}
struct stVariable stackt_get_var(ddString name)
{
	name.cstr[name.length] = '\0';
	for (sizet i = 0; i < stackt.top; i++)
	{
		stackt.vars[i].name.cstr[stackt.vars[i].name.length] = '\0';
		if (ddString_compare(name, stackt.vars[i].name))
		{
			return stackt.vars[i];
		}
	}
	struct stVariable out;
	out.size = -6969;
	return out;
}
struct stVariable stackt_set_var(ddString name, sizet size)
{
	stackt.vars[stackt.top].name = name;
	stackt.vars[stackt.top].size = size;
	stackt.top++;
	stackt.size += size;
	stackt.vars[stackt.top-1].spos = stackt.size;
	return stackt.vars[stackt.top-1];
}
static inline void switch_stacks(void)
{
	struct stackTracker temp;
	temp = stackt;
	stackt = fstackt;
	fstackt = temp;
}
#endif

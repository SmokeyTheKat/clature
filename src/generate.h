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
#define BTC_TAG		0x20
#define BTC_XOR		0x21
#define BTC_EXTERN	0x22

struct dtVariable;
struct dataTracker;
struct stVariable;
struct stackTracker;
struct function;
struct functionTracker;
struct bitcode;

struct bitcode* generate_bitcode_main(struct tokenNode** parseTrees, sizet _treeCount);
static inline struct tokenNode* next_tree(void);
void generate_write_btc(int opc, ddString lhs, ddString rhs);
static inline bool is_equals_new_assignemnt(struct tokenNode* node);
static inline bool is_dereference(struct tokenNode* node);
static inline bool is_dereference_assignment(struct tokenNode* node);
static inline bool is_array_def(struct tokenNode* node);
static void push_stack_var(struct stVariable var);
static void pop_stack_var(struct stVariable var);
static inline void push_result(ddString reg);
static inline void compare_equal_flag(void);
static inline void compare_lessthan_equal_flag(void);
static inline void compare_lessthan_flag(void);
static inline void compare_greaterthan_flag(void);
static inline void compare_greaterthan_equal_flag(void);
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
static inline void generate_lessthan_equals(struct tokenNode* node);
static inline void generate_lessthan(struct tokenNode* node);
static inline void generate_greaterthan_equals(struct tokenNode* node);
static inline void generate_greaterthan(struct tokenNode* node);
static inline void generate_mod_operator(struct tokenNode* node);
static inline void generate_logic_and(struct tokenNode* node);
static inline void generate_logic_or(struct tokenNode* node);
static inline struct bitcode* generate_write_function_headder(ddString name);// returns the code line of the sub rsp
static inline void generate_write_function_footer(void);
static void generate_if_statement(struct tokenNode* node);
static void generate_while_statement(struct tokenNode* node);
static void generate_continue(struct tokenNode* node);
static void generate_sub_statement(struct tokenNode* node);
static void generate_equels_make_set_asm(struct tokenNode* node);//@8 i = 9-3;
static void generate_equels_set_asm(struct tokenNode* node);//i = 2*3;
static void generate_1reg_operation(int opc, struct tokenNode* node);
static void generate_2reg_operation(int opc, struct tokenNode* node);
static void generate_function_call(struct tokenNode* node);
static void generate_function_return(struct tokenNode* node);
static void generate_dereference(struct tokenNode* node);
static void generate_array_def(struct tokenNode* node);
static void generate_reference(struct tokenNode* node);
static void generate_set_dereference(struct tokenNode* node);
static void define_variable(struct tokenNode* node);
static void generate_malloc(struct tokenNode* node);
sizet get_param_count(struct tokenNode* node);
void generate_asm_step(struct tokenNode* node);
void generate_trees_asm(void);
struct stVariable stackt_get_var(ddString name);
struct stVariable stackt_set_var(ddString name, sizet size);
void stackt_set_param_var(ddString name, sizet pos);
static inline void switch_stacks(void);
struct dtVariable datat_add_string(ddString value);
struct dtVariable datat_add_data(ddString name, ddString value);
void push_data_var(struct dtVariable var);
static void push_ref(ddString value, sizet size);
static void pop_ref(ddString value, sizet size);

static sizet treePosition = 0;
static sizet treeCount = 0;
static struct tokenNode** tokenTrees;
static struct tokenNode* cnode;
static struct bitcode* bitcode;
static struct bitcode* bitcodeHead;

#define MAX_SCOPES 40

struct stackTracker stackt;
struct stackTracker fstackt;
struct dataTracker datat;
sizet stringCount = 0;
struct functionTracker functiont;
struct functionTracker functiont;
sizet scope = 0;
sizet optrCount = 0;
sizet scopeCounts[MAX_SCOPES];
sizet scopeStackPos = 0;
sizet scopeStack[MAX_SCOPES];
extern bool inFunction;
extern struct bitcode* functionCode;
bool whileLoop = false;
const char* DATA_SIZES[9] = {
	"ERROR_SIZE",
	"BYTE",
	"WORD",
	"ERROR_SIZE",
	"DWORD",
	"ERROR_SIZE",
	"ERROR_SIZE",
	"ERROR_SIZE",
	"QWORD"
};

struct dtVariable
{
	ddString name;
	ddString data;
};
struct dataTracker
{
	sizet top;
	struct dtVariable data[500];
};
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
	struct stVariable vars[500]; };
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
 	for (sizet i = 0; i < MAX_SCOPES; i++) scopeStack[i] = 0;
	tokenTrees = parseTrees;
	treeCount = _treeCount;
	bitcode = make(struct bitcode, 1);
	struct bitcode* btcHead = bitcode;
	bitcodeHead = bitcode;
	generate_write_btc(BTC_EXTERN, make_constant_ddString("malloc"), REG_NONE);
	generate_write_btc(BTC_EXTERN, make_constant_ddString("free"), REG_NONE);
	generate_write_btc(BTC_GLOBAL, make_constant_ddString("main"), REG_NONE);
	generate_write_btc(BTC_LABEL, make_constant_ddString("main"), REG_NONE);
	generate_write_btc(BTC_PUSH, REG_RBP, REG_NONE);
	generate_write_btc(BTC_MOV, REG_RBP, REG_RSP);
	struct bitcode* bitcodeMoveStack = bitcode;
	generate_write_btc(BTC_SUB, REG_RSP, REG_NONE);
	generate_write_btc(BTC_MOV, make_constant_ddString("QWORD[RBP-8]"), REG_RSI);
	generate_write_btc(BTC_MOV, make_constant_ddString("DWORD[RBP-12]"), REG_EDI);
	stackt_set_var(make_ddString("argv"), 8);
	stackt_set_var(make_ddString("argc"), 4);
	generate_trees_asm();
	generate_write_btc(BTC_MOV, REG_EAX, make_constant_ddString("0"));
	generate_write_btc(BTC_POP, REG_RBP, REG_NONE);
	generate_write_btc(BTC_MOV, REG_RAX, make_constant_ddString("60"));
	generate_write_btc(BTC_MOV, REG_RDI, make_constant_ddString("0"));
	generate_write_btc(BTC_SYSCALL, REG_NONE, REG_NONE);
	bitcodeMoveStack->rhs = make_ddString_from_int(stackt.size);
	
	struct bitcode* temp = bitcode;
	struct bitcode* datacode = make(struct bitcode, 1);
	bitcode = datacode;
	generate_write_btc(BTC_ILA, make_constant_ddString("section .data"), REG_NONE);
	for (sizet i = 0; i < datat.top; i++)
	{
		generate_write_btc(BTC_LABEL, datat.data[i].name, REG_NONE);
		generate_write_btc(BTC_TAG, make_constant_ddString("db"), datat.data[i].data);
	}
	bitcodeHead->prev = bitcode->prev;
	bitcode->prev->next = bitcodeHead;
	bitcodeHead = datacode;
	bitcode = temp;
	
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
			case ',': break;
			case '[':
				break;
			case '{':
				scopeStack[scopeStackPos++] = stackt.top;
				scope++;
				break;
			case '}':
				stackt.top = scopeStack[--scopeStackPos];
				scope--;
				generate_write_btc(BTC_LABEL, make_format_ddString(".SC%d%d", scope, scopeCounts[scope]), REG_NONE);
				scopeCounts[scope]++;
				break;
			case '=':
				if (node->value->value.cstr[1] == '=')
					generate_equality(node);
				else if (is_dereference_assignment(node))
					generate_set_dereference(node);
				else if (is_equals_new_assignemnt(node))
					generate_equels_make_set_asm(node);
				else
					generate_equels_set_asm(node);
				break;
			case '!':
				if (node->value->value.cstr[1] == '=')
					generate_inequality(node);
				break;
			case '@':
				if (is_dereference(node))
					generate_dereference(node);
				if (is_array_def(node))
					generate_array_def(node);
				else
					define_variable(node);
				break;
			case '?':
				generate_reference(node);
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
				else if (node->value->value.cstr[1] == '+')
					generate_plus_plus(node);
				else
					generate_2reg_operation(BTC_ADD, node);
				break;
			case '-':
				if (node->value->value.cstr[1] == '=')
					generate_minus_equals(node);
				else if (node->value->value.cstr[1] == '-')
					generate_minus_minus(node);
				else
					generate_2reg_operation(BTC_SUB, node);
				break;
			case '<':
				if (node->value->value.cstr[1] == '=')
					generate_lessthan_equals(node);
				else if (node->value->value.cstr[1] == '<')
					;//generate_lessthan_lessthan(node);
				else
					generate_lessthan(node);
				break;
			case '>':
				if (node->value->value.cstr[1] == '=')
					generate_greaterthan_equals(node);
				else if (node->value->value.cstr[1] == '<')
					;//generate_greaterthan_lessthan(node);
				else
					generate_greaterthan(node);
				break;
			case '%':
				if (node->value->value.cstr[1] == '=')
					;//generate_mod_equals(node);
				else
					generate_mod_operator(node);
				break;
			case '&':
				if (node->value->value.cstr[1] == '&')
					generate_logic_and(node);
				break;
			case '|':
				if (node->value->value.cstr[1] == '|')
					generate_logic_or(node);
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
	else if (node->value->type == TKN_STRING)
	{
		push_data_var(datat_add_string(node->value->value));
	}
	else if (node->value->type == TKN_KEYWORD)
	{
		if (ddString_compare_cstring(node->value->value, "if"))
			generate_if_statement(node);
		else if (ddString_compare_cstring(node->value->value, "while"))
			generate_while_statement(node);
		else if (ddString_compare_cstring(node->value->value, "sub"))
			generate_sub_statement(node);
		else if (ddString_compare_cstring(node->value->value, "iso"))
			generate_function_call(node->right);
		else if (ddString_compare_cstring(node->value->value, "return"))
			generate_function_return(node);
		else if (ddString_compare_cstring(node->value->value, "continue"))
			generate_continue(node);
		else if (ddString_compare_cstring(node->value->value, "malloc"))
			generate_malloc(node);
	}
	else if (node->value->type == TKN_FUNCTION)
	{
		generate_function_call(node);
		push_result(REG_R8);
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
static void define_variable(struct tokenNode* node)
{
	stackt_set_var(node->right->right->value->value, ddString_to_int(node->right->value->value));
}
static void generate_malloc(struct tokenNode* node)
{
	sizet typesize = ddString_to_int(node->right->right->value->value);
	generate_split_right(node->right->right->right);
	generate_write_btc(BTC_POP, REG_RAX, REG_NONE);
	generate_write_btc(BTC_MOV, REG_R8, make_ddString_from_int(typesize));
	generate_write_btc(BTC_MUL, REG_R8, REG_NONE);
	generate_write_btc(BTC_MOV, REG_RAX, REG_R8);
	generate_write_btc(BTC_MOV, REG_EDI, REG_EAX);
	generate_write_btc(BTC_CALL, make_constant_ddString("malloc"), REG_NONE);
	generate_write_btc(BTC_PUSH, REG_RAX, REG_NONE);
}
static void generate_set_dereference(struct tokenNode* node)
{
	generate_split_right(node);
	generate_split_right(node->left->right->right);
	pop_input(REG_R8);
	pop_ref(REG_R8, ddString_to_int(node->left->right->value->value));
}
static void generate_reference(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->right->value->value);
	generate_write_btc(BTC_MOV, REG_R8, REG_RBP);
	generate_write_btc(BTC_SUB, REG_R8, make_ddString_from_int(var.spos));
	push_result(REG_R8);
}
static void generate_array_def(struct tokenNode* node)
{
	sizet typesize = ddString_to_int(node->right->value->value);
	sizet length = ddString_to_int(node->right->right->right->value->value);
	sizet size = typesize*length;
	stackt.size += size;
	generate_write_btc(BTC_MOV, REG_R8, REG_RBP);
	generate_write_btc(BTC_SUB, REG_R8, make_ddString_from_int(stackt.size));
	push_result(REG_R8);
}
static void generate_dereference(struct tokenNode* node)
{
	generate_split_right(node->right->right);
	pop_input(REG_R8);
	push_ref(make_constant_ddString("r8"), ddString_to_int(node->right->value->value));
}
static void generate_function_return(struct tokenNode* node)
{
	generate_split_right(node);
	pop_input(REG_R8);
	generate_write_btc(BTC_JMP, make_format_ddString(".SC%d%d", 0, scopeCounts[0]), REG_NONE);
}
static void generate_function_call(struct tokenNode* node)
{
	struct tokenNode* nameNode = node;
	if (node->right == nullptr) compile_error("BROKEN FUNCTION CALL\n");
	node = node->right;
	while (node->value->value.cstr[0] != ')' && node->value->value.cstr[0] != ';')
	{
		if (node->value->value.cstr[0] != '(') generate_asm_step(node);
		if (node->right != nullptr) node = node->right;
		else break;
	}
	generate_write_btc(BTC_CALL, nameNode->value->value, REG_NONE);
	generate_write_btc(BTC_ADD, REG_RSP, make_ddString_from_int((get_param_count(nameNode)) * 8));
	return;
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
	sizet paramCount = get_param_count(node->right->right->right->right);
	struct tokenNode* pnode = node->right->right->right->right->right;
	for (sizet i = paramCount-1; i >= 0; i--)
	{
		stackt_set_param_var(pnode->right->right->value->value, i);
		pnode = pnode->right->right->right->right;
	}
	sizet tscope = scope;
	cnode = next_tree();
	generate_asm_step(cnode);
	while (scope != tscope)
	{
		cnode = next_tree();
		generate_asm_step(cnode);
	}
/*
	while ((cnode = next_tree())->value->value.cstr[0] != '}')
		generate_asm_step(cnode);
	generate_asm_step(cnode);//also generate code for '}'
*/
	generate_write_btc(BTC_ADD, REG_RSP, make_ddString_from_int(stackt.size));
	generate_write_function_footer();
	btcMoveStack->rhs = make_ddString_from_int(stackt.size);
	bitcodeHead->prev = bitcode->prev;
	bitcode->prev->next = bitcodeHead;
	bitcodeHead = functionCodeHead;
	bitcode = temp;
	switch_stacks();
}
static void generate_continue(struct tokenNode* node)
{
	generate_write_btc(BTC_JMP, make_format_ddString(".SC%d%d", scope-1, scopeCounts[scope]), REG_NONE);
}
static void generate_while_statement(struct tokenNode* node)
{
	generate_write_btc(BTC_LABEL, make_format_ddString(".WL%d%d", scope, scopeCounts[scope]), REG_NONE);
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
	generate_write_btc(BTC_JE, make_format_ddString(".WL%d%d", scope, scopeCounts[scope]-1), REG_NONE);
}
static void generate_if_statement(struct tokenNode* node)
{
	generate_split_right(node);
	pop_input(REG_R8);
	generate_write_btc(BTC_CMP, REG_R8, make_constant_ddString("0"));
	generate_write_btc(BTC_JE, make_format_ddString(".SC%d%d", scope, scopeCounts[scope]), REG_NONE);
}
static void generate_equels_set_asm(struct tokenNode* node)//i = 2*3;
{
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
static inline void generate_lessthan_equals(struct tokenNode* node)
{
	generate_bisplit(node);
	pop_both_sides();
	compare_sides();
	compare_lessthan_equal_flag();
	push_result(REG_R8);
}
static inline void generate_lessthan(struct tokenNode* node)
{
	generate_bisplit(node);
	pop_both_sides();
	compare_sides();
	compare_lessthan_flag();
	push_result(REG_R8);
}
static inline void generate_greaterthan_equals(struct tokenNode* node)
{
	generate_bisplit(node);
	pop_both_sides();
	compare_sides();
	compare_greaterthan_equal_flag();
	push_result(REG_R8);
}
static inline void generate_greaterthan(struct tokenNode* node)
{
	generate_bisplit(node);
	pop_both_sides();
	compare_sides();
	compare_greaterthan_flag();
	push_result(REG_R8);
}
static inline void generate_mod_operator(struct tokenNode* node)
{
	generate_bisplit(node);
	generate_write_btc(BTC_XOR, REG_RAX, REG_RAX);
	generate_write_btc(BTC_XOR, REG_RDX, REG_RDX);
	pop_input(REG_R8);
	pop_input(REG_RAX);
	generate_write_btc(BTC_DIV, REG_R8, REG_NONE);
	generate_write_btc(BTC_MOV, REG_R8, REG_RDX);
	generate_write_btc(BTC_XOR, REG_RAX, REG_RAX);
	generate_write_btc(BTC_XOR, REG_RDX, REG_RDX);
	push_result(REG_R8);
}
static inline void generate_logic_and(struct tokenNode* node)
{
	generate_bisplit(node);
	pop_both_sides();
	generate_write_btc(BTC_CMP, REG_R8, make_constant_ddString("0"));
	generate_write_btc(BTC_JE, make_format_ddString(".OP%d", optrCount), REG_NONE);
	generate_write_btc(BTC_CMP, REG_R9, make_constant_ddString("0"));
	generate_write_btc(BTC_JE, make_format_ddString(".OP%d", optrCount), REG_NONE);
	generate_write_btc(BTC_PUSH, make_constant_ddString("1"), REG_NONE);
	generate_write_btc(BTC_JMP, make_format_ddString(".OP%d", optrCount+1), REG_NONE);
	generate_write_btc(BTC_LABEL, make_format_ddString(".OP%d", optrCount++), REG_NONE);
	generate_write_btc(BTC_PUSH, make_constant_ddString("0"), REG_NONE);
	generate_write_btc(BTC_LABEL, make_format_ddString(".OP%d", optrCount++), REG_NONE);
}
static inline void generate_logic_or(struct tokenNode* node)
{
	generate_bisplit(node);
	pop_both_sides();
	generate_write_btc(BTC_CMP, REG_R8, make_constant_ddString("0"));
	generate_write_btc(BTC_JNE, make_format_ddString(".OP%d", optrCount), REG_NONE);
	generate_write_btc(BTC_CMP, REG_R9, make_constant_ddString("0"));
	generate_write_btc(BTC_JE, make_format_ddString(".OP%d", optrCount+1), REG_NONE);
	generate_write_btc(BTC_LABEL, make_format_ddString(".OP%d", optrCount++), REG_NONE);
	generate_write_btc(BTC_PUSH, make_constant_ddString("1"), REG_NONE);
	generate_write_btc(BTC_JMP, make_format_ddString(".OP%d", optrCount+1), REG_NONE);
	generate_write_btc(BTC_LABEL, make_format_ddString(".OP%d", optrCount++), REG_NONE);
	generate_write_btc(BTC_PUSH, make_constant_ddString("0"), REG_NONE);
	generate_write_btc(BTC_LABEL, make_format_ddString(".OP%d", optrCount++), REG_NONE);
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
static inline void compare_lessthan_equal_flag(void)
{
	generate_write_btc(BTC_SETLE, REG_AL, REG_NONE);
	generate_write_btc(BTC_MOVZX, REG_R8, REG_AL);
}
static inline void compare_lessthan_flag(void)
{
	generate_write_btc(BTC_SETL, REG_AL, REG_NONE);
	generate_write_btc(BTC_MOVZX, REG_R8, REG_AL);
}
static inline void compare_greaterthan_flag(void)
{
	generate_write_btc(BTC_SETG, REG_AL, REG_NONE);
	generate_write_btc(BTC_MOVZX, REG_R8, REG_AL);
}
static inline void compare_greaterthan_equal_flag(void)
{
	generate_write_btc(BTC_SETGE, REG_AL, REG_NONE);
	generate_write_btc(BTC_MOVZX, REG_R8, REG_AL);
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
			generate_write_btc(BTC_MOV, make_format_ddString("BYTE[RBP-%d]", var.spos), REG_AL);
			break;
		}
		case 2:
		{
			pop_input(REG_RAX);
			generate_write_btc(BTC_MOV, make_format_ddString("WORD[RBP-%d]", var.spos), REG_AX);
			break;
		}
		case 4:
		{
			pop_input(REG_RAX);
			generate_write_btc(BTC_MOV, make_format_ddString("DWORD[RBP-%d]", var.spos), REG_EAX);
			break;
		}
		case 8:
		{
			generate_write_btc(BTC_POP, make_format_ddString("QWORD[RBP-%d]", var.spos), REG_NONE);
			break;
		}
		default:
			compile_error("UNDEFINED VARIABLE (POP)\n");
	}
}
static void pop_ref(ddString value, sizet size)
{
	switch (size)
	{
		case 1:
		{
			pop_input(REG_RAX);
			generate_write_btc(BTC_MOV, make_format_ddString("BYTE[%s]", value.cstr), REG_AL);
			break;
		}
		case 2:
		{
			pop_input(REG_RAX);
			generate_write_btc(BTC_MOV, make_format_ddString("WORD[%s]", value.cstr), REG_AX);
			break;
		}
		case 4:
		{
			pop_input(REG_RAX);
			generate_write_btc(BTC_MOV, make_format_ddString("DWORD[%s]", value.cstr), REG_EAX);
			break;
		}
		case 8:
		{
			generate_write_btc(BTC_POP, make_format_ddString("QWORD[%s]", value.cstr), REG_NONE);
			break;
		}
		default:
			compile_error("HOW THE FUCK DID THIS HAPPEN (POP)\n");
	}
}
static void push_ref(ddString value, sizet size)
{
	switch (size)
	{
		case 1:
		{
			generate_write_btc(BTC_MOVSX, REG_RAX, make_format_ddString("BYTE[%s]", value.cstr));
			push_result(REG_RAX);
			break;
		}
		case 2:
		{
			generate_write_btc(BTC_MOVSX, REG_RAX, make_format_ddString("WORD[%s]", value.cstr));
			push_result(REG_RAX);
			break;
		}
		case 4:
		{
			generate_write_btc(BTC_MOV, REG_EAX, make_format_ddString("DWORD[%s]", value.cstr));
			push_result(REG_RAX);
			break;
		}
		case 8:
		{
			generate_write_btc(BTC_PUSH, make_format_ddString("QWORD[%s]", value.cstr), REG_NONE);
			break;
		}
		default:
			compile_error("HOW THE FUCK DID THIS HAPPEN (PUSH)\n");
	}
}
static void push_stack_var(struct stVariable var)
{
	switch (var.size)
	{
		case 1:
		{
			generate_write_btc(BTC_MOVSX, REG_RAX, make_format_ddString("BYTE[RBP-%d]", var.spos));
			push_result(REG_RAX);
			break;
		}
		case 2:
		{
			generate_write_btc(BTC_MOVSX, REG_RAX, make_format_ddString("WORD[RBP-%d]", var.spos));
			push_result(REG_RAX);
			break;
		}
		case 4:
		{
			generate_write_btc(BTC_MOV, REG_EAX, make_format_ddString("DWORD[RBP-%d]", var.spos));
			push_result(REG_RAX);
			break;
		}
		case 8:
		{
			generate_write_btc(BTC_PUSH, make_format_ddString("QWORD[RBP-%d]", var.spos), REG_NONE);
			break;
		}
		default:
			compile_error("UNDEFINED VARIABLE (PUSH)\n");
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
void stackt_set_param_var(ddString name, sizet pos)
{
	stackt.vars[stackt.top].name = name;
	stackt.vars[stackt.top].size = 8;
	stackt.top++;
	stackt.vars[stackt.top-1].spos = -(16 + (pos*8));
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
sizet get_param_count(struct tokenNode* node)
{
	if (node->right->value->value.cstr[0] == '(' && node->right->right->value->value.cstr[0] == ')') return 0;
	sizet output = 0;
	while (node != nullptr && node->value->value.cstr[0] != ')' && node->value->value.cstr[0] != ';')
	{
		if (node->value->value.cstr[0] == ',') output++;
		node = node->right;
	}
	return output+1;
}
struct dtVariable datat_add_data(ddString name, ddString value)
{
	datat.data[datat.top].name = name;
	datat.data[datat.top].data = value;
	return datat.data[datat.top++];
}
struct dtVariable datat_add_string(ddString value)
{
	datat.data[datat.top].data = value;
	ddString_push_cstring_back(&(datat.data[datat.top].data), ",0");
	datat.data[datat.top].data.cstr[datat.data[datat.top].data.length] = '\0';
	datat.data[datat.top].name = make_format_ddString("_69_str%d", stringCount++);
	return datat.data[datat.top++];
}
void push_data_var(struct dtVariable var)
{
	generate_write_btc(BTC_PUSH, var.name, REG_NONE);
}
static inline bool is_array_def(struct tokenNode* node)
{
	if (node->right != nullptr && node->right->right != nullptr && node->right->right->right != nullptr && node->right->right->value->value.cstr[0] == '<')
		return true;
	return false;
}
static inline bool is_dereference(struct tokenNode* node)
{
	if (node->right != nullptr && node->right->right != nullptr && node->right->right->right != nullptr && node->right->right->value->value.cstr[0] == '[')
		return true;
	return false;
}
static inline bool is_dereference_assignment(struct tokenNode* node)
{
	if (node->left != nullptr && node->left->right != nullptr && node->left->right->right != nullptr && node->left->right->right->value->value.cstr[0] == '[')
		return true;
	return false;
}
#endif

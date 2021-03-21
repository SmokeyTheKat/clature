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

struct dtVariable;
struct dataTracker;
struct stVariable;
struct stackTracker;
struct function;
struct functionTracker;
struct bitcode;

struct bitcode* generate_bitcode_main(struct tokenNode** parseTrees, sizet _treeCount);
static inline struct tokenNode* next_tree(void);
static inline struct tokenNode* peek_tree(void);
void generate_write_btc(int opc, ddString lhs, ddString rhs);
static inline bool is_equals_new_assignemnt(struct tokenNode* node);
static inline bool is_dereference(struct tokenNode* node);
static inline bool is_dereference_assignment(struct tokenNode* node);
static inline bool is_array_def(struct tokenNode* node);
static inline bool is_array_def_var(struct tokenNode* node);
static inline bool is_global(struct tokenNode* node);
static bool is_size_operatable(int size);
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
static inline void generate_split(struct tokenNode* node, int v);
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
static void generate_for_statement(struct tokenNode* node);
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
static void generate_array_def_var(struct tokenNode* node);
static void generate_reference(struct tokenNode* node);
static void generate_set_dereference(struct tokenNode* node);
static void generate_multiply(struct tokenNode* node);
static void generate_divide(struct tokenNode* node);
static void generate_plus(struct tokenNode* node);
static void generate_minus(struct tokenNode* node);
static void define_variable(struct tokenNode* node);
static void generate_malloc(struct tokenNode* node);
static void generate_extern(struct tokenNode* node);
static void generate_format(struct tokenNode* node);
static void generate_global(struct tokenNode* node);
sizet get_param_count(struct tokenNode* node);
void generate_asm_step(struct tokenNode* node);
void generate_trees_asm(void);
struct stVariable stackt_get_var(ddString name);
struct stVariable stackt_set_var(ddString name, sizet size);
void stackt_set_param_var(ddString name, int size, sizet pos);
static inline void switch_stacks(void);
struct dtVariable datat_add_string(ddString value);
struct dtVariable datat_add_data(ddString name, ddString value, sizet size);
struct dtVariable datat_get_data(ddString name);
void push_data_var(struct dtVariable var);
static void push_ref(ddString value, sizet size);
static void pop_ref(ddString value, sizet size);
static void pop_nsize(ddString value, sizet size);
static void push_nsize(ddString value, sizet size);
static void push_param_nsize(struct stVariable var);

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
bool statementIsEquality = false;
bool addSize = false;
sizet addSizeVal = 0;
const char* DEFINE_SIZES[9] = {
	"ERROR_SIZE",
	"DB",
	"DW",
	"ERROR_SIZE",
	"DD",
	"ERROR_SIZE",
	"ERROR_SIZE",
	"ERROR_SIZE",
	"DQ"
};
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
	ddString defineSize;
	sizet size;
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
	int size;
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
	bitcodeHead = bitcode;
	generate_write_btc(BTC_GLOBAL, make_constant_ddString("main"), REG_NONE);
	generate_write_btc(BTC_LABEL, make_constant_ddString("main"), REG_NONE);
	generate_write_btc(BTC_EXTERN, make_constant_ddString("malloc"), REG_NONE);
	generate_write_btc(BTC_EXTERN, make_constant_ddString("free"), REG_NONE);
	generate_write_btc(BTC_PUSH, REG_RBP, REG_NONE);
	generate_write_btc(BTC_MOV, REG_RBP, REG_RSP);
	struct bitcode* bitcodeMoveStack = bitcode;
	generate_write_btc(BTC_SUB, REG_RSP, REG_NONE);

	datat_add_data(make_constant_ddString("argv"), make_constant_ddString("0"), 8);
	datat_add_data(make_constant_ddString("argc"), make_constant_ddString("0"), 4);
	generate_write_btc(BTC_MOV, make_constant_ddString("QWORD[argv]"), REG_RSI);
	generate_write_btc(BTC_MOV, make_constant_ddString("DWORD[argc]"), REG_EDI);

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
		generate_write_btc(BTC_TAG, datat.data[i].defineSize, datat.data[i].data);
	}
	generate_write_btc(BTC_ILA, make_constant_ddString("section .text"), REG_NONE);
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
struct tokenNode* node_zip(struct tokenNode* node)
{
	while (node->nodes[0]->nodeCount == 1) node = node->nodes[0];
	return node;
}
void generate_asm_step(struct tokenNode* node)
{
	// lr operation   3*2
	if (node->nodeCount == 1 && node->nodes[0]->value->type == TKN_ASSEMBLY)
	{
		node->nodes[0]->value->value.cstr[0] = '	';
		generate_write_btc(BTC_ILA, node->nodes[0]->value->value, REG_NONE);
	}
	else if (node->nodeCount == 1 && node->nodes[0]->value->type == TKN_LITERAL)
	{
		//generate_write_btc(BTC_ILA, node->nodes[0]->value->value, REG_NONE);
		ddPrint_cstring(node->nodes[0]->value->value.cstr);
		ddPrint_nl();
	}
	else if (node->nodeCount > 1)
	{
		switch (node->nodes[1]->value->value.cstr[0])
		{
			case ',': break;
			case '[':
				break;
			case '?':
				generate_reference(node);
				break;
			case '+':
				if (node->nodes[1]->value->value.cstr[1] == '=')
					generate_plus_equals(node);
				else
					generate_plus(node);
				break;
			case '-':
				if (node->nodes[1]->value->value.cstr[1] == '=')
					generate_minus_equals(node);
				else if (node->nodes[1]->value->value.cstr[1] == '-')
					generate_minus_minus(node);
				else
					generate_minus(node);
				break;
			case '*':
				if (node->nodes[1]->value->value.cstr[1] == '=')
					generate_times_equals(node);
				else
					generate_multiply(node);
				break;
			case '%':
				if (node->nodes[1]->value->value.cstr[1] == '=')
					;//generate_mod_equals(node);
				else
					generate_mod_operator(node);
				break;
			case '/':
				if (node->nodes[1]->value->value.cstr[1] == '=')
					generate_divide_equals(node);
				else
					generate_divide(node);
				break;
///////////////////////////////////////////////////
			case '<':
				if (node->nodes[1]->value->value.cstr[1] == '=')
					generate_lessthan_equals(node);
				else if (node->nodes[1]->value->value.cstr[1] == '<')
					;//generate_lessthan_lessthan(node);
				else
					generate_lessthan(node);
				break;
			case '>':
				if (is_array_def_var(node))
					generate_array_def_var(node);
				else if (node->nodes[1]->value->value.cstr[1] == '=')
					generate_greaterthan_equals(node);
				else if (node->nodes[1]->value->value.cstr[1] == '<')
					;//generate_greaterthan_lessthan(node);
				else
					generate_greaterthan(node);
				break;
			case '!':
				if (node->nodes[1]->value->value.cstr[1] == '=')
					generate_inequality(node);
				break;
			case '&':
				if (node->nodes[1]->value->value.cstr[1] == '&')
					generate_logic_and(node);
				break;
			case '|':
				if (node->nodes[1]->value->value.cstr[1] == '|')
					generate_logic_or(node);
				break;
///////////////////////////////////////////////////
			case '=':
				if (node->nodes[1]->value->value.cstr[1] == '=')
					generate_equality(node);
				else if (is_dereference_assignment(node))
					generate_set_dereference(node);
				else if (is_equals_new_assignemnt(node))
					generate_equels_make_set_asm(node);
				else
					generate_equels_set_asm(node);
				break;
			default:
				switch (node->nodes[0]->value->value.cstr[0])
				{
					case ']':
						if (is_dereference(node))
							generate_dereference(node);
						//else
						//	define_variable(node);
						break;
					case '>':
						if (is_array_def(node))
							generate_array_def(node);
						break;
					case '+':
						if (node->nodes[0]->value->value.cstr[1] == '+')
							generate_plus_plus(node);
						break;
					case '-':
						if (node->nodes[0]->value->value.cstr[1] == '-')
							generate_minus_minus(node);
						break;
					default:
						if (node->nodes[1]->value->symbol == G_KW_IF)
						{
							generate_if_statement(node);
						}
						else if (node->value->type == TKN_STRING)
						{
							push_data_var(datat_add_string(node->value->value));
						}
						break;
				}
				if (node->nodeCount == 3 && node->nodes[2]->value->symbol == G_AT)
				{
					define_variable(node);
				}
				else if (node->nodeCount == 2 && node->nodes[0]->value->symbol == G_FP && node->nodes[1]->value->symbol == G_I)
				{
					generate_function_call(node);
					if (statementIsEquality)
						push_result(REG_R8);
				}
				else if (node->nodeCount == 5 && node->nodes[4]->value->symbol == G_KW_SUB)
				{
					generate_sub_statement(node);
				}
				else if (node->nodeCount == 2 && node->nodes[1]->value->symbol == G_KW_RETURN)
				{
					generate_function_return(node);
				}
				else if (node->nodeCount == 2 && node->nodes[1]->value->symbol == G_KW_FOR)
				{
					generate_for_statement(node);
				}
				else if (node->nodeCount == 2 && node->nodes[1]->value->symbol == G_KW_WHILE)
				{
					generate_while_statement(node);
				}
				else if (node->nodeCount == 2 && node->nodes[1]->value->symbol == G_KW_GLOBAL)
				{
					generate_global(node);
				}
				else if (node->nodeCount == 2 && node->nodes[1]->value->symbol == G_KW_EXTERN)
				{
					generate_extern(node);
				}
				break;
		}
	}
	else if (node->nodeCount == 1 && node->nodes[0]->value->value.cstr[0] == '{')
	{
		scopeStack[scopeStackPos++] = stackt.top;
		scope++;
	}
	else if (node->nodeCount == 1 && node->nodes[0]->value->value.cstr[0] == '}')
	{
		stackt.top = scopeStack[--scopeStackPos];
		scope--;
		generate_write_btc(BTC_LABEL, make_format_ddString(".SC%d%d", scope, scopeCounts[scope]), REG_NONE);
		scopeCounts[scope]++;
	}
	else if (node->value->type == TKN_STRING)
	{
		push_data_var(datat_add_string(node->value->value));
	}
	else if (node->value->type == TKN_LITERAL)
	{
		if (is_global(node))
		{
			struct dtVariable var = datat_get_data(node->value->value);
			push_ref(var.name, var.size);
		}
		else
		{
			struct stVariable var = stackt_get_var(node->value->value);
			if (var.size != -6969)
			{
				push_stack_var(var);
			}
			else
			{
				if (node->value->symbol == G_N && addSize) addSizeVal += 8;
				generate_write_btc(BTC_PUSH, node->value->value, REG_NONE);
			}
		}
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
static void generate_plus(struct tokenNode* node)
{
	generate_split(node, 0);
	generate_split(node, 2);
	pop_input(REG_R8);
	pop_input(REG_R9);
	generate_write_btc(BTC_ADD, REG_R8, REG_R9);
	push_result(REG_R8);
}
static void generate_minus(struct tokenNode* node)
{
	generate_split(node, 0);
	generate_split(node, 2);
	pop_input(REG_R8);
	pop_input(REG_R9);
	generate_write_btc(BTC_SUB, REG_R8, REG_R9);
	push_result(REG_R8);
}
static void generate_multiply(struct tokenNode* node)
{
	generate_split(node, 0);
	generate_split(node, 2);
	pop_input(REG_R8);
	pop_input(REG_RAX);
	generate_write_btc(BTC_MUL, REG_R8, REG_NONE);
	push_result(REG_RAX);
}
static void generate_divide(struct tokenNode* node)
{
	generate_split(node, 2);
	generate_split(node, 0);
	generate_write_btc(BTC_XOR, REG_RAX, REG_RAX);
	generate_write_btc(BTC_XOR, REG_RDX, REG_RDX);
	pop_input(REG_R8);
	pop_input(REG_RAX);
	generate_write_btc(BTC_DIV, REG_R8, REG_NONE);
	push_result(REG_RAX);
	generate_write_btc(BTC_XOR, REG_RAX, REG_RAX);
	generate_write_btc(BTC_XOR, REG_RDX, REG_RDX);
}
static void generate_global(struct tokenNode* node)
{
	datat_add_data(node->nodes[0]->nodes[0]->value->value, make_constant_ddString("0"), ddString_to_int(node->nodes[0]->nodes[1]->value->value));
}
static void generate_format(struct tokenNode* node)
{
	sizet size = ddString_to_int(node->nodes[1]->nodes[1]->value->value);
	ddPrintf("size: %d\n", size);
}
static void define_variable(struct tokenNode* node)
{
	stackt_set_var(node->nodes[0]->value->value, ddString_to_int(node->nodes[1]->value->value));
}
static void generate_extern(struct tokenNode* node)
{
	generate_write_btc(BTC_EXTERN, node->nodes[0]->value->value, REG_NONE);
}
static void generate_malloc(struct tokenNode* node)
{
	sizet typesize = ddString_to_int(node->nodes[1]->nodes[1]->value->value);
	generate_split_right(node->nodes[1]->nodes[1]->nodes[1]);
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
	statementIsEquality = true;
	generate_split(node, 0);
	int typesize = 0;
	if (node->nodes[2]->nodes[2]->value->value.cstr[0] == ':')
	{
		typesize = ddString_to_int(node->nodes[2]->nodes[5]->value->value);
		generate_split(node->nodes[2], 1);
		pop_input(REG_R8);
		generate_write_btc(BTC_MOV, REG_RAX, node->nodes[2]->nodes[5]->value->value);
		generate_write_btc(BTC_MUL, REG_R8, REG_NONE);
		push_result(REG_RAX);
		generate_split(node->nodes[2], 3);
		pop_both_sides();
		generate_write_btc(BTC_ADD, REG_R8, REG_R9);
	}
	else
	{
		typesize = ddString_to_int(node->nodes[2]->nodes[3]->value->value);
		generate_split(node->nodes[2], 1);
		pop_input(REG_R8);
	}
	pop_ref(REG_R8, typesize);
}
static void generate_reference(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->nodes[0]->value->value);
	generate_write_btc(BTC_MOV, REG_R8, REG_RBP);
	generate_write_btc(BTC_SUB, REG_R8, make_ddString_from_int(var.spos));
	push_result(REG_R8);
}
static void generate_array_def_var(struct tokenNode* node)
{
	struct stVariable var = stackt_set_var(node->nodes[0]->value->value, 8);

	sizet typesize = ddString_to_int(node->nodes[4]->value->value);
	sizet length = ddString_to_int(node->nodes[2]->value->value);
	sizet size = typesize*length;
	stackt.size += size;
	generate_write_btc(BTC_MOV, REG_R8, REG_RBP);
	generate_write_btc(BTC_SUB, REG_R8, make_ddString_from_int(stackt.size));
	push_result(REG_R8);
	pop_stack_var(var);
}
static void generate_array_def(struct tokenNode* node)
{
	sizet typesize = ddString_to_int(node->nodes[3]->value->value);
	sizet length = ddString_to_int(node->nodes[1]->value->value);
	sizet size = typesize*length;
	stackt.size += size;
	if (statementIsEquality)
	{
		generate_write_btc(BTC_MOV, REG_R8, REG_RBP);
		generate_write_btc(BTC_SUB, REG_R8, make_ddString_from_int(stackt.size));
		push_result(REG_R8);
	}
}
static void generate_dereference(struct tokenNode* node)
{
	if (node->nodes[2]->value->value.cstr[0] == ':')
	{
		generate_split(node, 1);
		pop_input(REG_R8);
		generate_write_btc(BTC_MOV, REG_RAX, node->nodes[5]->value->value);
		generate_write_btc(BTC_MUL, REG_R8, REG_NONE);
		push_result(REG_RAX);
		generate_split(node, 3);
		pop_both_sides();
		generate_write_btc(BTC_ADD, REG_R8, REG_R9);
		push_ref(make_constant_ddString("R8"), ddString_to_int(node->nodes[5]->value->value));
	}
	else
	{
		generate_split(node, 1);
		pop_input(REG_R8);
		push_ref(make_constant_ddString("R8"), ddString_to_int(node->nodes[3]->value->value));
	}
}
static void generate_function_return(struct tokenNode* node)
{
	generate_split(node, 0);
	pop_input(REG_R8);
	generate_write_btc(BTC_JMP, make_format_ddString(".SC%d%d", 0, scopeCounts[0]), REG_NONE);
}
static void generate_function_call(struct tokenNode* node)
{
	addSize = true;
	sizet paramCount = (node->nodes[0]->nodeCount)/2;
	for (sizet i = (paramCount*2)-1; i >= 0; i -= 2)
	{
		generate_asm_step(node->nodes[0]->nodes[i-1]);
	}
	addSize = false;
	generate_write_btc(BTC_CALL, node->nodes[1]->value->value, REG_NONE);
	generate_write_btc(BTC_ADD, REG_RSP, make_ddString_from_int(addSizeVal));
	addSizeVal = 0;
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
static bool is_size_operatable(int size)
{
	switch (size)
	{
		case 1:
		case 2:
		case 4:
		case 8:
			return true;
		default:
			return false;
	}
}
static void generate_sub_statement(struct tokenNode* node)
{
	struct bitcode* temp = bitcode;
	struct bitcode* functionCodeHead= make(struct bitcode, 1);
	bitcode = functionCodeHead;
	struct bitcode* btcMoveStack = generate_write_function_headder(node->nodes[1]->value->value);
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
	cnode = next_tree();
	generate_asm_step(cnode);
	while (scope != tscope)
	{
		cnode = next_tree();
		generate_asm_step(cnode);
	}
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
static void generate_for_statement(struct tokenNode* node)
{
	sizet stotop = stackt.top;
	generate_split(node->nodes[0], 4);
	generate_write_btc(BTC_LABEL, make_format_ddString(".WL%d%d", scope, scopeCounts[scope]), REG_NONE);
	sizet tscope = scope;
	cnode = next_tree();
	generate_asm_step(cnode);
	while (scope != tscope)
	{
		cnode = next_tree();
		generate_asm_step(cnode);
	}
	generate_split(node->nodes[0], 0);
	statementIsEquality = true;
	generate_split(node->nodes[0], 2);
	pop_input(REG_R8);
	generate_write_btc(BTC_CMP, REG_R8, make_constant_ddString("1"));
	generate_write_btc(BTC_JE, make_format_ddString(".WL%d%d", scope, scopeCounts[scope]-1), REG_NONE);
	statementIsEquality = false;
	stackt.top = stotop;
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
	statementIsEquality = true;
	generate_split(node, 0);
	pop_input(REG_R8);
	generate_write_btc(BTC_CMP, REG_R8, make_constant_ddString("1"));
	generate_write_btc(BTC_JE, make_format_ddString(".WL%d%d", scope, scopeCounts[scope]-1), REG_NONE);
	statementIsEquality = false;
}
static void generate_if_statement(struct tokenNode* node)
{
	statementIsEquality = true;
	generate_split(node, 0);
	pop_input(REG_R8);
	generate_write_btc(BTC_CMP, REG_R8, make_constant_ddString("0"));
	generate_write_btc(BTC_JE, make_format_ddString(".SC%d%d", scope, scopeCounts[scope]), REG_NONE);
	bool iselse = false;
	sizet tscope = scope;
	statementIsEquality = false;
	cnode = next_tree();
	generate_asm_step(cnode);
	while (scope != tscope)
	{
		cnode = next_tree();
		if (cnode->nodeCount == 1 && cnode->nodes[0]->value->value.cstr[0] == '}')
		{
			struct tokenNode* nextTree = peek_tree();
			if (nextTree && nextTree->nodeCount == 1 && nextTree->nodes[0]->value->symbol == G_KW_ELSE)
			{
				iselse = true;
				generate_write_btc(BTC_JMP, make_format_ddString(".SC%d%d", scope-1, scopeCounts[scope-1]+1), REG_NONE);
			}
		}
		generate_asm_step(cnode);
	}
	if (iselse)
	{
		next_tree();
		tscope = scope;
		do
		{
			cnode = next_tree();
			generate_asm_step(cnode);
		} while (scope != tscope);
	}
}
static void generate_equels_set_asm(struct tokenNode* node)//i = 2*3;
{
	statementIsEquality = true;
	if (is_global(node->nodes[2]))
	{
		struct dtVariable var = datat_get_data(node->nodes[2]->value->value);
		generate_split(node, 0);
		pop_ref(var.name, var.size);
	}
	else
	{
		struct stVariable var = stackt_get_var(node->nodes[2]->value->value);
		generate_split(node, 0);
		pop_stack_var(var);
	}
}
static void generate_equels_make_set_asm(struct tokenNode* node)//@8 i = 9-3;
{
	statementIsEquality = true;
	struct stVariable var = stackt_set_var(node->nodes[2]->nodes[0]->value->value, ddString_to_int(node->nodes[2]->nodes[1]->value->value));
	generate_split(node, 0);
	pop_stack_var(var);
}
static inline void generate_equality(struct tokenNode* node)//2 == 3-1
{
	generate_split(node, 2);
	generate_split(node, 0);
	pop_both_sides();
	compare_sides();
	compare_equal_flag();
	push_result(REG_R8);
}
static inline void generate_inequality(struct tokenNode* node)//2 != 3
{
	generate_split(node, 2);
	generate_split(node, 0);
	pop_both_sides();
	compare_sides();
	compare_not_equal_flag();
	push_result(REG_R8);
}
static inline void generate_times_equals(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->nodes[2]->value->value);
	generate_split(node, 0);
	generate_split(node, 2);
	pop_input(REG_R8);
	pop_input(REG_RAX);
	generate_write_btc(BTC_MUL, REG_R8, REG_NONE);
	push_result(REG_RAX);
	pop_stack_var(var);
}
static inline void generate_divide_equals(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->nodes[0]->value->value);
	generate_1reg_operation(BTC_DIV, node);
	pop_stack_var(var);
}
static inline void generate_plus_equals(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->nodes[2]->value->value);
	generate_split(node, 2);
	generate_split(node, 0);
	pop_input(REG_R8);
	pop_input(REG_R9);
	generate_write_btc(BTC_ADD, REG_R8, REG_R9);
	push_result(REG_R9);
	pop_stack_var(var);
}
static inline void generate_minus_equals(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->nodes[2]->value->value);
	generate_split(node, 2);
	generate_split(node, 0);
	pop_input(REG_R8);
	pop_input(REG_R9);
	generate_write_btc(BTC_SUB, REG_R8, REG_R9);
	push_result(REG_R9);
	pop_stack_var(var);
}
static inline void generate_plus_plus(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->nodes[1]->value->value);
	generate_split(node, 1);
	pop_input(REG_R8);
	if (statementIsEquality)
		generate_write_btc(BTC_MOV, REG_R9, REG_R8);
	generate_write_btc(BTC_INC, REG_R8, REG_NONE);
	push_result(REG_R8);
	pop_stack_var(var);
	if (statementIsEquality)
		push_result(REG_R9);
}
static inline void generate_minus_minus(struct tokenNode* node)
{
	struct stVariable var = stackt_get_var(node->nodes[1]->value->value);
	generate_split(node, 1);
	pop_input(REG_R8);
	if (statementIsEquality)
		generate_write_btc(BTC_MOV, REG_R9, REG_R8);
	generate_write_btc(BTC_DEC, REG_R8, REG_NONE);
	push_result(REG_R8);
	pop_stack_var(var);
	if (statementIsEquality)
		push_result(REG_R9);
}
static inline void generate_lessthan_equals(struct tokenNode* node)
{
	generate_split(node, 2);
	generate_split(node, 0);
	pop_both_sides();
	compare_sides();
	compare_lessthan_equal_flag();
	push_result(REG_R8);
}
static inline void generate_lessthan(struct tokenNode* node)
{
	generate_split(node, 2);
	generate_split(node, 0);
	pop_both_sides();
	compare_sides();
	compare_lessthan_flag();
	push_result(REG_R8);
}
static inline void generate_greaterthan_equals(struct tokenNode* node)
{
	generate_split(node, 2);
	generate_split(node, 0);
	pop_both_sides();
	compare_sides();
	compare_greaterthan_equal_flag();
	push_result(REG_R8);
}
static inline void generate_greaterthan(struct tokenNode* node)
{
	generate_split(node, 2);
	generate_split(node, 0);
	pop_both_sides();
	compare_sides();
	compare_greaterthan_flag();
	push_result(REG_R8);
}
static inline void generate_mod_operator(struct tokenNode* node)
{
	generate_split(node, 2);
	generate_split(node, 0);
	generate_write_btc(BTC_XOR, REG_RAX, REG_RAX);
	generate_write_btc(BTC_XOR, REG_RDX, REG_RDX);
	generate_write_btc(BTC_POP, REG_R8, REG_NONE);
	generate_write_btc(BTC_POP, REG_RAX, REG_NONE);
	generate_write_btc(BTC_DIV, REG_R8, REG_NONE);
	generate_write_btc(BTC_MOV, REG_R8, REG_RDX);
	generate_write_btc(BTC_XOR, REG_RAX, REG_RAX);
	generate_write_btc(BTC_XOR, REG_RDX, REG_RDX);
	push_result(REG_R8);
}
static inline void generate_logic_and(struct tokenNode* node)
{
	generate_split(node, 2);
	generate_split(node, 0);
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
	generate_split(node, 2);
	generate_split(node, 0);
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
	generate_asm_step(node->nodes[0]);
	generate_asm_step(node->nodes[1]);
}
static inline void generate_split_right(struct tokenNode* node)
{
	generate_asm_step(node->nodes[1]);
}
static inline void generate_split(struct tokenNode* node, int v)
{
	generate_asm_step(node->nodes[v]);
}
static inline void generate_split_left(struct tokenNode* node)
{
	generate_asm_step(node->nodes[0]);
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
	return (node->nodeCount == 3 && node->nodes[2]->nodeCount == 3 && node->nodes[2]->nodes[2]->value->value.cstr[0] == '@');
}
static inline struct tokenNode* next_tree(void)
{
	statementIsEquality = false;
	return (treePosition < treeCount) ? tokenTrees[treePosition++] : (struct tokenNode*)nullptr;
}
static inline struct tokenNode* peek_tree(void)
{
	return (treePosition < treeCount) ? tokenTrees[treePosition] : (struct tokenNode*)nullptr;
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
			pop_nsize(make_format_ddString("RBP-%d", var.spos), var.size);
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
			pop_nsize(value, size);
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
			push_nsize(value, size);
	}
}
static void push_stack_var(struct stVariable var)
{
	if (addSize && var.size >= 0) addSizeVal += var.size;
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
			push_param_nsize(var);
	}
}
struct stVariable stackt_get_var(ddString name)
{
	for (sizet i = 0; i < stackt.top; i++)
	{
		if (ddString_compare(name, stackt.vars[i].name))
		{
			return stackt.vars[i];
		}
	}
	struct stVariable out;
	out.size = -6969;
	return out;
}
void stackt_set_param_var(ddString name, int size, sizet tsize)
{
	stackt.vars[stackt.top].name = name;
	stackt.vars[stackt.top].size = size;
	stackt.top++;
	stackt.vars[stackt.top-1].spos = -(16 + tsize);
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
static void pop_nsize(ddString value, sizet size)
{
	int remain = size;
	int done = 0;
	int com[100] = {0};
	int cpos = 0;
	while (remain != 0)
	{
		if (remain >= 8)
		{
			com[cpos++] = 8;
			remain -= 8;
		}
		else if (remain >= 4)
		{
			com[cpos++] = 4;
			remain -= 4;
		}
		else if (remain >= 2)
		{
			com[cpos++] = 2;
			remain -= 2;
		}
		else if (remain >= 1)
		{
			com[cpos++] = 1;
			remain -= 1;
		}

	}
	cpos--;
	while (cpos >= 0)
	{
		if (com[cpos] == 8)
		{
			generate_write_btc(BTC_POP, make_format_ddString("QWORD[%s+%d]", value.cstr, done), REG_NONE);
			done += 8;
		}
		else if (com[cpos] == 4)
		{
			pop_input(REG_RAX);
			generate_write_btc(BTC_MOV, make_format_ddString("DWORD[%s+%d]", value.cstr, done), REG_EAX);
			done += 4;
		}
		else if (com[cpos] == 2)
		{
			pop_input(REG_RAX);
			generate_write_btc(BTC_MOV, make_format_ddString("WORD[%s+%d]", value.cstr, done), REG_AX);
			done += 2;
		}
		else if (com[cpos] == 1)
		{
			pop_input(REG_RAX);
			generate_write_btc(BTC_MOV, make_format_ddString("BYTE[%s+%d]", value.cstr, done), REG_AL);
			done += 1;
		}
		cpos--;
	}
}
static void push_nsize(ddString value, sizet size)
{
	int remain = size;
	int done = 0;
	while (remain != 0)
	{
		if (remain >= 8)
		{
			generate_write_btc(BTC_PUSH, make_format_ddString("QWORD[%s+%d]", value.cstr, size-done), REG_NONE);
			remain -= 8;
			done += 8;
		}
		else if (remain >= 4)
		{
			generate_write_btc(BTC_MOV, REG_EAX, make_format_ddString("DWORD[%s+%d]", value.cstr, size-done));
			push_result(REG_RAX);
			remain -= 4;
			done += 4;
		}
		else if (remain >= 2)
		{
			generate_write_btc(BTC_MOVSX, REG_RAX, make_format_ddString("WORD[%s+%d]", value.cstr, size-done));
			push_result(REG_RAX);
			remain -= 2;
			done += 2;
		}
		else if (remain >= 1)
		{
			generate_write_btc(BTC_MOVSX, REG_RAX, make_format_ddString("BYTE[%s+%d]", value.cstr, size-done));
			push_result(REG_RAX);
			remain -= 1;
			done += 1;
		}
	}
}
static void push_param_nsize(struct stVariable var)
{
	int remain = var.size;
	int done = 0;
	while (remain != 0)
	{
		if (remain >= 8)
		{
			generate_write_btc(BTC_PUSH, make_format_ddString("QWORD[RBP-%d]", var.spos+(var.size-done)), REG_NONE);
			remain -= 8;
			done += 8;
		}
		else if (remain >= 4)
		{
			generate_write_btc(BTC_MOV, REG_EAX, make_format_ddString("DWORD[RBP-%d]", var.spos+(var.size-done)));
			push_result(REG_RAX);
			remain -= 4;
			done += 4;
		}
		else if (remain >= 2)
		{
			generate_write_btc(BTC_MOVSX, REG_RAX, make_format_ddString("WORD[RBP-%d]", var.spos+(var.size-done)));
			push_result(REG_RAX);
			remain -= 2;
			done += 2;
		}
		else if (remain >= 1)
		{
			generate_write_btc(BTC_MOVSX, REG_RAX, make_format_ddString("BYTE[RBP-%d]", var.spos+(var.size-done)));
			push_result(REG_RAX);
			remain -= 1;
			done += 1;
		}
	}
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
	if (node->nodes[1]->value->value.cstr[0] == '(' && node->nodes[1]->nodes[1]->value->value.cstr[0] == ')') return 0;
	sizet output = 0;
	while (node != nullptr && node->value->value.cstr[0] != ')' && node->value->value.cstr[0] != ';')
	{
		if (node->value->value.cstr[0] == ',') output++;
		node = node->nodes[1];
	}
	return output+1;
}
struct dtVariable datat_get_data(ddString name)
{
	for (sizet i = 0; i < datat.top; i++)
	{
		if (ddString_compare(name, datat.data[i].name)) return datat.data[i];
	}
	struct dtVariable errorout;
	errorout.size = -6969;
	return errorout;
}
struct dtVariable datat_add_data(ddString name, ddString value, sizet size)
{
	datat.data[datat.top].name = name;
	datat.data[datat.top].data = value;
	datat.data[datat.top].size = size;
	datat.data[datat.top].defineSize = make_constant_ddString(DEFINE_SIZES[size]);
	return datat.data[datat.top++];
}
struct dtVariable datat_add_string(ddString value)
{
	datat.data[datat.top].data = value;
	ddString_push_cstring_back(&(datat.data[datat.top].data), ",0");
	datat.data[datat.top].data.cstr[datat.data[datat.top].data.length] = '\0';
	datat.data[datat.top].name = make_format_ddString("_69_str%d", stringCount++);
	datat.data[datat.top].defineSize = make_constant_ddString("DB");
	return datat.data[datat.top++];
}
void push_data_var(struct dtVariable var)
{
	generate_write_btc(BTC_PUSH, var.name, REG_NONE);
}
static inline bool is_array_def_var(struct tokenNode* node)
{
	return (node->nodeCount == 6 && node->nodes[1]->value->value.cstr[0] == '>' && node->nodes[3]->value->value.cstr[0] == '<' && node->nodes[5]->value->value.cstr[0] == '@');
}
static inline bool is_array_def(struct tokenNode* node)
{
	return (node->nodeCount == 5 && node->nodes[0]->value->value.cstr[0] == '>' && node->nodes[2]->value->value.cstr[0] == '<' && node->nodes[4]->value->value.cstr[0] == '@');
}
static inline bool is_dereference(struct tokenNode* node)
{
	return ((node->nodeCount == 5 && node->nodes[0]->value->value.cstr[0] == ']' && node->nodes[4]->value->value.cstr[0] == '@') ||
		(node->nodeCount == 7 && node->nodes[0]->value->value.cstr[0] == ']' && node->nodes[6]->value->value.cstr[0] == '@'));
}
static inline bool is_dereference_assignment(struct tokenNode* node)
{
	return ((node->nodeCount == 3 && node->nodes[2]->nodeCount == 5 && node->nodes[2]->nodes[4]->value->value.cstr[0] == '@') ||
		(node->nodeCount == 3 && node->nodes[2]->nodeCount == 7 && node->nodes[2]->nodes[6]->value->value.cstr[0] == '@'));
}
static inline bool is_global(struct tokenNode* node)
{
	for (sizet i = 0; i < datat.top; i++)
	{
		if (ddString_compare(node->value->value, datat.data[i].name)) return true;
	}
	return false;
}
#endif

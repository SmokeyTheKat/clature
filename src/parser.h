#ifndef __ddScript_parser_h__
#define __ddScript_parser_h__

#include "./lexer.h"
#include <ddcKeyboard.h>

struct tokenNode;

extern bool debug;

struct tokenNode
{
	struct tokenNode** nodes;
	sizet nodeCount;
	struct token* value;
};
struct syntax
{
	int result;
	int csum;
	int pcsum;
	int contains[5];
	int len;
};

sizet syntaxCount = 22;

struct syntax syntax[22] = {
	{ G_ASSIGN, 	G_AT*G_NUM*G_ID*G_EQUL*G_SUM,	G_AT*G_NUM*G_ID*G_EQUL,	{ G_AT,		G_SUM,		G_ID, 	G_NUM, G_EQUL 	}, 5},
	{ G_ASSIGN, 	G_ID*G_EQUL*G_SUM, 		G_ID*G_EQUL, 		{ G_ID,		G_SUM,		G_NULL, G_NULL, G_NULL 	}, 3},
	{ G_SUM, 	G_SUM*G_PLUS*G_SUM, 		G_SUM*G_PLUS, 		{ G_SUM,	G_SUM,		G_NULL, G_NULL, G_NULL 	}, 3},
	{ G_SUM, 	G_SUM*G_MUL*G_SUM, 		G_SUM*G_MUL, 		{ G_SUM,	G_SUM,		G_NULL, G_NULL, G_NULL 	}, 3},
	{ G_SUM, 	G_SUM*G_DIV*G_SUM, 		G_SUM*G_DIV, 		{ G_SUM,	G_SUM,		G_NULL, G_NULL, G_NULL 	}, 3},
	{ G_SUM, 	G_SUM*G_PLUS*G_SUM, 		G_SUM*G_PLUS, 		{ G_SUM,	G_SUM,		G_NULL, G_NULL, G_NULL 	}, 3},
	{ G_SUM, 	G_SUM*G_MINUS*G_SUM, 		G_SUM*G_MINUS, 		{ G_SUM,	G_SUM,		G_NULL, G_NULL, G_NULL	}, 3},
	{ G_SUM, 	G_SUM*G_MUL*G_PRODUCT,	 	G_SUM*G_MUL, 		{ G_SUM,	G_PRODUCT,	G_NULL, G_NULL, G_NULL	}, 3},
	{ G_SUM, 	G_SUM*G_DIV*G_PRODUCT, 		G_SUM*G_DIV, 		{ G_SUM,	G_PRODUCT,	G_NULL, G_NULL, G_NULL	}, 3},
	{ G_SUM, 	G_SUM*G_PLUS*G_PRODUCT, 	G_SUM*G_PLUS, 		{ G_SUM,	G_PRODUCT,	G_NULL, G_NULL, G_NULL	}, 3},
	{ G_SUM, 	G_SUM*G_MINUS*G_PRODUCT, 	G_SUM*G_MINUS, 		{ G_SUM,	G_PRODUCT,	G_NULL, G_NULL, G_NULL	}, 3},
	{ G_SUM, 	G_PRODUCT, 			G_PRODUCT, 		{ G_PRODUCT,	G_NULL,	 	G_NULL, G_NULL, G_NULL	}, 1},
	{ G_PRODUCT, 	G_PRODUCT*G_MUL*G_VALUE, 	G_PRODUCT*G_MUL,	{ G_PRODUCT,	G_VALUE,	G_NULL, G_NULL, G_NULL	}, 3},
	{ G_PRODUCT, 	G_PRODUCT*G_DIV*G_VALUE, 	G_PRODUCT*G_DIV,	{ G_PRODUCT,	G_VALUE,	G_NULL, G_NULL, G_NULL	}, 3},
	{ G_PRODUCT, 	G_VALUE, 			G_VALUE,		{ G_VALUE,	G_NULL,	 	G_NULL, G_NULL, G_NULL	}, 1},
	{ G_VALUE,	G_ID,				G_ID,			{ G_ID,		G_NULL,	 	G_NULL, G_NULL, G_NULL	}, 1},
	{ G_VALUE,	G_NUM,				G_NUM,			{ G_NUM,	G_NULL,	 	G_NULL, G_NULL, G_NULL	}, 1},
	{ G_SUM,	G_OP*G_SUM*G_CP,		G_OP*G_SUM,		{ G_OP,		G_SUM,		G_NULL,	G_NULL, G_NULL	}, 3},
	{ G_SUM,	G_OP*G_SUM*G_CP,		G_CP*G_SUM,		{ G_OP,		G_SUM,		G_NULL,	G_NULL, G_NULL	}, 3},
	{ G_SUM,	G_OS*G_SUM*G_CS,		G_OS*G_SUM,		{ G_OS,		G_SUM,		G_NULL,	G_NULL, G_NULL	}, 3},
	{ G_SUM,	G_OS*G_SUM*G_CS,		G_CS*G_SUM,		{ G_OS,		G_SUM,		G_NULL,	G_NULL, G_NULL	}, 3},
	{ G_SUM,	G_SUM*G_NUM*G_AT,		G_SUM*G_NUM*G_AT,	{ G_NUM,	G_SUM,		G_AT,	G_NULL, G_NULL	}, 3},
	//{ "s", "fs.",  'f', 's', '.', "fs"},
	////{ "s", "(s)", 's', '(', '.', "(s)"}
};

bool is_pgrammer_match(int top, int up)
{
	if (up == 0) return false;
	//check for partial matches
	for (int i = 0; i < syntaxCount; i++)
	{
		if (up == syntax[i].contains[0] || up == syntax[i].contains[1] || up == syntax[i].contains[2] ) return true;
		if (up * top == syntax[i].pcsum) return true;
	}
	return false;
}

//void shift(

struct stack
{
	struct tokenNode** data;
	int top;
};

struct stack make_stack(int len)
{
	struct stack output;
	output.data = make(struct tokenNode*, len);
	output.top = -1;
	return output;
}
void stack_push(struct stack* st, struct tokenNode* value)
{
	st->data[++(st->top)] = value;
}
void stack_pop_length(struct stack* st, int len)
{
	st->top -= len;
}
struct tokenNode* stack_pop(struct stack* st)
{
	return st->data[(st->top)--];
}
int stack_values(struct stack st, int n)
{
	int output = 1;
	for (int i = 0; i < n; i++)
		output *= st.data[st.top-i]->value->symbol;
	return output;
}
struct tokenNode* stack_value(struct stack st)
{
	return st.data[st.top];
}
void stack_print(struct stack st)
{
	if (st.top == -1) return;
	for (int i = 0; i <= st.top; i++)
		printf("%d ", st.data[i]->value->symbol);
}

struct tokenNode* make_tokenNode(struct token* value)
{
	struct tokenNode* output = make(struct tokenNode, 1);
	output->nodeCount = 0;
	output->nodes = make(struct tokenNode*, 10);
	output->value = value;
	return output;
}

struct tokenNode* make_tokenNode_nont(int sym, char* val)
{
	struct token* newt = make(struct token, 1);
	newt->symbol = sym;
	newt->value = make_constant_ddString(val);
	return make_tokenNode(newt);
}

struct tokenNode* make_tokenNode_children(struct tokenNode** children, int cc, int psymbol)
{
	struct tokenNode* nparent = make_tokenNode_nont(psymbol, " ");
	for (int i = 0; i < cc; i++) nparent->nodes[i] = children[i];
	nparent->nodeCount = cc;
	return nparent;
}


void print_tree(struct tokenNode* root, int depth)
{
	printf("#%d:", depth);
	for (int i = 0; i < depth-1; i++) printf("|	");
	printf("+-------");
	printf("|%d: %s|\n", root->value->symbol, root->value->value.cstr);
	for (int i = 0; i < root->nodeCount; i++)
		print_tree(root->nodes[i], depth+1);
}

void tree_zip(struct tokenNode* node)
{
	for (int i = 0; i < node->nodeCount; i++)
	{
		while (node->nodes[i]->nodeCount == 1)
		{
			node->nodes[i] = node->nodes[i]->nodes[0];
		}
		tree_zip(node->nodes[i]);
	}
}

//#define DDEBUG 1

struct tokenNode* parse_line(struct token* tokens, sizet min, sizet max, sizet len)
{
	tokens += min;
	struct tokenNode** in = make(struct tokenNode*, len);
	*in = make_tokenNode(tokens++);
	struct tokenNode** ine = in + (max-min);
	struct stack st = make_stack(1000);
	st.data[-1] = make_tokenNode_nont(G_NULL, " ");
	while (st.top == -1 || (stack_value(st)->value->symbol != G_ASSIGN))
	{
		struct tokenNode* up = (in != ine) ? *in : st.data[-1];
		struct tokenNode* top = stack_value(st);
if (debug){
		printf("-------------------------------------------------------------------\n");
		stack_print(st);
		printf("	%d\n", up->value->symbol);
		ddKey_getch_noesc();
}
		if (is_pgrammer_match(top->value->symbol, up->value->symbol)) //shift
		{
if (debug){
			printf("^ %d\n", up->value->symbol);
}
			stack_push(&st, up);
			in++;
			*in = make_tokenNode(tokens++);
		}
		else  //reduce
		{
			for (int i = 0; i < syntaxCount; i++)
			{
				if (stack_values(st, syntax[i].len) == syntax[i].csum)
				{
if (debug){
					printf("%d <- ", syntax[i].result);
}
					struct tokenNode** children = make(struct tokenNode*, syntax[i].len);
if (debug){
					for (int j = 0; j < syntax[i].len; j++) printf("%d ", st.data[st.top-j]->value->symbol);
					printf("\n");
}
					for (int j = 0; j < syntax[i].len; j++) children[j] = st.data[st.top-j];
					stack_pop_length(&st, syntax[i].len);
					stack_push(&st, make_tokenNode_children(children, syntax[i].len, syntax[i].result));
					break;
				}
			}
		}
	}
	printf("\n");
	tree_zip(stack_value(st));
	if (1) print_tree(stack_value(st), 2);

	
	return stack_value(st);
}

struct tokenNode** parser_main(struct token* tokens, sizet tokenCount, sizet* _treeCount)
{
	struct tokenNode** trees = make(struct tokenNode*, 1000000);
	sizet treeCount = 0;

	sizet commandPos = 0;
	sizet nextCommandPos = tokens_get_next_command(tokens, commandPos, tokenCount);
	while (commandPos < tokenCount)
	{
		if (debug) printf("c: %lld    nc: %lld\n", commandPos, nextCommandPos);
		struct tokenNode* commandHead = parse_line(tokens, commandPos, nextCommandPos, tokenCount);
		trees[treeCount++] = commandHead;
		commandPos = nextCommandPos+1;
		nextCommandPos = tokens_get_next_command(tokens, commandPos, tokenCount);
	}
	(*_treeCount) = treeCount;
	return trees;
}



#endif

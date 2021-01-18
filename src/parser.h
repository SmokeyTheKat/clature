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
	int contains[6];
	int len;
};


sizet syntaxCount = 27;

struct syntax syntax[27] = {
/*A<@NI=S      */{ G_A,         G_AT*G_N*G_I*G_EQ*G_S,                  G_I*G_EQ,       { G_I,          G_S,            G_NULL, G_NULL, G_NULL, G_NULL          }, 5},
/*A<I=S        */{ G_A,         G_I*G_EQ*G_S,                           G_I*G_EQ,       { G_I,          G_S,            G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*A<S=S        */{ G_A,         G_S*G_EQ*G_S,                           G_S*G_EQ,       { G_S,          G_S,            G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*Z<@N         */{ G_Z,         G_AT*G_N,                               G_AT*G_N,       { G_AT,         G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 2},
/*S<S+S        */{ G_S,         G_S*G_SO*G_S,                           G_S*G_SO,       { G_S,          G_S,            G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*S<S+P        */{ G_S,         G_S*G_SO*G_P,                           G_S*G_SO,       { G_S,          G_P,            G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*S<S*S        */{ G_S,         G_S*G_PO*G_S,                           G_S*G_PO,       { G_S,          G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*S<S^S        */{ G_S,         G_S*G_EO*G_S,                           G_S*G_EO,       { G_S,          G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*Q<S==S       */{ G_Q,         G_S*G_QO*G_S,                           G_S*G_QO,       { G_S,          G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*Q<S==P       */{ G_Q,         G_S*G_QO*G_P,                           G_S*G_QO,       { G_P,          G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*S<P*S        */{ G_S,         G_P*G_PO*G_S,                           G_S*G_PO,       { G_S,          G_P,            G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*S<E*S        */{ G_S,         G_E*G_PO*G_S,                           G_S*G_EO,       { G_S,          G_E,            G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*S<Q*S        */{ G_S,         G_Q*G_PO*G_S,                           G_S*G_QO,       { G_S,          G_Q,            G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*P<P*P        */{ G_P,         G_P*G_PO*G_P,                           G_P*G_PO,       { G_P,          G_P,            G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*S<(S)        */{ G_S,         G_OBP*G_S*G_CBP,                        G_OBP*G_S,      { G_OBP,        G_S,            G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*S<(S)        */{ G_S,         G_OBP*G_S*G_CBP,                        G_CBP*G_S,      { G_OBP,        G_S,            G_NULL, G_NULL, G_NULL, G_NULL          }, 3},
/*S<Z[S:S]     */{ G_S,         G_Z*G_OBS*G_S*G_COLON*G_S*G_CBS,        G_Z*G_OBS,      { G_Z,          G_S,            G_NULL, G_NULL, G_NULL, G_NULL          }, 6},
/*S<Z[S:S]     */{ G_S,         G_Z*G_OBS*G_S*G_COLON*G_S*G_CBS,        G_S*G_COLON,    { G_Z,          G_S,            G_NULL, G_NULL, G_NULL, G_NULL          }, 6},
/*S<Z[S:S]     */{ G_S,         G_Z*G_OBS*G_S*G_COLON*G_S*G_CBS,        G_S*G_CBS,      { G_Z,          G_S,            G_NULL, G_NULL, G_NULL, G_NULL          }, 6},
/*A<IF S / KF S*/{ G_A,         G_KF*G_S,                               G_KF*G_S,       { G_KF,         G_S,            G_NULL, G_NULL, G_NULL, G_NULL          }, 2},
/*S<P          */{ G_S,         G_P,                                    G_P,            { G_P,          G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 1},
/*P<E          */{ G_P,         G_E,                                    G_E,            { G_E,          G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 1},
/*E<Q          */{ G_E,         G_Q,                                    G_Q,            { G_Q,          G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 1},
/*Q<V          */{ G_Q,         G_V,                                    G_V,            { G_V,          G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 1},
/*V<I          */{ G_V,         G_I,                                    G_I,            { G_I,          G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 1},
/*V<N          */{ G_V,         G_N,                                    G_N,            { G_N,          G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 1},
/*V<ILASM      */{ G_A,         G_ILASM,                                G_ILASM,        { G_ILASM,      G_NULL,         G_NULL, G_NULL, G_NULL, G_NULL          }, 1},
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
	if (st.top+1 < n) return 0;
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
		if (node->nodes[i]->nodeCount == 3 && node->nodes[i]->nodes[0]->value->symbol == ')')
		{
			node->nodes[i] = node->nodes[i]->nodes[1];
		}
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
	while (st.top == -1 || (stack_value(st)->value->symbol != G_A))
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

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
	int cnsum;
	int pcsum;
	int bpcsum;
	int contains[6];
	int len;
};


sizet syntaxCount = 29;

struct syntax syntax[29] = {
/*A<S=S        */{ G_A, G_S * '=' * G_S, 				G_NULL,			G_S * '=', 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 3},
/*S<@S[S]      */{ G_S, '@' * G_S * '[' * G_S * ']',			G_NULL,	 		G_S * '[', 	'[' + G_S + ']',{ '@', G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 5},
/*S<@S[S:S]    */{ G_S, '@' * G_S * '[' * G_S * ':' * G_S * ']',	G_NULL,	 		G_S * '[', 	'[' + G_S + ':',{ '@', G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 7},
/*S<@S[S:S]    */{ G_S, '@' * G_S * '[' * G_S * ':' * G_S * ']',	G_NULL,	 		G_S * '[', 	':' + G_S + ']',{ '@', G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 7},
/*S<@NI        */{ G_S, '@' * G_N * G_I, 				G_NULL,			G_NULL, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 3},
/*S<?I         */{ G_S, '?' * G_I,					G_NULL,			G_NULL, 	G_NULL,		{ '?', G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 2},
/*S<S+S        */{ G_S, G_S * G_SO * G_S, 				G_NULL,			G_S * G_SO, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 3},
/*S<Q+S        */{ G_S, G_Q * G_SO * G_S, 				G_NULL,			G_S * G_SO, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 3},
/*S<S*P        */{ G_S, G_S * G_PO * G_P, 				G_NULL,			G_S * G_PO, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 3},
/*P<P*P        */{ G_P, G_P * G_PO * G_P, 				G_NULL,			G_P * G_PO, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 3},
/*S<P          */{ G_S, G_P, 						G_NULL,			G_NULL, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 1},
/*E<E^E        */{ G_E, G_E * G_EO * G_E, 				G_NULL,			G_E * G_EO, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 3},
/*P<E          */{ G_P, G_E,						G_NULL,			G_NULL, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 1},
/*S<@N<S>      */{ G_S, '@' * G_N * '<' * G_S * '>',			G_NULL,			G_N*G_OBT, 	'<' + G_S + '>',{ G_OBT, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 5},
/*E<Q          */{ G_E, G_Q, 						G_NULL,			G_NULL, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 1},
/*Q<Q==Q       */{ G_Q, G_Q * G_QO * G_Q, 				G_NULL,			G_Q * G_QO, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 3},
/*Q<Q==S       */{ G_Q, G_Q * G_QO * G_S, 				G_NULL,			G_Q * G_QO, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 3},
/*Q<V          */{ G_Q, G_V, 						G_NULL,			G_NULL, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 1},
/*V<I          */{ G_V, G_I, 						G_NULL,			G_NULL, 	G_NULL,		{ G_I, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 1},
/*V<N          */{ G_V, G_N, 						G_NULL,			G_NULL, 	G_NULL,		{ G_N, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 1},
/*A<IF(S)      */{ G_A, G_KW_IF * '(' * G_S * ')',			G_NULL,	 		G_NULL, 	'(' + G_S+ ')',	{ G_KW_IF, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 4},
/*A<SUB@NIFP   */{ G_A, G_KW_SUB * '@' * G_N * G_I * G_FP,		G_NULL,	 		G_NULL, 	'(' +G_FP+ ')',	{ G_KW_SUB, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 5},
/*FP<(FP)      */{ G_FP, '(' * G_FP * ')', 				G_NULL,			G_NULL, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 3},
/*FP<FPS',     */{ G_FP, G_NULL, 					G_FP * G_S * ',',	G_NULL, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 2},
/*FP<S',       */{ G_FP, G_NULL, 					G_S * ',',		G_NULL, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 1},
/*S<(S)        */{ G_S, '(' * G_S * ')', 				G_NULL,			G_NULL, 	'(' + G_S+ ')',	{ '(', G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 3},
/*FP<S)        */{ G_FP, G_S * ')', 					G_NULL,			G_NULL, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 2},
/*S<IFP        */{ G_S, G_I*G_FP, 					G_NULL,			G_NULL, 	G_NULL,		{ G_I, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 2},
/*A<S NULL     */{ G_A, G_NULL, 					G_S * G_NULL,		G_NULL, 	G_NULL,		{ G_NULL, G_NULL, G_NULL, G_NULL, G_NULL, G_NULL }, 1},
};


bool is_pgrammer_match(int top, int up, int back)
{
	if (up == G_NULL) return false;
	//check for partial matches
	for (int i = 0; i < syntaxCount; i++)
	{
		if (up == syntax[i].contains[0] || up == syntax[i].contains[1] || up == syntax[i].contains[2] ) { if(debug)ddPrintf("Fmatch on r%d\n", i); return true; };
		if (up * top == syntax[i].pcsum) { if(debug)ddPrintf("Pmatch on r%d\n", i); return true; };
		if (back + top + up == syntax[i].bpcsum) { if(debug)ddPrintf("BPmatch on r%d\n", i); return true; };
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
struct tokenNode* stack_prev_value(struct stack st)
{
	if (st.top == -1) return st.data[-1];
	return st.data[st.top-1];
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
	if (tokens->type == TKN_ASSEMBLY || tokens->value.cstr[0] == '{' || tokens->value.cstr[0] == '}') return make_tokenNode(tokens);
	struct tokenNode** in = make(struct tokenNode*, len);
	*in = make_tokenNode(tokens++);
	struct tokenNode** ine = in + (max-min);
	struct stack st = make_stack(1000);
	st.data[-1] = make_tokenNode_nont(G_NULL, " ");
	while (st.top == -1 || (stack_value(st)->value->symbol != G_A))
	{
		struct tokenNode* up = (in != ine) ? *in : st.data[-1];
		struct tokenNode* top = stack_value(st);
		struct tokenNode* back = stack_prev_value(st);
		if (is_pgrammer_match(top->value->symbol, up->value->symbol, back->value->symbol)) //shift
		{
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
					struct tokenNode** children = make(struct tokenNode*, syntax[i].len);
					for (int j = 0; j < syntax[i].len; j++) children[j] = st.data[st.top-j];
					stack_pop_length(&st, syntax[i].len);
					stack_push(&st, make_tokenNode_children(children, syntax[i].len, syntax[i].result));
					break;
				}
				else if (stack_values(st, syntax[i].len) * up->value->symbol == syntax[i].cnsum)
				{
					stack_push(&st, up);
					in++;
					*in = make_tokenNode(tokens++);

					struct tokenNode** children = make(struct tokenNode*, syntax[i].len+1);
					for (int j = 0; j < syntax[i].len+1; j++) children[j] = st.data[st.top-j];
					stack_pop_length(&st, syntax[i].len+1);
					stack_push(&st, make_tokenNode_children(children, syntax[i].len+1, syntax[i].result));
					break;
				}
			}
		}
	}
	if (debug) printf("\n");
	tree_zip(stack_value(st));
	if (debug) print_tree(stack_value(st), 2);
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

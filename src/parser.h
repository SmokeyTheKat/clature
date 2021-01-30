#ifndef __ddScript_parser_h__
#define __ddScript_parser_h__

#include "./lexer.h"
#include "./utils.h"
#include <ddcKeyboard.h>

struct tokenNode;

extern bool debug;

struct tokenNode* make_tokenNode(struct token* value);
struct tokenNode* make_tokenNode_children(struct tokenNode** children, int cc, int psymbol);
static void push(int v);
static void pop(int count);
static int top(void);
static int print_stack(struct token tkn);
static int shift(int v);
static int reduce(int v);
static int agoto(int v);
static void zip(struct tokenNode* node);
static int accept(int v);
struct tokenNode** parser_main(struct token* tokens, sizet tokenCount, sizet* _treeCount);
void print_tree(struct tokenNode* root, int depth);

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


static struct token* stream;
static struct token* streamEnd;
static int sstack[100];
static struct tokenNode* tstack[100];
static int spos = -1;
static int tpos = -1;
static sizet treeCount;
static struct tokenNode** trees;
/*
grammar:
G_A -> G_S
G_S -> G_ILASM
G_S -> G_S G_EQ G_S
G_S -> G_AT G_N G_I
G_S -> G_AT G_N G_OBT G_S G_CBT
G_S -> G_AT G_N G_OBS G_S G_CBS
G_S -> G_AT G_N G_OBS G_S G_COLON G_S G_CBS
G_S -> G_OBP G_S G_CBP
G_S -> G_S G_SO G_S
G_S -> G_QUEST G_I
G_S -> G_P
G_P -> G_E
G_E -> G_Q
G_Q -> G_V
G_V -> G_I
G_V -> G_N
*/
enum
{
G_ILASM,G_EQ,G_AT,G_N,G_I,G_OBT,G_CBT,G_OBS,G_CBS,G_COLON,G_OBP,G_CBP,G_SO,G_QUEST,G_Z,G_A,G_S,G_P,G_E,G_Q,G_V,G_SEMI=G_Z
};
const struct syntax grammar[16] = {
{G_A,1},{G_S,1},{G_S,3},{G_S,3},{G_S,5},{G_S,5},{G_S,7},{G_S,3},{G_S,3},{G_S,2},{G_S,1},{G_P,1},{G_E,1},{G_Q,1},{G_V,1},{G_V,1},};
struct action stb[30][21] = {
{{shift,2},{0,0},{shift,3},{shift,11},{shift,10},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,4},{0,0},{0,0},{shift,5},{0,0},{0,0},{agoto,1},{agoto,6},{agoto,7},{agoto,8},{agoto,9},},
{{0,0},{shift,12},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,13},{0,0},{accept,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,1},{0,0},{0,0},{0,0},{0,0},{reduce,1},{0,0},{reduce,1},{reduce,1},{0,0},{reduce,1},{reduce,1},{0,0},{reduce,1},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{0,0},{0,0},{shift,14},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{shift,2},{0,0},{shift,3},{shift,11},{shift,10},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,4},{0,0},{0,0},{shift,5},{0,0},{0,0},{agoto,15},{agoto,6},{agoto,7},{agoto,8},{agoto,9},},
{{0,0},{0,0},{0,0},{0,0},{shift,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,10},{0,0},{0,0},{0,0},{0,0},{reduce,10},{0,0},{reduce,10},{reduce,10},{0,0},{reduce,10},{reduce,10},{0,0},{reduce,10},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,11},{0,0},{0,0},{0,0},{0,0},{reduce,11},{0,0},{reduce,11},{reduce,11},{0,0},{reduce,11},{reduce,11},{0,0},{reduce,11},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,12},{0,0},{0,0},{0,0},{0,0},{reduce,12},{0,0},{reduce,12},{reduce,12},{0,0},{reduce,12},{reduce,12},{0,0},{reduce,12},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,13},{0,0},{0,0},{0,0},{0,0},{reduce,13},{0,0},{reduce,13},{reduce,13},{0,0},{reduce,13},{reduce,13},{0,0},{reduce,13},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,14},{0,0},{0,0},{0,0},{0,0},{reduce,14},{0,0},{reduce,14},{reduce,14},{0,0},{reduce,14},{reduce,14},{0,0},{reduce,14},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,15},{0,0},{0,0},{0,0},{0,0},{reduce,15},{0,0},{reduce,15},{reduce,15},{0,0},{reduce,15},{reduce,15},{0,0},{reduce,15},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{shift,2},{0,0},{shift,3},{shift,11},{shift,10},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,4},{0,0},{0,0},{shift,5},{0,0},{0,0},{agoto,17},{agoto,6},{agoto,7},{agoto,8},{agoto,9},},
{{shift,2},{0,0},{shift,3},{shift,11},{shift,10},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,4},{0,0},{0,0},{shift,5},{0,0},{0,0},{agoto,18},{agoto,6},{agoto,7},{agoto,8},{agoto,9},},
{{0,0},{0,0},{0,0},{0,0},{shift,19},{shift,20},{0,0},{shift,21},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{shift,12},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,22},{shift,13},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,9},{0,0},{0,0},{0,0},{0,0},{reduce,9},{0,0},{reduce,9},{reduce,9},{0,0},{reduce,9},{reduce,9},{0,0},{reduce,9},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{shift,12},{0,0},{0,0},{0,0},{0,0},{reduce,2},{0,0},{reduce,2},{reduce,2},{0,0},{reduce,2},{shift,13},{0,0},{reduce,2},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{shift,12},{0,0},{0,0},{0,0},{0,0},{reduce,8},{0,0},{reduce,8},{reduce,8},{0,0},{reduce,8},{shift,13},{0,0},{reduce,8},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,3},{0,0},{0,0},{0,0},{0,0},{reduce,3},{0,0},{reduce,3},{reduce,3},{0,0},{reduce,3},{reduce,3},{0,0},{reduce,3},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{shift,2},{0,0},{shift,3},{shift,11},{shift,10},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,4},{0,0},{0,0},{shift,5},{0,0},{0,0},{agoto,23},{agoto,6},{agoto,7},{agoto,8},{agoto,9},},
{{shift,2},{0,0},{shift,3},{shift,11},{shift,10},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,4},{0,0},{0,0},{shift,5},{0,0},{0,0},{agoto,24},{agoto,6},{agoto,7},{agoto,8},{agoto,9},},
{{0,0},{reduce,7},{0,0},{0,0},{0,0},{0,0},{reduce,7},{0,0},{reduce,7},{reduce,7},{0,0},{reduce,7},{reduce,7},{0,0},{reduce,7},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{shift,12},{0,0},{0,0},{0,0},{0,0},{shift,25},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,13},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{shift,12},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,26},{shift,27},{0,0},{0,0},{shift,13},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,4},{0,0},{0,0},{0,0},{0,0},{reduce,4},{0,0},{reduce,4},{reduce,4},{0,0},{reduce,4},{reduce,4},{0,0},{reduce,4},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,5},{0,0},{0,0},{0,0},{0,0},{reduce,5},{0,0},{reduce,5},{reduce,5},{0,0},{reduce,5},{reduce,5},{0,0},{reduce,5},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{shift,2},{0,0},{shift,3},{shift,11},{shift,10},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,4},{0,0},{0,0},{shift,5},{0,0},{0,0},{agoto,28},{agoto,6},{agoto,7},{agoto,8},{agoto,9},},
{{0,0},{shift,12},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{shift,29},{0,0},{0,0},{0,0},{shift,13},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
{{0,0},{reduce,6},{0,0},{0,0},{0,0},{0,0},{reduce,6},{0,0},{reduce,6},{reduce,6},{0,0},{reduce,6},{reduce,6},{0,0},{reduce,6},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},},
};
static void push(int v)
{
	sstack[++spos] = v;
}
static void pop(int count)
{
	spos -= count;
}
static int top(void)
{
	return sstack[spos];
}
static int print_tstack(struct token tkn)
{
	for (int i = 0; i < tpos+1; i++)
		printf("%s ", tstack[i]->value->value.cstr);
	printf("		%s\n", tkn.value.cstr);
	return spos+1;
}
static int print_stack(struct token tkn)
{
	for (int i = 0; i < spos+1; i++)
		printf("%d ", sstack[i]);
	printf("		%d\n", tkn.symbol);
	return spos+1;
}

static int shift(int v)
{
	if(debug)ddPrintf("SHIFT %d\n", v);
	push(v);
	tstack[++tpos] = make_tokenNode(stream++);
	return 0;
}
static int reduce(int v)
{
	if(debug)ddPrintf("REDUCE %d\n", v);
	pop(grammar[v].length);

	struct tokenNode** children = make(struct tokenNode*, grammar[v].length);
	for (int i = 0; i < grammar[v].length; i++) children[i] = tstack[tpos-i];
	tpos -= grammar[v].length;
	tstack[++tpos] = make_tokenNode_children(children, grammar[v].length, grammar[v].result);

	struct action cact = stb[top()][grammar[v].result];
	(*(cact.action))(cact.value);
	return 0;
}
static int agoto(int v)
{
	if(debug)ddPrintf("GOTO %d\n", v);
	push(v);
	return 0;
}
static void zip(struct tokenNode* node)
{
	for (int i = 0; i < node->nodeCount; i++)
	{
		if (node->nodes[i]->nodeCount == 3 && node->nodes[i]->nodes[0]->value->symbol == G_CBP)
		{
			node->nodes[i] = node->nodes[i]->nodes[1];
		}
		while (node->nodes[i]->nodeCount == 1)
		{
			node->nodes[i] = node->nodes[i]->nodes[0];
		}
		zip(node->nodes[i]);
	}
}
static int accept(int v)
{
	//ddPrintf("ACCEPT at %s\n", stream->value.cstr);
	zip(tstack[0]);
	if(debug)print_tree(tstack[0], 2);
	trees[treeCount++] = tstack[0];
	tpos = -1;
	spos = -1;
	push(0);
	return 1;
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


struct tokenNode** parser_main(struct token* tokens, sizet tokenCount, sizet* _treeCount)
{
	treeCount = 0;
	trees = make(struct tokenNode*, 1000000);
	stream = tokens;
	streamEnd = tokens+tokenCount;
	push(0);
	printf("start\n");
	int i = 1;
	while (1)
	{
		//if (debug)printf("run %d\n", i);
		//if (debug)print_stack(*stream);
		//if (debug)print_tstack(*stream);
		struct action cact = stb[top()][(*stream).symbol];
		if (cact.action == 0) { compile_error(make_format_ddString("SYNTAX ERROR AT STATE %d\n", i).cstr); exit(0); };
		if (((*(cact.action))(cact.value)) == 1)
			if (stream+1 == streamEnd) goto parser_exit;
			else stream++;
		i++;
	}

parser_exit:
	*_treeCount = treeCount;
	return trees;
}

#endif

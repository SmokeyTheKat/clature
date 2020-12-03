#include <ddcDef.h>
#include <ddcString.h>

#include <stdio.h>

#include "./utils.h"

#define TKN_LITERAL 0x00
#define TKN_KEYWORD 0x01
#define TKN_OPERATOR 0x02
#define TKN_SYNTAX 0x03


const char* const TKN_STRS[] = {
	"LITERAL",
	"KEYWORD",
	"OPERATOR",
	"SYNTAX"
};

struct token
{
	int type;
	ddString value;
};

struct token* tokenize_file(ddString file, sizet* tokenCount)
{
	struct token* tokens = make(struct token, 100);
	(*tokenCount) = -1;
	bool inLiteral = false;
	bool inLiteralString = false;
	for (int i = 0; i < file.length; i++)
	{
		if (inLiteralString)
		{
			if (file.cstr[i] == '\'' || file.cstr[i] == '\"')
			{
				inLiteralString = false;
			}
			ddString_push_char_back(&(tokens[*tokenCount].value), file.cstr[i]);
			continue;
		}
		switch (file.cstr[i])
		{
			case ';': case '{': case '}': case '[': case ']': case '(': case ')':
			{
				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_SYNTAX;
				tokens[*tokenCount].value = make_multi_ddString_cstring(" ", 1);
				tokens[*tokenCount].value.cstr[0] = file.cstr[i];
				inLiteral = false;
				break;
			}
			case '+': case '-': case '*': case '/': case '<': case '>':
			{
				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_OPERATOR;
				tokens[*tokenCount].value = make_multi_ddString_cstring(" ", 1);
				tokens[*tokenCount].value.cstr[0] = file.cstr[i];
				inLiteral = false;
				break;
			}
			case '@':
			{
				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_OPERATOR;
				tokens[*tokenCount].value = make_ddString("@");
				//ddString_push_char_back(&(tokens[*tokenCount].value), file.cstr[i+1]);
				//i++;
				inLiteral = false;
				break;
			}
			case '=':
			{
				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_OPERATOR;
				if (file.cstr[i+1] == '=')
				{
					tokens[*tokenCount].value = make_ddString("==");
					i++;
				}
				else
					tokens[*tokenCount].value = make_ddString("=");
				inLiteral = false;
				break;
			}
			case '!':
			{
				(*tokenCount)++;
				tokens[i].type = TKN_OPERATOR;
				if (file.cstr[i+1] == '=')
				{
					tokens[*tokenCount].value = make_ddString("!=");
					i++;
				}
				else
					tokens[*tokenCount].value = make_ddString("!");
				inLiteral = false;
				break;
			}

			case ' ':
			{
				inLiteral = false;
				break;
			}


			default:
			{
				if (!inLiteral)
				{
					(*tokenCount)++;
					inLiteral = true;
					tokens[*tokenCount].type = TKN_LITERAL;
					tokens[*tokenCount].value = make_ddString("");
				}
				if (file.cstr[i] == '\'' || file.cstr[i] == '\"')
				{
					inLiteralString = true;
				}
				ddString_push_char_back(&(tokens[*tokenCount].value), file.cstr[i]);
				break;
			}
		}
	}
	(*tokenCount)++;
	return tokens;
}

struct tokenNode
{
	struct tokenNode* parent;
	struct tokenNode* left;
	struct tokenNode* right;
	struct token* value;
};

struct tokenNode make_tokenNode(struct tokenNode* parent, struct tokenNode* left, struct tokenNode* right, struct token* value)
{
	struct tokenNode output;
	output.parent = parent;
	output.left = left;
	output.right = right;
	output.value = value;
	return output;
}

const char charKeys[] = {
	'=', '@', '+', '-', '*', '/'
};

void parser(struct token* tokens, struct tokenNode* node, sizet min, sizet max, sizet len)
{
	//if (min == max) return;
	for (int k = 0; k < 6; k++)
	{
		bool keyFound = false;
		for (int i = min; i < max; i++)
		{
			if (tokens[i].value.cstr[0] == charKeys[k])
			{
				ddPrintf("min:%d   max:%d    i:%d    tkv:%s\n", min, max, i, tokens[i].value.cstr);
				if (i != min)
				{
					struct tokenNode* left = make(struct tokenNode, 1); 
					(*left) = make_tokenNode(node, nullptr, nullptr, nullptr);
					parser(tokens, left, min, i, len);
					node->left = left;
				}

				if (i != max)
				{
					struct tokenNode* right = make(struct tokenNode, 1); 
					(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
					parser(tokens, right, i+1, max, len);
					node->right = right;
				}

				node->value = &(tokens[i]);
				return;
			}
		}
	}
	int i = min;
	if (i == max) return;
	ddPrintf("min:%d   max:%d    i:%d    tkv:%s\n", min, max, i, tokens[i].value.cstr);
	if (i != min)
	{
		struct tokenNode* left = make(struct tokenNode, 1); 
		(*left) = make_tokenNode(node, nullptr, nullptr, nullptr);
		parser(tokens, left, min, i, len);
		node->left = left;
	}

	if (i != max)
	{
		struct tokenNode* right = make(struct tokenNode, 1); 
		(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
		parser(tokens, right, i+1, max, len);
		node->right = right;
	}

	node->value = &(tokens[i]);
	return;
}

void generate_asm(struct tokenNode* node);

void generate_asm_2reg(struct tokenNode* node, const char* opc)
{
	ddPrintf("	push r15;\n");
	ddPrintf("	%s r15, r14;\n", opc);
	ddPrintf("	pop r15;\n");
	ddPrintf("	pop r14;\n");
	generate_asm(node->right);
	generate_asm(node->left);
	return;
}
void generate_asm_1reg(struct tokenNode* node, const char* opc)
{
	ddPrintf("	push r15;\n");
	ddPrintf("	%s r15;\n", opc);
	ddPrintf("	pop r15;\n");
	ddPrintf("	pop rax;\n");
	generate_asm(node->right);
	generate_asm(node->left);
	return;
}


void generate_asm(struct tokenNode* node)
{
	if (node == nullptr) return;
	switch (node->value->value.cstr[0])
	{
		case '=':
		{
			if (node->left->value->value.cstr[0] == '@')
			{
				ddPrintf("	pop %s;\n", node->left->right->right->value->value.cstr);
				generate_asm(node->right);
				return;
			}
			break;
		}
		case '*':
			generate_asm_1reg(node, "mul");
			return;
		case '/':
			generate_asm_1reg(node, "div");
			return;
		case '+':
			generate_asm_2reg(node, "add");
			return;
		case '-':
			generate_asm_2reg(node, "sub");
			return;
		default:
			ddPrintf("	push %s;\n", node->value->value.cstr);
			break;
	}
}

#include <stdio.h>
#include <stdlib.h>

ddString read_file(const char* path)
{
	FILE* fp = fopen(path, "r");
	if (fp == null) return make_constant_ddString("ERROR");
	fseek(fp, 0L, SEEK_END);
	long nb = ftell(fp) - 1;
	fseek(fp, 0L, SEEK_SET);
	char* buffer = (char*)calloc(nb, sizeof(char));
	fread(buffer, sizeof(char), nb, fp);
	fclose(fp);
	return make_ddString(buffer);
}

int main(int agsc, char** ags)
{
	ddPrint_cstring("\x1b[38;2;255;255;255m");
	if (agsc < 2) compile_error("NO INPUT FILES");

	sizet tokenCount = 0;
	ddString file = read_file(ags[1]);
	ddPrint_ddString_nl(file);
	ddPrint_int_nl(file.length);
	struct token* tokens = tokenize_file(file, &tokenCount);
	for (sizet i = 0; i < tokenCount; i++)
	{
		printf("%s: %s - %d\n", TKN_STRS[tokens[i].type], tokens[i].value.cstr, tokens[i].value.length);
	}
	struct tokenNode* head = make(struct tokenNode, 1);
	(*head) = make_tokenNode(nullptr, nullptr, nullptr, nullptr);
	parser(tokens, head, 0, tokenCount, tokenCount);
	ddPrint_nl();
	ddPrint_nl();
	ddPrintf("global _start\n_start:\n");
	generate_asm(head);
	ddPrint_nl();
	return 0;
}

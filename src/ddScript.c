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
	struct token* tokens = make(struct token, 1000000);
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
			case '\n': break;
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
	'{', '=', '@', '+', '-', '*', '/', '('
};

int parser_find_closer(struct token* tokens, sizet len, int start, char obracket, char cbracket)
{
	int i = start+1;
	int layer = 1;
	for (; layer != 0; i++)
	{
		if (tokens[i].value.cstr[0] == obracket)
		{
			layer++;
		}
		if (tokens[i].value.cstr[0] == cbracket)
		{
			layer--;
		}
	}
	return i-1;
}

struct tokenTree
{
	struct tokenNode* head;
};

void parser(struct token* tokens, struct tokenNode* node, sizet min, sizet max, sizet len)
{
	for (int k = 0; k < 8; k++)
	{
		bool keyFound = false;
		for (int i = min; i < max; i++)
		{
			if (tokens[i].value.cstr[0] == charKeys[k])
			{
				if (tokens[i].type == TKN_SYNTAX)
				{
					if (tokens[i].value.cstr[0] == '(')
					{
						int ndb = parser_find_closer(tokens, len, i, '(', ')');
						parser(tokens, node, i+1, ndb, len);
						return;
					}
				}
				//ddPrintf("min:%d   max:%d    i:%d    tkv:%s\n", min, max, i, tokens[i].value.cstr);
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
			if (tokens[i].type == TKN_SYNTAX)
			{
				if (tokens[i].value.cstr[0] == '(')
				{
					int ndb = parser_find_closer(tokens, max, i, '(', ')');
					i = ndb;
					continue;
				}
			}
		}
	}
	int i = min;
	if (i == max) return;
	//ddPrintf("min:%d   max:%d    i:%d    tkv:%s\n", min, max, i, tokens[i].value.cstr);
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

struct bitCode
{
	int* v;
};

void generate_asm(struct tokenNode* node)
{
	if (node == nullptr) return;
	switch (node->value->value.cstr[0])
	{
		case ';': return;
		case '{':
		{
			generate_asm(node->right);
			break;
		}
		case '}':
		{
			break;
		}
		case '=':
		{
			if (node->left->value->value.cstr[0] == '@')
			{
				ddPrintf("	pop %s;\n", node->left->right->right->value->value.cstr);
				generate_asm(node->right);
				return;
			}
			else
			{
				ddPrintf("	pop %s;\n", node->left->value->value.cstr);
				generate_asm(node->right);
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
		{
			ddPrintf("	push %s;\n", node->value->value.cstr);
			break;
		}
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
	char* buffer = (char*)calloc(nb+1, sizeof(char));
	fread(buffer, sizeof(char), nb, fp);
	fclose(fp);
	buffer[nb] = '\0';
	return make_ddString(buffer);
}

sizet tokens_get_command_count(struct token* tokens, sizet tokenCount)
{
	sizet semiColons = 0;
	for (sizet i = 0; i < tokenCount; i++)
		if (tokens[i].value.cstr[0] == ';') semiColons++;
	return semiColons;
}
int tokens_get_next_command(struct token* tokens, int start, sizet tokenCount)
{
	for (sizet i = start; i >= 0; i--)
		if (tokens[i].value.cstr[0] == ';') return i;
	return 0;
}

int main(int agsc, char** ags)
{
	ddPrint_cstring("\x1b[38;2;255;255;255m");
	if (agsc < 2) compile_error("NO INPUT FILES");

	sizet tokenCount = 0;
	ddString file = read_file(ags[1]);
	struct token* tokens = tokenize_file(file, &tokenCount);
	ddPrint_int_nl(tokenCount);
	raze_ddString(&file);
	for (sizet i = 0; i < tokenCount; i++)
	{
		tokens[i].value.cstr[tokens[i].value.length] = '\0';
		ddPrintf("%d: %s: %s\n", i, TKN_STRS[tokens[i].type], tokens[i].value.cstr);
	}
	ddPrint_nl();
	ddPrint_nl();
	ddPrint_nl();
	ddPrint_nl();
	ddPrintf("global _start\n_start:\n	push rbp;\n	mov rbp, rsp;\n");
	sizet commandPos = tokenCount-1;
	sizet nextCommandPos = tokens_get_next_command(tokens, commandPos-1, tokenCount)+1;
	while (commandPos > 0)
	{
		struct tokenNode* commandHead = make(struct tokenNode, 1);
		(*commandHead) = make_tokenNode(nullptr, nullptr, nullptr, nullptr);
		parser(tokens, commandHead, nextCommandPos, commandPos, tokenCount);
		generate_asm(commandHead);
		commandPos = nextCommandPos-1;
		nextCommandPos = tokens_get_next_command(tokens, commandPos-1, tokenCount)+1;
		if (nextCommandPos == 1) nextCommandPos = 0;
	}
	ddPrintf("	mov eax, 0;\n	pop rbp;\n	ret;");
	ddPrint_nl();
	ddPrint_nl();
	raze(tokens);
	return 0;
}

#ifndef __ddScript_tokens_h__
#define __ddScript_tokens_h__

#include "./utils.h"

#define TKN_LITERAL 0x00
#define TKN_KEYWORD 0x01
#define TKN_OPERATOR 0x02
#define TKN_SYNTAX 0x03
#define TKN_ASSEMBLY 0x04
#define TKN_FUNCTION 0x05

const char* const TKN_STRS[] = {
	"LITERAL",
	"KEYWORD",
	"OPERATOR",
	"SYNTAX",
	"ASSEMBLY",
	"FUNCTION"
};

struct token
{
	int type;
	ddString value;
};

ddString keywords[9];

sizet tokens_get_command_count(struct token* tokens, sizet tokenCount)
{
	sizet semiColons = 0;
	for (sizet i = 0; i < tokenCount; i++)
		if (tokens[i].value.cstr[0] == ';') semiColons++;
	return semiColons;
}
int tokens_get_next_command(struct token* tokens, int start, sizet tokenCount)
{
	for (sizet i = start; i < tokenCount; i++)
		if (tokens[i].value.cstr[0] == ';') return i;
	return 0;
}

void tokenize_get_keyword(struct token* tokens, sizet tc)
{
	if (tc < 0) return;
	tokens[tc].value.cstr[tokens[tc].value.length] = '\0';
	for (int i = 0; i < 9; i++)
		if (ddString_compare(tokens[tc].value, keywords[i]))
			tokens[tc].type = TKN_KEYWORD;
}
ddString tokenize_until_semicolon(ddString file, sizet start)
{
	ddString output = make_ddString("");
	for (sizet i = start; file.cstr[i] != ';'; i++)
	{
		ddString_push_char_back(&output, file.cstr[i]);
	}
	return output;
}

sizet tokenize_goto_semicolon(ddString file, sizet start)
{
	for (sizet i = start; i < file.length; i++)
		if (file.cstr[i] == ';') return i-1;
}

struct token* tokenize_file(ddString file, sizet* tokenCount)
{
	keywords[0] = make_constant_ddString("if");
	keywords[1] = make_constant_ddString("while");
	keywords[2] = make_constant_ddString("for");
	keywords[3] = make_constant_ddString("sub");
	keywords[4] = make_constant_ddString("fun");
	keywords[5] = make_constant_ddString("struct");
	keywords[6] = make_constant_ddString("return");
	keywords[7] = make_constant_ddString("iso");
	keywords[8] = make_constant_ddString("global");

	struct token* tokens = make(struct token, 100000000);
	(*tokenCount) = -1;
	bool inComment = false;
	bool inLiteral = false;
	bool inLiteralString = false;
	for (sizet i = 0; i < file.length; i++)
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
			case ';':
			{
				inComment = true;
				break;
			}
			case '\n':
			{
				if (tokens[*tokenCount].value.cstr[0] == ';') break;
				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_SYNTAX;
				tokens[*tokenCount].value = make_ddString(";");
				inLiteral = false;
				inComment = false;
				break;
			}
			case '.':
				if (tokens[(*tokenCount)].value.cstr[0] == ';' || tokens[(*tokenCount)].value.cstr[0] == '{')
				{
					(*tokenCount)++;
					tokens[*tokenCount].type = TKN_ASSEMBLY;
					tokens[*tokenCount].value = tokenize_until_semicolon(file, i);
					i = tokenize_goto_semicolon(file, i);
					break;
				}
			case '{': case '}':
			{
				if (tokens[*tokenCount].value.cstr[0] != ';')
				{
					(*tokenCount)++;
					tokens[*tokenCount].type = TKN_SYNTAX;
					tokens[*tokenCount].value = make_ddString(";");
				}
				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_SYNTAX;
				tokens[*tokenCount].value = make_multi_ddString_cstring(" ", 1);
				tokens[*tokenCount].value.cstr[0] = file.cstr[i];

				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_SYNTAX;
				tokens[*tokenCount].value = make_ddString(";");

				inLiteral = false;
				break;
			}
			case '[': case ']': case '(': case ')': case ',':
			{
				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_SYNTAX;
				tokens[*tokenCount].value = make_multi_ddString_cstring(" ", 1);
				tokens[*tokenCount].value.cstr[0] = file.cstr[i];
				inLiteral = false;
				break;
			}
			case '+': case '-': case '*': case '/':
			{
				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_OPERATOR;
				tokens[*tokenCount].value = make_multi_ddString_cstring(" ", 1);
				tokens[*tokenCount].value.cstr[0] = file.cstr[i];
				inLiteral = false;
				break;
			}
			case '<':
			{
				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_OPERATOR;
				if (file.cstr[i+1] == '=')
				{
					tokens[*tokenCount].value = make_ddString("<=");
					i++;
				}
				else
					tokens[*tokenCount].value = make_ddString("<");
				inLiteral = false;
				break;
			}
			case '>':
			{
				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_OPERATOR;
				if (file.cstr[i+1] == '=')
				{
					tokens[*tokenCount].value = make_ddString(">=");
					i++;
				}
				else
					tokens[*tokenCount].value = make_ddString(">");
				inLiteral = false;
				break;
			}
			case '@':
			{
				(*tokenCount)++;
				tokens[*tokenCount].type = TKN_OPERATOR;
				tokens[*tokenCount].value = make_ddString("@");
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
				tokens[*tokenCount].type = TKN_OPERATOR;
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
			case '	':
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
	for (sizet i = 0; i < *tokenCount; i++)
	{
		if (tokens[i].type == TKN_LITERAL)
			tokenize_get_keyword(tokens, i);
	}
	return tokens;
}

#endif

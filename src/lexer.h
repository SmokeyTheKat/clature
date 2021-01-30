#ifndef __ddScript_lexer_h__
#define __ddScript_lexer_h__ 
#include <ddcString.h>
#include <ddcPrint.h>
#include <stdarg.h>

#include "./utils.h"
#include "./parser.h"

void init_lexer(void);
struct token* lexer_main(ddString _file, sizet* tokenCount);
static inline char next(void);
static inline char peek(int rpos);
static inline void skip(char cend);
static int get_keyword_symbol(ddString value);
static inline void reset_literal(void);
static inline void set_token(int type, ddString value, int symbol);
struct token* oset_token(int type, ddString value, int symbol);
static bool is_last_linebreak(void);
static void tokenize(char chr);

static ddString file;
static ddString literal;
static bool inBT = false;
static sizet fileCount = 0;
static sizet tokenCount = 0;
static sizet otokenCount = 0;
static struct token* tokens;
ddString keywords[13];

void init_lexer(void)
{
	literal = make_ddString("");
	
	tokens = make(struct token, 10000000);

	keywords[0] = make_constant_ddString("if");
	keywords[1] = make_constant_ddString("while");
	keywords[2] = make_constant_ddString("for");
	keywords[3] = make_constant_ddString("sub");
	keywords[4] = make_constant_ddString("fun");
	keywords[5] = make_constant_ddString("format");
	keywords[6] = make_constant_ddString("return");
	keywords[7] = make_constant_ddString("iso");
	keywords[8] = make_constant_ddString("global");
	keywords[9] = make_constant_ddString("continue");
	keywords[10] = make_constant_ddString("malloc");
	keywords[11] = make_constant_ddString("extern");
	keywords[12] = make_constant_ddString("format");
}

struct token* lexer_main(ddString _file, sizet* _tokenCount)
{
	init_lexer();
	file = _file;
	while (fileCount < _file.length)
	{
		char chr = next();
		tokenize(chr);
	}
	if (!is_last_linebreak())
			set_token(TKN_LINEBREAK, make_ddString_length(";", 1), G_SEMI);
	(*_tokenCount) = tokenCount;
	return tokens;
}

static void tokenize(char chr)
{
	switch (chr)
	{
		case ' ': case '	':
			break;
		case ';':
			skip('\n');
			if (!is_last_linebreak())
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1), G_SEMI);
			break;
		case '\n':
			if (!is_last_linebreak())
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1), G_SEMI);
			break;
		case '.':
		{
			if (is_last_linebreak())
			{
				do
				{
					ddString_push_char_back(&literal, chr);
				} while ((chr = next()) != '\n' && chr != ';');
				set_token(TKN_ASSEMBLY, literal, G_ILASM);
				reset_literal();
				skip('\n');
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1), G_SEMI);
				break;
			}
		} break;
		case '[':
			set_token(TKN_SYNTAX, make_ddString_length("[", 1), G_OBS);
			break;
		case ']':
			set_token(TKN_SYNTAX, make_ddString_length("]", 1), G_CBS);
			break;
		case '(':
			set_token(TKN_SYNTAX, make_ddString_length("(", 1), G_OBP);
			break;
		case ')':
			set_token(TKN_SYNTAX, make_ddString_length(")", 1), G_CBP);
			break;
		case ',':
			set_token(TKN_SYNTAX, make_ddString_length(",", 1), G_COMMA);
			break;
		case ':':
			set_token(TKN_SYNTAX, make_ddString_length(":", 1), G_COLON);
			break;
		case '{': case '}':
			if (!is_last_linebreak())
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1), G_SEMI);
			set_token(TKN_SYNTAX, make_ddString_length(&chr, 1), chr);
			set_token(TKN_LINEBREAK, make_ddString_length(";", 1), G_SEMI);
			break;
		case '@':
			set_token(TKN_OPERATOR, make_ddString_length("@", 1), G_AT);
			break;
		case '?':
			set_token(TKN_OPERATOR, make_ddString_length("?", 1), G_QUEST);
			break;
		case '*':
		{
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length("*=", 2), G_EQ);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length("*", 1), G_PO);
					break;
			}
			break;
		}
		case '/':
		{
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length("/=", 1), G_EQ);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length("/", 1), G_PO);
					break;
			}
			break;
		}
		case '=':
		{
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length("==", 2), G_QO);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length(&chr, 1), G_EQ);
					break;
			}
			break;
		}
		case '~':
		{
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length("~=", 2), G_EQ);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length("~", 1), G_EO);
					break;
			}
			break;
		}
		case '!':
		{
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length("!=", 2), G_EQ);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length("+", 1), '!');
					break;
			}
			break;
		}
		case '%':
		{
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length("%=", 2), G_EQ);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length("%", 1), G_EO);
					break;
			}
			break;
		}
		case '-':
		{
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length("-=", 2), G_EQ);
					next();
					break;
				case '-':
					set_token(TKN_OPERATOR, make_ddString_length("--", 2), G_MM);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length("-", 1), G_SO);
					break;
			}
			break;
		}
		case '+':
		{
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length("+=", 2), G_EQ);
					next();
					break;
				case '+':
					set_token(TKN_OPERATOR, make_ddString_length("++", 2), G_PP);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length("+", 1), G_SO);
					break;
			}
			break;
		}
		case '&':
		{
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length("&=", 2), G_EQ);
					next();
					break;
				case '&':
					set_token(TKN_OPERATOR, make_ddString_length("&&", 2), G_QO);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length("&", 1), G_EO);
					break;
			}
			break;
		}
		case '|':
		{
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length("|=", 2), G_EQ);
					next();
					break;
				case '|':
					set_token(TKN_OPERATOR, make_ddString_length("||", 2), G_QO);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length("|", 1), G_EO);
					break;
			}
			break;
		}
		case '<':
		{
			if (tokens[tokenCount-2].value.cstr[0] == '@')
			{
				set_token(TKN_OPERATOR, make_ddString_length("<", 1), G_OBT);
				inBT = true;
				break;
			}
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length("<=", 2), G_EQ);
					next();
					break;
				case '<':
					if (peek(1) == '<')
					{
						set_token(TKN_OPERATOR, make_ddString_length("<<<", 3), G_EO);
						next();
					}
					else set_token(TKN_OPERATOR, make_ddString_length("<<", 2), G_EO);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length("<", 1), G_QO);
					break;
			}
			break;
		}
		case '>':
		{
			if (inBT)
			{
				set_token(TKN_OPERATOR, make_ddString_length(">", 1), G_CBT);
				inBT = false;
				break;
			}
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_OPERATOR, make_ddString_length(">=", 2), G_EQ);
					next();
					break;
				case '>':
					if (peek(1) == '>')
					{
						set_token(TKN_OPERATOR, make_ddString_length(">>>", 3), G_EO);
						next();
					}
					else set_token(TKN_OPERATOR, make_ddString_length(">>", 2), G_EO);
					next();
					break;
				default:
					set_token(TKN_OPERATOR, make_ddString_length(">", 1), G_QO);
					break;
			}
			break;
		}
		case '\'':
		{
			ddString_push_char_back(&literal, chr);
			chr = next();
			ddString_push_char_back(&literal, chr);
			if (chr == '\\')
			{
				chr = next();
				ddString_push_char_back(&literal, chr);
			}
			chr = next();
			if (chr != '\'') compile_error("UNCLOSED CHARACTER LITERAL\n");
			ddString_push_char_back(&literal, chr);
		} break;
		case '"':
		{
			do
			{
				ddString_push_char_back(&literal, chr);
			} while ((chr = next()) != '`');
			ddString_push_char_back(&literal, chr);
			set_token(TKN_STRING, literal, G_I);
			tokenize(chr);
		} break;
		case '`':
		{
			do
			{
				ddString_push_char_back(&literal, chr);
			} while ((chr = next()) != '"');
			ddString_push_char_back(&literal, chr);
			set_token(TKN_STRING, literal, G_I);
			reset_literal();
			tokenize(chr);
		} break;
		case '0' ... '9':
		{
			do
			{
				ddString_push_char_back(&literal, chr);
			} while ((chr = next()) >= '0' && chr <= '9');
			set_token(TKN_LITERAL, literal, G_N);
			reset_literal();
			tokenize(chr);
		} break;
		case 'a' ... 'z':
		case 'A' ... 'Z':
		case '_':
		{
			do
			{
				ddString_push_char_back(&literal, chr);
			} while (((chr = next()) >= 'a' && chr <= 'z') ||
					(chr >= 'A' && chr <= 'Z') ||
					(chr == '_'));
			int sym;
			if ((sym = get_keyword_symbol(literal)) != -1)
				set_token(TKN_KEYWORD, literal, sym);
			else
				set_token(TKN_LITERAL, literal, G_I);
			reset_literal();
			tokenize(chr);
		} break;
		default:
			compile_error(make_format_ddString("UNEXPECTED CHARACTER OR SYMBOL '%c'\n", chr).cstr);
			break;
	}
}

static inline char next(void)
{
	return (fileCount < file.length) ? file.cstr[fileCount++] : 0;
}
static inline char peek(int rpos)
{
	return file.cstr[fileCount+rpos];
}
static inline void skip(char cend)
{
	char chr;
	while ((chr = next()) != cend && chr != 0);
}
static inline void reset_literal(void)
{
	literal = make_ddString_length(" ", 25);
	literal.length = 0;
}
static int get_keyword_symbol(ddString value)
{
	if (ddString_compare(value, keywords[0]))
		return G_KW_IF;
	else if (ddString_compare(value, keywords[1]))
		return G_KW_WHILE;
	else if (ddString_compare(value, keywords[2]))
		return G_KW_FOR;
	else if (ddString_compare(value, keywords[3]))
		return G_KW_SUB;
	else if (ddString_compare(value, keywords[4]))
		return G_KW_FUN;
	else if (ddString_compare(value, keywords[5]))
		return G_KW_FORMAT;
	else if (ddString_compare(value, keywords[6]))
		return G_KW_RETURN;
	else if (ddString_compare(value, keywords[7]))
		return G_KW_ISO;
	else if (ddString_compare(value, keywords[8]))
		return G_KW_GLOBAL;
	else if (ddString_compare(value, keywords[9]))
		return G_KW_CONTINUE;
	else if (ddString_compare(value, keywords[10]))
		return G_KW_MALLOC;
	else if (ddString_compare(value, keywords[11]))
		return G_KW_EXTERN;
	else if (ddString_compare(value, keywords[12]))
		return G_KW_FORMAT;
	else return -1;
}
struct token* oset_token(int type, ddString value, int symbol)
{
	tokens[otokenCount].type = type;
	tokens[otokenCount].value = value;
	tokens[otokenCount].symbol = symbol;
	return &(tokens[otokenCount++]);
}
static inline void set_token(int type, ddString value, int symbol)
{
	tokens[tokenCount].type = type;
	tokens[tokenCount].value = value;
	tokens[tokenCount].symbol = symbol;
	tokenCount++;
	otokenCount++;
}
static bool is_last_linebreak(void)
{
	return (tokens[tokenCount-1].type == TKN_LINEBREAK);
}

#endif

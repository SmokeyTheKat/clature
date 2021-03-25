#ifndef __clature_lexer_h__
#define __clature_lexer_h__ 

#include "./ddcLib/ddcString.h"
#include "./ddcLib/ddcPrint.h"
#include <stdarg.h>

#include "./utils.h"
#include "./parser.h"

struct token* lexer_main(ddString _file, sizet* _tokenCount);
static void tokenize(char chr);
static inline char next(void);
static inline char peek(int rpos);
static inline void skip(char cend);
static int get_keyword_symbol(ddString value);
static inline void reset_literal(void);
static inline void set_token(int type, ddString value, int symbol);

static char* fileStream;
static char* fileStreamEnd;
static struct token* tokenStream;
static sizet tokenPos;
static ddString literal;
static bool inBT;// in syntax '@8<10>' when stream is at '<' set true so '>' is known to be closer for array def

#define is_last_linebreak() (tokenStream[tokenPos-1].type == TKN_SEMI)

struct token* lexer_main(ddString _file, sizet* _tokenCount)
{
	tokenStream = make(struct token, MAX_TOKEN_SIZE);
	literal = make_ddString("");
	inBT = false;
	tokenPos = 0;
	fileStream = _file.cstr;
	fileStreamEnd = fileStream + _file.length;
	while (fileStream <= fileStreamEnd)
	{
		tokenize(next());
	}
	if (!is_last_linebreak())
		set_token(TKN_SEMI, make_ddString_length_f(";", 1), G_SEMI);
	(*_tokenCount) =  tokenPos;
	return tokenStream;
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
				set_token(TKN_SEMI, make_ddString_length_f(";", 1), G_SEMI);
			break;
		case '\n':
			if (!is_last_linebreak())
				set_token(TKN_SEMI, make_ddString_length_f(";", 1), G_SEMI);
			break;
		case '.':
		{
			if (is_last_linebreak())
			{
				do
				{
					ddString_push_char_back(&literal, chr);
				} while ((chr = next()) != '\n' && chr != ';');
				set_token(TKN_ASM, literal, G_ILASM);
				reset_literal();
				if (chr != '\n') skip('\n');
				set_token(TKN_SEMI, make_ddString_length_f(";", 1), G_SEMI);
				break;
			}
		} break;
		case '[':
			set_token(TKN_OBS, make_ddString_length_f("[", 1), G_OBS);
			break;
		case ']':
			set_token(TKN_CBS, make_ddString_length_f("]", 1), G_CBS);
			break;
		case '(':
			set_token(TKN_OBP, make_ddString_length_f("(", 1), G_OBP);
			break;
		case ')':
			set_token(TKN_CBP, make_ddString_length_f(")", 1), G_CBP);
			break;
		case ',':
			set_token(TKN_COMMA, make_ddString_length_f(",", 1), G_COMMA);
			break;
		case ':':
			set_token(TKN_COLON, make_ddString_length_f(":", 1), G_COLON);
			break;
		case '{':
			if (!is_last_linebreak())
				set_token(TKN_SEMI, make_ddString_length_f(";", 1), G_SEMI);
			set_token(TKN_OBU, make_ddString_length_f("{", 1), G_OBU);
			set_token(TKN_SEMI, make_ddString_length_f(";", 1), G_SEMI);
			break;
		case '}':
			if (!is_last_linebreak())
				set_token(TKN_SEMI, make_ddString_length_f(";", 1), G_SEMI);
			set_token(TKN_CBU, make_ddString_length_f("}", 1), G_CBU);
			set_token(TKN_SEMI, make_ddString_length_f(";", 1), G_SEMI);
			break;
		case '@':
			set_token(TKN_AT, make_ddString_length_f("@", 1), G_AT);
			break;
		case '?':
			set_token(TKN_QUEST, make_ddString_length_f("?", 1), G_QUEST);
			break;
		case '*':
		{
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_MUL_EQUALS, make_ddString_length_f("*=", 2), G_EQ);
					next();
					break;
				default:
					set_token(TKN_MUL, make_ddString_length_f("*", 1), G_PO);
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
					set_token(TKN_DIV_EQUALS, make_ddString_length_f("/=", 1), G_EQ);
					next();
					break;
				default:
					set_token(TKN_DIV, make_ddString_length_f("/", 1), G_PO);
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
					set_token(TKN_EQUALS_EQUALS, make_ddString_length_f("==", 2), G_QO);
					next();
					break;
				default:
					set_token(TKN_EQUALS, make_ddString_length_f("=", 1), G_EQ);
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
					set_token(TKN_B_NOT_EQUALS, make_ddString_length_f("~=", 2), G_EQ);
					next();
					break;
				default:
					set_token(TKN_B_NOT, make_ddString_length_f("~", 1), G_EO);
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
					set_token(TKN_NOT_EQUALS, make_ddString_length_f("!=", 2), G_QO);
					next();
					break;
				default:
					set_token(TKN_NOT, make_ddString_length_f("!", 1), '!');
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
					set_token(TKN_MOD_EQUALS, make_ddString_length_f("%=", 2), G_EQ);
					next();
					break;
				default:
					set_token(TKN_MOD, make_ddString_length_f("%", 1), G_EO);
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
					set_token(TKN_SUB_EQUALS, make_ddString_length_f("-=", 2), G_EQ);
					next();
					break;
				case '-':
					set_token(TKN_SUB_SUB, make_ddString_length_f("--", 2), G_MM);
					next();
					break;
				default:
					set_token(TKN_SUB, make_ddString_length_f("-", 1), G_SO);
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
					set_token(TKN_ADD_EQUALS, make_ddString_length_f("+=", 2), G_EQ);
					next();
					break;
				case '+':
					set_token(TKN_ADD_ADD, make_ddString_length_f("++", 2), G_PP);
					next();
					break;
				default:
					set_token(TKN_ADD, make_ddString_length_f("+", 1), G_SO);
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
					set_token(TKN_B_AND_EQUALS, make_ddString_length_f("&=", 2), G_EQ);
					next();
					break;
				case '&':
					set_token(TKN_L_AND, make_ddString_length_f("&&", 2), G_QO);
					next();
					break;
				default:
					set_token(TKN_B_AND, make_ddString_length_f("&", 1), G_EO);
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
					set_token(TKN_B_OR_EQUALS, make_ddString_length_f("|=", 2), G_EQ);
					next();
					break;
				case '|':
					set_token(TKN_L_OR, make_ddString_length_f("||", 2), G_QO);
					next();
					break;
				default:
					set_token(TKN_B_OR, make_ddString_length_f("|", 1), G_EO);
					break;
			}
			break;
		}
		case '<':
		{
			if (tokenStream[tokenPos-2].value.cstr[0] == '@')
			{
				set_token(TKN_OBT, make_ddString_length_f("<", 1), G_OBT);
				inBT = true;
				break;
			}
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_LESS_THAN_EQUALS, make_ddString_length_f("<=", 2), G_EQ);
					next();
					break;
				case '<':
					if (peek(1) == '<')
					{
						set_token(TKN_TSHL, make_ddString_length_f("<<<", 3), G_EO);
						next();
					}
					else set_token(TKN_SHIFT_LEFT, make_ddString_length_f("<<", 2), G_EO);
					next();
					break;
				default:
					set_token(TKN_LESS_THAN, make_ddString_length_f("<", 1), G_QO);
					break;
			}
			break;
		}
		case '>':
		{
			if (inBT)
			{
				set_token(TKN_CBT, make_ddString_length_f(">", 1), G_CBT);
				inBT = false;
				break;
			}
			char cnx = peek(0);
			switch (cnx)
			{
				case '=':
					set_token(TKN_GREATER_THAN_EQUALS, make_ddString_length_f(">=", 2), G_EQ);
					next();
					break;
				case '>':
					if (peek(1) == '>')
					{
						set_token(TKN_TSHR, make_ddString_length_f(">>>", 3), G_EO);
						next();
					}
					else set_token(TKN_SHIFT_RIGHT, make_ddString_length_f(">>", 2), G_EO);
					next();
					break;
				default:
					set_token(TKN_GREATER_THAN, make_ddString_length_f(">", 1), G_QO);
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
			set_token(TKN_CHAR_LITERAL, literal, G_I);
			reset_literal();
		} break;
		case '"':
		{
			do
			{
				ddString_push_char_back(&literal, chr);
			} while ((chr = next()) != '"');
			ddString_push_char_back(&literal, chr);
			set_token(TKN_STRING, literal, G_I);
			reset_literal();
		} break;
		case '`':
		{
			do
			{
				ddString_push_char_back(&literal, chr);
			} while ((chr = next()) != '`');
			ddString_push_char_back(&literal, chr);
			set_token(TKN_STRING, literal, G_I);
			reset_literal();
		} break;
		case '0' ... '9':
		{
			do
			{
				ddString_push_char_back(&literal, chr);
			} while ((chr = next()) >= '0' && chr <= '9');
			set_token(TKN_NUMBER, literal, G_N);
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
				set_token(TKN_ID, literal, G_I);
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
	return *(fileStream++);
}

static inline char peek(int rpos)
{
	return *(fileStream+rpos);
}

static inline void skip(char cend)
{
	char chr;
	while ((chr = next()) != cend && chr != 0);
}

static inline void set_token(int type, ddString value, int symbol)
{
	tokenStream[tokenPos].type = type;
	tokenStream[tokenPos].value = value;
	tokenStream[tokenPos].symbol = symbol;
	tokenPos++;
}

static inline void reset_literal(void)
{
	literal = make_ddString_length_fa(" ", 25);
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
	else if (ddString_compare(value, keywords[13]))
		return G_KW_ELSE;
	else return -1;
}

#endif

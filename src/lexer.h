#ifndef __ddScript_lexer_h__
#define __ddScript_lexer_h__ 
#include <ddcString.h>
#include <ddcPrint.h>
#include <stdarg.h>

#include "./utils.h"

#define TKN_LITERAL 0x00
#define TKN_KEYWORD 0x01
#define TKN_OPERATOR 0x02
#define TKN_SYNTAX 0x03
#define TKN_ASSEMBLY 0x04
#define TKN_FUNCTION 0x05
#define TKN_LINEBREAK 0x06
#define TKN_STRING 0x07
#define TKN_NONTERMINAL 0x08

enum
{
        G_NULL = -1,
        G_A = 2,
        G_S = 295,
        G_P,
        G_E,
        G_Q,
        G_V,
        G_I,
        G_N,
        G_KN,
        G_KF,
        G_Z,
        G_F,
        G_FP,
        G_CP,
        G_SO,
        G_PO,
        G_EO,
        G_QO,
        G_VO,
        G_IO,
        G_PB,
        G_PP,
        G_MM,
        G_ILASM,
	G_KW_IF,
	G_KW_WHILE,
	G_KW_FOR,
	G_KW_SUB,
	G_KW_FUN,
	G_KW_RETURN,
	G_KW_ISO,
	G_KW_GLOBAL,
	G_KW_CONTINUE,
	G_KW_MALLOC,
	G_KW_EXTERN,
	G_KW_FORMAT,
        G_EQ = '=',
        G_OBP = '(',
        G_CBP = ')',
        G_OBS = '[',
        G_CBS = ']',
        G_OBT = '<',
        G_CBT = '>',
        G_AT = '@',
        G_QUESTION = '?',
        G_COLON = ':',
        G_SEMI = ';',
        G_COMMA = ',',
};

struct token;

static inline char next(void);
static inline char peek(int rpos);
static inline bool is_number(char chr);
static int get_keyword_symbol(void);
static bool is_string(void);
static inline void set_literal(void);
static inline void set_token(int type, ddString value, int symbol);
static void handel_literal(char chr);
static inline void goto_next_line(void);
static bool is_unclosed_string(void);
static bool is_last_linebreak(void);
static bool is_ds_number(ddString str);
static void try_tkns(int type, char leftOpc, int len, ...);
static void sift_token(char chr);
static ddString tokenize_until_semicolon(void);
void init_lexer(void);
struct token* tokenize_file(ddString _file, sizet* tokenCount);
void get_token(void);

static ddString file;
static ddString literal;
static bool inBT = false;
static bool inLiteral = false;
static bool inString = false;
static sizet fileCount = 0;
static sizet tokenCount = 0;
static struct token* tokens;
ddString keywords[13];

struct token
{
	int type;
	ddString value;
	int symbol;
};

const char* const TKN_STRS[] = {
	"LITERAL",
	"KEYWORD",
	"OPERATOR",
	"SYNTAX",
	"ASSEMBLY",
	"FUNCTION", "LINEBREAK",
	"STRING"
};

void init_lexer(void)
{
	literal = make_ddString("");
	
	tokens = make(struct token, 100000000);

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

struct token* tokenize_file(ddString _file, sizet* _tokenCount)
{
	init_lexer();
	file = _file;
	while (fileCount < _file.length)
	{
		get_token();
	}
	if (!is_last_linebreak())
			set_token(TKN_LINEBREAK, make_ddString_length(";", 1), G_SEMI);
	(*_tokenCount) = tokenCount;
	return tokens;
}

void get_token(void)
{
	char chr = next();
	sift_token(chr);
}

static void sift_token(char chr)
{
	if (inString)
	{
		if (chr == '"' || chr == '`') inString = false;
		if (chr == '\n' && is_unclosed_string()) compile_error("UNCLOSED STRING LITERAL\n");
		handel_literal(chr);
		return;
	}
	switch (chr)
	{
		case ' ': case '	':
			if (inLiteral) set_literal();
			break;
		case ';':
			if (inLiteral) set_literal();
			goto_next_line();
			if (!is_last_linebreak())
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1), G_SEMI);
			break;
		case '\n':
			if (!is_last_linebreak())
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1), G_SEMI);
			break;
		case '.':
			if (is_last_linebreak())
			{
				set_token(TKN_ASSEMBLY, tokenize_until_semicolon(), G_ILASM);
				goto_next_line();
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1), G_SEMI);
				break;
			}
			break;
		case '[': case ']': case '(': case ')': case ',': case ':':
			set_token(TKN_SYNTAX, make_ddString_length(&chr, 1), chr);
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
			set_token(TKN_OPERATOR, make_ddString_length("?", 1), G_QUESTION);
			break;
		case '*':
		{
			char cnx = next();
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
			char cnx = next();
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
			char cnx = next();
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
			//if (!is_number( peek(0)))
			//	handel_literal(chr);
			//else
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
			if (tokens[tokenCount-1].value.cstr[0] == '@')
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
			handel_literal(chr);
			chr = next();
			handel_literal(chr);
			if (chr == '\\')
			{
				chr = next();
				handel_literal(chr);
			}
			chr = next();
			if (chr != '\'') compile_error("UNCLOSED CHARACTER LITERAL\n");
			handel_literal(chr);
			break;
		case '"':
		case '`':
			inString = !inString;
		case '0' ... '9':
		case 'a' ... 'z':
		case 'A' ... 'Z':
		case '_':
			handel_literal(chr);
			break;
		default:
			compile_error(make_format_ddString("UNEXPECTED CHARACTER OR SYMBOL '%c'\n", chr).cstr);
			break;
	}
}

static inline char next(void)
{
	return (fileCount < file.length) ? file.cstr[fileCount++] : 0;
	//return file.cstr[fileCount++];
}
static inline char peek(int rpos)
{
	return file.cstr[fileCount+rpos];
}
static inline bool is_number(char chr)
{
	return (chr >= 48 && chr < 58);
}
static int get_keyword_symbol(void)
{
	if (ddString_compare(tokens[tokenCount].value, keywords[0]))
	{
		return G_KW_IF;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[1]))
	{
		return G_KW_WHILE;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[2]))
	{
		return G_KW_FOR;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[3]))
	{
		return G_KW_SUB;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[4]))
	{
		return G_KW_FUN;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[5]))
	{
		return G_KW_FORMAT;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[6]))
	{
		return G_KW_RETURN;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[7]))
	{
		return G_KW_ISO;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[8]))
	{
		return G_KW_GLOBAL;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[9]))
	{
		return G_KW_CONTINUE;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[10]))
	{
		return G_KW_MALLOC;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[11]))
	{
		return G_KW_EXTERN;
	}
	if (ddString_compare(tokens[tokenCount].value, keywords[12]))
	{
		return G_KW_FORMAT;
	}
	return -1;
}
static bool is_string(void)
{
	if (tokens[tokenCount].value.cstr[tokens[tokenCount].value.length-1] == '"' || tokens[tokenCount].value.cstr[tokens[tokenCount].value.length-1] == '`')
		return true;
	return false;
}
static void handel_literal(char chr)
{
	if (!inLiteral) inLiteral = true;
	ddString_push_char_back(&literal, chr);
}
static inline void set_literal(void)
{
	literal.cstr[literal.length] = '\0';
	inLiteral = false;
	tokens[tokenCount].type = TKN_LITERAL;
	tokens[tokenCount].value = literal;
	if (is_ds_number(tokens[tokenCount].value))
		tokens[tokenCount].symbol = G_N;
	else
		tokens[tokenCount].symbol = G_I;
	int kwsymbol;
	if ((kwsymbol = get_keyword_symbol()) != -1) { tokens[tokenCount].type = TKN_KEYWORD; 
						       tokens[tokenCount].symbol = kwsymbol; }
	else if (is_string()) tokens[tokenCount].type = TKN_STRING;
	tokenCount++;
	literal = make_ddString("");
}
static inline void set_token(int type, ddString value, int symbol)
{
	if (inLiteral) set_literal();
	tokens[tokenCount].type = type;
	tokens[tokenCount].value = value;
	tokens[tokenCount].symbol = symbol;
	tokenCount++;
}
static inline void goto_next_line(void)
{
	char chr;
	while ((chr = next()) != '\n' && chr != 0);
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
	for (sizet i = start; i < tokenCount; i++)
		if (tokens[i].value.cstr[0] == ';') return i;
	return 0;
}
static bool is_unclosed_string(void)
{
	if (inString) return true;
	return false;
}
static bool is_last_linebreak(void)
{
	if (tokens[tokenCount-1].type == TKN_LINEBREAK) return true;
	return false;
}
static void try_tkns(int type, char leftOpc, int len, ...)
{
	char chr = next();
	va_list ap;
	va_start(ap, len);
	for (int i = 0; i < len; i++)
	{
		char rightOpc = (char)va_arg(ap, int);
		if (chr == rightOpc)
		{
			ddString value = make_ddString_length(&leftOpc, 2);
			value.cstr[value.length-1] = rightOpc;
			set_token(type, value, rightOpc);
			va_end(ap);
			fileCount++;
			return;
		}
	}
	set_token(type, make_ddString_length(&leftOpc, 1), leftOpc);
	sift_token(chr);
}
static ddString tokenize_until_semicolon(void)
{
	ddString output = make_ddString("");
	for (sizet i = fileCount-1; file.cstr[i] != ';'; i++)
	{
		ddString_push_char_back(&output, file.cstr[i]);
	}
	return output;
}
static bool is_ds_number(ddString str)
{
	for (int i = 0; i < str.length; i++)
	{
		if (!(str.cstr[i] >=48 && str.cstr[i] <= 57)) return false;
	}
	return true;
}

#endif

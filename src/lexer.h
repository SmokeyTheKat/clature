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

struct token;

static inline char read_char(void);
static bool is_keyword(void);
static inline void set_literal(void);
static inline void set_token(int type, ddString value);
static void handel_literal(char chr);
static inline void goto_next_line(void);
static bool is_unclosed_string(void);
static bool is_last_linebreak(void);
static void try_tkns(int type, char leftOpc, int len, ...);
static void sift_token(char chr);
void init_lexer(void);
struct token* tokenize_file(ddString _file, sizet* tokenCount);
void get_token(void);

static ddString file;
static ddString literal;
static bool inLiteral = false;
static bool inString = false;
static sizet fileCount = 0;
static sizet tokenCount = 0;
static struct token* tokens;
ddString keywords[9];

struct token
{
	int type;
	ddString value;
};

const char* const TKN_STRS[] = {
	"LITERAL",
	"KEYWORD",
	"OPERATOR",
	"SYNTAX",
	"ASSEMBLY",
	"FUNCTION",
	"LINEBREAK"
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
	keywords[5] = make_constant_ddString("struct");
	keywords[6] = make_constant_ddString("return");
	keywords[7] = make_constant_ddString("iso");
	keywords[8] = make_constant_ddString("global");
}

struct token* tokenize_file(ddString _file, sizet* _tokenCount)
{
	file = _file;
	while (fileCount < _file.length)
	{
		get_token();
	}
	(*_tokenCount) = tokenCount;
	return tokens;
}

void get_token()
{
	char chr = read_char();
	sift_token(chr);
}

static void sift_token(char chr)
{
	if (inString)
	{
		if (chr == '"') inString = false;
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
			goto_next_line();
			set_token(TKN_LINEBREAK, make_ddString_length(";", 1));
			break;
		case '\n':
			if (!is_last_linebreak())
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1));
			break;
		case '[': case ']': case '(': case ')': case ',': case '.':
			set_token(TKN_SYNTAX, make_ddString_length(&chr, 1));
			break;
		case '{': case '}':
			if (!is_last_linebreak())
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1));
			set_token(TKN_SYNTAX, make_ddString_length(&chr, 1));
			set_token(TKN_LINEBREAK, make_ddString_length(";", 1));
			break;
		case '@':
			set_token(TKN_OPERATOR, make_ddString_length("@", 1));
			break;
		case '*': case '/': case '~': case '!': case '=':
			try_tkns(TKN_OPERATOR, chr, 1, '=');
			break;
		case '&': case '|': case '<': case '>': case '+': case '-':
			try_tkns(TKN_OPERATOR, chr, 2, '=', chr);
			break;
		case '\'':
			handel_literal(chr);
			chr = read_char();
			handel_literal(chr);
			chr = read_char();
			if (chr != '\'') compile_error("UNCLOSED CHARACTER LITERAL\n");
			handel_literal(chr);
			break;
		case '"':
			inString = !inString;
		case '0' ... '9':
		case 'a' ... 'z':
		case 'A' ... 'Z':
			handel_literal(chr);
			break;
		default:
			compile_error(make_format_ddString("UNEXPECTED CHARACTER OR SYMBOL '%c'\n", chr).cstr);
			break;
	}
}

static inline char read_char(void)
{
	//return (fileCount < file.length) ? file.cstr[fileCount++] : 0;
	return file.cstr[fileCount++];
}
static bool is_keyword(void)
{
	for (int i = 0; i < 9; i++)
		if (ddString_compare(tokens[tokenCount].value, keywords[i]))
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
	inLiteral = false;
	tokens[tokenCount].type = TKN_LITERAL;
	tokens[tokenCount].value = literal;
	if (is_keyword()) tokens[tokenCount].type = TKN_KEYWORD;
	tokenCount++;
	literal = make_ddString("");
}
static inline void set_token(int type, ddString value)
{
	if (inLiteral) set_literal();
	tokens[tokenCount].type = type;
	tokens[tokenCount].value = value;
	tokenCount++;
}
static inline void goto_next_line(void)
{
	while (read_char() != '\n');
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
	char chr = read_char();
	va_list ap;
	va_start(ap, len);
	for (int i = 0; i < len; i++)
	{
		char rightOpc = (char)va_arg(ap, int);
		if (chr == rightOpc)
		{
			ddString value = make_ddString_length(&leftOpc, 2);
			value.cstr[value.length-1] = rightOpc;
			set_token(type, value);
			va_end(ap);
			return;
		}
	}
	set_token(type, make_ddString_length(&leftOpc, 1));
	sift_token(chr);
}
#endif

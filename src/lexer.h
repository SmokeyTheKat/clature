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

struct token;

static inline char read_char(void);
static inline char read_next_char(void);
static inline bool is_number(char chr);
static bool is_keyword(void);
static bool is_string(void);
static inline void set_literal(void);
static inline void set_token(int type, ddString value);
static void handel_literal(char chr);
static inline void goto_next_line(void);
static bool is_unclosed_string(void);
static bool is_last_linebreak(void);
static void try_tkns(int type, char leftOpc, int len, ...);
static void sift_token(char chr);
static ddString tokenize_until_semicolon(void);
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
ddString keywords[13];

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
	keywords[5] = make_constant_ddString("struct");
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
			set_token(TKN_LINEBREAK, make_ddString_length(";", 1));
	(*_tokenCount) = tokenCount;
	return tokens;
}

void get_token(void)
{
	char chr = read_char();
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
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1));
			break;
		case '\n':
			if (!is_last_linebreak())
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1));
			break;
		case '.':
			if (is_last_linebreak())
			{
				set_token(TKN_ASSEMBLY, tokenize_until_semicolon());
				goto_next_line();
				set_token(TKN_LINEBREAK, make_ddString_length(";", 1));
				break;
			}
			break;
		case '[': case ']': case '(': case ')': case ',': case ':':
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
		case '?':
			set_token(TKN_OPERATOR, make_ddString_length("?", 1));
			break;
		case '*': case '/': case '~': case '!': case '=': case '%':
			try_tkns(TKN_OPERATOR, chr, 1, '=');
			break;
		case '-':
			//if (!is_number(read_next_char()))
			//	handel_literal(chr);
			//else
				try_tkns(TKN_OPERATOR, chr, 2, '=', chr);
			break;
		case '&': case '|': case '<': case '>': case '+':
			try_tkns(TKN_OPERATOR, chr, 2, '=', chr);
			break;
		case '\'':
			handel_literal(chr);
			chr = read_char();
			handel_literal(chr);
			if (chr == '\\')
			{
				chr = read_char();
				handel_literal(chr);
			}
			chr = read_char();
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

static inline char read_char(void)
{
	return (fileCount < file.length) ? file.cstr[fileCount++] : 0;
	//return file.cstr[fileCount++];
}
static inline char read_next_char(void)
{
	return file.cstr[fileCount];
}
static inline bool is_number(char chr)
{
	return (chr >= 48 && chr < 58);
}
static bool is_keyword(void)
{
	for (int i = 0; i < 13; i++)
		if (ddString_compare(tokens[tokenCount].value, keywords[i]))
			return true;
	return false;
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
	if (is_keyword()) tokens[tokenCount].type = TKN_KEYWORD;
	else if (is_string()) tokens[tokenCount].type = TKN_STRING;
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
	char chr;
	while ((chr = read_char()) != '\n' && chr != 0);
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
			fileCount++;
			return;
		}
	}
	set_token(type, make_ddString_length(&leftOpc, 1));
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

#endif

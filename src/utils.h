#ifndef __ddScript_utils_h__
#define __ddScript_utils_h__

#include <ddcDef.h>
#include <ddcString.h>
#include <ddcPrint.h>
#include "./args.h"

#define TKN_LITERAL 0x00
#define TKN_KEYWORD 0x01
#define TKN_OPERATOR 0x02
#define TKN_SYNTAX 0x03
#define TKN_ASSEMBLY 0x04
#define TKN_FUNCTION 0x05
#define TKN_LINEBREAK 0x06
#define TKN_STRING 0x07
#define TKN_NONTERMINAL 0x08

void compile_reset_color(void);
void compile_message(const char* message);
void compile_warning(const char* warning);
void compile_error(const char* error);

struct token
{
	int type;
	ddString value;
	int symbol;
};

struct tokenNode
{
	struct tokenNode** nodes;
	sizet nodeCount;
	struct token* value;
};
struct action
{
	int(*action)(int);
	int value;
};
struct syntax
{
	int result;
	int length;
};


enum
{
        G_PP,
        G_PO,
        G_QO,
        G_EO,
        G_MM,
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
        G_COMMA = ',',
};

const char* const TKN_STRS[] = {
	"LITERAL",
	"KEYWORD",
	"OPERATOR",
	"SYNTAX",
	"ASSEMBLY",
	"FUNCTION",
	"LINEBREAK",
	"STRING"
};

void compile_reset_color(void)
{
	ddPrint_cstring("\x1b[38;2;255;255;255m");
}
void compile_message(const char* message)
{
	compile_reset_color();
	if (!args_if_def(make_constant_ddString("--silent")))
		ddPrint_cstring_nl(message);
}
void compile_warning(const char* warning)
{
	if (!args_if_def(make_constant_ddString("--silent")))
	{
		ddPrint_cstring("[\x1b[38;2;255;255;0mWARNING\x1b[38;2;255;255;255m] ");
		ddPrint_cstring_nl(warning);
	}
}
void compile_error(const char* error)
{
	ddPrintf("[\x1b[38;2;255;0;0mERROR\x1b[38;2;255;255;255m] %s", error);
	exit(0);
}

#endif

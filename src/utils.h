#ifndef __clature_utils_h__
#define __clature_utils_h__

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

typedef char byte;

void init_compiler(void);
void compile_reset_color(void);
void compile_message(const char* message);
void compile_warning(const char* warning);
void compile_error(const char* error);

ddString keywords[14];

struct token;
struct tokenNode;
struct action;
struct syntax;

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
        G_KW_FUN,
        G_KW_ISO,
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
	exit(1);
}

void init_compiler(void)
{
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
	keywords[13] = make_constant_ddString("else");
}

ddString make_ddString_length_f(const char* _c, ddsize _l)
{
	ddString _o;
	_o.status = DOS_ACTIVE;
	_o.aDelete = DOD_MANUAL;
	_o.type = DDSTRING_CONSTANT;

	_o.length = _l;
	_o.capacity = _l;

	_o.cstr = _c;

	return _o;
}

ddString make_ddString_length_fa(const char* _c, ddsize _l)
{
	ddString _o;
	_o.status = DOS_ACTIVE;
	_o.aDelete = DOD_MANUAL;
	_o.type = DDSTRING_DYNAMIC;

	_o.length = _l;
	_o.capacity = _l + __BYINC;

	_o.cstr = make(char, _o.capacity);
	cstring_copy(_o.cstr, _c, _o.length);
	ddString_close(_o);
	return _o;
}

#endif

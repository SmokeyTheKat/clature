#ifndef __clature_utils_h__
#define __clature_utils_h__

#include "./ddcLib/ddcDef.h"
#include "./ddcLib/ddcString.h"
#include "./ddcLib/ddcPrint.h"
#include "./args.h"

enum
{
	OPATR_DEREF=200,
	OPATR_BDEREF=201,
	OPATR_WDEREF=202,
	OPATR_DDEREF=204,
	OPATR_QDEREF=208,
};
enum
{
	TKN_SEMI,
	TKN_COLON,
	TKN_OBU,
	TKN_CBU,
	TKN_ASM,
	TKN_OBS,
	TKN_CBS,
	TKN_OBP,
	TKN_CBP,
	TKN_COMMA,
	TKN_AT,
	TKN_QUEST,
	TKN_MUL_EQUALS,
	TKN_MUL,
	TKN_DIV_EQUALS,
	TKN_DIV,
	TKN_EQUALS_EQUALS,
	TKN_EQUALS,
	TKN_B_NOT_EQUALS,
	TKN_B_NOT,
	TKN_NOT_EQUALS,
	TKN_NOT,
	TKN_MOD_EQUALS,
	TKN_MOD,
	TKN_SUB_EQUALS,
	TKN_SUB_SUB,
	TKN_SUB,
	TKN_ADD_EQUALS,
	TKN_ADD_ADD,
	TKN_ADD,
	TKN_B_AND_EQUALS,
	TKN_L_AND,
	TKN_B_AND,
	TKN_B_OR_EQUALS,
	TKN_L_OR,
	TKN_B_OR,
	TKN_OBT,
	TKN_LESS_THAN_EQUALS,
	TKN_TSHL,
	TKN_SHIFT_LEFT,
	TKN_LESS_THAN,
	TKN_CBT,
	TKN_GREATER_THAN_EQUALS,
	TKN_TSHR,
	TKN_SHIFT_RIGHT,
	TKN_GREATER_THAN,
	TKN_CHAR_LITERAL,
	TKN_STRING,
	TKN_NUMBER,
	TKN_KEYWORD,
	TKN_ID,
};

const char* tkn_strs[400] = {
	[TKN_SEMI]=";",
	[TKN_COLON]=":",
	[TKN_OBU]="{",
	[TKN_CBU]="}",
	[TKN_ASM]=".asm",
	[TKN_OBS]="[",
	[TKN_CBS]="]",
	[TKN_OBP]="(",
	[TKN_CBP]=")",
	[TKN_COMMA]=",",
	[TKN_AT]="@",
	[TKN_QUEST]="?",
	[TKN_MUL_EQUALS]="*=",
	[TKN_MUL]="*",
	[TKN_DIV_EQUALS]="/=",
	[TKN_DIV]="/",
	[TKN_EQUALS_EQUALS]="==",
	[TKN_EQUALS]="=",
	[TKN_B_NOT_EQUALS]="~=",
	[TKN_B_NOT]="~",
	[TKN_NOT_EQUALS]="!=",
	[TKN_NOT]="!",
	[TKN_MOD_EQUALS]="%=",
	[TKN_MOD]="%",
	[TKN_SUB_EQUALS]="-=",
	[TKN_SUB_SUB]="--",
	[TKN_SUB]="-",
	[TKN_ADD_EQUALS]="+=",
	[TKN_ADD_ADD]="++",
	[TKN_ADD]="+",
	[TKN_B_AND_EQUALS]="&=",
	[TKN_L_AND]="&&",
	[TKN_B_AND]="&",
	[TKN_B_OR_EQUALS]="|=",
	[TKN_L_OR]="||",
	[TKN_B_OR]="|",
	[TKN_OBT]="<",
	[TKN_LESS_THAN_EQUALS]="<=",
	[TKN_TSHL]="<<<",
	[TKN_SHIFT_LEFT]="<<",
	[TKN_LESS_THAN]="<",
	[TKN_CBT]=">",
	[TKN_GREATER_THAN_EQUALS]=">=",
	[TKN_TSHR]=">>>",
	[TKN_SHIFT_RIGHT]=">>",
	[TKN_GREATER_THAN]=">",
	[TKN_CHAR_LITERAL]="'x'",
	[TKN_STRING]="\"xxx\"",
	[TKN_NUMBER]="#x",
	[TKN_KEYWORD]="KEY",
	[TKN_ID]="ID",
	[OPATR_DEREF]="@0",
	[OPATR_BDEREF]="@1",
	[OPATR_WDEREF]="@2",
	[OPATR_DDEREF]="@4",
	[OPATR_QDEREF]="@8",
};

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
	{
		ddPrint_cstring(message);
		ddPrint_nl();
	}
}
void compile_warning(const char* warning)
{
	if (!args_if_def(make_constant_ddString("--silent")))
	{
		ddPrint_cstring("[\x1b[38;2;255;255;0mWARNING\x1b[38;2;255;255;255m] ");
		ddPrint_cstring(warning);
		ddPrint_nl();
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

	_o.cstr = (char*)_c;

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

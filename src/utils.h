#ifndef __ddScript_utils_h__
#define __ddScript_utils_h__

#include <ddcDef.h>
#include <ddcString.h>
#include <ddcPrint.h>
#include "./args.h"

#define CNT_NUMBER        0x01
#define CNT_STRING        0x02
#define CNT_FUNCTION      0x03
#define CNT_DEREF         0x04
#define CNT_ADD           0x05
#define CNT_SUB           0x06
#define CNT_MUL           0x07
#define CNT_DIV           0x08

void compile_message(const char* message);
void compile_warning(const char* warning);
void compile_error(const char* error);
sizet parse_get_line_count(ddString file);
ddString left_split(ddString str, int ptr);
ddString right_split(ddString str, int ptr);
struct compileNode* make_compileNode(struct compileNode* parent, int type);

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
	ddPrint_cstring("[\x1b[38;2;255;255;0mWARNING\x1b[38;2;255;255;255m] ");
	ddPrint_cstring_nl(warning);
}
void compile_error(const char* error)
{
	ddPrintf("[\x1b[38;2;255;0;0mERROR\x1b[38;2;255;255;255m] %s", error);
	exit(0);
}

#endif

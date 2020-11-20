#ifndef __ddcomp_compile_h__
#define __ddcomp_compile_h__

union atype;
struct bitCommand;
struct bitCode;
struct bvar;
struct codeLine;

void compile_error(int code);

const char* const r_rax = "rax";
const char* const r_rbp = "rbp";
const char* const r_rdx = "rdx";
const char* const r_rsx = "rsx";

#define DC_MOV 0x01
#define DC_DEF1 0x02
#define DC_DEF2 0x03
#define DC_DEF4 0x04
#define DC_DEF8 0x05

#define DT_INT 0x01
#define DT_REG 0x02

struct bitCommand
{
	int command;
	int value1Type;
	int value1Offset;
	union atype r1;
	int value2Type;
	int value2Offset;
	union atype r2;
};
struct bitCode
{
	struct bitCommand* bcommands;
	sizet length;
	sizet stackSize;
};

struct bvar
{
	ddString name;
	int type;
	union atype value;
};

struct codeLine
{
	ddString* data;
	sizet length;
};

void compile_error(int code)
{
	ddPrint_cstring_nl("ERROR");
	exit(0);
}


#endif

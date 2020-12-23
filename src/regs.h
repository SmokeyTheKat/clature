#ifndef __ddScript_regs_h__
#define __ddScript_regs_h__

ddString REG_RAX;
ddString REG_RCX;
ddString REG_RDX;
ddString REG_RBX;
ddString REG_RSP;
ddString REG_RBP;
ddString REG_RSI;
ddString REG_RDI;
ddString REG_R8;
ddString REG_R9;
ddString REG_R10;
ddString REG_R11;
ddString REG_R12;
ddString REG_R13;
ddString REG_R14;
ddString REG_R15;

ddString REG_EAX;
ddString REG_ECX;
ddString REG_EDX;
ddString REG_EBX;
ddString REG_ESP;
ddString REG_EBP;
ddString REG_ESI;
ddString REG_EDI;
ddString REG_R8d;
ddString REG_R9d;
ddString REG_R10d;
ddString REG_R11d;
ddString REG_R12d;
ddString REG_R13d;
ddString REG_R14d;
ddString REG_R15d;

ddString REG_AX;
ddString REG_CX;
ddString REG_DX;
ddString REG_BX;
ddString REG_SP;
ddString REG_BP;
ddString REG_SI;
ddString REG_DI;
ddString REG_R8w;
ddString REG_R9w;
ddString REG_R10w;
ddString REG_R11w;
ddString REG_R12w;
ddString REG_R13w;
ddString REG_R14w;
ddString REG_R15w;

ddString REG_AL;
ddString REG_CL;
ddString REG_DL;
ddString REG_BL;

ddString REG_NONE;

void init_regs(void)
{
	REG_RAX = make_constant_ddString("RAX");
	REG_RCX = make_constant_ddString("RCX");
	REG_RDX = make_constant_ddString("RDX");
	REG_RBX = make_constant_ddString("RBX");
	REG_RSP = make_constant_ddString("RSP");
	REG_RBP = make_constant_ddString("RBP");
	REG_RSI = make_constant_ddString("RSI");
	REG_RDI = make_constant_ddString("RDI");
	REG_R8 = make_constant_ddString( "R8");
	REG_R9 = make_constant_ddString( "R9");
	REG_R10 = make_constant_ddString("R10");
	REG_R11 = make_constant_ddString("R11");
	REG_R12 = make_constant_ddString("R12");
	REG_R13 = make_constant_ddString("R13");
	REG_R14 = make_constant_ddString("R14");
	REG_R15 = make_constant_ddString("R15");

	REG_EAX =  make_constant_ddString("EAX");
	REG_ECX =  make_constant_ddString("ECX");
	REG_EDX =  make_constant_ddString("EDX");
	REG_EBX =  make_constant_ddString("EBX");
	REG_ESP =  make_constant_ddString("ESP");
	REG_EBP =  make_constant_ddString("EBP");
	REG_ESI =  make_constant_ddString("ESI");
	REG_EDI =  make_constant_ddString("EDI");
	REG_R8d =  make_constant_ddString("R8D");
	REG_R9d =  make_constant_ddString("R9D");
	REG_R10d = make_constant_ddString("R10D");
	REG_R11d = make_constant_ddString("R11D");
	REG_R12d = make_constant_ddString("R12D");
	REG_R13d = make_constant_ddString("R13D");
	REG_R14d = make_constant_ddString("R14D");
	REG_R15d = make_constant_ddString("R15D");

	REG_AX =   make_constant_ddString("AX");
	REG_CX =   make_constant_ddString("CX");
	REG_DX =   make_constant_ddString("DX");
	REG_BX =   make_constant_ddString("BX");
	REG_SP =   make_constant_ddString("SP");
	REG_BP =   make_constant_ddString("BP");
	REG_SI =   make_constant_ddString("SI");
	REG_DI =   make_constant_ddString("DI");
	REG_R8w  = make_constant_ddString("R8w");
	REG_R9w =  make_constant_ddString("R9w");
	REG_R10w = make_constant_ddString("R10w");
	REG_R11w = make_constant_ddString("R11w");
	REG_R12w = make_constant_ddString("R12w");
	REG_R13w = make_constant_ddString("R13w");
	REG_R14w = make_constant_ddString("R14w");
	REG_R15w = make_constant_ddString("R15w");

	REG_AL = make_constant_ddString("AL");
	REG_CL = make_constant_ddString("CL");
	REG_DL = make_constant_ddString("DL");
	REG_BL = make_constant_ddString("BL");

	REG_NONE = make_constant_ddString("");

}

#endif

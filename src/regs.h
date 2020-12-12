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
	REG_RAX = make_constant_ddString("rax");
	REG_RCX = make_constant_ddString("rcx");
	REG_RDX = make_constant_ddString("rdx");
	REG_RBX = make_constant_ddString("rbx");
	REG_RSP = make_constant_ddString("rsp");
	REG_RBP = make_constant_ddString("rbp");
	REG_RSI = make_constant_ddString("rsi");
	REG_RDI = make_constant_ddString("rdi");
	REG_R8 = make_constant_ddString("r8");
	REG_R9 = make_constant_ddString("r9");
	REG_R10 = make_constant_ddString("r10");
	REG_R11 = make_constant_ddString("r11");
	REG_R12 = make_constant_ddString("r12");
	REG_R13 = make_constant_ddString("r13");
	REG_R14 = make_constant_ddString("r14");
	REG_R15 = make_constant_ddString("r15");

	REG_EAX = make_constant_ddString("eax");
	REG_ECX = make_constant_ddString("ecx");
	REG_EDX = make_constant_ddString("edx");
	REG_EBX = make_constant_ddString("ebx");
	REG_ESP = make_constant_ddString("esp");
	REG_EBP = make_constant_ddString("ebp");
	REG_ESI = make_constant_ddString("esi");
	REG_EDI = make_constant_ddString("edi");
	REG_R8d = make_constant_ddString("r8D");
	REG_R9d = make_constant_ddString("r9D");
	REG_R10d = make_constant_ddString("r10D");
	REG_R11d = make_constant_ddString("r11D");
	REG_R12d = make_constant_ddString("r12D");
	REG_R13d = make_constant_ddString("r13D");
	REG_R14d = make_constant_ddString("r14D");
	REG_R15d = make_constant_ddString("r15D");

	REG_AX = make_constant_ddString("ax");
	REG_CX = make_constant_ddString("cx");
	REG_DX = make_constant_ddString("dx");
	REG_BX = make_constant_ddString("bx");
	REG_SP = make_constant_ddString("sp");
	REG_BP = make_constant_ddString("bp");
	REG_SI = make_constant_ddString("si");
	REG_DI = make_constant_ddString("di");
	REG_R8w = make_constant_ddString("r8W");
	REG_R9w = make_constant_ddString("r9W");
	REG_R10w = make_constant_ddString("r10W");
	REG_R11w = make_constant_ddString("r11W");
	REG_R12w = make_constant_ddString("r12W");
	REG_R13w = make_constant_ddString("r13W");
	REG_R14w = make_constant_ddString("r14W");
	REG_R15w = make_constant_ddString("r15W");

	REG_AL = make_constant_ddString("al");
	REG_CL = make_constant_ddString("cl");
	REG_DL = make_constant_ddString("dl");
	REG_BL = make_constant_ddString("bl");

	REG_NONE = make_constant_ddString("");

}

#endif

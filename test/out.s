section .data;
_69_str0:
	DW	`\x1b[38;2;255;0;0mFAILED\n`,0;
_69_str1:
	DW	`\x1b[38;2;0;255;0mPASSED\n`,0;
value:
	DQ	0;
section .text;
changeGlobal:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 0;
	PUSH	10;
	POP	QWORD[value];
.SC03:
	ADD	RSP, 0;
	POP	RBP;
	RET;
expect:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 0;
	PUSH	QWORD[RBP--24];
	PUSH	QWORD[RBP--16];
	POP	R8;
	POP	R9;
	CMP	R9, R8;
	SETNE	AL;
	MOVZX	R8, AL;
	PUSH	R8;
	POP	R8;
	CMP	R8, 0;
	JE	.SC11;
	PUSH	_69_str0;
	CALL	pstr;
	ADD	RSP, 8;
	PUSH	1;
	CALL	exit;
	ADD	RSP, 8;
.SC11:
	PUSH	_69_str1;
	CALL	pstr;
	ADD	RSP, 8;
.SC02:
	ADD	RSP, 0;
	POP	RBP;
	RET;
strlen:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 9;
	PUSH	0;
	POP	QWORD[RBP-8];
.WL10:
	PUSH	QWORD[RBP-8];
	POP	R8;
	INC	R8;
	PUSH	R8;
	POP	QWORD[RBP-8];
.SC10:
	PUSH	QWORD[RBP--16];
	PUSH	QWORD[RBP-8];
	POP	R8;
	POP	R9;
	ADD	R9, R8;
	PUSH	R9;
	POP	R8;
	MOVSX	RAX, BYTE[r8];
	PUSH	RAX;
	PUSH	0;
	POP	R8;
	POP	R9;
	CMP	R9, R8;
	SETNE	AL;
	MOVZX	R8, AL;
	PUSH	R8;
	POP	R8;
	CMP	R8, 1;
	JE	.WL10;
	PUSH	QWORD[RBP-8];
	POP	R8;
	JMP	.SC01;
.SC01:
	ADD	RSP, 9;
	POP	RBP;
	RET;
pstr:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 8;
	PUSH	QWORD[RBP--16];
	CALL	strlen;
	ADD	RSP, 8;
	PUSH	R8;
	POP	QWORD[RBP-8];
	mov	rax, 1;
	mov	rdi, 1;
	mov	rsi, [rbp+16];
	mov	rdx, [rbp-8];
	syscall;
.SC00:
	ADD	RSP, 8;
	POP	RBP;
	RET;
global main
main:
	EXTERN	malloc;
	EXTERN	free;
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 12;
	MOV	QWORD[RBP-8], RSI;
	MOV	DWORD[RBP-12], EDI;
	extern	exit;
	PUSH	5;
	POP	QWORD[value];
	CALL	changeGlobal;
	ADD	RSP, 0;
	PUSH	QWORD[value];
	PUSH	10;
	CALL	expect;
	ADD	RSP, 16;
	MOV	EAX, 0;
	POP	RBP;
	MOV	RAX, 60;
	MOV	RDI, 0;
	SYSCALL;

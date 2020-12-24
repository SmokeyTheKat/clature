section .data;
printNum:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 0;
	PUSH	QWORD[RBP--16];
	PUSH	48;
	POP	R8;
	POP	R9;
	ADD	R9, R8;
	PUSH	R9;
	POP	QWORD[RBP--16];
	mov	rax, 1;
	mov	rdi, 1;
	add	rbp, 16;
	mov	rsi, rbp;
	sub	rbp, 16;
	mov	rdx, 1;
	syscall;
.SC04:
	ADD	RSP, 0;
	POP	RBP;
	RET;
strlen:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 8;
	PUSH	0;
	POP	QWORD[RBP-8];
.WL14:
	PUSH	QWORD[RBP-8];
	POP	R8;
	INC	R8;
	PUSH	R8;
	POP	QWORD[RBP-8];
.SC14:
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
	JE	.WL14;
	PUSH	QWORD[RBP-8];
	POP	R8;
	JMP	.SC03;
.SC03:
	ADD	RSP, 8;
	POP	RBP;
	RET;
pnl:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 8;
	PUSH	10;
	POP	QWORD[RBP-8];
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 8;
	mov	rsi, rbp;
	add	rbp, 8;
	mov	rdx, 1;
	syscall;
.SC02:
	ADD	RSP, 8;
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
.SC01:
	ADD	RSP, 8;
	POP	RBP;
	RET;
numToString:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 32;
	PUSH	QWORD[RBP--32];
	PUSH	0;
	POP	R8;
	POP	R9;
	CMP	R9, R8;
	SETE	AL;
	MOVZX	R8, AL;
	PUSH	R8;
	POP	R8;
	CMP	R8, 0;
	JE	.SC10;
	PUSH	'0';
	PUSH	QWORD[RBP--24];
	POP	R8;
	POP	RAX;
	MOV	BYTE[R8], AL;
	PUSH	0;
	PUSH	QWORD[RBP--24];
	PUSH	1;
	POP	R8;
	POP	R9;
	ADD	R9, R8;
	PUSH	R9;
	POP	R8;
	POP	RAX;
	MOV	BYTE[R8], AL;
	PUSH	0;
	POP	R8;
	JMP	.SC00;
.SC10:
	PUSH	'+';
	POP	QWORD[RBP-8];
	PUSH	QWORD[RBP-8];
	PUSH	0;
	POP	R8;
	POP	R9;
	CMP	R9, R8;
	SETL	AL;
	MOVZX	R8, AL;
	PUSH	R8;
	POP	R8;
	CMP	R8, 0;
	JE	.SC11;
	PUSH	'-';
	POP	QWORD[RBP-8];
.SC11:
	PUSH	0;
	POP	QWORD[RBP-16];
.WL12:
	PUSH	'0';
	PUSH	QWORD[RBP--24];
	PUSH	QWORD[RBP-16];
	POP	R8;
	POP	R9;
	ADD	R9, R8;
	PUSH	R9;
	POP	R8;
	POP	RAX;
	MOV	BYTE[R8], AL;
	PUSH	QWORD[RBP-16];
	POP	R8;
	INC	R8;
	PUSH	R8;
	POP	QWORD[RBP-16];
.SC12:
	PUSH	QWORD[RBP-16];
	PUSH	QWORD[RBP--16];
	POP	R8;
	POP	R9;
	CMP	R9, R8;
	SETL	AL;
	MOVZX	R8, AL;
	PUSH	R8;
	POP	R8;
	CMP	R8, 1;
	JE	.WL12;
	PUSH	0;
	PUSH	QWORD[RBP--24];
	PUSH	QWORD[RBP--16];
	PUSH	1;
	POP	R8;
	POP	R9;
	SUB	R9, R8;
	PUSH	R9;
	POP	R8;
	POP	R9;
	ADD	R9, R8;
	PUSH	R9;
	POP	R8;
	POP	RAX;
	MOV	BYTE[R8], AL;
	PUSH	QWORD[RBP--16];
	PUSH	2;
	POP	R8;
	POP	R9;
	SUB	R9, R8;
	PUSH	R9;
	POP	QWORD[RBP-24];
	PUSH	0;
	POP	QWORD[RBP-32];
.WL13:
	PUSH	QWORD[RBP--32];
	PUSH	10;
	XOR	RAX, RAX;
	XOR	RDX, RDX;
	POP	R8;
	POP	RAX;
	DIV	R8;
	MOV	R8, RDX;
	XOR	RAX, RAX;
	XOR	RDX, RDX;
	PUSH	R8;
	POP	QWORD[RBP-32];
	PUSH	QWORD[RBP--32];
	PUSH	10;
	POP	R8;
	POP	RAX;
	DIV	R8;
	PUSH	RAX;
	POP	QWORD[RBP--32];
	PUSH	'0';
	PUSH	QWORD[RBP-32];
	POP	R8;
	POP	R9;
	ADD	R9, R8;
	PUSH	R9;
	PUSH	QWORD[RBP--24];
	PUSH	QWORD[RBP-24];
	POP	R8;
	POP	R9;
	ADD	R9, R8;
	PUSH	R9;
	POP	R8;
	POP	RAX;
	MOV	BYTE[R8], AL;
	PUSH	QWORD[RBP-24];
	POP	R8;
	DEC	R8;
	PUSH	R8;
	POP	QWORD[RBP-24];
.SC13:
	PUSH	QWORD[RBP--32];
	PUSH	0;
	POP	R8;
	POP	R9;
	CMP	R9, R8;
	SETG	AL;
	MOVZX	R8, AL;
	PUSH	R8;
	POP	R8;
	CMP	R8, 1;
	JE	.WL13;
	PUSH	QWORD[RBP-8];
	PUSH	QWORD[RBP--24];
	PUSH	QWORD[RBP-24];
	POP	R8;
	POP	R9;
	ADD	R9, R8;
	PUSH	R9;
	POP	R8;
	POP	RAX;
	MOV	BYTE[R8], AL;
.SC00:
	ADD	RSP, 32;
	POP	RBP;
	RET;
global main
main:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 43;
	MOV	R8, RBP;
	SUB	R8, 35;
	PUSH	R8;
	POP	QWORD[RBP-8];
	PUSH	0;
	POP	QWORD[RBP-43];
.WL05:
	PUSH	'a';
	PUSH	QWORD[RBP-43];
	POP	R8;
	POP	R9;
	ADD	R9, R8;
	PUSH	R9;
	PUSH	QWORD[RBP-8];
	PUSH	QWORD[RBP-43];
	POP	R8;
	POP	R9;
	ADD	R9, R8;
	PUSH	R9;
	POP	R8;
	POP	RAX;
	MOV	BYTE[R8], AL;
	PUSH	QWORD[RBP-43];
	POP	R8;
	INC	R8;
	PUSH	R8;
	POP	QWORD[RBP-43];
.SC05:
	PUSH	QWORD[RBP-43];
	PUSH	26;
	POP	R8;
	POP	R9;
	CMP	R9, R8;
	SETL	AL;
	MOVZX	R8, AL;
	PUSH	R8;
	POP	R8;
	CMP	R8, 1;
	JE	.WL05;
	PUSH	0;
	PUSH	QWORD[RBP-8];
	PUSH	27;
	PUSH	1;
	POP	R8;
	POP	R9;
	SUB	R9, R8;
	PUSH	R9;
	POP	R8;
	POP	R9;
	ADD	R9, R8;
	PUSH	R9;
	POP	R8;
	POP	RAX;
	MOV	BYTE[R8], AL;
	PUSH	QWORD[RBP-8];
	CALL	pstr;
	ADD	RSP, 8;
	MOV	EAX, 0;
	POP	RBP;
	MOV	RAX, 60;
	MOV	RDI, 0;
	SYSCALL;

section .data;
argv:
	DQ	0;
argc:
	DD	0;
var:
	DQ	0;
section .text;
pnum:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 0;
	PUSH	48;
	PUSH	QWORD[RBP--16];
	POP	R8;
	POP	R9;
	ADD	R8, R9;
	PUSH	R8;
	POP	QWORD[RBP--16];
	mov	rax, 1;
	mov	rdi, 1;
	add	rbp, 16;
	mov	rsi, rbp;
	sub	rbp, 16;
	mov	rdx, 1;
	syscall;
.SC02:
	ADD	RSP, 0;
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
strlen:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 8;
	PUSH	0;
	POP	QWORD[RBP-8];
.WL10:
	PUSH	1;
	PUSH	QWORD[RBP-8];
	POP	R8;
	POP	R9;
	ADD	R8, R9;
	PUSH	R8;
	POP	QWORD[RBP-8];
.SC10:
	PUSH	QWORD[RBP-8];
	PUSH	QWORD[RBP--16];
	POP	R8;
	POP	R9;
	ADD	R8, R9;
	PUSH	R8;
	POP	R8;
	MOVSX	RAX, BYTE[R8];
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
	JMP	.SC00;
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
	SUB	RSP, 0;
	MOV	QWORD[argv], RSI;
	MOV	DWORD[argc], EDI;
	EXTERN	printf;
	PUSH	5;
	POP	QWORD[var];
	PUSH	QWORD[var];
	CALL	pnum;
	ADD	RSP, 0;
	MOV	EAX, 0;
	POP	RBP;
	MOV	RAX, 60;
	MOV	RDI, 0;
	SYSCALL;

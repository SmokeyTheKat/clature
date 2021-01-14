section .data;
argv:
	DQ	0;
argc:
	DD	0;
section .text;
global main
main:
	EXTERN	malloc;
	EXTERN	free;
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 8;
	MOV	QWORD[argv], RSI;
	MOV	DWORD[argc], EDI;
	PUSH	10;
	PUSH	2;
	POP	R8;
	POP	RAX;
	MUL	R8;
	PUSH	RAX;
	POP	QWORD[RBP-8];
	PUSH	QWORD[RBP-8];
	PUSH	2;
	POP	R8;
	POP	R9;
	ADD	R8, R9;
	PUSH	R8;
	POP	QWORD[RBP-8];
	MOV	EAX, 0;
	POP	RBP;
	MOV	RAX, 60;
	MOV	RDI, 0;
	SYSCALL;

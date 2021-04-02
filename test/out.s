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
	MOV	R8, 3;
	MOV	R9, 2;
	MOV	RAX, R8;
	MUL	R9;
	MOV	R8, RAX;
	MOV	R9, 1;
	ADD	R9, R8;
	MOV	R8, R9;
	MOV	QWORD[RBP-8], R8;
	MOV	EAX, 0;
	POP	RBP;
	MOV	RAX, 60;
	MOV	RDI, 0;
	SYSCALL;

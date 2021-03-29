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
	MOV	R8, 6;
	MOV	R9, 5;
	MOV	RAX, R8;
	MUL	R9;
	MOV	R8, RAX;
	MOV	R9, 4;
	MOV	RAX, R8;
	MUL	R9;
	MOV	R8, RAX;
	MOV	R9, 3;
	MOV	R10, 2;
	MOV	RAX, R9;
	MUL	R10;
	MOV	R9, RAX;
	ADD	R8, R9;
	MOV	R9, 1;
	ADD	R8, R9;
	MOV	QWORD[RBP-8], R8;
	;
	MOV	R8, 2;
	MOV	R9, QWORD[RBP-8];
	SUB	R9, R8;
	MOV	QWORD[RBP-8], R9;
	MOV	EAX, 0;
	POP	RBP;
	MOV	RAX, 60;
	MOV	RDI, 0;
	SYSCALL;

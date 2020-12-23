section .data;
global _start
_start:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 28;
	MOV	R8, RBP;
	SUB	R8, 20;
	PUSH	R8;
	POP	QWORD[RBP-8];
	MOV	EAX, 0;
	POP	RBP;
	MOV	RAX, 60;
	MOV	RDI, 0;
	SYSCALL;

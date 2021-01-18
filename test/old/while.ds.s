section .data;
	EXTERN	malloc;
	EXTERN	free;
global main
main:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 12;
	MOV	QWORD[RBP-8], RSI;
	MOV	DWORD[RBP-12], EDI;
	PUSH	test;
	PUSH	whileds;
	POP	R8;
	POP	RAX;
	DIV	R8;
	PUSH	RAX;
	MOV	EAX, 0;
	POP	RBP;
	MOV	RAX, 60;
	MOV	RDI, 0;
	SYSCALL;

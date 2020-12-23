section .data;
.str0:
	db	"hello",0;
global _start
_start:
	PUSH	RBP;
	MOV	RBP, RSP;
	SUB	RSP, 21;
	PUSH	0;
	POP	QWORD[RBP-8];
	PUSH	.str0;
	POP	QWORD[RBP-16];
	PUSH	'L';
	POP	RAX;
	MOV	BYTE[RBP-17], AL;
	PUSH	4;
	POP	RAX;
	MOV	DWORD[RBP-21], EAX;
	MOV	EAX, 0;
	POP	RBP;
	MOV	RAX, 60;
	MOV	RDI, 0;
	SYSCALL;

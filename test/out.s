section .data;
.str0:
	db	"hello",0;
global _start
_start:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 21;
	push	0;
	pop	QWORD[rbp-8];
	push	.str0;
	pop	QWORD[rbp-16];
	push	'L';
	pop	rax;
	mov	BYTE[rbp-17], al;
	push	4;
	pop	rax;
	mov	DWORD[rbp-21], eax;
	mov	eax, 0;
	pop	rbp;
	mov	rax, 60;
	mov	rdi, 0;
	syscall;

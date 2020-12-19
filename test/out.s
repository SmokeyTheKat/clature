section .data;
.str0:
	db	"yo bro what is going on yo0";
printChar:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 0;
	mov	rax, 1;
	mov	rdi, 1;
	add	rbp, 16;
	mov	rsi, rbp;
	sub	rbp, 16;
	mov	rdx, 1;
	syscall;
.SC01:
	add	rsp, 0;
	pop	rbp;
	ret;
none:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 0;
.SC00:
	add	rsp, 0;
	pop	rbp;
	ret;
global _start
_start:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 17;
	push	.str0;
	pop	QWORD[rbp-8];
	push	QWORD[rbp-8];
	push	4;
	pop	r8;
	pop	r9;
	add	r9, r8;
	push	r9;
	pop	r8;
	movsx	rax, BYTE[r8];
	push	rax;
	pop	rax;
	mov	BYTE[rbp-9], al;
	movsx	rax, BYTE[rbp-9];
	push	rax;
	call	printChar;
	add	rsp, 8;
	push	r8;
	push	0;
	pop	QWORD[rbp-17];
	mov	eax, 0;
	pop	rbp;
	mov	rax, 60;
	mov	rdi, 0;
	syscall;

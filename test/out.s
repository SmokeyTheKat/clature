printNum:
	push	rbp;
	mov	rbp, rsp;
	push	qword[rbp--16];
	push	48;
	pop	r9;
	pop	r8;
	add	r8, r9;
	push	r8;
	pop	qword[rbp--16];
	mov	rax, 1;
	mov	rdi, 1;
	add	rbp, 16;
	mov	rsi, rbp;
	sub	rbp, 16;
	mov	rdx, 1;
	syscall;
SC00:
	pop	rbp;
	ret;
global _start
_start:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 8;
	push	10;
	pop	qword[rbp-8];
	push	3;
	call	printNum;
	add	rsp, 8;
	mov	eax, 0;
	pop	rbp;
	mov	rax, 60;
	mov	rdi, 0;
	syscall;

caller:
	push	rbp;
	mov	rbp, rsp;
	mov	byte[rbp-5], 'h';
	mov	byte[rbp-4], 'e';
	mov	byte[rbp-3], 'l';
	mov	byte[rbp-2], 'l';
	mov	byte[rbp-1], 'o';
	mov	byte[rbp-0], 0;
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 5;
	mov	rsi, rbp;
	add	rbp, 5;
	mov	rdx, 5;
	syscall;
SC1:
	pop	rbp;
	ret;
caller2:
	push	rbp;
	mov	rbp, rsp;
	mov	byte[rbp-6], 'y';
	mov	byte[rbp-5], 'o';
	mov	byte[rbp-4], ' ';
	mov	byte[rbp-3], 'b';
	mov	byte[rbp-2], 'r';
	mov	byte[rbp-1], 'o';
	mov	byte[rbp-0], 0;
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 6;
	mov	rsi, rbp;
	add 	bp, 6;
	mov	rdx, 6;
	syscall;
SC2:
	pop	rbp;
	ret;
global _start
_start:
	push	rbp;
	mov	rbp, rsp;
	push	69;
	pop	qword[rbp-8];
	push	2;
	push	5;
	pop	r15;
	pop	rax;
	mul	r15;
	push	rax;
	pop	qword[rbp-16];
	call caller;
	call caller2;
	mov	eax, 0;
	pop	rbp;
	mov	rax, 60;
	mov	rdi, 0;
	syscall;

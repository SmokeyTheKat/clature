printRaw:
	push	rbp;
	mov	rbp, rsp;
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
SC01:
	pop	rbp;
	ret;
global _start
_start:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 8;
	push	0;
	pop	qword[rbp-8];
WL02:
	push	10;
	push	qword[rbp-8];
	pop	r9;
	pop	r8;
	cmp	r9, r8;
	setl	al;
	movzx	r8, al;
	push	r8;
	pop	r8;
	cmp	r8, 0;
	je	SC02;
	push	qword[rbp-8];
	call	printNum;
	add	rsp, 8;
	push	qword[rbp-8];
	pop	r8;
	inc	r8;
	push	r8;
	pop	qword[rbp-8];
	jmp	WL02;
SC02:
	mov	eax, 0;
	pop	rbp;
	mov	rax, 60;
	mov	rdi, 0;
	syscall;

printZero:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 0;
	mov	byte[rbp-1], '0';
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 1;
	mov	rsi, rbp;
	add	rbp, 1;
	mov	rdx, 1;
	syscall;
SC05:
	add	rsp, 0;
	pop	rbp;
	ret;
printOne:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 0;
	mov	byte[rbp-1], '1';
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 1;
	mov	rsi, rbp;
	add	rbp, 1;
	mov	rdx, 1;
	syscall;
SC04:
	add	rsp, 0;
	pop	rbp;
	ret;
printTwo:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 0;
	mov	byte[rbp-1], '2';
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 1;
	mov	rsi, rbp;
	add	rbp, 1;
	mov	rdx, 1;
	syscall;
SC03:
	add	rsp, 0;
	pop	rbp;
	ret;
printThree:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 0;
	mov	byte[rbp-1], '3';
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 1;
	mov	rsi, rbp;
	add	rbp, 1;
	mov	rdx, 1;
	syscall;
SC02:
	add	rsp, 0;
	pop	rbp;
	ret;
printFour:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 0;
	mov	byte[rbp-1], '4';
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 1;
	mov	rsi, rbp;
	add	rbp, 1;
	mov	rdx, 1;
	syscall;
SC01:
	add	rsp, 0;
	pop	rbp;
	ret;
printFive:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 0;
	mov	byte[rbp-1], '5';
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 1;
	mov	rsi, rbp;
	add	rbp, 1;
	mov	rdx, 1;
	syscall;
SC00:
	add	rsp, 0;
	pop	rbp;
	ret;
global _start
_start:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 32;
	push	0;
	pop	qword[rbp-8];
	push	0;
	pop	qword[rbp-16];
	push	0;
	pop	qword[rbp-24];
WL06:
	push	qword[rbp-8];
	push	0;
	pop	r8;
	pop	r9;
	cmp	r9, r8;
	sete	al;
	movzx	r8, al;
	push	r8;
	pop	r8;
	cmp	r8, 0;
	je	SC10;
	call	printZero;
SC10:
	push	qword[rbp-8];
	push	1;
	pop	r8;
	pop	r9;
	cmp	r9, r8;
	sete	al;
	movzx	r8, al;
	push	r8;
	pop	r8;
	cmp	r8, 0;
	je	SC11;
	call	printOne;
SC11:
	push	qword[rbp-8];
	push	2;
	pop	r8;
	pop	r9;
	cmp	r9, r8;
	sete	al;
	movzx	r8, al;
	push	r8;
	pop	r8;
	cmp	r8, 0;
	je	SC12;
	call	printTwo;
SC12:
	push	qword[rbp-8];
	push	3;
	pop	r8;
	pop	r9;
	cmp	r9, r8;
	sete	al;
	movzx	r8, al;
	push	r8;
	pop	r8;
	cmp	r8, 0;
	je	SC13;
	call	printThree;
SC13:
	push	qword[rbp-8];
	push	4;
	pop	r8;
	pop	r9;
	cmp	r9, r8;
	sete	al;
	movzx	r8, al;
	push	r8;
	pop	r8;
	cmp	r8, 0;
	je	SC14;
	call	printFour;
SC14:
	push	qword[rbp-8];
	pop	r8;
	inc	r8;
	push	r8;
	pop	qword[rbp-8];
SC06:
	push	qword[rbp-8];
	push	5;
	pop	r8;
	pop	r9;
	cmp	r9, r8;
	setne	al;
	movzx	r8, al;
	push	r8;
	pop	r8;
	cmp	r8, 1;
	je	WL06;
	push	520;
	pop	qword[rbp-32];
	mov	eax, 0;
	pop	rbp;
	mov	rax, 60;
	mov	rdi, 0;
	syscall;

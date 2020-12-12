printTrue:
	push	rbp;
	mov	rbp, rsp;
	push	9;
	push	2;
	pop	r8;
	pop	rax;
	mul	r8;
	push	rax;
	pop	qword[rbp-8];
	mov	byte[rbp-54], 't';
	mov	byte[rbp-53], 'r';
	mov	byte[rbp-52], 'u';
	mov	byte[rbp-51], 'e';
	mov	byte[rbp-50], 0;
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 54;
	mov	rsi, rbp;
	add	rbp, 54;
	mov	rdx, 4;
	syscall;
SC1:
	pop	rbp;
	ret;
pv:
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
SC2:
	pop	rbp;
	ret;
pr8:
	push	rbp;
	mov	rbp, rsp;
	mov	r10, r8;
	add	r10, 48;
	mov	qword[rbp-8], r10;
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 8;
	mov	rsi, rbp;
	add	rbp, 8;
	mov	rdx, 1;
	syscall;
SC3:
	pop	rbp;
	ret;
timestwo:
	push	rbp;
	mov	rbp, rsp;
	push	qword[rbp--16];
	push	2;
	pop	r8;
	pop	rax;
	mul	r8;
	push	rax;
	pop	qword[rbp--16];
	push	qword[rbp--16];
	pop	r8;
SC4:
	pop	rbp;
	ret;
global _start
_start:
	push	rbp;
	mov	rbp, rsp;
	sub	rsp, 0;
	push	4;
	call	timestwo;
	add	rsp, 0;
	call	pr8;
	add	rsp, 0;
	mov	eax, 0;
	pop	rbp;
	mov	rax, 60;
	mov	rdi, 0;
	syscall;

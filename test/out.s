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
printHead:
	push	rbp;
	mov	rbp, rsp;
	mov	byte[rbp-15], 'T';
	mov	byte[rbp-14], 'h';
	mov	byte[rbp-13], 'n';
	mov	byte[rbp-12], ' ';
	mov	byte[rbp-11], 'n';
	mov	byte[rbp-10], 'u';
	mov	byte[rbp-9], 'm';
	mov	byte[rbp-8], 'b';
	mov	byte[rbp-7], 'e';
	mov	byte[rbp-6], 'r';
	mov	byte[rbp-5], ' ';
	mov	byte[rbp-4], 'i';
	mov	byte[rbp-3], 's';
	mov	byte[rbp-2], ':';
	mov	byte[rbp-1], ' ';
	mov	byte[rbp-0], 0;
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 15;
	mov	rsi, rbp;
	add 	rbp, 15;
	mov	rdx, 15;
	syscall;
SC2:
	pop	rbp;
	ret;
R11Add2:
	push	rbp;
	mov	rbp, rsp;
	add	r11, 2;
SC3:
	pop	rbp;
	ret;
printR11:
	push	rbp;
	mov	rbp, rsp;
	mov	r12, r11;
	add	r12, 48;
	mov	qword[rbp-8], r12;
	mov	rax, 1;
	mov	rdi, 1;
	sub	rbp, 8;
	mov	rsi, rbp;
	add 	rbp, 8;
	mov	rdx, 1;
	syscall;
SC4:
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
	call	printHead;
	mov	r11, 6;
	call	R11Add2;
	call	printR11;
	mov	eax, 0;
	pop	rbp;
	mov	rax, 60;
	mov	rdi, 0;
	syscall;

global _start
_start:
	push	rbp;
	mov	rbp, rsp;
	mov	eax, 0b1011;
	pop	rbp;
	mov	rax, 60;
	mov	rdi, 0;
	syscall;

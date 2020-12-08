global _start
_start:
	push rbp;
	mov rbp, rsp;
	push 9;
	push 60;
	pop r14;
	pop r15;
	add r15, r14;
	push rax;
	pop qword [rbp-4];
	mov byte[rbp-12], 52;
	mov byte[rbp-11], 53;
	mov byte[rbp-10], 54;
	mov rax, 1;
	mov rdi, 1;
	sub rbp, 12;
	mov rsi, rbp;
	add rbp, 12;
	mov rdx, 4;
	syscall;
	mov eax, 0;
	pop rbp;
	mov rax, 60;
	mov rdi, 0;
	syscall;

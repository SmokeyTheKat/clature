sub @8 pnum(@8 num)
{
	@8 v = num + 48;
	.mov	rax, 1;
	.mov	rdi, 1;
	.sub	rbp, 8;
	.mov	rsi, rbp;
	.add	rbp, 8;
	.mov	rdx, 1;
	.syscall;
}

pnum(2);

@8 a = 1 + 2 * 3 + 4 * 5 * 6;
@8 b = a * 2;
@10 c;
@8[?c] = 69 == 70-1;
@20 d;
@10[?d+10] = c;
a -= 2;

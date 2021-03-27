sub @0 pnum(@8 b, @10 a)
{
	@8 v = @8[?a] + 48;
	.mov	rax, 1;
	.mov	rdi, 1;
	.sub	rbp, 8;
	.mov	rsi, rbp;
	.add	rbp, 8;
	.mov	rdx, 1;
	.syscall;
}
sub @8 add(@8 a, @8 b)
{
	@8 value = a + b;
	return value;
}


@8 n;
@10 v;
@8[?v] = 3;
@10 p = v;
n = @8[?p];

n = 2;
if n
{
	n = add(2, 5);
}

@10 num;
@8[?num] = n;

for @8 i = 0, i < 10, i = i + 1
{
	;pnum(num);
}
@8 w = 0;

sub @8 strlen(@8 str)
{
	@8 output = 0;
	while @1[str+output] != 0 { output = output + 1 }
	return output;
}
sub @8 pstr(@8 str)
{
	@8 len = strlen(str);
	.mov	rax, 1;
	.mov	rdi, 1;
	.mov	rsi, [rbp+16];
	.mov	rdx, [rbp-8];
	.syscall;
	
}
sub @8 pnum(@8 num)
{
	num = num + 48;
	.mov	rax, 1;
	.mov	rdi, 1;
	.add	rbp, 16;
	.mov	rsi, rbp;
	.sub	rbp, 16;
	.mov	rdx, 1;
	.syscall;
}

extern printf;

global @8 var;
var = 5;
pnum(var);


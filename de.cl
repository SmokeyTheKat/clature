sub @8 nts(@8 value, @8 string, @8 length)
{
	if value == 0
	{
		@1[string] = 48;
		@1[string+1] = 0;
		return 0;
	}
	@8 i = 0;
	while i < length
	{
		@1[string+i] = 48;
		i = i + 1;
	}
	@1[string+(length-1)] = 0;
	@8 pos = length-2;
	@8 x = 0;
	while value
	{
		x = value % 10;
		value /= 10;
		@1[string+pos] = 48 + x;
		pos = pos - 1;
	}
}
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

@8 str = "hello";
pstr(str);


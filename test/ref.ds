sub @0 pstr(@8 str)
{
	@8 len = strlen(str);
	.mov	rax, 1;
	.mov	rdi, 1;
	.mov	rsi, [rbp+16];
	.mov	rdx, [rbp-8];
	.syscall;
}
sub @8 strlen(@8 str)
{
	@8 output = 0;
	while @1[str+output] != 0 { output++ }
	return output;
}

sub @8 expect(@8 a, @8 b)
{
	if a != b
	{
		iso pstr(`\x1b[38;2;255;0;0mFAILED\n`);
		iso exit(1);
	}
	iso pstr(`\x1b[38;2;0;255;0mPASSED\n`);
}

.extern	exit;

sub @8 changeNum(@8 ptr)
{
	@8[ptr] = 4;
}

@8 value = 0;
@4[?value] = 1;
@4[?value+4] = '2';

@8 mynum = 3;
@8 myptr = ?mynum;
iso changeNum(myptr);

iso expect((@8[myptr]), 4);


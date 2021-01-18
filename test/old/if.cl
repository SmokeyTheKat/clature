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

@8 valueA = 75;
@8 valueB = 25;
if valueA + valueB == 100
{
	valueA = 50;
	valueB = 50;
}

iso expect(valueA, valueB);

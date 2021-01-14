@8 none = 0;
.extern	exit;
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

sub @0 test_a(@0)
{
	@8 arr = @4<4>;
	@4[arr] = 10 * 2;
	@4[arr+4] = 5;
	expect((@4[arr]) + (@4[arr+4]), 25);
}
sub @0 test_b(@0)
{
	@8 arr = @8<4>;
	@8 i = 0;
	while i < 4
	{
		@8[arr:i)] = @4<4>;
		i++;
	}
	i = 0;
	@8 j = 0;
	while i < 4
	{
		j = 0;
		while j < 4
		{
			@4[@8[arr:i]:j] = i*j;
			j++;
		}
		i++;
	}
	;expect((@4[@8[arr:1]:2]) + (@4[@8[arr:2]:4]), 10);
}
sub @0 test_c(@0)
{
	@8 arr = @8<5>;
	@8 i = 0;
	while i < 5
	{
		@8[arr:i] = @4<5>;
		i++;
	}
	@4[@8[arr:4]:2] = 9*2;
	expect((@4[@8[arr:4]:2]), 18);
}


test_a();
test_b();
test_c();



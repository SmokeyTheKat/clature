sub @8 nts(@8 value, @8 string, @8 length)
{
	if value == 0
	{
		@1[string] = '0';
		@1[string:1] = 0;
		return 0;
	}
	@8 sign = '+';
	if value < 0
	{
		sign = '-';
		value = value - value - value;
	}
	@8 i = 0;
	while i < length
	{
		@1[string:i] = '0';
		i++;
	}
	@1[string:length-1] = 0;
	@8 pos = length-2;
	@8 x = 0;
	while value != 0
	{
		x = value % 10;
		value = value / 10;
		@1[string:pos] = '0' + x;
		pos = pos - 1;
	}
	@1[string:pos] = sign;
}
sub @8 strlen(@8 str)
{
	@8 output = 0;
	while @1[str:output] != 0 { output++ }
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
	num += 48;
	.mov	rax, 1;
	.mov	rdi, 1;
	.add	rbp, 16;
	.mov	rsi, rbp;
	.sub	rbp, 16;
	.mov	rdx, 1;
	.syscall;
}
for @8 i = 0, i < 10, i++
{
	pstr(`\x1b[38;2;255;0;255mYO\n`);
}
@1<10> array;
@23 st;
@8[?st] = 54;
@1<10> str;
@8 n = 365;
@8 num = 369;
if 1 == 1
{
	pstr(`\n`);
	nts(num, str, 10);
	pstr(str);
	pstr(`\n`);
} 
@8 none = 1  + 3 - 2;

pstr(`\n`);

@20 a;
@8[?a] = 2;
@20 b = a;
@8 z = @8[?b];
nts(z, str, 10);
pstr(str);

pstr(`\n`);



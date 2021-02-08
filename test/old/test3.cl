sub @8 numToString(@8 value, @8 string, @8 length)
{
	if value == 0
	{
		@1[string] = '0';
		@1[string+1] = 0;
		return 0;
	}
	@8 i = 0;
	while i < length
	{
		@1[string+i] = '0';
		i++;
	}
	@1[string:length-1] = 0;
	@8 pos = length-2;
	@8 x = 0;
	while value
	{
		x = value % 10;
		value /= 10;
		@1[string+pos] = '0' + x;
		pos--;
	}
}
sub @8 ddPrint_cstring(@8 strconst)
{
	@8 strlen = getLength(strconst);
	.mov	rax, 1;
	.mov	rdi, 1;
	.mov	rsi, [rbp+16];
	.mov	rdx, [rbp-8];
	.syscall;
	
}
sub @8 printNum(@8 num)
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
sub @8 getLength(@8 str)
{
	@8 output = 0;
	while @1[str+output] != 0 { output++ }
	return output;
}
sub @8 printChar(@8 chr)
{
	.mov	rax, 1;
	.mov	rdi, 1;
	.add	rbp, 16;
	.mov	rsi, rbp;
	.sub	rbp, 16;
	.mov	rdx, 1;
	.syscall;
}
@1 endl = 10;

@8 istr = "          ";
@8 istrlen = 9;
@8 inum = 369;
numToString(inum, istr, istrlen);
ddPrint_cstring(istr);
;while inum < 15
;{
;	numToString(inum, istr, istrlen);
;	ddPrint_cstring(istr);
;	inum++;
;	printChar(endl);
;}

@8 value = 2 == 1 && 5 == 4+1 || 2 == 1*2;
;printNum(value);

@8 text_length = "the length is: ";

printChar(endl);
printChar(endl);


@8 hello = "hello";
@8 chello = hello;
ddPrint_cstring(chello);

printChar(endl);

@8 helloLen = getLength(hello);
ddPrint_cstring(text_length);
printNum(helloLen);

printChar(endl);

@8 str = "yo bro what is going on yo";
ddPrint_cstring(str);
printChar(endl);
@1 chr = 0;
@8 i = 0;
while @1[str+i] != 0
{
	chr = @1[str+i];
	printChar(chr);
	i++;
}
@8 empty = 4;
printChar(endl);

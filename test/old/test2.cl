sub @8 printSix(@8 var)
{
	.mov	byte[rbp-1], '6';
	.mov	rax, 1;
	.mov	rdi, 1;
	.sub	rbp, 1;
	.mov	rsi, rbp;
	.add	rbp, 1;
	.mov	rdx, 1;
	.syscall;
}
sub @8 printFive(@8 var)
{
	.mov	byte[rbp-1], '5';
	.mov	rax, 1;
	.mov	rdi, 1;
	.sub	rbp, 1;
	.mov	rsi, rbp;
	.add	rbp, 1;
	.mov	rdx, 1;
	.syscall;
}
sub @8 printFour(@8 var)
{
	.mov	byte[rbp-1], '4';
	.mov	rax, 1;
	.mov	rdi, 1;
	.sub	rbp, 1;
	.mov	rsi, rbp;
	.add	rbp, 1;
	.mov	rdx, 1;
	.syscall;
}
sub @8 printThree(@8 var)
{
	.mov	byte[rbp-1], '3';
	.mov	rax, 1;
	.mov	rdi, 1;
	.sub	rbp, 1;
	.mov	rsi, rbp;
	.add	rbp, 1;
	.mov	rdx, 1;
	.syscall;
}
sub @8 printTwo(@8 var)
{
	.mov	byte[rbp-1], '2';
	.mov	rax, 1;
	.mov	rdi, 1;
	.sub	rbp, 1;
	.mov	rsi, rbp;
	.add	rbp, 1;
	.mov	rdx, 1;
	.syscall;
}
sub @8 printOne(@8 var)
{
	.mov	byte[rbp-1], '1';
	.mov	rax, 1;
	.mov	rdi, 1;
	.sub	rbp, 1;
	.mov	rsi, rbp;
	.add	rbp, 1;
	.mov	rdx, 1;
	.syscall;
}
sub @8 printZero(@8 var)
{
	.mov	byte[rbp-1], '0';
	.mov	rax, 1;
	.mov	rdi, 1;
	.sub	rbp, 1;
	.mov	rsi, rbp;
	.add	rbp, 1;
	.mov	rdx, 1;
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
@8 i = 0;
@8 j = 0;
@8 k = 0;
while i != 5
{
	if i == 0
	{
		iso printZero();
	}
	if i == 1
	{
		iso printOne();
	}
	if i == 2
	{
		iso printTwo();
	}
	if i == 3
	{
		iso printThree();
	}
	if i == 4
	{
		iso printFour();
	}
	i++;
}
;iso printStr(" ", 1);
;sub @8 printStr(@8 str, @8 len)
;{
;	.mov	rax, 1;
;	.mov	rdi, 1;
;	.mov	rsi, [rbp+24];
;	.mov	rdx, [rbp+16];
;	.syscall;
;}

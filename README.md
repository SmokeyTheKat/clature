# ddScript
ddScript is a small language with x86_64 nasm assembly code generation. The language is 

## Build
make targets
| Target | Description |
|-------:|-------------|
| _(none)_ | complies to ./ddScript |
| Check | runs test scripts in ./test/ |
| install | compiles to /usr/bin/cds and adds config files to /usr/share/cds/ |
| unstall | removes config files and file from /usr/bin |

## Usage
Compile to executable
```
$ cds file.ds -o file
```
Compile file to assembly
```
$ cds file.ds -s file.s
```

## Syntax
Variables are signified with sizes using the symbol '@'.
```
@8 long = 587832;
@1 char = 124;
```

Variables can hold any size but operations can only be done in 1, 2, 4, and 8 byte chunks.
```
@16 data;
@8[?data+0] = 529;
@4[?data+8] = 37;
@4[?data+12] = 85;
```

Comments are started with ';' and end at newlines. Semicolons are not needed.
```
@4 number = 2 * 4; this is not code teheheh
```

Square brackets \[pointer\] are used to dereference a pointer.
```
@8 str = "Test String";
@1 chr = @1[str+3];
--------------------------
chr == 't'
```

A '?' is used to reference a variable.
```
@8 value1 = 10;
@8 value2 = ?value1;    value2 is a pointer
@8[value2] = 5;
--------------------------
value1 == 5
```

If statements and loops are like most other but parenthesis are not necessary.
```
@8 value = 2 * 5 / 2;
if value >= 3
{
  value = 10;
}
while value <= 100
{
  value += 10;
}
```

Functions.
```
sub @8 multiply(@8 a, @8b)
{
  return a * b;
}
```

Calling a function with no return value need an iso keyword for now.
```
@8 str = "hello";
iso printString(str, 5);
```

Inline assembly can be used anywhere and will not get messed around with. An assembly line is started with a '.'. Variables can be used in the assebmly.
```
sub @8 printString(@8 string, @8 length)
{
  .mov  rax, 1;
  .mov  rdi, 1;
  .mov  rsi, string;
  .mov  rdx, length;
  .syscall;
}
```

Macros are started with a '/'.
```
/def @8 ptr
/def @8 long
/def @4 int
/def @2 short
/def @1 char
/inc "./print.ds"
int vara = 4;
char varb = 'L';
ptr str = "this is a string";
print_string(str);
```

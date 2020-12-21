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
Square brackets \[pointer\] are used to dereference a pointer
```
@8 str = "Test String";
@1 chr = @1[str+3];
--------------------------
chr == 't'
```

A '?' is used to reference a variable
```
@8 value1 = 10;
@8 value2 = ?value1; value2 is a pointer
[value2] = 5;
--------------------------
value1 == 5
```

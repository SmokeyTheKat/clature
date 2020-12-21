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

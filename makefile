all:
	gcc ./src/ddScript.c -o ddScript -lm
run: all
	./ddScript
tc:
	./ddScript ./test/test2.ds ./test/out.s
	nasm -f elf64 ./test/out.s -o ./test/out.o
	ld ./test/out.o -o ./test/out
	./test/out
t:
	nasm -f elf64 ./test/out.s -o ./test/out.o
	ld ./test/out.o -o ./test/out
	./test/out

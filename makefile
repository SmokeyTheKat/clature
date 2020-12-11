all:
	gcc ./src/ddScript.c -o ddScript -lm
run: all
	./ddScript
testm:
	./ddScript ./test/test.ds ./test/out.s
	nasm -f elf64 ./test/out.s -o ./test/out.o
	ld ./test/out.o -o ./test/out
	./test/out

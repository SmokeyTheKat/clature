all:
	gcc ./src/ddScript.c -o ./ddScript -lm
	sudo cp ./ddScript /usr/bin/cds
run: all
	./ddScript
tc: all
	./ddScript ./test/test3.ds ./test/out
	#nasm -f elf64 ./test/out.s -o ./test/out.o
	#ld ./test/out.o -o ./test/out
	./test/out

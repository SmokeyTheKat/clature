all:
	gcc ./src/main.c -o ./clature -lm -Wall
install: all
	sudo cp ./clature /usr/bin/ccl
check: all
	@./clature ./test/var.cl	-o test/out --silent
	@./test/out
	@echo "PASSED var.cl"
	@./clature ./test/asm.cl	-o test/out --silent
	@./test/out
	@echo "PASSED asm.cl"
	@echo "PASSED ALL TESTS WOW :)"
tc: all
	./clature ./de.cl -o ./test/out --no-clean-up
	#nasm -f elf64 ./test/out.s -o ./test/out.o
	#ld ./test/out.o -o ./test/out
	./test/out
td: all
	./clature ./de.cl -o ./test/out --no-clean-up -debug
	#nasm -f elf64 ./test/out.s -o ./test/out.o
	#ld ./test/out.o -o ./test/out
	./test/out

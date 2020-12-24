all:
	gcc ./src/ddScript.c -o ./ddScript -lm
install: all
	sudo cp ./ddScript /usr/bin/cds
check: all
	@./ddScript ./test/variable.ds ./test/out
	@echo "PASSED variable.ds"
	@./ddScript ./test/operators.ds ./test/out
	@echo "PASSED operators.ds"
	@./ddScript ./test/if.ds ./test/out
	@echo "PASSED if.ds"
	@./ddScript ./test/while.ds ./test/out
	@echo "PASSED while.ds"
	@./ddScript ./test/function.ds ./test/out
	@echo "PASSED function.ds"
	@./ddScript ./test/string.ds ./test/out
	@echo "PASSED string.ds"
	@./ddScript ./test/deref.ds ./test/out
	@echo "PASSED deref.ds"
	@./ddScript ./test/ref.ds ./test/out
	@echo "PASSED ref.ds"
	@./ddScript ./test/macros.ds ./test/out
	@echo "PASSED macros.ds"
	@./ddScript ./test/arr.ds ./test/out
	@echo "PASSED arr.ds"
	@./ddScript ./test/malloc.ds ./test/out
	@echo "PASSED malloc.ds"
	@echo "PASSED ALL TESTS WOW :)"
tc: all
	./ddScript ./test/test3.ds ./test/out
	#nasm -f elf64 ./test/out.s -o ./test/out.o
	#ld ./test/out.o -o ./test/out
	./test/out

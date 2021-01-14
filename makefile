all:
	gcc ./src/main.c -o ./clature -lm -Wall
install: all
	sudo cp ./clature /usr/bin/ccl
check: all
	@./clature test/variable.cl	-o test/out --silent
	@./test/out
	@echo "PASSED variable.cl"
	@./clature test/operators.cl	-o test/out --silent
	@./test/out
	@echo "PASSED operators.cl"
	@./clature test/if.cl		-o test/out --silent
	@./test/out
	@echo "PASSED if.cl"
	@./clature test/while.cl	-o test/out --silent
	@./test/out
	@echo "PASSED while.cl"
	@./clature test/function.cl	-o test/out --silent
	@./test/out
	@echo "PASSED function.cl"
	@./clature test/string.cl	-o test/out --silent
	@./test/out
	@echo "PASSED string.cl"
	@./clature test/deref.cl	-o test/out --silent
	@./test/out
	@echo "PASSED deref.cl"
	@./clature test/ref.cl		-o test/out --silent
	@./test/out
	@echo "PASSED ref.cl"
	@./clature test/macros.cl	-o test/out --silent
	@./test/out
	@echo "PASSED macros.cl"
	@./clature test/arr.cl		-o test/out --silent
	@./test/out
	@echo "PASSED arr.cl"
	@./clature test/malloc.cl	-o test/out --silent
	@./test/out
	@echo "PASSED malloc.cl"
	@./clature test/scopes.cl	-o test/out --silent
	@./test/out
	@echo "PASSED scopes.cl"
	@./clature test/include.cl	-o test/out --silent
	@./test/out
	@echo "PASSED include.cl"
	@./clature test/global.cl	-o test/out --silent
	@./test/out
	@echo "PASSED global.cl"
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

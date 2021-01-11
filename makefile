all:
	gcc ./src/ddScript.c -o ./ddScript -lm -Wall
install: all
	sudo cp ./ddScript /usr/bin/cds
check: all
	@./ddScript test/variable.ds	-o test/out --silent
	@./test/out
	@echo "PASSED variable.ds"
	@./ddScript test/operators.ds	-o test/out --silent
	@./test/out
	@echo "PASSED operators.ds"
	@./ddScript test/if.ds		-o test/out --silent
	@./test/out
	@echo "PASSED if.ds"
	@./ddScript test/while.ds	-o test/out --silent
	@./test/out
	@echo "PASSED while.ds"
	@./ddScript test/function.ds	-o test/out --silent
	@./test/out
	@echo "PASSED function.ds"
	@./ddScript test/string.ds	-o test/out --silent
	@./test/out
	@echo "PASSED string.ds"
	@./ddScript test/deref.ds	-o test/out --silent
	@./test/out
	@echo "PASSED deref.ds"
	@./ddScript test/ref.ds		-o test/out --silent
	@./test/out
	@echo "PASSED ref.ds"
	@./ddScript test/macros.ds	-o test/out --silent
	@./test/out
	@echo "PASSED macros.ds"
	@./ddScript test/arr.ds		-o test/out --silent
	@./test/out
	@echo "PASSED arr.ds"
	@./ddScript test/malloc.ds	-o test/out --silent
	@./test/out
	@echo "PASSED malloc.ds"
	@./ddScript test/scopes.ds	-o test/out --silent
	@./test/out
	@echo "PASSED scopes.ds"
	@./ddScript test/include.ds	-o test/out --silent
	@./test/out
	@echo "PASSED include.ds"
	@./ddScript test/global.ds	-o test/out --silent
	@./test/out
	@echo "PASSED global.ds"
	@echo "PASSED ALL TESTS WOW :)"
tc: all
	./ddScript ./de.ds -o ./test/out --no-clean-up
	#nasm -f elf64 ./test/out.s -o ./test/out.o
	#ld ./test/out.o -o ./test/out
	./test/out
td: all
	./ddScript ./de.ds -o ./test/out --no-clean-up -debug
	#nasm -f elf64 ./test/out.s -o ./test/out.o
	#ld ./test/out.o -o ./test/out
	./test/out

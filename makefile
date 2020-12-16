all:
	gcc ./src/ddScript.c -o ./ddScript -lm
	sudo cp ./ddScript /usr/bin/cds
run: all
	./ddScript
tc:
	./ddScript ./test/test2.ds ./test/out.s
	./test/out

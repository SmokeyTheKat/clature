all:
	gcc ./src/ddScript.c -o ddScript -lm
run: all
	./ddScript

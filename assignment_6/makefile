CC = gcc
# CFLAGS = -Wall -g

all: compiler

compiler: lex.yy.c prog.tab.c codegen.c
	$(CC) -o compiler lex.yy.c prog.tab.c codegen.c -lfl

lex.yy.c: prog.l prog.tab.h
	flex prog.l

prog.tab.c prog.tab.h: prog.y
	bison -d prog.y

run: compiler
	./compiler < sample.txt > output.txt
	rm -f compiler lex.yy.c prog.tab.c prog.tab.h *.o

clean:
	rm -f compiler lex.yy.c prog.tab.c prog.tab.h *.o output.txt
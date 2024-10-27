CC = gcc
CFLAGS = -Wall -g

all: poly

poly: lex.yy.c poly.tab.c polyutils.c
	$(CC) $(CFLAGS) -o poly lex.yy.c poly.tab.c polyutils.c -lfl -lm

lex.yy.c: poly.l poly.tab.h
	flex poly.l

poly.tab.c poly.tab.h: poly.y
	bison -d poly.y

run: poly
	./poly < sample.txt

clean:
	rm -f poly lex.yy.c poly.tab.c poly.tab.h *.o
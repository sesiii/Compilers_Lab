CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

all: icgen

icgen: lex.yy.o expr.tab.o intcodegen.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $<

expr.tab.o: expr.tab.c
	$(CC) $(CFLAGS) -c $<

intcodegen.o: intcodegen.c expr.tab.h expr.h
	$(CC) $(CFLAGS) -c $<

lex.yy.c: expr.l expr.tab.h
	flex $<

expr.tab.c expr.tab.h: expr.y expr.h
	bison -d $<

run: icgen
	./icgen sample.txt
	$(CC) $(CFLAGS) -o ic intcode.c $(LIBS)
	./ic

clean:
	rm -f *.o lex.yy.c expr.tab.c expr.tab.h icgen ic intcode.c

.PHONY: all run clean
start:
	rm -f expr.tab.c expr.tab.h lex.yy.c expr
	bison -d expr.y
	flex expr.l
	g++ -o expr expr.tab.c lex.yy.c expr.cpp -lm
	./expr > output.txt
	rm -f expr.tab.c expr.tab.h lex.yy.c expr
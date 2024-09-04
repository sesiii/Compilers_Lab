#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.c"
#include "expr.tab.h"

extern int yyparse();

int main() {
    printf("Enter expressions:\n");
    yyparse();
    return 0;
}
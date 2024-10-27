#include <stdio.h>
#include <stdlib.h>

extern int yyparse();
extern FILE *yyin;
extern char *yytext;
extern int yylineno;
extern int yychar;

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
    fprintf(stderr, "Near token: %s\n", yytext);
    fprintf(stderr, "Unexpected character: %d\n", yychar);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    if (yyparse() == 0) {
        printf("Parsing successful!\n");
    } else {
        printf("Parsing failed!\n");
    }

    fclose(yyin);
    return 0;
}
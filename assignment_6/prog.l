%{
#include <stdio.h>
#include <stdlib.h>
#include "prog.tab.h"
extern YYSTYPE yylval;
int line_number = 1;
%}

%%
[ \t]           ;   /* Skip whitespace */
\n              { line_number++; }
"set"           { return SET; }
"when"          { return WHEN; }
"loop"          { return LOOP; }
"while"         { return WHILE; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULTIPLY; }
"/"             { return DIVIDE; }
"%"             { return MODULO; }
"="             { return EQ; }
"/="            { return NE; }
"<"             { return LT; }
">"             { return GT; }
"<="            { return LE; }
">="            { return GE; }
"("             { return LPAREN; }
")"             { return RPAREN; }
[0-9]+          { yylval.num = atoi(yytext); return NUMBER; }
-[0-9]+         { yylval.num = atoi(yytext); return NUMBER; }
[a-zA-Z][a-zA-Z0-9_]* { 
    yylval.id = strdup(yytext); 
    return IDENTIFIER; 
}
.               { printf("Lexical error: unexpected character %s\n", yytext); }

%%

int yywrap() {
    return 1;
}
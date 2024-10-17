%{
#include "polyutils.h"
#include "poly.tab.h"
%}

%%
"+"     { return PLUS; }
"-"     { return MINUS; }
"x"     { return XVAR; }
"^"     { return POWER; }
[0-9]   { yylval.ival = atoi(yytext); return DIGIT; }
[ \t\n] { /* Ignore whitespace */ }
.       { /* Ignore any other character */ }
%%

int yywrap(void) {
    return 1;
}
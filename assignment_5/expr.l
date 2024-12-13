%{
#include "expr.h"
#include "expr.tab.h"
#include <stdio.h>

#define DEBUG 1

#define DEBUG_PRINT(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, "LEXER DEBUG: " fmt "\n", ##__VA_ARGS__); } while (0)

%}

sp	[ \t\n]
sps	{sp}+
digit	[0-9]
nonzero    [1-9]
digits	0|[+-]?{nonzero}{digit}*
frac	\.{digits}
expt	[Ee][+-]?{digits}
num	{digits}{frac}?{expt}?
identifier [a-zA-Z_]+
id	{identifier}[a-zA-Z_0-9]*
newline \n

%%

{newline} { DEBUG_PRINT("Skipping newline"); }
{sps} { DEBUG_PRINT("Skipping whitespace"); }
{num} { 
    yylval.ival=atoi(yytext); 
    DEBUG_PRINT("Recognized NUM: %d", yylval.ival);
    return(NUM); 
}
"(" { DEBUG_PRINT("Recognized LP"); return(LP); }
")" { DEBUG_PRINT("Recognized RP"); return(RP); }
"**" { DEBUG_PRINT("Recognized POW"); return(POW); }
"+" { DEBUG_PRINT("Recognized ADD"); return(ADD); }
"/" { DEBUG_PRINT("Recognized DIV"); return(DIV); }
"-" { DEBUG_PRINT("Recognized SUB"); return(SUB); }
"*" { DEBUG_PRINT("Recognized MUL"); return(MUL); }
"%" { DEBUG_PRINT("Recognized MOD"); return(MOD); }
"set" { DEBUG_PRINT("Recognized SET"); return(SET); }
{id} { 
    yylval.sval = strdup(yytext); 
    DEBUG_PRINT("Recognized ID: %s", yylval.sval);
    return(ID); 
}
. { DEBUG_PRINT("Unrecognized character: %s", yytext); }

%%

int yywrap(void){
    return 1;
}
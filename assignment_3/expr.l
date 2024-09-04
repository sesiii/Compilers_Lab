%{
#include "expr.tab.h"
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

%%

{sps}	{ }
{num}	{ yylval.ival=atoi(yytext); return(NUM); }
"("		{ return(lp); }
")"		{ return(rp); }
"**"		{ return(POW); }
"+"		{ return(add); }
"/"		{ return(DIV); }
"-"		{ return(sub); }
"*"		{ return(mul); }
"%"		{ return(MOD); }
"set"		{ return(SET); }
{id}		{ yylval.sval = strdup(yytext); return(ID); }


%%

int yywrap(void){
    return 1;
}
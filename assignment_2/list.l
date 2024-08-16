%{
#define leftPar  0
#define rightPar  1
#define PLUS  2
#define sub  3
#define mul  4
#define div  5
#define MOD  6
#define ID  7
#define NUM  8
#define UNKNOWN  9
extern char* yytext;
%}

%%

[ \t\n]+                   ; 

"("                        { return leftPar; }
")"                        { return rightPar; }

"+"                        { return PLUS; }
"-"                        { return sub; }
"*"                        { return mul; }
"/"                        { return div; }
"%"                        { return MOD; }

[a-zA-Z][a-zA-Z0-9]*       { return ID; }
[+-]?[0-9]+                { return NUM; }

.                          { return UNKNOWN; }

%%

int yywrap() {
    return 1;
}
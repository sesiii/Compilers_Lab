%{
extern char* yytext;
int line_no = 1;
#define LPAREN  0
#define RPAREN  1
#define PLUS  2
#define MINUS  3
#define TIMES  4
#define DIVIDE  5
#define MOD  6
#define ID  7
#define NUM  8
#define UNKNOWN  9
%}

%%

\n                         { line_no++; } 
[ \t\n]+                   ; 
"("                        { return LPAREN; }
")"                        { return RPAREN; }
"+"                        { return PLUS; }
"-"                        { return MINUS; }
"*"                        { return TIMES; }
"/"                        { return DIVIDE; }
"%"                        { return MOD; }
[a-zA-Z][a-zA-Z0-9]*       { return ID; }
[+-]?[0-9]+                { return NUM; }
.                          { return UNKNOWN; }


%%

int yywrap(){
    return 1;
}

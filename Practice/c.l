%{
#include<stdio.h>
%}

%%

[0-9]+"."[0-9]+ {printf("Floating Number: %s\n",yytext);}
[0-9]+ {}{printf("Integer: %s\n",yytext);}
[+-=\*/()] {printf("Special symbol: %s\n",yytext);}
[ \t\n] {}
[a-zA-Z_][a-zA-Z0-9_]* {printf("Identifier: %s\n",yytext);}



%%

int yywrap(){
    return 1;

}
int main(){
    yylex();
    return 0;
}
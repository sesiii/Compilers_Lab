
%{
   #include<stdio.h>

%}

%%
[0-9]+ {printf("Integer: %s\n",yytext);}
[a-zA-Z_][a-zA-Z0-9_]* {printf("Identifier: %s\n",yytext);}
[+-/\*()=] {printf("Operator/Symbol:%s\n",yytext);}
[ \t\n] {}
%%
int yywrap(){
    return 1;
}
int main(){
    yylex();
    return 0;
}
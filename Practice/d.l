%{
    #include<stdio.h>
%}

%%

"//".* {printf("Comment: %s\n",yytext);}
[0-9]+"."[0-9]+ {printf("Floating Point number: %s\n",yytext);}
[0-9]+ {printf("Integer: %s\n",yytext);}
[a-zA-Z_][a-zA-Z0-9_]* {printf("Identifier: %s\n",yytext);}
"if"|"else"|"while"|"for" {printf("Keyword: %s\n",yytext);}
[+-/*=\()] {printf("Symbol/Operator: %s\n",yytext);}
[ \t\n] {}

%%

int yywrap(){
    return 1;
}

int main(){
    yylex();
    return 0;
}
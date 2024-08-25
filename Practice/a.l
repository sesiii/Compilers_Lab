INT "int"
ID [a-zA-Z][a-zA-Z0-9]*
NUM [0-9]+
PUNC [;]
CONST [0-9]+
WS [ \t\n]

%%

{INT} {printf("<Keyword,int>\n");}
{ID} {printf("<ID,%s>\n",yytext);}
"+" {printf("<Symbol, +>\n");}
"-" {printf("<Symbol, ->\n");}
"*" {printf("<Symbol, *>\n");}
"=" {printf("<Symbol, =>\n");}
"{" {printf("<Special Symbol, {>\n");}
"}" {printf("<Special Symbol, }>\n");}
{PUNC} {printf("<Punctuation,;>\n");}
{CONST} {printf("<CONST,%s>\n",yytext);}
{WS} ;

%%
int yywrap(){
    return 1;
}
int main(){
    yylex();
    return 0;
}

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "mylexer.h"  

    extern int yylineno; 
    extern struct SymbolTableEntry symbolTable[INFINITY];
    extern int symbolTableCount;

%}
%option noyywrap 
%%

"int"|"enum"|"restrict"|"short"|"float"|"char"|"double"|"case"|"void"|"while"|"break"|"goto"|"typedef"|"long"|"auto"|"unsigned"|"static"|"extern"|"return"|"sizeof"|"const"|"volatile"|"continue"|"switch"|"default"|"register"|"inline"|"do"|"if"|"struct"|"union"|"else"|"_Bool"|"_Complex"|"_Imaginary" {
    addToSymbolTable("keyword", yytext);
     return 1; 
}

[Ee][+-]?[0-9]+ { 
    addToSymbolTable("exponent-part", yytext); 
    return 3; 
}

[a-zA-Z_][a-zA-Z0-9_]* {
    addToSymbolTable("identifier", yytext);
    return 2; 
}

[+-] { 
    addToSymbolTable("sign", yytext); 
    return 11; 
}

[0-9]+\.[0-9]+([eE][+-]?[0-9]+)?  { 
    addToSymbolTable("floating-constant", yytext); 
    return 3; 
}

[0-9]+  { 
    addToSymbolTable("integer-constant", yytext); 
    return 3; 
}

\"[^\"]*\" {
    addToSymbolTable("string-literal", yytext);
    return 4; 
}

\'[^\']*\' {
    addToSymbolTable("character-constant", yytext);
    return 3; 
}

[+\-*/%=&|<>!^~?:,;.()\[\]{}#] {
    addToSymbolTable("punctuator", yytext);
    return 6; 
}

"//".* {
    addToSymbolTable("Single line comment",yytext);
    return 7;
}

"/*"([^*]|\*+[^*/])*\*+\/ {
    addToSymbolTable("Multi line comment",yytext);
    return 8;
}

"\n" {
    yylineno++;
     // Or return a specific token type for a newline
}
. {
    //unknown is being ignored
 }

[ \t]+ {
     
 }
"\\\'"|"\\\""|"\?"|"\\"|"\a"|"\b"|"\f"|"\n"|"\r"|"\t"|"\v" {
    addToSymbolTable("Escape Sequence",yytext);
    return 5;
}


%%

void initializeLexer(const char *filename) {
    yyin = fopen(filename, "r");
    if (yyin == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }
}

int getNextToken() {
     int token = yylex();
     if (token == 0) {
         return EOF;  // Return EOF if yylex returns 0
     } else {
         return token;
     }
 }

void printSymbolTable() {
    printf("\nSymbol Table:\n");
    printf("-------------------------------------\n");
    printf("Token Name | Lexeme | Line Number\n");
    printf("-------------------------------------\n");
    for (int i = 0; i < symbolTableCount; i++) {
        printf("%-10s | %-10s | %d\n",
               symbolTable[i].tokenName,
               symbolTable[i].lexeme,
               symbolTable[i].lineNumber);
               printf("\n");
    }
    printf("-------------------------------------\n");
}
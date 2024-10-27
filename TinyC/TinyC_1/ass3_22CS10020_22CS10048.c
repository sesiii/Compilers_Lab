#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylexer.h"
#include "lex.yy.c"


struct SymbolTableEntry symbolTable[INFINITY];
int symbolTableCount = 0;

void addToSymbolTable(char *tokenName, char *lexeme) {
    strcpy(symbolTable[symbolTableCount].tokenName, tokenName);
    strcpy(symbolTable[symbolTableCount].lexeme, lexeme);
    symbolTable[symbolTableCount].lineNumber = yylineno; 
    symbolTableCount++;
}


void processToken(int token) {
    switch (token) {
        case 1: 
            printf("<Keyword: %s>\n", symbolTable[symbolTableCount - 1].lexeme);
            break;
        case 2: 
            printf("<Identifier: %s>\n", symbolTable[symbolTableCount - 1].lexeme);
            break;
        case 3: 
            printf("<Constant: %s>\n", symbolTable[symbolTableCount - 1].lexeme);
            break;
        case 4: 
            printf("<String Literal: %s>\n", symbolTable[symbolTableCount - 1].lexeme);
            break;
        case 5: 
            printf("<Escape Sequence: %s>\n", symbolTable[symbolTableCount - 1].lexeme);
            break;
        case 6: 
            printf("<Punctuator: %s>\n", symbolTable[symbolTableCount - 1].lexeme);
            break;
        case 7: 
            printf("<Single line comment: %s>\n", symbolTable[symbolTableCount - 1].lexeme);
            break;
        case 8: 
            printf("<Multi line comment: %s>\n", symbolTable[symbolTableCount - 1].lexeme);
            break;
        case 11:
            printf("<Sign: %s>\n", symbolTable[symbolTableCount - 1].lexeme);
            break;
        
        

        
    }
}

int main() {
    initializeLexer("ass3_22CS10020_22CS10048_test.c"); 

    int token; 
    while ((token = getNextToken()) != EOF) { 
        processToken(token); 
    }

    printSymbolTable();
    

    return 0;
}
#ifndef MYLEXER_H
#define MYLEXER_H

#define INFINITY 30000

// Structure for storing the symbol table entries
struct SymbolTableEntry {
  char tokenName[INFINITY]; // Name of the token
  char lexeme[INFINITY];   // The actual lexeme
  int lineNumber;       // Line number where the token was found
};

// Global array to store the symbol table
extern struct SymbolTableEntry symbolTable[INFINITY];
extern int symbolTableCount;

// Function prototypes
void initializeLexer(const char *filename);
int getNextToken();
void printSymbolTable();
void addToSymbolTable(char *tokenName, char *lexeme); 

#endif
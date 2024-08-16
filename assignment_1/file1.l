%{
#include <iostream>
#include <cstring> 

const int MAX_ENTRIES = 1000;
struct TableEntry {
    char name[100];
    int count;
};

extern TableEntry commandTable[MAX_ENTRIES];
extern int numCommands;
extern TableEntry envTable[MAX_ENTRIES];
extern int numEnvs ;
extern TableEntry inlineMathTable[MAX_ENTRIES];
extern int numInlineMath;
extern TableEntry displayedMathTable[MAX_ENTRIES];
extern int numDisplayedMath;


void addOrUpdateEntry(TableEntry table[], int* numEntries, const char* name);
void printTable(TableEntry table[], int numEntries);
void print_statistics();

%}

%%
~               {  }
\$\$[^$]*\$\$ {
    
    char math[1000];
    strncpy(math, yytext + 2, strlen(yytext) - 4);
    math[strlen(yytext) - 4] = '\0'; // Remove $$ markers
    addOrUpdateEntry(displayedMathTable, &numDisplayedMath, math);
}

\\\[([^]]*)\\\] {
    
    char math[1000];
    strncpy(math, yytext + 2, strlen(yytext) - 4);
    math[strlen(yytext) - 4] = '\0'; // Remove \[ and \]
    addOrUpdateEntry(displayedMathTable, &numDisplayedMath, math);
}

\$[^$]*\$ {
    
    char math[1000];
    strncpy(math, yytext + 1, strlen(yytext) - 2);
    math[strlen(yytext) - 2] = '\0'; // Remove $ markers
    addOrUpdateEntry(inlineMathTable, &numInlineMath, math);
}
\\[a-zA-Z]+     { addOrUpdateEntry(commandTable, &numCommands, yytext); }
\\[^\n\da-zA-Z] { addOrUpdateEntry(commandTable, &numCommands, yytext); }
\\begin\{([a-zA-Z]+)\}  { 
    char envName[100];
    sscanf(yytext, "\\begin{%99[^}]}", envName); 
    //std::cout << "Begin Environment: " << envName << std::endl; // Debug
    addOrUpdateEntry(envTable, &numEnvs, envName); 
}
\\end\{([a-zA-Z]+)\}    { 
}

\%[^\n]*             {  }
.                   {  }

%%

int yywrap() { return 1; }
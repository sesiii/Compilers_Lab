#include <iostream>
#include <cstring>
#include "lex.yy.c"

using namespace std;

TableEntry commandTable[MAX_ENTRIES];
int numCommands = 0;
TableEntry envTable[MAX_ENTRIES];
int numEnvs = 0;
TableEntry inlineMathTable[MAX_ENTRIES];
int numInlineMath = 0;
TableEntry displayedMathTable[MAX_ENTRIES];
int numDisplayedMath = 0;

void addOrUpdateEntry(TableEntry table[], int *numEntries, const char *name)
{
    // cout << "Adding/Updating Entry: " << name << endl; // Debug

    if (*numEntries >= MAX_ENTRIES)
    {
        cerr << "Error: Table is full. Cannot add more entries." << endl;
        return;
    }

    if (strlen(name) == 0)
    {
        cerr << "Error: Invalid entry name." << endl;
        return;
    }

    for (int i = 0; i < *numEntries; ++i)
    {
        if (strcmp(table[i].name, name) == 0)
        {
            table[i].count++;
            return;
        }
    }

    strcpy(table[*numEntries].name, name);
    table[*numEntries].count = 1;
    (*numEntries)++;
}

#include <iostream>
#include <iomanip> 

void printTable(TableEntry table[], int numEntries) {

    for (int i = 0; i < numEntries; i++) {
        std::cout << table[i].name << " (" << table[i].count << ")" << std::endl;
    }
}

void print_statistics()
{
    using namespace std;


    cout << "Commands used:" << endl;
    if (numCommands == 0) {
        cout << "No commands found." << endl;
    } else {
        printTable(commandTable, numCommands);
    }

    cout << endl;

 
    cout << "Environments used:" << endl;
    if (numEnvs == 0) {
        cout << "No environments found." << endl;
    } else {
        printTable(envTable, numEnvs);
    }

    cout << endl;

   
    cout << "Inline maths equations used:" << endl;
    if (numInlineMath == 0) {
        cout << "No inline maths equations found." << endl;
    } else {
        printTable(inlineMathTable, numInlineMath);
    }

    cout << endl;

   
    cout << "Displayed maths equations used:" << endl;
    if (numDisplayedMath == 0) {
        cout << "No displayed maths equations found." << endl;
    } else {
        printTable(displayedMathTable, numDisplayedMath);
    }
}

int main()
{
    yylex();
    // cout << "Command Table:" << endl;
    // printTable(commandTable, numCommands);
    // cout << "********************" << endl;
    // cout << "Environment Table:" << endl;
    // printTable(envTable, numEnvs);

    // cout << "Inline Math Equations:" << endl;
    // printTable(inlineMathTable, numInlineMath);

    // cout << "********************" << endl;

    // cout << "Displayed Math Equations:" << endl;
    // printTable(displayedMathTable, numDisplayedMath);
    print_statistics();
    return 0;
}
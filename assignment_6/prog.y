%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

extern int line_number;
extern int yylex();
void yyerror(const char *s);

// Quad array and symbol table
struct quad quads[1000];
int quad_index = 0;
struct symbol symbols[100];
int symbol_count = 0;

// Temporary variable counter
int temp_count = 0;

// Forward declarations
int new_temp();
char* int_to_str(int num);
int lookup_symbol(char *name);
void insert_symbol(char *name);
void emit(enum opcode op, char *arg1, char *arg2, char *result);
char* new_temp_name();
void backpatch(int list, int label);
int merge(int list1, int list2);
%}

%union {
    int num;
    char *id;
    struct {
        char *addr;
        int true_list;
        int false_list;
        int next_list;
    } expr;
}
%token <num> NUMBER
%token <id> IDENTIFIER
%token SET WHEN LOOP WHILE
%token PLUS MINUS MULTIPLY DIVIDE MODULO
%token EQ NE LT GT LE GE
%token LPAREN RPAREN

%type <expr> EXPR BOOL ATOM LIST STMT ASGN COND LOOP_STMT

%%

LIST       : STMT                 { $$.next_list = $1.next_list; }
           | STMT LIST            { $$.next_list = $1.next_list; }
           ;

STMT       : ASGN                 { $$.next_list = $1.next_list; }
           | COND                 { $$.next_list = $1.next_list; }
           | LOOP_STMT            { $$.next_list = $1.next_list; }
           ;

ASGN       : LPAREN SET IDENTIFIER ATOM RPAREN {
                insert_symbol($3);
                emit(ASSIGN, $4.addr, "", $3);
                $$.next_list = 0;
            }
           ;

COND       : LPAREN WHEN BOOL LIST RPAREN {
                int label = quad_index;
                backpatch($3.true_list, label);
                $$.next_list = merge($3.false_list, $4.next_list);
            }
           ;

LOOP_STMT  : LPAREN LOOP WHILE BOOL LIST RPAREN {
                int start_label = quad_index;
                backpatch($5.next_list, start_label);
                backpatch($4.true_list, start_label + 1);
                $$.next_list = $4.false_list;
                emit(GOTO, "", "", int_to_str(start_label));
            }
           ;

EXPR       : LPAREN PLUS ATOM ATOM RPAREN      { 
                $$.addr = new_temp_name();
                emit(ADD, $3.addr, $4.addr, $$.addr);
            }
           | LPAREN MINUS ATOM ATOM RPAREN     { 
                $$.addr = new_temp_name();
                emit(SUB, $3.addr, $4.addr, $$.addr);
            }
           | LPAREN MULTIPLY ATOM ATOM RPAREN  { 
                $$.addr = new_temp_name();
                emit(MUL, $3.addr, $4.addr, $$.addr);
            }
           | LPAREN DIVIDE ATOM ATOM RPAREN    { 
                $$.addr = new_temp_name();
                emit(DIV, $3.addr, $4.addr, $$.addr);
            }
           | LPAREN MODULO ATOM ATOM RPAREN    { 
                $$.addr = new_temp_name();
                emit(MOD, $3.addr, $4.addr, $$.addr);
            }
           ;

BOOL       : LPAREN EQ ATOM ATOM RPAREN        { 
                $$.true_list = quad_index;
                $$.false_list = quad_index + 1;
                emit(IF_EQ, $3.addr, $4.addr, "");
                emit(GOTO, "", "", "");
            }
           | LPAREN NE ATOM ATOM RPAREN        { 
                $$.true_list = quad_index;
                $$.false_list = quad_index + 1;
                emit(IF_NE, $3.addr, $4.addr, "");
                emit(GOTO, "", "", "");
            }
           | LPAREN LT ATOM ATOM RPAREN        { 
                $$.true_list = quad_index;
                $$.false_list = quad_index + 1;
                emit(IF_LT, $3.addr, $4.addr, "");
                emit(GOTO, "", "", "");
            }
           | LPAREN GT ATOM ATOM RPAREN        { 
                $$.true_list = quad_index;
                $$.false_list = quad_index + 1;
                emit(IF_GT, $3.addr, $4.addr, "");
                emit(GOTO, "", "", "");
            }
           | LPAREN LE ATOM ATOM RPAREN        { 
                $$.true_list = quad_index;
                $$.false_list = quad_index + 1;
                emit(IF_LE, $3.addr, $4.addr, "");
                emit(GOTO, "", "", "");
            }
           | LPAREN GE ATOM ATOM RPAREN        { 
                $$.true_list = quad_index;
                $$.false_list = quad_index + 1;
                emit(IF_GE, $3.addr, $4.addr, "");
                emit(GOTO, "", "", "");
            }
           ;

ATOM       : IDENTIFIER            { 
                insert_symbol($1);
                $$.addr = $1;
            }
           | NUMBER                 { 
                char num_str[20];
                sprintf(num_str, "%d", $1);
                $$.addr = strdup(num_str);
            }
           | EXPR                   { 
                $$.addr = $1.addr;
            }
           ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d: %s\n", line_number, s);
}

int new_temp() {
    return temp_count++;
}

char* new_temp_name() {
    char *temp = (char*)malloc(20);
    sprintf(temp, "$%d", new_temp());
    return temp;
}

int lookup_symbol(char *name) {
    for(int i = 0; i < symbol_count; i++) {
        if(strcmp(symbols[i].name, name) == 0)
            return i;
    }
    return -1;
}

void insert_symbol(char *name) {
    if(lookup_symbol(name) == -1) {
        symbols[symbol_count].name = strdup(name);
        symbols[symbol_count].offset = symbol_count * 4;  // Simple offset allocation
        symbol_count++;
    }
}

void emit(enum opcode op, char *arg1, char *arg2, char *result) {
    quads[quad_index].op = op;
    quads[quad_index].arg1 = strdup(arg1);
    quads[quad_index].arg2 = strdup(arg2);
    quads[quad_index].result = strdup(result);
    quad_index++;
}

char* int_to_str(int num) {
    static char buffer[50];  // Ensure the buffer stays in scope
    snprintf(buffer, sizeof(buffer), "%d", num);  // Convert int to string
    return buffer;

}

void backpatch(int list, int label) {
    if(list == 0) return;
    char label_str[20];
    sprintf(label_str, "%d", label);
    strcpy(quads[list-1].result, label_str);
}

int merge(int list1, int list2) {
    if(list1 == 0) return list2;
    if(list2 == 0) return list1;
    return list1;  // Since we only have singleton lists
}
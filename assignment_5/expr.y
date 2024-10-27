%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expr.h"

extern int yylex();
void yyerror(const char* s);

#define DEBUG 1

#define DEBUG_PRINT(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, "PARSER DEBUG: " fmt "\n", ##__VA_ARGS__); } while (0)


exprNode* createConstNode(int value);
exprNode* createIdNode(char* name);
exprNode* createOpNode(char op, exprNode* left, exprNode* right);
extern void generate_set_stmt(char* id, exprNode* expr);
extern void generate_expr_stmt(exprNode* expr);
%}

%union {
    int ival;
    char* sval;
    exprNode* expr;
}

%token <ival> NUM
%token <sval> ID
%token ADD SUB MUL DIV MOD POW SET LP RP

%type <expr> EXPR ARG
%type <sval> OP

%start PROGRAM

%%

PROGRAM : STMT PROGRAM { DEBUG_PRINT("Reduced: PROGRAM -> STMT PROGRAM"); }
        | STMT         { DEBUG_PRINT("Reduced: PROGRAM -> STMT"); }
        ;

STMT    : LP SET ID EXPR RP { 
            DEBUG_PRINT("Reduced: STMT -> LP SET ID EXPR RP"); 
            generate_set_stmt($3, $4); 
        }
        | EXPR              { 
            DEBUG_PRINT("Reduced: STMT -> EXPR"); 
            generate_expr_stmt($1); 
        }
        ;

EXPR    : LP OP ARG ARG RP  { 
            DEBUG_PRINT("Reduced: EXPR -> LP OP ARG ARG RP"); 
            $$ = createOpNode($2[0], $3, $4); 
        }
        | NUM { 
            DEBUG_PRINT("Reduced: EXPR -> NUM (%d)", $1); 
            $$ = createConstNode($1); 
        }
        | ID { 
            DEBUG_PRINT("Reduced: EXPR -> ID (%s)", $1); 
            $$ = createIdNode($1); 
        }
        ;

OP      : ADD { DEBUG_PRINT("Reduced: OP -> ADD"); $$ = strdup("+"); }
        | SUB { DEBUG_PRINT("Reduced: OP -> SUB"); $$ = strdup("-"); }
        | MUL { DEBUG_PRINT("Reduced: OP -> MUL"); $$ = strdup("*"); }
        | DIV { DEBUG_PRINT("Reduced: OP -> DIV"); $$ = strdup("/"); }
        | MOD { DEBUG_PRINT("Reduced: OP -> MOD"); $$ = strdup("%"); }
        | POW { DEBUG_PRINT("Reduced: OP -> POW"); $$ = strdup("^"); }
        ;

ARG     : NUM   { DEBUG_PRINT("Reduced: ARG -> NUM (%d)", $1); $$ = createConstNode($1); }
        | ID    { DEBUG_PRINT("Reduced: ARG -> ID (%s)", $1); $$ = createIdNode($1); }
        | EXPR  { DEBUG_PRINT("Reduced: ARG -> EXPR"); $$ = $1; }
        ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Error: %s\n", s);
}

exprNode* createConstNode(int value) {
    exprNode* node = (exprNode*)malloc(sizeof(exprNode));
    node->type = CONST_NODE;
    node->value = value;
    return node;
}

exprNode* createIdNode(char* name) {
    exprNode* node = (exprNode*)malloc(sizeof(exprNode));
    node->type = ID_NODE;
    node->name = strdup(name);
    return node;
}

exprNode* createOpNode(char op, exprNode* left, exprNode* right) {
    exprNode* node = (exprNode*)malloc(sizeof(exprNode));
    node->type = OP_NODE;
    node->opNode.op = op;
    node->opNode.left = left;
    node->opNode.right = right;
    return node;
}
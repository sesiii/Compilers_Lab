#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <unistd.h>
#include "expr.tab.h"

extern int stct;
extern int ERROR_CNT;
extern int yyerror(const char *s);


typedef struct _node
{
    char *name;
    int value;
    struct _node *next;
} node;


typedef enum
{
    OP_TYPE,
    NUM_TYPE,
    ID_TYPE
} nodeType;


typedef struct _exprNode
{
    nodeType type;
    struct _exprNode *left;
    struct _exprNode *right;
    union
    {
        char op;      
        int numValue; 
        char *idName; 
    } data;
} exprNode;

typedef node *symbolTable;

node *insert(char *name);
node *lookup(char *name);
void set(node *name, int value);

exprNode *createNodeID(symbolTable SymbolTable);
exprNode *createNodeConst(int value);
exprNode *createNodeOP(char op, exprNode *left, exprNode *right);
void printExprTree(exprNode *root);
int result(exprNode *root);
void delTree(exprNode *root);

#endif // EXPR_H
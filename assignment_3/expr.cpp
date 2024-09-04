#include <execinfo.h>
#include "expr.h"

int stct = 0;


extern FILE *yyin; 

symbolTable SymbolTable;

node *insert(char *name)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->name = strdup(name);
    newNode->next = SymbolTable;
    SymbolTable = newNode;
    return newNode;
}

node *lookup(char *name)
{
    node *temp = SymbolTable;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void set(node *name, int value)
{
    if (name == NULL)
    {
        return;
    }
    name->value = value;
}

exprNode *createNodeConst(int value)
{
    exprNode *newNode = (exprNode *)malloc(sizeof(exprNode));
    newNode->type = NUM_TYPE;
    newNode->data.numValue = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

exprNode *createNodeID(symbolTable SymbolTable)
{
    exprNode *newNode = (exprNode *)malloc(sizeof(exprNode));
    newNode->type = ID_TYPE;
    newNode->data.idName = SymbolTable->name;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

exprNode *createNodeOP(char op, exprNode *left, exprNode *right)
{
    exprNode *newNode = new exprNode;
    newNode->type = OP_TYPE;
    newNode->data.op = op;
    newNode->left = left;
    newNode->right = right;

    return newNode;
}

void printExprTree(exprNode *root)
{
    if (root == NULL)
    {
        return;
    }
    if (root->type == NUM_TYPE)
    {
        printf("%d", root->data.numValue);
    }
    else if (root->type == ID_TYPE)
    {
        printf("%s", root->data.idName);
    }
    else
    {
        printf("(");
        printExprTree(root->left);
        printf("%c", root->data.op);
        printExprTree(root->right);
        printf(")");
    }
}

int exponent(int base, int e)
{
    if (e == 0)
        return 1;
    if (e % 2 == 1)
        return exponent(base, e - 1) * base;
    else
    {
        int b = exponent(base, e / 2);
        return b * b;
    }
}

int result(exprNode *root)
{
    if (root == NULL)
    {
        return 0;
    }
    if (root->type == NUM_TYPE)
    {
        return root->data.numValue;
    }
    else if (root->type == ID_TYPE)
    {
        return lookup(root->data.idName)->value;
    }
    else
    {
        int left = result(root->left);
        int right = result(root->right);
        switch (root->data.op)
        {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
            return left * right;
        case '/':
        {
            if (right == 0)
            {
                return 0;
            }
            return left / right;
        }
        case '%':
        {
            if (right == 0)
            {
                return 0;
            }
            return left % right;
        }
        case '^':
        {
            return exponent(left, right);
        }
        default:
            return 0;
        }
    }
}

void delTree(exprNode *root)
{
    if (root == NULL)
    {
        return;
    }
    delTree(root->left);
    delTree(root->right);
    free(root);
}

int main()
{
    SymbolTable = NULL;
    FILE *inputFile = fopen("input.txt", "r");
    if (inputFile == NULL)
    {
        printf("Failed to open input file\n");
        return 1;
    }
    yyin = inputFile;
    yyparse();
    fclose(inputFile);
    return 0;
}
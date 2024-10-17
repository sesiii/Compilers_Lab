#ifndef POLYUTILS_H
#define POLYUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char type[10];
    int inh;
    int val;
    struct Node* left;
    struct Node* right;
};

extern struct Node* root;

struct Node* createNode(const char* type, struct Node* left, struct Node* right);
struct Node* createLeaf(int value);
void setatt(struct Node* node);
void printTree(struct Node* node, int depth);
long long evalpoly(struct Node* node, int x);
void printderivative(struct Node* node, int isFirst);

#endif // POLYUTILS_H
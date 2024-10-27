#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "poly.tab.h"

struct Node {
    char type[20];
    int inh;
    int val;
    struct Node* left;
    struct Node* right;
};

struct Node* root = NULL;

struct Node* createNode(const char* type, struct Node* left, struct Node* right) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    strcpy(node->type, type);
    node->inh = 0;
    node->val = 0;
    node->left = left;
    node->right = right;
    return node;
}

struct Node* createLeaf(int value) {
    struct Node* leaf = createNode("LEAF", NULL, NULL);
    leaf->val = value;
    return leaf;
}

void setatt(struct Node* node) {
    if (node == NULL) return;

    // Set inherited attributes
    if (strcmp(node->type, "P") == 0 || strcmp(node->type, "T") == 0) {
        if (node->left) node->left->inh = node->inh;
        if (node->right && node->right->left) node->right->left->inh = (node->right->type[0] == '+') ? 1 : -1;
    } else if (strcmp(node->type, "M") == 0) {
        if (node->left) node->left->inh = node->inh * 10;
    }

    // Recursively set attributes for child nodes
    setatt(node->left);
    setatt(node->right);

    // Set synthesized attributes
    if (strcmp(node->type, "N") == 0 || strcmp(node->type, "M") == 0) {
        if (node->right) {
            node->val = node->left->val * pow(10, strlen(node->right->type)) + node->right->val;
        } else {
            node->val = node->left->val;
        }
    } else if (strcmp(node->type, "T") == 0) {
        if (node->right) {
            node->val = node->left->val * node->inh;
        } else if (strcmp(node->left->type, "X") == 0) {
            node->val = node->inh;
        } else {
            node->val = node->left->val * node->inh;
        }
    } else if (strcmp(node->type, "X") == 0) {
        if (node->right) {
            node->val = node->right->left->val;
        } else {
            node->val = 1;
        }
    }
}

void printTree(struct Node* node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++) printf("  ");
    
    printf("%s", node->type);
    if (strcmp(node->type, "LEAF") == 0) {
        printf(" %d", node->val);
    } else {
        printf(" (inh: %d, val: %d)", node->inh, node->val);
    }
    printf("\n");

    printTree(node->left, depth + 1);
    printTree(node->right, depth + 1);
}

int evalpoly(struct Node* node, int x) {
    if (node == NULL) return 0;

    if (strcmp(node->type, "S") == 0) {
        if (node->left->type[0] == '+') return evalpoly(node->right, x);
        if (node->left->type[0] == '-') return -evalpoly(node->right, x);
        return evalpoly(node->left, x);
    }

    if (strcmp(node->type, "P") == 0) {
        int leftVal = evalpoly(node->left, x);
        if (node->right == NULL) return leftVal;
        if (node->right->type[0] == '+') return leftVal + evalpoly(node->right->right, x);
        return leftVal - evalpoly(node->right->right, x);
    }

    if (strcmp(node->type, "T") == 0) {
        if (node->right == NULL) {
            if (strcmp(node->left->type, "X") == 0) return node->val * x;
            return node->val;
        }
        return node->val * pow(x, node->right->val);
    }

    return 0;
}

void printTerm(int coeff, int exp) {
    if (coeff == 0) return;
    if (coeff > 0 && exp != 0) printf("+");
    if (abs(coeff) != 1 || exp == 0) printf("%d", coeff);
    if (exp > 0) printf("x");
    if (exp > 1) printf("^%d", exp);
}

void printderivative(struct Node* node) {
    if (node == NULL) return;

    if (strcmp(node->type, "S") == 0) {
        if (node->left->type[0] == '-') printf("-");
        printderivative(node->right ? node->right : node->left);
    } else if (strcmp(node->type, "P") == 0) {
        printderivative(node->left);
        if (node->right) {
            printf(" %c ", node->right->type[0]);
            printderivative(node->right->right);
        }
    } else if (strcmp(node->type, "T") == 0) {
        int coeff = node->val;
        int exp = (node->right) ? node->right->val : (strcmp(node->left->type, "X") == 0 ? 1 : 0);
        
        if (exp == 0) {
            // Constant term, derivative is 0
            return;
        } else if (exp == 1) {
            // Term is of the form ax, derivative is a
            printTerm(coeff, 0);
        } else {
            // Term is of the form ax^n, derivative is (a*n)x^(n-1)
            printTerm(coeff * exp, exp - 1);
        }
    }
}

int main() {
    printf("Enter a polynomial: ");
    yyparse();
    
    if (root == NULL) {
        printf("Failed to parse the input.\n");
        return 1;
    }

    setatt(root);

    printf("\nAnnotated Parse Tree:\n");
    printTree(root, 0);

    printf("\nPolynomial evaluation for x in [-5, 5]:\n");
    for (int x = -5; x <= 5; x++) {
        printf("x = %2d: %d\n", x, evalpoly(root, x));
    }

    printf("\nDerivative of the polynomial:\n");
    printderivative(root);
    printf("\n");

    return 0;
}
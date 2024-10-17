#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "polyutils.h"
#include "poly.tab.h"

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
            node->val = node->left->val * 10 + node->right->val;
        } else {
            node->val = node->left->val;
        }
    } else if (strcmp(node->type, "T") == 0) {
        if (node->right && strcmp(node->right->type, "X") == 0) {
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
    } else if (strcmp(node->type, "P") == 0 || strcmp(node->type, "S") == 0) {
        if (node->left) node->val = node->left->val;
        if (node->right && node->right->left) node->val += node->right->left->val * node->right->left->inh;
    }
}

void printTree(struct Node* node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++) printf("  ");
    
    if (strcmp(node->type, "LEAF") == 0) {
        printf("==> %d [val = %d]\n", node->val, node->val);
    } else {
        printf("==> %s [inh = %d, val = %d]\n", node->type, node->inh, node->val);
    }

    printTree(node->left, depth + 1);
    printTree(node->right, depth + 1);
}

long long int_pow(int base, int exp) {
    long long result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

long long evalpoly(struct Node* node, int x) {
    if (node == NULL) return 0;

    if (strcmp(node->type, "S") == 0) {
        return node->left && node->left->type[0] == '-' ? -evalpoly(node->right, x) : evalpoly(node->right, x);
    }

    if (strcmp(node->type, "P") == 0) {
        long long result = evalpoly(node->left, x);
        if (node->right) {
            long long rightVal = evalpoly(node->right->right, x);
            result += (node->right->type[0] == '+') ? rightVal : -rightVal;
        }
        return result;
    }

    if (strcmp(node->type, "T") == 0) {
        int coeff = node->val;
        int exp = 0;
        if (node->right && strcmp(node->right->type, "X") == 0) {
            exp = node->right->val;
        } else if (strcmp(node->left->type, "X") == 0) {
            exp = 1;
        }
        return coeff * int_pow(x, exp);
    }

    if (strcmp(node->type, "N") == 0) {
        return node->val;
    }

    return 0;
}

void printTerm(int coeff, int exp, int isFirst) {
    if (coeff == 0) return;

    if (!isFirst && coeff > 0) printf("+");

    if (coeff == -1 && exp > 0) {
        printf("-");
    } else if (coeff != 1 || exp == 0) {
        printf("%d", coeff);
    }

    if (exp > 0) printf("x");
    if (exp > 1) printf("^%d", exp);
}

// void printderivative(struct Node* node, int isFirst) {
//     if (node == NULL) return;

//     if (strcmp(node->type, "S") == 0) {
//         if (node->left && node->left->type[0] == '-') {
//             printf("-");
//             printderivative(node->right, 1);
//         } else {
//             printderivative(node->right, isFirst);
//         }
//     } else if (strcmp(node->type, "P") == 0) {
//         printderivative(node->left, isFirst);
//         if (node->right) {
//             printderivative(node->right->right, 0);
//         }
//     } else if (strcmp(node->type, "T") == 0) {
//         int coeff = node->val;
//         int exp = 0;
//         if (node->right && strcmp(node->right->type, "X") == 0) {
//             exp = node->right->val;
//         } else if (strcmp(node->left->type, "X") == 0) {
//             exp = 1;
//         }

//         if (exp == 0) {
//             // Derivative of a constant is 0, so skip it
//             return;
//         } else if (exp == 1) {
//             // Derivative of ax is just a
//             printTerm(coeff, 0, isFirst);
//         } else {
//             // Derivative of ax^n is (a * n)x^(n-1)
//             printTerm(coeff * exp, exp - 1, isFirst);
//         }
//     }
// }


void printderivative(struct Node* node, int isFirst) {
    if (node == NULL) return;

    if (strcmp(node->type, "X") == 0) {
        if (node->right) {
            int exponent = node->right->left->val;
            printf("%dx^%d", exponent, exponent - 1);
        } else {
            printf("1");
        }
    } else if (strcmp(node->type, "T") == 0) {
        if (!isFirst) printf(" + ");
        if (node->right && strcmp(node->right->type, "X") == 0) {
            int coeff = node->left->val;
            int exponent = node->right->right->left->val;
            printf("%d*x^%d", coeff * exponent, exponent - 1);
        } else {
            printf("%d", node->left->val);
        }
    } else {
        printderivative(node->left, isFirst);
        printderivative(node->right, 0);
    }
}
int main() {
    yyparse();
    
    if (root == NULL) {
        printf("Failed to parse the input.\n");
        return 1;
    }

    setatt(root);

    printf("\nAnnotated Parse Tree:\n");
    printTree(root, 0);

    // printf("\nPolynomial evaluation for x in [-5, 5]:\n");
    // for (int x = -5; x <= 5; x++) {
    //     printf("f(%2d) = %lld\n", x, evalpoly(root, x));
    // }

    printf("\nDerivative of the polynomial: ");
    printderivative(root, 1);
    printf("\n");

    return 0;
}
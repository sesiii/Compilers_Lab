#ifndef EXPR_H
#define EXPR_H

typedef enum {
    CONST_NODE,
    ID_NODE,
    OP_NODE
} NodeType;

typedef struct _exprNode {
    NodeType type;
    union {
        int value;  // for CONST_NODE
        char *name; // for ID_NODE
        struct {
            char op;
            struct _exprNode *left;
            struct _exprNode *right;
        } opNode;   // for OP_NODE
    };
} exprNode;

// Function declarations
void generate_set_stmt(char* id, exprNode* expr);
void generate_expr_stmt(exprNode* expr);

#endif // EXPR_H
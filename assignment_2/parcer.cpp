#include <iostream>
#include <fstream>
#include <climits>
#include <cstring>  // For strcmp
#include "lex.yy.c"

using namespace std;

typedef struct Table {
    char *id;
    int val;
    struct Table *next;
} Table;

Table *table = NULL,*table_end=NULL;

Table* find_id(const char *id) {
    Table *temp = table;
    while (temp != NULL) {
        if (strcmp(temp->id, id) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

Table* add_id(const char *id) {
    Table *temp = find_id(id);
    if (temp != NULL) return temp;

    temp = new Table;
    temp->id = strdup(id);
    temp->val = INT_MAX;
    temp->next = NULL;

    if (table == NULL) {
        table = temp;
        table_end = temp;
    } else {
        table_end->next = temp;
        table_end = temp;
    }

    return temp;
}

int nxt_token, line_no=0;
int get_next_token() {
    nxt_token = yylex();
    return nxt_token;
}

class Expr;  // Forward declaration

class Data {
public:
    int val = INT_MAX;
    Table *id = NULL;
    Expr *expr = NULL;
};

class Expr {
public:
    char op;
    Data left, right;
};

Expr* parse_expr() {
    Expr *root = new Expr;  // Allocate memory for new node
    get_next_token();
    if (nxt_token > 6 || nxt_token < 2) {
        cout << "Missing operator in line no: " << line_no << '\n';
        delete root;
        return NULL;
    }
    root->op = yytext[0];
    get_next_token();

    if (nxt_token == 0) {
        root->left.expr = parse_expr();
    } else if (nxt_token == 7) {
        root->left.id = add_id(yytext);
    } else if (nxt_token == 8) {
        root->left.val = stoi(yytext);
    } else {
        cout << "Expected a value or id as 1st argument at line: " << line_no << '\n';
        delete root;
        return NULL;
    }

    get_next_token();
    if (nxt_token == 0) {
        root->right.expr = parse_expr();
    } else if (nxt_token == 7) {
        root->right.id = add_id(yytext);
    } else if (nxt_token == 8) {
        root->right.val = stoi(yytext);
    } else {
        cout << "Expected a value or id as 2nd argument at line: " << line_no << '\n';
        delete root;
        return NULL;
    }

    if (get_next_token() != 1) {
        cout << "Expected a closed bracket at line: " << line_no << '\n';
        delete root;
        return NULL;
    }
    return root;
}

void print_expr_tree(const Expr *expr, int level = 0) {
    if (expr == NULL) {
        cout << string(level * 4, ' ') << "NULL" << endl;
        return;
    }

    // Indent to represent tree level
    cout << string(level * 4, ' ');

    // Print the current node's operator
    cout << "Operator: " << expr->op << endl;

    // Print the left child
    cout << string((level + 1) * 4, ' ') << "Left:" << endl;
    if (expr->left.id != NULL) {
        cout << string((level + 2) * 4, ' ') << "ID: " << expr->left.id->id << endl;
    } else if (expr->left.val != INT_MAX) {
        cout << string((level + 2) * 4, ' ') << "Value: " << expr->left.val << endl;
    } else if (expr->left.expr != NULL) {
        print_expr_tree(expr->left.expr, level + 2);
    }

    // Print the right child
    cout << string((level + 1) * 4, ' ') << "Right:" << endl;
    if (expr->right.id != NULL) {
        cout << string((level + 2) * 4, ' ') << "ID: " << expr->right.id->id << endl;
    } else if (expr->right.val != INT_MAX) {
        cout << string((level + 2) * 4, ' ') << "Value: " << expr->right.val << endl;
    } else if (expr->right.expr != NULL) {
        print_expr_tree(expr->right.expr, level + 2);
    }
}

void delete_expr_tree(Expr *expr) {
    if (expr != NULL) {
        delete_expr_tree(expr->left.expr);
        delete_expr_tree(expr->right.expr);
        delete expr;
    }
}

void assign(){
    Table *temp=table;
    while(get_next_token()==8){
        temp->val=stoi(yytext);
        temp=temp->next;
    }
    if(temp!=NULL){
        cout<<"Identifiers are not assigned values from line: "<<line_no<<'\n';
    }
    return;
}

int expression(Expr *root){
    if(root == NULL) return 0;
    int left,right;
    if(root->left.expr!=NULL){
        left=expression(root->left.expr);
    }
    else if(root->left.id!=NULL){
        left=root->left.id->val;
    }
    else{
        left=root->left.val;
    }
    if(root->right.expr!=NULL){
        right=expression(root->right.expr);
    }
    else if(root->right.id!=NULL){
        right=root->right.id->val;
    }
    else{
        right=root->right.val;
    }

    if(root->op=='+'){
        return left+right;
    }
    else if(root->op=='-'){
        return left-right;
    }
    else if(root->op=='*'){
        return left*right;
    }
    else if(root->op=='/'){
        return left/right;
    }
    return (int)left%(int)right;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Incorrect arguments\n";
        return 0;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        cerr << "Error: Could not open input file.\n";
        return 1;
    }

    yyin = input_file;
    get_next_token();  // Initialize lexer
    Expr* tree = parse_expr();
    assign();
    print_expr_tree(tree, 0);
    fclose(input_file);
    cout<<"Value of Expression:"<<expression(tree)<<'\n';

    // Clean up memory for the symbol table
    while (table != NULL) {
        Table *temp = table;
        table = table->next;
        free(temp->id);
        delete temp;
    }

    // Clean up memory for the expression tree
    delete_expr_tree(tree);

    return 0;
}

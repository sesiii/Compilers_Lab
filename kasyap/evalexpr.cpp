#include <iostream>
#include <fstream>
#include <climits>
#include <cstring>  
#include "lex.yy.c"

using namespace std;

typedef struct Node {
    char *id;
    int val;
    struct Node *next;
} Node;

Node *head = NULL;
Node *tail = NULL;

int tok;
int token() {
    tok = yylex();
    return tok;
}

Node* find(const char *id) {
    Node *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->id, id) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

void assvals(){
    Node *temp = head;
    while(token() == 8){
        temp->val = stoi(yytext);
        temp = temp->next;
    }
    return;
}

Node* add(const char *id) {
    Node *temp = find(id);
    if (temp != NULL) return temp;

    temp = new Node;
    temp->id = strdup(id);
    temp->val = INT_MAX;
    temp->next = NULL;

    if (head == NULL) {
        head = temp;
        tail = temp;
    } else {
        tail->next = temp;
        tail = temp;
    }

    return temp;
}

int err_flag = 0;

class Exp; 

class Data {
public:
    int val = INT_MAX;
    Node *id = NULL;
    Exp *Exp = NULL;
};

class Exp {
public:
    char op;
    Data lf, rg;
};

Exp* parse() {
    Exp *r = new Exp;  
    token();
    if (tok > 6 || tok < 2) {
        if(err_flag != 1) cout << "*** Error: Operator expected in place of " << yytext << " at line: " << line_no << '\n';
        err_flag = 1;
        delete r;
        return NULL;
    }
    r->op = yytext[0];
    token();

    if (tok == 0) {
        r->lf.Exp = parse();
    } else if (tok == 7) {
        r->lf.id = add(yytext);
    } else if (tok == 8) {
        r->lf.val = stoi(yytext);
    } else {
        if(err_flag != 1) cout << "*** Error: ID/NUM/LP expected in place of " << yytext << " at line: " << line_no << '\n';
        err_flag = 1;
        delete r;
        return NULL;
    }

    token();
    if (tok == 0) {
        r->rg.Exp = parse();
    } else if (tok == 7) {
        r->rg.id = add(yytext);
    } else if (tok == 8) {
        r->rg.val = stoi(yytext);
    } else {
        if(err_flag != 1) cout << "*** Error: ID/NUM/LP expected in place of " << yytext << " at line: " << line_no << '\n';
        err_flag = 1;
        delete r;
        return NULL;
    }

    if (token() != 1) {
        if(err_flag != 1) cout << "*** Error: rg parenthesis expected in place of " << yytext << " at line: " << line_no << '\n';
        err_flag = 1;
        delete r;
        return NULL;
    }
    return r;
}

void prtree(const Exp *Exp, int level = 0) {
    if (Exp == NULL) {
        cout << string(level * 4, ' ') << "NULL" << endl;
        return;
    }

    cout << "OP(" << Exp->op <<")"<< endl;

    cout << string((level ) * 4, ' ') << "---->" ;
    if (Exp->lf.id != NULL) {
        cout <<  "ID(" << Exp->lf.id->id <<")"<< endl;
    } else if (Exp->lf.val != INT_MAX) {
        cout << "NUM(" << Exp->lf.val <<")"<< endl;
    } else if (Exp->lf.Exp != NULL) {
        prtree(Exp->lf.Exp, level + 2);
    }

    cout << string((level ) * 4, ' ') << "---->" ;
    if (Exp->rg.id != NULL) {
        cout  << "ID(" << Exp->rg.id->id<<")" << endl;
    } else if (Exp->rg.val != INT_MAX) {
        cout  << "NUM(" << Exp->rg.val<<")" << endl;
    } else if (Exp->rg.Exp != NULL) {
        prtree(Exp->rg.Exp, level + 2);
    }
}

void deltree(Exp *Exp) {
    if (Exp != NULL) {
        deltree(Exp->lf.Exp);
        deltree(Exp->rg.Exp);
        delete Exp;
    }
}

int eval(Exp *r){
    if(r == NULL) return 0;
    int lf, rg;
    if(r->lf.Exp != NULL){
        lf = eval(r->lf.Exp);
    }
    else if(r->lf.id != NULL){
        lf = r->lf.id->val;
    }
    else{
        lf = r->lf.val;
    }
    if(r->rg.Exp != NULL){
        rg = eval(r->rg.Exp);
    }
    else if(r->rg.id != NULL){
        rg = r->rg.id->val;
    }
    else{
        rg = r->rg.val;
    }

    if(r->op == '+'){
        return lf + rg;
    }
    else if(r->op == '-'){
        return lf - rg;
    }
    else if(r->op == '*'){
        return lf * rg;
    }
    else if(r->op == '/'){
        return lf / rg;
    }
    return (int)lf % (int)rg;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Incorrect arguments\n";
        return 0;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        cerr << "Could not open input file.\n";
        return 1;
    }

    yyin = input_file;
    token();  
    Exp* tree = parse();
    if(err_flag) exit(0);
    assvals();
    prtree(tree, 0);
    fclose(input_file);
    cout<<"Value of Expression:"<<eval(tree)<<'\n';

    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp->id);
        delete temp;
    }

    deltree(tree);

    return 0;
}
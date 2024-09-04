%{
    #include "expr.h"
    int yylex();
%}

%union{
    int ival;
    char *sval;
    char cval;
    struct _exprNode *expr;
    
}

%token lp rp add sub mul DIV MOD POW SET
%token <ival> NUM
%token <sval> ID
%start PROGRAM
%type <expr> EXPR
%type <cval> OP
%type <expr> ARG
%type <expr> SETSTMT

%%
PROGRAM: STMT PROGRAM
        | STMT
        ;

STMT: SETSTMT { stct++;}
    | EXPRSTMT {stct++;}
    ;

SETSTMT: lp SET ID NUM rp { 
            node* cur = lookup($3);
            if(cur == NULL){
                cur = insert($3);
            }
            set(cur, $4);
            printf("Variable %s set is to %d\n", $3, $4);
            
            }
        | lp SET ID ID rp { 
            node* n1 = lookup($3);
            node* n2 = lookup($4);
            if(n1 == NULL){
                n1 = insert($3);
            }
            if(n2 != NULL){
                set(n1, n2->value);
                printf("Variable %s set to %d\n", $3, n2->value);
            }
            else{
                printf("Variable %s not found\n", $4);
            }
        }
        | lp SET ID EXPR rp { 
            node* cur = lookup($3);
            if(cur == NULL){
                cur = insert($3);
            }
            int value = result($4);
            set(cur, value);
            printf("Variable %s set to %d\n", $3, value);
            delTree($4);                                     
        } 
        ;

EXPRSTMT: EXPR { 
            int value = result($1);
            printf("Standalone expression evaluates to %d\n\n", value);
            delTree($1);
        }
        ;

EXPR: lp OP ARG ARG rp {
        exprNode *root = createNodeOP($2, $3, $4);
        $$ = root;
        }
    ;

OP: add { $$ = '+'; }
    | sub { $$ = '-'; }
    | DIV { $$ = '/'; }
    | MOD { $$ = '%'; }
    | mul { $$ = '*'; }
    | POW { $$ = '^'; }
    ;

ARG: ID { exprNode *root = createNodeID(lookup($1)); $$ = root; }
    | NUM { exprNode *root = createNodeConst($1); $$ = root; }
    | EXPR {
        exprNode *root = $1; $$ = root;
        }
    ;

%%


int yyerror(const char *s) {

    printf("\n"); 
    fprintf(stderr, "%s\n", s);
    sleep(0.5);
    printf("\n");
    return 0;
}
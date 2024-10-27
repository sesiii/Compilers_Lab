%{
#include <stdio.h>
#include "polyutils.h"
extern int yylex();
void yyerror(const char* s);
%}

%union {
    int ival;
    struct Node* node;
}

%token <ival> DIGIT
%token PLUS MINUS XVAR POWER
%type <node> S P T X N M

%%

S : P                { $$ = createNode("S", $1, NULL); root = $$; }
  | PLUS P           { $$ = createNode("S", createNode("+", NULL, $2), NULL); root = $$; }
  | MINUS P          { $$ = createNode("S", createNode("-", NULL, $2), NULL); root = $$; }
  ;

P : T                { $$ = createNode("P", $1, NULL); }
  | T PLUS P         { $$ = createNode("P", $1, createNode("+", NULL, $3)); }
  | T MINUS P        { $$ = createNode("P", $1, createNode("-", NULL, $3)); }
  ;

T : DIGIT            { $$ = createNode("T", createLeaf($1), NULL); }
  | N                { $$ = createNode("T", $1, NULL); }
  | X                { $$ = createNode("T", $1, NULL); }
  | N X              { $$ = createNode("T", $1, $2); }
  ;

X : XVAR             { $$ = createNode("X", createLeaf('x'), NULL); }
  | XVAR POWER N     { $$ = createNode("X", createLeaf('x'), createNode("^", $3, NULL)); }
  ;

N : DIGIT            { $$ = createNode("N", createLeaf($1), NULL); }
  | DIGIT M          { $$ = createNode("N", createLeaf($1), $2); }
  ;

M : DIGIT            { $$ = createNode("M", createLeaf($1), NULL); }
  | DIGIT M          { $$ = createNode("M", createLeaf($1), $2); }
  ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE* yyin;
void yyerror(const char* s);

#define INFINITY 1000

struct SymbolTableEntry {
    char tokenName[50];
    char lexeme[100];
    int lineNumber;
};

struct SymbolTableEntry symbolTable[INFINITY];
int symbolTableCount = 0;

void addToSymbolTable(const char* tokenName, const char* lexeme) {
    if (symbolTableCount < INFINITY) {
        strncpy(symbolTable[symbolTableCount].tokenName, tokenName, 49);
        symbolTable[symbolTableCount].tokenName[49] = '\0';
        strncpy(symbolTable[symbolTableCount].lexeme, lexeme, 99);
        symbolTable[symbolTableCount].lexeme[99] = '\0';
        symbolTable[symbolTableCount].lineNumber = yylineno;
        symbolTableCount++;
    } else {
        fprintf(stderr, "Symbol table full!\n");
    }
}

void printSymbolTable() {
    printf("\nSymbol Table:\n");
    printf("-------------------------------------\n");
    printf("Token Name | Lexeme | Line Number\n");
    printf("-------------------------------------\n");
    for (int i = 0; i < symbolTableCount; i++) {
        printf("%-10s | %-10s | %d\n",
               symbolTable[i].tokenName,
               symbolTable[i].lexeme,
               symbolTable[i].lineNumber);
    }
    printf("-------------------------------------\n");
}

%}

%union {
    int ival;
    float fval;
    char* sval;
}

%token <sval> IDENTIFIER STRING_LITERAL CHARACTER_CONSTANT
%token <ival> INTEGER_CONSTANT
%token <fval> FLOAT_CONSTANT
%token KEYWORD EXPONENT_PART ESCAPE_SEQUENCE
%token LEFT_PARENTHESIS RIGHT_PARENTHESIS LEFT_SQUARE_BRACE RIGHT_SQUARE_BRACE  LEFT_CURLY_BRACE RIGHT_CURLY_BRACE
%token INCREMENT DECREMENT PLUS MINUS ASTERISK SLASH MODULO 
%token EQUALS NOT_EQUALS LESS_THAN GREATER_THAN LESS_EQUAL_THAN GREATER_EQUAL_THAN
%token BIT
%token SWITCH CASE DEFAULT
/* Relational Operators */
%token EQUALS NOT_EQUALS LESS_THAN GREATER_THAN LESS_EQUAL_THAN GREATER_EQUAL_THAN

/* Bitwise Operators */
%token BITWISE_AND BITWISE_OR BITWISE_XOR LEFT_SHIFT RIGHT_SHIFT TILDE

/* Logical Operators */
%token LOGICAL_AND LOGICAL_OR EXCLAMATION

/* Assignment Operators */
%token ASSIGNMENT PLUS_ASSIGNMENT MINUS_ASSIGNMENT ASTERISK_ASSIGNMENT SLASH_ASSIGNMENT MODULO_ASSIGNMENT
%token BITWISE_AND_ASSIGNMENT BITWISE_OR_ASSIGNMENT BITWISE_XOR_ASSIGNMENT LEFT_SHIFT_ASSIGNMENT RIGHT_SHIFT_ASSIGNMENT
%token AND_ASSIGNMENT OR_ASSIGNMENT XOR_ASSIGNMENT
%token QUESTION_MARK_COLON
%token COMMA

/* Other Punctuators */
%token QUESTION_MARK DOT ARROW COLON SEMI_COLON ELLIPSIS COMMA HASH

/* Control Flow */
%token IF ELSE FOR WHILE

%token INVALID_TOKEN
 

%start translation_unit

%%

translation_unit
    : external_declaration
    | translation_unit external_declaration
    ;

external_declaration
    : function_definition
    | declaration
    ;

function_definition
    : type_specifier IDENTIFIER '(' parameter_list ')' compound_statement
    ;

parameter_list
    : parameter_declaration
    | parameter_list ',' parameter_declaration
    |
    ;

parameter_declaration
    : type_specifier IDENTIFIER
    ;

declaration
    : type_specifier init_declarator_list ';'
    ;

init_declarator_list
    : init_declarator
    | init_declarator_list ',' init_declarator
    ;

init_declarator
    : IDENTIFIER
    | IDENTIFIER '=' initializer
    ;

initializer
    : assignment_expression
    | '{' initializer_list '}'
    | '{' initializer_list ',' '}'
    ;

initializer_list
    : initializer
    | initializer_list ',' initializer
    ;

type_specifier
    : KEYWORD
    ;

compound_statement
    : '{' block_item_list '}'
    ;

block_item_list
    : block_item
    | block_item_list block_item
    ;

block_item
    : declaration
    | statement
    ;

statement
    : expression_statement 
    | compound_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    ;

expression_statement
    : ';'
    | expression ';'
    ;

selection_statement
    : IF '(' expression ')' statement
    | IF '(' expression ')' '{' statement '}' ELSE '{' statement '}'
    | SWITCH '(' expression ')' statement
    | CASE constant ':' statement
    | DEFAULT ':' statement
    ;

iteration_statement
    : WHILE '(' expression ')' statement
    | FOR '(' expression_statement expression_statement ')' statement
    | FOR '(' expression_statement expression_statement expression ')' statement
    | FOR '(' declaration expression_statement expression ')' statement
    | FOR '(' declaration expression_statement expression_statement ')' statement

    ;

jump_statement
    : KEYWORD ';'
    | KEYWORD expression ';'
    | KEYWORD IDENTIFIER ';'

    ;

expression
    : assignment_expression
    | expression ',' assignment_expression
    | expression KEYWORD assignment_expression
    | expression KEYWORD
    | '{' block_item_list '}'
    | '{' '}'

    ;

arithmetic_operation
    : additive_expression
    | multiplicative_expression
    | shift_expression
    | relational_expression
    | equality_expression
    | and_expression
    | exclusive_or_expression
    | inclusive_or_expression
    | logical_and_expression
    | logical_or_expression
    | conditional_expression
    | assignment_expression
    ;
assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression

    ;

assignment_operator
    : '='
    | KEYWORD

    ;

conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression

    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression KEYWORD logical_and_expression
    | logical_or_expression KEYWORD
    
    ;

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression KEYWORD inclusive_or_expression
    | logical_and_expression KEYWORD
    ;

inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression
    | inclusive_or_expression KEYWORD exclusive_or_expression
    | inclusive_or_expression KEYWORD
    ;

exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression
    | exclusive_or_expression KEYWORD and_expression
    | exclusive_or_expression KEYWORD

    ;

and_expression
    : equality_expression
    | and_expression '&' equality_expression
    ;

equality_expression
    : relational_expression
    | equality_expression KEYWORD relational_expression
    | equality_expression KEYWORD
    | equality_expression "==" relational_expression
    ;

relational_expression
    : shift_expression
    | relational_expression '<' shift_expression
    | relational_expression '>' shift_expression
    | relational_expression KEYWORD shift_expression
    ;

shift_expression
    : additive_expression
    | shift_expression KEYWORD additive_expression
    ;

additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;



multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression
    | multiplicative_expression '/' cast_expression
    | multiplicative_expression '%' cast_expression
    ;

cast_expression
    : unary_expression
    | '(' type_specifier ')' cast_expression
    ;

unary_expression
    : postfix_expression
    | KEYWORD unary_expression
    | KEYWORD '(' type_specifier ')'
    ;

postfix_expression
    : primary_expression
    | postfix_expression '[' expression ']'
    | postfix_expression '(' ')'
    | postfix_expression '(' argument_expression_list ')'
    | postfix_expression '.' IDENTIFIER
    | postfix_expression KEYWORD IDENTIFIER
    | postfix_expression KEYWORD
    | postfix_expression KEYWORD
    ;

primary_expression
    : IDENTIFIER
    | constant
    | STRING_LITERAL
    | '(' expression ')'
    ;

constant
    : INTEGER_CONSTANT
    | FLOAT_CONSTANT
    | CHARACTER_CONSTANT
    ;

argument_expression_list
    : assignment_expression
    | argument_expression_list ',' assignment_expression
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
    fprintf(stderr, "Near token: %s\n", yytext);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }

    yyin = input_file;
    int parse_result = yyparse();

    fclose(input_file);

    if (parse_result == 0) {
        printf("Parsing completed successfully.\n");
        printSymbolTable();
    } else {
        printf("Parsing failed.\n");
    }

    return parse_result;
}
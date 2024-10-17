/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_A_TAB_H_INCLUDED
# define YY_YY_A_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    AUTO = 258,                    /* AUTO  */
    BREAK = 259,                   /* BREAK  */
    CASE = 260,                    /* CASE  */
    CHARTYPE = 261,                /* CHARTYPE  */
    CONST = 262,                   /* CONST  */
    CONTINUE = 263,                /* CONTINUE  */
    DEFAULT = 264,                 /* DEFAULT  */
    DO = 265,                      /* DO  */
    DOUBLE = 266,                  /* DOUBLE  */
    ELSE = 267,                    /* ELSE  */
    ENUM = 268,                    /* ENUM  */
    EXTERN = 269,                  /* EXTERN  */
    FLOATTYPE = 270,               /* FLOATTYPE  */
    FOR = 271,                     /* FOR  */
    GOTO = 272,                    /* GOTO  */
    IF = 273,                      /* IF  */
    INLINE = 274,                  /* INLINE  */
    INTTYPE = 275,                 /* INTTYPE  */
    LONG = 276,                    /* LONG  */
    REGISTER = 277,                /* REGISTER  */
    RESTRICT = 278,                /* RESTRICT  */
    RETURN = 279,                  /* RETURN  */
    SHORT = 280,                   /* SHORT  */
    SIGNED = 281,                  /* SIGNED  */
    SIZEOF = 282,                  /* SIZEOF  */
    STATIC = 283,                  /* STATIC  */
    STRUCT = 284,                  /* STRUCT  */
    SWITCH = 285,                  /* SWITCH  */
    TYPEDEF = 286,                 /* TYPEDEF  */
    UNION = 287,                   /* UNION  */
    UNSIGNED = 288,                /* UNSIGNED  */
    VOIDTYPE = 289,                /* VOIDTYPE  */
    VOLATILE = 290,                /* VOLATILE  */
    WHILE = 291,                   /* WHILE  */
    _BOOL = 292,                   /* _BOOL  */
    _COMPLEX = 293,                /* _COMPLEX  */
    _IMAGINARY = 294,              /* _IMAGINARY  */
    IDENTIFIER = 295,              /* IDENTIFIER  */
    INTEGER_CONSTANT = 296,        /* INTEGER_CONSTANT  */
    FLOATING_CONSTANT = 297,       /* FLOATING_CONSTANT  */
    CHARACTER_CONSTANT = 298,      /* CHARACTER_CONSTANT  */
    STRING_LITERAL = 299,          /* STRING_LITERAL  */
    LEFT_PARENTHESIS = 300,        /* LEFT_PARENTHESIS  */
    RIGHT_PARENTHESIS = 301,       /* RIGHT_PARENTHESIS  */
    LEFT_SQUARE_BRACE = 302,       /* LEFT_SQUARE_BRACE  */
    RIGHT_SQUARE_BRACE = 303,      /* RIGHT_SQUARE_BRACE  */
    LEFT_CURLY_BRACE = 304,        /* LEFT_CURLY_BRACE  */
    RIGHT_CURLY_BRACE = 305,       /* RIGHT_CURLY_BRACE  */
    INCREMENT = 306,               /* INCREMENT  */
    DECREMENT = 307,               /* DECREMENT  */
    PLUS = 308,                    /* PLUS  */
    MINUS = 309,                   /* MINUS  */
    ASTERISK = 310,                /* ASTERISK  */
    SLASH = 311,                   /* SLASH  */
    MODULO = 312,                  /* MODULO  */
    EQUALS = 313,                  /* EQUALS  */
    NOT_EQUALS = 314,              /* NOT_EQUALS  */
    LESS_THAN = 315,               /* LESS_THAN  */
    GREATER_THAN = 316,            /* GREATER_THAN  */
    LESS_EQUAL_THAN = 317,         /* LESS_EQUAL_THAN  */
    GREATER_EQUAL_THAN = 318,      /* GREATER_EQUAL_THAN  */
    BITWISE_AND = 319,             /* BITWISE_AND  */
    BITWISE_OR = 320,              /* BITWISE_OR  */
    BITWISE_XOR = 321,             /* BITWISE_XOR  */
    LEFT_SHIFT = 322,              /* LEFT_SHIFT  */
    RIGHT_SHIFT = 323,             /* RIGHT_SHIFT  */
    TILDE = 324,                   /* TILDE  */
    LOGICAL_AND = 325,             /* LOGICAL_AND  */
    LOGICAL_OR = 326,              /* LOGICAL_OR  */
    EXCLAMATION = 327,             /* EXCLAMATION  */
    ASSIGNMENT = 328,              /* ASSIGNMENT  */
    PLUS_ASSIGNMENT = 329,         /* PLUS_ASSIGNMENT  */
    MINUS_ASSIGNMENT = 330,        /* MINUS_ASSIGNMENT  */
    ASTERISK_ASSIGNMENT = 331,     /* ASTERISK_ASSIGNMENT  */
    SLASH_ASSIGNMENT = 332,        /* SLASH_ASSIGNMENT  */
    MODULO_ASSIGNMENT = 333,       /* MODULO_ASSIGNMENT  */
    BITWISE_AND_ASSIGNMENT = 334,  /* BITWISE_AND_ASSIGNMENT  */
    BITWISE_OR_ASSIGNMENT = 335,   /* BITWISE_OR_ASSIGNMENT  */
    BITWISE_XOR_ASSIGNMENT = 336,  /* BITWISE_XOR_ASSIGNMENT  */
    LEFT_SHIFT_ASSIGNMENT = 337,   /* LEFT_SHIFT_ASSIGNMENT  */
    RIGHT_SHIFT_ASSIGNMENT = 338,  /* RIGHT_SHIFT_ASSIGNMENT  */
    QUESTION_MARK = 339,           /* QUESTION_MARK  */
    DOT = 340,                     /* DOT  */
    ARROW = 341,                   /* ARROW  */
    COLON = 342,                   /* COLON  */
    SEMI_COLON = 343,              /* SEMI_COLON  */
    ELLIPSIS = 344,                /* ELLIPSIS  */
    COMMA = 345,                   /* COMMA  */
    HASH = 346,                    /* HASH  */
    INVALID_TOKEN = 347,           /* INVALID_TOKEN  */
    THEN = 348                     /* THEN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "a.y"

    int intVal;
    char *floatVal;
    char *charVal;
    char *strVal;
    char *identifierVal;
    char *unaryOperator;
    int instrNo;
    int parameterCount;
    Array *array;
    Statement *statement;
    Expression *expression;
    Symbol *symbol;
    SymbolType *symbolType;

#line 173 "a.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_A_TAB_H_INCLUDED  */

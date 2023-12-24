#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TEXTLEN     512 
#define NSYMTABLES  1024
// Tokens
enum {
    T_EOF, T_PLUS, T_MINUS, T_STAR, T_SLASH,
    T_EQ, T_NE, T_LT, T_GT, T_LE,T_GE,
    T_INTLIT, T_SEMI, T_ASSIGN,
    T_IDENT,
    T_LBRACE, T_RBRACE, T_LPAREN, T_RPAREN,
    T_PRINT, T_INT,T_IF, T_ELSE, T_WHILE, T_FOR, T_VOID

};

// ASP node type 
enum {
    A_ADD=1, A_SUBTRACT, A_MULTIPLY, A_DIVIDE,
    A_EQ, A_NE, A_LT, A_GT, A_LE, A_GE,
    A_INTLIT, A_IDENT, A_LVIDENT,
    A_ASSIGN,A_PRINT,  A_IF, A_GLUE, A_WHILE, A_FUNCTION,
};

#define NOREG   -1 // when genAST function have no register to return 

// Symbol table structure
struct symtable{
    char *name;
};

// Abstract Syntax tree structure
struct ASTnode{
    int op;  // Operation 
    struct ASTnode *left;
    struct ASTnode *mid;
    struct ASTnode *right;
    union{
        int intvalue; // For interger value 'A_INTLIT'
        int id;       // for A_IDENT
    }v;
};

// Tokens structure
struct token{
    int token;
    int intvalue; // For interger value 'T_INT'
};

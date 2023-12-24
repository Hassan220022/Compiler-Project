#include "defs.h"
#include "decl.h" 
#include "data.h"

// Miscellaneous functions 

// check if the word is a token 
// die of not
void match(int token, char *word){
    if(Token.token == token){
        scan(&Token);
    }
    else {
        fprintf(stderr,"%s expected in the line %d\n",word,Line);
        exit(1);
    }
}

// check if the word is a token 
// return 1 if token found otherwise -1 
int cmatch(int token, char *word){
    if(Token.token == token){
        scan(&Token);
        return 1;
    }
    return -1;
}
// match a ';'
void semi(){
    match(T_SEMI, ";");
}

// Match an identifier and fetch the next token
void ident(void) {
  match(T_IDENT, "identifier");
}
// Match a left brace and fetch the next token
void lbrace(void) {
  match(T_LBRACE, "{");
}

// Match a right brace and fetch the next token
void rbrace(void) {
  match(T_RBRACE, "}");
}

// Match a left parenthesis and fetch the next token
void lparen(void) {
  match(T_LPAREN, "(");
}

// Match a right parenthesis and fetch the next token
void rparen(void) {
  match(T_RPAREN, ")");
}
// Print out fatal messages
void fatal(char *s) {
  fprintf(stderr, "%s on line %d\n", s, Line); exit(1);
}

void fatals(char *s1, char *s2) {
  fprintf(stderr, "%s:%s on line %d\n", s1, s2, Line); exit(1);
}

void fatald(char *s, int d) {
  fprintf(stderr, "%s:%d on line %d\n", s, d, Line); exit(1);
}

void fatalc(char *s, int c) {
  fprintf(stderr, "%s:%c on line %d\n", s, c, Line); exit(1);
}

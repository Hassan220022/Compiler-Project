#include "defs.h"
#include "decl.h" 
#include "data.h"
#include <inttypes.h>




// compound_statement:          // empty, i.e. no statement
//      |      statement
//      |      statement statements
//      ;
//
// statement: print_statement
//      |     declaration
//      |     assignment_statement
//      |     if_statement
//      ;
//
// print_statement: 'print' expression ';'  ;
//
// declaration: 'int' identifier ';'  ;
//
// assignment_statement: identifier '=' expression ';'   ;
//
// if_statement: if_head
//      |        if_head 'else' compound_statement
//      ;
//
// if_head: 'if' '(' true_false_expression ')' compound_statement  ;
//
// identifier: T_IDENT ;


// Parse print statement
static struct ASTnode *print_statement(void) {
  struct ASTnode *tree;
  int reg;

  // Match a 'print' as the first token
  match(T_PRINT, "print");

  // Parse the following expression
  tree = binexpr(0);

  // Make an print AST tree
  tree = mkastunary(A_PRINT, 0, tree);

  return (tree);
}
// Parse the declaration of a variable 
static void var_declaration(void){
    // statement: 'int' identifier ';'
    //
    // identifier: T_IDENT

    match(T_INT, "int");
    ident();
    addglob(Text);
    genglobsym(Text);
}

// Parse the initialization of a variable
static struct ASTnode *var_initialization(void){
    // statement: 'int' identifier '=' expression ';'
    //
    // identifier: T_IDENT

    // Ensure we have int token 
    struct ASTnode *left, *right, *t;
    int iden; 

    // declaration part 
    var_declaration();

    // check it's been define 
    // then make a leaf node for it 
    if((iden = findglob(Text)) == -1)
        fatals("Undeclared variable", Text);
    right = mkastleaf(A_LVIDENT, iden);

    // Ensure we have equal sign 
    // if not check of semicolon and return 
    // i.e. stop at declaration
    if(cmatch(T_ASSIGN, "=") == -1){
        semi();
        return NULL;
    }

    // Parse following expression
    left = binexpr(0);

    // make an assignment tree 
    t = mkastnode(A_ASSIGN, left, NULL,  right, 0);

    return t;

}

// Parse Function declaration
// and return its AST
struct ASTnode *function_declaration(void){
    struct ASTnode *tree;
    int nameslot;

    // Find void identifier and '(' ')'
    match(T_VOID, "void");
    ident();
    nameslot = addglob(Text);
    lparen();
    rparen();

    // Parse compound statements
    tree = compound_statement();

    // Return a A_FUNCTION tree 
    return mkastunary(A_FUNCTION, nameslot, tree);

}

// Parse an IF statement including 
// any ELSE clause return its AST
struct ASTnode *if_statement(void){
    struct ASTnode *condAST, *trueAST, *falseAST = NULL;
    
    // Esure we have 'if('
    match(T_IF, "if");
    lparen();

    // Parse the following expression
    // and ensure the tree's operation is comparison 
    condAST = binexpr(0);

    if(condAST->op < A_EQ || condAST->op > A_GE)
        fatal("Bad comparison operator");
    rparen();

    // get the ast from compound_statement
    trueAST = compound_statement();

    // If we have an 'else', skip it
    // and get the AST for the compound statement
    if (Token.token == T_ELSE) {
        scan(&Token);
        falseAST = compound_statement();
    }
    // Build and return the AST for this statement
    return (mkastnode(A_IF, condAST, trueAST, falseAST, 0));
}


struct ASTnode *while_statement(void){
    struct ASTnode *condAST, *bodyAST;

    // Ensure we have while(
    match(T_WHILE,"while");
    lparen();

    // Parse the following expression
    // and ')'
    condAST = binexpr(0);
    if(condAST->op < A_EQ || condAST->op > A_GE)
        fatal("Bad comparison operator");
    rparen();
    
    // Parse compound statements
    bodyAST = compound_statement();

    return mkastnode(A_WHILE, condAST,NULL, bodyAST,0);
}

static struct ASTnode *assignment_statement(void){
    // statement: identifier '=' expression 
    //
    // identifier: T_IDENT
    struct ASTnode *left, *right, *t;
    int iden; 

    // Ensure we have an identifier
    ident();

    // check it's been define 
    // then make a leaf node for it 
    if((iden = findglob(Text)) == -1)
        fatals("Undeclared variable", Text);
    right = mkastleaf(A_LVIDENT, iden);

    // Ensure we have equal sign 
    match(T_ASSIGN, "=");

    // Parse following expression
    left = binexpr(0); // make an assignment tree 
    t = mkastnode(A_ASSIGN, left, NULL,  right, 0);



    return t;

}

// Parse a FOR loop statement 
// return its AST
struct ASTnode *for_statement(void){
    // statement: 'for' '(' assignment_statement ';' true_false_expression ';' assignment_statement ')' compound_statement
    // assignment_statement: identifier '=' expression ';'
    // true_false_expression: expression ('<' | '>' | '<=' | '>=' | '==' | '!=') expression
    // compound_statement: '{' statements '}'
    //
    //  retrun AST
    // seqASR;
    // while(true_false_expression){
    //    compound_statement;
    //    incrAST;
    // }
    //
    struct ASTnode *tree, *condAST, *bodyAST, *incrAST, *seqAST;

    // Ensure we have for(
    match(T_FOR, "for");
    lparen();

    // Get preoperation
    seqAST = single_statement(); 
    semi();

    // Parse the following expression
    // and ensure the tree's operation is comparison 
    condAST = binexpr(0);

    if(condAST->op < A_EQ || condAST->op > A_GE)
        fatal("Bad comparison operator");
    semi();

    // Parse the following expression
    // and ')'
    incrAST = single_statement();
    rparen();

    // Parse compound statements
    bodyAST = compound_statement();

    // Build and return the AST for this statement
    tree = mkastnode(A_GLUE, bodyAST, NULL, incrAST, 0);

    tree = mkastnode(A_WHILE, condAST, NULL, tree, 0);
     
    return mkastnode(A_GLUE, seqAST, NULL, tree, 0);

}


// Parse single statements 
// and return its AST 
struct ASTnode *single_statement(void){
    struct ASTnode *tree = NULL;

    switch (Token.token) {
        case T_PRINT:
            tree =print_statement();
            break;
        case T_WHILE:
            tree =while_statement();
            break;
        case T_INT:
            tree =var_initialization();
            break;
        case T_IDENT:
            tree =assignment_statement();
            break;
        case T_IF:
            tree = if_statement();
            break;
        case T_FOR:
            tree = for_statement();
            break;
        default:
            fatald("Syntax error, token", Token.token);
    }
    return tree;
}

// Parse statements
struct ASTnode *compound_statement(){
    struct ASTnode *left = NULL;
    struct ASTnode *tree;

    // Ensure that we have left curly bracket
    lbrace();

    for(;;){
        
        // Parse single statements
        tree = single_statement();

        // statements that must be followed by semicolon
        if(tree && (tree->op == A_PRINT || tree->op == A_ASSIGN)) 
            semi();
        
        // for each new tree, either save it in left 
        // if left is empty of glue the left and the 
        // new tree together 
        if(tree){
            if(left == NULL)
                left = tree ;
            else
                left = mkastnode(A_GLUE, left, NULL, tree, 0);
        }

        // If we hit a right curly bracket, skip it
        // and return the AST we've built up so far
        if(Token.token == T_RBRACE){
            rbrace();
            return left;
        }

    }

}

#include "defs.h"
#include "data.h"
#include "decl.h"


// Generic code generator 


// generate a label and return its number 
static int label(void){
    static int id = 1;
    return id++;
}

// generate the assembly code for while statement
static int genWHILE(struct ASTnode *n){
    int lstart, lend ;
    
    //generate two label for 
    //start loop and end loop
    lstart = label();
    lend = label();
    cglabel(lstart);
    
    // generate the condition code 
    genAST(n->left, lend, n->op);
    genfreeregs();



    //generate the compound statements for the body 
    genAST(n->right, NOREG, n->op);
    genfreeregs();

    // jump back to condition
    // and end label
    cgjump(lstart);
    cglabel(lend);
    return  NOREG;
}


//generate the code for as IF statement 
//and an optional ELSE clause
static int genIFAST(struct ASTnode *n){

    int lfalse, lend;

    // generate two label one for 
    // false compound statement and 
    // one for the end of the IF statement
    // when there is no ELSE statement, lfalse is 
    // the ending label
    lfalse = label();
    if(n->right)
        lend = label();
    
    // generate the condition code 
    // followed by a zero jump to the 
    // false label 
    genAST(n->left, lfalse, n->op);
    genfreeregs();

    // Generate the true compound statement
    genAST(n->mid, NOREG, n->op);
    genfreeregs();

    // if there is an ELSE clause
    // generate the jump to skip to the end label 
    if(n->right)
        cgjump(lend);

    // false label 
    cglabel(lfalse);

    // for ELSE clause generate the 
    // false compound statement and the end label
    if(n->right){
        genAST(n->right, NOREG, n->op);
        genfreeregs();
        cglabel(lend);
    }

    return NOREG;

    // In previous code we just do 
    // generate condition and jump to lfalse
    // generate statements after if 
    // jump to lend
    // lfalse label 
    // generate statement after else 
    // lend label
}

// Given an AST, generate
// assembly code
int genAST(struct ASTnode *n, int reg, int parentASTop) {
    int leftreg, rightreg;

    // We now have specific AST node handling at the top
    switch (n->op) {
        case A_IF:
            return (genIFAST(n));
        case A_WHILE:
            return (genWHILE(n));
        case A_GLUE:
            // Do each child statement, and free the
            // registers after each child
            genAST(n->left, NOREG, n->op);
            genfreeregs();
            genAST(n->right, NOREG, n->op);
            genfreeregs();
            return (NOREG);
        case A_FUNCTION:
            // Generate the prologue
            cgfuncpreamble(Gsym[n->v.id].name);
            // Generate the code for the function body
            genAST(n->left, NOREG, n->op);
            // Generate the epilogue
            cgfuncpostamble();
            return (NOREG);
    }

    // General AST node handling below

    // Get the left and right sub-tree values
    if (n->left)
        leftreg = genAST(n->left, NOREG, n->op);
    if (n->right)
        rightreg = genAST(n->right, leftreg, n->op);

    switch (n->op) {
        case A_ADD:
            return (cgadd(leftreg, rightreg));
        case A_SUBTRACT:
            return (cgsub(leftreg, rightreg));
        case A_MULTIPLY:
            return (cgmul(leftreg, rightreg));
        case A_DIVIDE:
            return (cgdiv(leftreg, rightreg));
        case A_EQ:
        case A_NE:
        case A_LT:
        case A_GT:
        case A_LE:
        case A_GE:
            // If the parent AST node is an A_IF, A_WHILE, generate a compare
            // followed by a jump. Otherwise, compare registers and
            // set one to 1 or 0 based on the comparison.
            if (parentASTop == A_IF || parentASTop == A_WHILE)
                return (cgcompare_and_jump(n->op, leftreg, rightreg, reg));
            else
                return (cgcompare_and_set(n->op, leftreg, rightreg));
        case A_INTLIT:
            return (cgloadint(n->v.intvalue));
        case A_IDENT:
            return (cgloadglob(Gsym[n->v.id].name));
        case A_LVIDENT:
            return (cgstorglob(reg, Gsym[n->v.id].name));
        case A_ASSIGN:
            // The work has already been done, return the result
            return (rightreg);
        case A_PRINT:
            // Print the left-child's value
            // and return no register
            genprintint(leftreg);
            genfreeregs();
            return (NOREG);
        default:
            fatald("Unknown AST operator", n->op);
    }
}

void genpreamble() {
  cgpreamble();
}
void genpostamble() {
  cgpostamble();
}
void genfreeregs() {
  freeall_registers();
}
void genprintint(int reg) {
  cgprintint(reg);
}
void genglobsym(char *s) {
  cgglobsym(s);
}

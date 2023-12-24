#include "defs.h"
#include "data.h"
#include "decl.h"

int interpretAST(struct ASTnode *node){
    int leftval, rightval;

    // get left and right sub-tree value

    if(node->left)
        leftval = interpretAST(node->left);
    if(node->right)
        rightval = interpretAST(node->right);

    switch (node->op) {
        case A_ADD:
            return leftval + rightval;
        case A_SUBTRACT:
            return leftval - rightval;
        case A_MULTIPLY:
            return leftval * rightval;
        case A_DIVIDE:
            return leftval / rightval;
        case A_INTLIT:
            return node->v.intvalue;
        default:
            fprintf(stderr,"Unknown AST operator %d\n",node->op);
            exit(1);
    }
}

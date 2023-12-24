
int scan(struct token *t);

// ast.c
struct ASTnode *mkastnode(int op, struct ASTnode *left, struct ASTnode *mid,
        struct ASTnode *right,int intvalue);

struct ASTnode *mkastleaf(int op, int intvalue);

struct ASTnode *mkastunary(int op, int intvalue, struct ASTnode *left);

// expr.c
struct ASTnode *binexpr(int ptp);

int interpretAST(struct ASTnode *node);


// gen.c
int genAST(struct ASTnode *n, int reg, int perantASTop);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(char *s);
//cg.c
void freeall_registers(void);
void cgpreamble();
void cgpostamble();
int cgloadint(int value);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
void cgglobsym(char *sym);
int cgstorglob(int r, char *identifier);
int cgloadglob(char *identifier);
int cgequal(int r1, int r2); 
int cgnotequal(int r1, int r2);
int cglessthan(int r1, int r2);
int cggreaterthan(int r1, int r2);
int cglessequal(int r1, int r2);
int cggreaterequal(int r1, int r2);
int cgcompare_and_set(int ASTop, int r1, int r2);
int cgcompare_and_jump(int ASTop, int r1, int r2, int label);
void cglabel(int l);
void cgjump(int l);
void cgfuncpreamble(char *name);
void cgfuncpostamble();

// misc.c
void match(int t, char *what);
int cmatch(int t, char *what);
void semi(void);
void ident(void);
void lbrace(void);
void rbrace(void);
void lparen(void);
void rparen(void);
void fatal(char *s);
void fatals(char *s1, char *s2);
void fatald(char *s, int d);
void fatalc(char *s, int c);

// stmt.c
struct ASTnode *compound_statement();
struct ASTnode *single_statement(void);
struct ASTnode *function_declaration(void);

//sym.c
int addglob(char *name);
int findglob(char *s);

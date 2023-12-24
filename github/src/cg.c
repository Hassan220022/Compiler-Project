#include "defs.h"
#include "data.h"
#include "decl.h"


// code generator for x86-64 AT&T syntax


// List of registers and freed register 
static int freereg[4];
static char *reglist[4] = { "%r8", "%r9", "%r10", "%r11" };
static char *breglist[4] = { "%r8b", "%r9b", "%r10b", "%r11b" };


// Mark all registers as free 
void freeall_registers(){
    freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

// Allocat free register 
// and retrun register number 
// exit of no available register 
static int alloc_register(){
   for(int i =0 ; i < 4 ;i++){
        if(freereg[i]){
            freereg[i] = 0;
            return i;
        }
   } 
   // no available register
   fprintf(stderr,"Out of registers\n");
   exit(1);
}

// Mark a register as free 
// check if the register is already freed
static void free_register(int r){
    if(freereg[r] != 0){
        fprintf(stderr,"Error: trying to free register %d\n",r);
        exit(1);
    }
    freereg[r] = 1;
}
// Print out the assembly preamble
void cgpreamble() {
  freeall_registers();
  fputs("\t.text\n"
	".LC0:\n"
	"\t.string\t\"%d\\n\"\n"
	"printint:\n"
	"\tpushq\t%rbp\n"
	"\tmovq\t%rsp, %rbp\n"
	"\tsubq\t$16, %rsp\n"
	"\tmovl\t%edi, -4(%rbp)\n"
	"\tmovl\t-4(%rbp), %eax\n"
	"\tmovl\t%eax, %esi\n"
	"\tleaq	.LC0(%rip), %rdi\n"
	"\tmovl	$0, %eax\n"
	"\tcall	printf@PLT\n"
	"\tnop\n"
	"\tleave\n"
	"\tret\n"
	"\n", Outfile);
}

// print function preamble
void cgfuncpreamble(char *name){
    fprintf(Outfile, "\t.globl\t%s\n", name);
    fprintf(Outfile, "\t.type\t%s, @function\n", name);
    fprintf(Outfile, "%s:\n", name);
    fprintf(Outfile, "\tpushq\t%%rbp\n");
    fprintf(Outfile, "\tmovq\t%%rsp, %%rbp\n");
}

// print function postamble
void cgfuncpostamble(){
    fputs("\tmovl	$0, %eax\n" "\tpopq	%rbp\n" "\tret\n", Outfile);
}

// Print out the assembly postamble
void cgpostamble() {
  fputs("\tmovl	$0, %eax\n" "\tpopq	%rbp\n" "\tret\n", Outfile);
}

// Load an integer literal value into a register.
// Return the number of the register
int cgloadint(int value) {
  // Get a new register
  int r = alloc_register();

  // Print out the code to initialise it
  fprintf(Outfile, "\tmovq\t$%d, %s\n", value, reglist[r]);
  return (r);
}
// Load a value from a variable into a register.
// Return the number of the register
int cgloadglob(char *identifier) {
  // Get a new register
  int r = alloc_register();

  // Print out the code to initialise it
  fprintf(Outfile, "\tmovq\t%s(\%%rip), %s\n", identifier, reglist[r]);
  return (r);
}

// Add two registers together and return
// the number of the register with the result
int cgadd(int r1, int r2) {
  fprintf(Outfile, "\taddq\t%s, %s\n", reglist[r1], reglist[r2]);
  free_register(r1);
  return (r2);
}

// Subtract the second register from the first and
// return the number of the register with the result
int cgsub(int r1, int r2) {
  fprintf(Outfile, "\tsubq\t%s, %s\n", reglist[r2], reglist[r1]);
  free_register(r2);
  return (r1);
}

// Multiply two registers together and return
// the number of the register with the result
int cgmul(int r1, int r2) {
  fprintf(Outfile, "\timulq\t%s, %s\n", reglist[r1], reglist[r2]);
  free_register(r1);
  return (r2);
}

// Divide the first register by the second and
// return the number of the register with the result
int cgdiv(int r1, int r2) {
  fprintf(Outfile, "\tmovq\t%s,%%rax\n", reglist[r1]);
  fprintf(Outfile, "\tcqo\n");
  fprintf(Outfile, "\tidivq\t%s\n", reglist[r2]);
  fprintf(Outfile, "\tmovq\t%%rax,%s\n", reglist[r1]);
  free_register(r2);
  return (r1);
}

// Call printint() with the given register
void cgprintint(int r) {
  fprintf(Outfile, "\tmovq\t%s, %%rdi\n", reglist[r]);
  fprintf(Outfile, "\tcall\tprintint\n");
  free_register(r);
}

// Store a register's value into a variable
int cgstorglob(int r, char *identifier) {
  fprintf(Outfile, "\tmovq\t%s, %s(\%%rip)\n", reglist[r], identifier);
  return (r);
}

// Generate a global symbol
void cgglobsym(char *sym) {
  fprintf(Outfile, "\t.comm\t%s,8,8\n", sym);
}

// Compare two registers
static int cgcompare(int r1, int r2, char *how){
    fprintf(Outfile, "\tcmpq\t%s, %s\n",reglist[r2], reglist[r1]);
    fprintf(Outfile, "\t%s\t%s\n",how, breglist[r2]); // move value in flag register to r2
    fprintf(Outfile, "\tandq\t$255, %s\n",reglist[r2]); // remove all bits in r2
    free_register(r1);
    return r2;
}

int cgequal(int r1, int r2) { return cgcompare(r1, r2, "sete");}
int cgnotequal(int r1, int r2) { return cgcompare(r1, r2, "setne");}
int cglessthan(int r1, int r2) { return cgcompare(r1, r2, "setl");}
int cggreaterthan(int r1, int r2) { return cgcompare(r1, r2, "setg");}
int cglessequal(int r1, int r2) { return cgcompare(r1, r2, "setle");}
int cggreaterequal(int r1, int r2) { return cgcompare(r1, r2, "setge");}

// list of comparison instructions
static char *cmplist[] = {"sete", "setne", "setl", "setg", "setle", "setge"};

// compare two register and set if true 
int cgcompare_and_set(int ASTop, int r1, int r2){
    if(ASTop < A_EQ || ASTop > A_GE)
        fatal("Bad ASTop in cgcompare_and_set()");

    fprintf(Outfile, "\tcmpq\t%s, %s\n", reglist[r2], reglist[r1]);
    fprintf(Outfile, "\t%s\t%s\n", cmplist[ASTop - A_EQ], breglist[r2]);
    // movzbq that move lowest byte from the register and extends it 
    // to fit into a 64bit register 
    fprintf(Outfile, "\tmovzbq\t%s, %s\n", breglist[r2], reglist[r2]);
    free_register(r1);
  return (r2);
}
// inverted jump instructions 
static char *invcmplist[] = { "jne", "je", "jge", "jle", "jg", "jl" };

// compare two register and jump if false 
int cgcompare_and_jump(int ASTop, int r1, int r2, int label){
    if(ASTop < A_EQ || ASTop > A_GE)
        fatal("Bad ASTop in cgcompare_and_set()");

    fprintf(Outfile, "\tcmpq\t%s, %s\n", reglist[r2], reglist[r1]);
    fprintf(Outfile, "\t%s\tL%d\n", invcmplist[ASTop - A_EQ], label);
    freeall_registers();
    return NOREG;
    
}
void cglabel(int l){
    fprintf(Outfile, "L%d:\n",l);
}
void cgjump(int l){
    fprintf(Outfile, "\tjmp\tL%d\n",l);
}

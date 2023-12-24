#include "defs.h"
#include "data.h"
#include "decl.h"

// Symbol table functions 

// position of the next free symbol slot
static int Globs = 0;


// Determine if the symbol is in the global table 
// return slot or -1 if not found 
int findglob(char *s){
    for(int i =0 ; i < Globs; i++){
        if(*s == *Gsym[i].name && !strcmp(s,Gsym[i].name))
            return i;
    }
    return -1;
}


// Get a new position of a global symbol slot 
// of exit if we run out of slot
static int newglob(void){
    int p; 

    if((p = Globs++) >= NSYMTABLES)
        fatal("Too many global symbols");
    return p;
}

// Add a new symbol in global symbol table 
// return the slot number 
int addglob(char *name){
    int p;

    // if the symbol is already in the global symbol table
    if( (p = findglob(name)) != -1)
        return p;

    // otherwise get a new slot 
    // fill it and return solt number 
    p = newglob();
    Gsym[p].name = strdup(name);
    return p;
}

#include "defs.h"
#include "decl.h" 
#include "data.h"

// Read next character from the intput file 
static int next(void) {
    int c; 

    // Use the character putback if there it one 
    if(Putback){
        c = Putback;
        Putback = 0;
        return c;
    }

    c = fgetc(Infile); // read from file 
    if(c == '\n'){
        Line++; // Increment line count 
    }
    return c;
}

// Put back unwanted character
static void putback(int c){
    Putback = c;
}

// Skip the input that we don't need to use
// i.e. whitespace, newline, taps. retrun first character we need to deal with
static int skip(void){
    int c;
    c = next();
    while(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f')
        c= next();


    return c;
}

// Reterun the position of character in string 
// and return -1 if character not found 
static int chrpos(char *s, int c){
    char *p;
    
    p = strchr(s,c);
    
    return p ? p - s : -1;
}

// Scan and return an interger literal 
static int scanint(int c){
    int i , val = 0;
    while((i = chrpos("0123456789", c)) >= 0){
        val = val * 10 + i ;
        c= next();
    }
    putback(c);
    return val;
}

// scan an identifier from the input file and 
// stor it in buf[], return indentifier's lengh
static int scanident(int c, char *buf, int limit){
    int i = 0 ;

    // allow digit and alpha and underscore
    while(isdigit(c) || isalpha(c) || c == '_'){
        // throw error if we hit identifier limit lengh
        // else appent to but and get next character
        if(limit -1 == i){
            fprintf(stderr,"Identifier too long on line %d\n",Line);
            exit(1);
        }else{
            buf[i++] = c;
        }
        c = next();
    }
    
    // hit unvalid character
    putback(c);
    buf[i] = '\0';
    return i;
}


// given a word form the input return the mathcing token 
// or 0 if not a keyword
static int keyword(char *s){
    switch (*s) {
        case 'p':
            if(!strcmp(s,"print"))
                return  T_PRINT;
            break;
        case 'i':
            if(!strcmp(s, "if"))
                return T_IF;
            if(!strcmp(s,"int"))
                return T_INT;
            break;
        case 'e':
            if(!strcmp(s, "else"))
                return T_ELSE;
        case 'w':
            if(!strcmp(s, "while"))
                return T_WHILE;
        case 'f':
            if(!strcmp(s, "for"))
                return T_FOR;
        case 'v':
            if(!strcmp(s, "void"))
                return T_VOID;
    }
    return 0;
}

// Scan funciton return next token found in input 
// return value 1 if token valid, 0 if no token left 
int scan(struct token *t) {
    int c, tokentype;

    // Skip whitespace
    c = skip();

    // Determine the token based on
    // the input character
    switch (c) {
        case EOF:
            t->token = T_EOF;
            return (0);
        case '+':
            t->token = T_PLUS;
            break;
        case '-':
            t->token = T_MINUS;
            break;
        case '*':
            t->token = T_STAR;
            break;
        case '/':
            t->token = T_SLASH;
            break;
        case ';':
            t->token = T_SEMI;
            break;
        case '=':
            if((c = next()) == '=')
                t->token = T_EQ;
            else{
                putback(c);
                t->token = T_ASSIGN;
            }
            break;
        case '!':
            if((c = next()) == '=')
                t->token = T_NE;
            else{
                fatalc("Unrecognised character", c);
            }
            break;
        case '<':
            if((c = next()) == '=')
                t->token = T_LE;
            else{
                putback(c);
                t->token = T_LT;
            }
            break;
        case '>':
            if((c = next()) == '=')
                t->token = T_GE;
            else{
                putback(c);
                t->token = T_GT;
            }
            break;
        case '{':
            t->token = T_LBRACE;
            break;
        case '}':
            t->token = T_RBRACE;
            break;
        case '(':
            t->token = T_LPAREN;
            break;
        case ')':
            t->token = T_RPAREN;
            break;
        default:

            // If it's a digit, scan the
            // literal integer value in
            if (isdigit(c)) {
                t->intvalue = scanint(c);
                t->token = T_INTLIT;
                break;
            } else if (isalpha(c) || '_' == c) {
                // Read in a keyword or identifier
                scanident(c, Text, TEXTLEN);

                // If it's a recognised keyword, return that token
                if ((tokentype = keyword(Text))) {
                    t->token = tokentype;
                    break;
                }
                // Not a recognised keyword, it must be an identifier
                t->token = T_IDENT;
                break;
            }
            // The character isn't part of any recognised token, error
            printf("Unrecognised character %c on line %d\n", c, Line);
            exit(1);
    }

    // We found a token
    return (1);
}


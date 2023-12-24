%{
#include <stdio.h>
%}

%option noyywrap

DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT})*
NUMBER      {DIGIT}+(\.{DIGIT}+)?([eE][+-]?{DIGIT}+)?

%%

"for"       { printf("FOR\n"); return FOR; }
"int"       { printf("INT\n"); return INT; }
"="         { printf("=\n"); return '='; }
"<="        { printf("<=\n"); return LEQ; }
"+"         { printf("+\n"); return '+'; }
";"         { printf(";\n"); return ';'; }
"{"         { printf("{\n"); return '{'; }
"}"         { printf("}\n"); return '}'; }
"("         { printf("(\n"); return '('; }
")"         { printf(")\n"); return ')'; }
"++"        { printf("++\n"); return INC; }
"Sum"       { printf("ID: Sum\n"); return ID; }
"X"         { printf("ID: X\n"); return ID; }

{NUMBER}    { printf("NUMBER: %s\n", yytext); return NUMBER; }
{ID}        { printf("ID: %s\n", yytext); return ID; }

[ \t\n]     ;  // Ignore whitespace and newline characters

.           { printf("Unexpected character: %s\n", yytext); }

%%

int main() {
    yylex();
    return 0;
}

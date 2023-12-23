#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*FUNCTIONS PROTOCAL*/

char *parse_for_loop(char *code);

/* Check if a word is a reserved word */

int is_reserved_word(char *word);

/* Tokenize the code */
char **tokenize(char *code);

/* Parse the tokens */
void parse(char **tokens);

#endif

#include "main.h"

/* Tokenize the code */
char **tokenize(char *code)
{
	const char *delimiters = " ();{}";
	char *token = strtok(code, delimiters);
	char **tokens = malloc(100 * sizeof(char *));
	int i = 0;

	while (token != NULL)
	{
		tokens[i] = token;
		i++;
		token = strtok(NULL, delimiters);
	}
	tokens[i] = NULL; // End of tokens
	return tokens;
}

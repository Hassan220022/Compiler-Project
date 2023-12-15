#include "main.h"

/* Parse the tokens */
void parse(char **tokens)
{
	for (int i = 0; tokens[i] != NULL; i++)
	{
		if (is_reserved_word(tokens[i]))
		{
			printf("RESERVED_WORD: %s\n", tokens[i]);
		}
		else if (isdigit(tokens[i][0]))
		{
			printf("NUMBER: %s\n", tokens[i]);
		}
		else if (strchr("=+-*/", tokens[i][0]))
		{
			printf("OPERATOR: %s\n", tokens[i]);
		}
		else
		{
			printf("IDENTIFIER: %s\n", tokens[i]);
		}
	}
}

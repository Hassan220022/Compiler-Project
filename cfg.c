#include "main.h"

/**
 * parse_for_loop - Parses a for loop in the code.
 * @code: The code to parse.
 *
 * Return: The parsed token.
 */
char *parse_for_loop(char *code)
{
	char *token = strtok(code, " ;,(){}");

	while (token != NULL)
	{
		if (strcmp(token, "for") == 0)
		{
			printf("Found a for loop\n");
		}
		else if (strcmp(token, "=") == 0)
		{
			printf("Found an assignment\n");
		}
		else if (strcmp(token, "<") == 0)
		{
			printf("Found a comparison\n");
		}
		else if (strcmp(token, "++") == 0)
		{
			printf("Found an increment\n");
		}
		else if (strcmp(token, "printf") == 0)
		{
			printf("Found a printf statement\n");
		}
		token = strtok(NULL, " ;,(){}");
	}
	return (token);
}

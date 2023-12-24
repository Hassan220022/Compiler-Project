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


#define MAX_RULES 100
#define MAX_SYMBOLS 10



typedef struct
{
	char nonterminal[10];
	char production[MAX_SYMBOLS][10];
	int production_length;
} Rule;

typedef struct
{
	Rule rules[MAX_RULES];
	int num_rules;
} CFG;

void add_rule(CFG *cfg, const char *nonterminal, const char *production[], int production_length)
{
	Rule *rule = &cfg->rules[cfg->num_rules++];
	strcpy(rule->nonterminal, nonterminal);
	for (int i = 0; i < production_length; i++)
	{
		strcpy(rule->production[i], production[i]);
	}
	rule->production_length = production_length;
}

void print_cfg(const CFG *cfg)
{
	for (int i = 0; i < cfg->num_rules; i++)
	{
		printf("%s ->", cfg->rules[i].nonterminal);
		for (int j = 0; j < cfg->rules[i].production_length; j++)
		{
			printf(" %s", cfg->rules[i].production[j]);
		}
		printf("\n");
	}
}

int main()
{
	CFG cfg = {0};
	const char *production[] = {"identifier", "=", "expression"};
	add_rule(&cfg, "assignment", production, 3);
	print_cfg(&cfg);
	return 0;
}

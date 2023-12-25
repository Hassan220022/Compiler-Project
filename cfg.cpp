#include "main.h"

// Load CFG from file
void loadCFG(struct CFG *cfg, const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		perror("Error opening file");
		return;
	}

	while (fscanf(file, "%s ::= ", cfg->rules[cfg->numRules].nonTerminal) == 1)
	{
		int numSymbols = 0;
		while (fscanf(file, "%s", cfg->rules[cfg->numRules].production[numSymbols]) == 1)
		{
			numSymbols++;
		}
		cfg->rules[cfg->numRules].numSymbols = numSymbols;
		cfg->numRules++;
	}

	fclose(file);
}

// Parse input string using CFG
int parseCFG(struct CFG *cfg, const char *input, const char *nonTerminal)
{
	if (*input == '\0')
	{
		return 1; // Input is empty, successful parse
	}

	for (int i = 0; i < cfg->numRules; i++)
	{
		if (strcmp(cfg->rules[i].nonTerminal, nonTerminal) == 0)
		{
			int matched = 1;
			const char *currentInput = input;

			for (int j = 0; j < cfg->rules[i].numSymbols; j++)
			{
				const char *symbol = cfg->rules[i].production[j];

				if (symbol[0] == '"' && symbol[strlen(symbol) - 1] == '"')
				{
					// Terminal symbol
					int symbolLength = strlen(symbol) - 2;
					if (strncmp(currentInput, symbol + 1, symbolLength) != 0)
					{
						matched = 0;
						break;
					}
					currentInput += symbolLength;
				}
				else
				{
					// Non-terminal symbol
					if (!parseCFG(cfg, currentInput, symbol))
					{
						matched = 0;
						break;
					}
				}
			}

			if (matched)
			{
				return 1; // Successful parse
			}
		}
	}

	return 0; // Unable to parse
}

/**
 * cfg.txt
 *
 * program     ::= statement
 * statement   ::= "for" "(" initialization ";" condition ";" update ")" block
 * initialization ::= data_type identifier "=" expression ";"
 * condition   ::= expression
 * update      ::= identifier "++" | identifier "--"
 * block       ::= "{" statements "}"
 * statements  ::= statement statements | ε
 * expression  ::= term expression'
 * expression' ::= "+" term expression' | "-" term expression' | ε
 * term        ::= factor term'
 * term'       ::= "*" factor term' | "/" factor term' | ε
 * factor      ::= identifier | constant | "(" expression ")"
 * data_type   ::= "int" | "float" | "char"
 * constant    ::= integer_constant | real_constant | string_literal
 */

// Function to print the loaded CFG
void printCFG(struct CFG *cfg)
{
	printf("Loaded CFG:\n");
	for (int i = 0; i < cfg->numRules; i++)
	{
		printf("%s ::= ", cfg->rules[i].nonTerminal);
		for (int j = 0; j < cfg->rules[i].numSymbols; j++)
		{
			printf("%s ", cfg->rules[i].production[j]);
		}
		printf("\n");
	}
}

int main()
{
	struct CFG cfg;
	cfg.numRules = 0;

	loadCFG(&cfg, "cfg.txt");

	// Print the loaded CFG
	printCFG(&cfg);

	const char *inputString = "for (int i = 0; i < 10; i++) { printf('Hello, world!'); }";
	int result = parseCFG(&cfg, inputString, "program");

	if (result)
	{
		printf("Input string matches CFG.\n");
	}
	else
	{
		printf("Input string does not match CFG.\n");
	}

	return 0;
}

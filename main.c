#include "main.h"

int main()
{
	char code[] = "for (int X = 0; X <= 100; X++) { Sum += X; }";
	char **tokens = tokenize(code);
	parse(tokens);
	free(tokens);
	return 0;
}

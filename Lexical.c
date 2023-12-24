#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LENGTH 1000

enum TokenType
{
	KEYWORD,
	IDENTIFIER,
	CONSTANT,
	OPERATOR,
	WHITESPACE,
	DIRECTIVE,
	OTHER
};

enum Keyword
{
	INT,
	FLOAT,
	CHAR,
	IF,
	ELSE,
	WHILE,
	FOR,
	RETURN,
	DEFINE,
	NONE
};

struct Tokens
{
	enum TokenType type;
	enum Keyword value;
};

bool isDelimiter(char ch)
{
	if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
		ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
		ch == '[' || ch == ']' || ch == '{' || ch == '}')
		return (true);
	return (false);
}
// Returns 'true' if the character is a SYMBOL.
bool isSymbol(char ch)
{
	if (ch == '{' || ch == '}' || ch == '(' || ch == ')' ||
		ch == '[' || ch == ']' || ch == ';' || ch == '#')
		return true;
	return false;
}

// Returns 'true' if the character is an OPERATOR.
bool isOperator(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == '>' || ch == '<' ||
		ch == '=')
		return (true);
	return (false);
}

// Returns 'true' if the string is a VALID IDENTIFIER.
bool validIdentifier(char *str)
{
	if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
		str[0] == '3' || str[0] == '4' || str[0] == '5' ||
		str[0] == '6' || str[0] == '7' || str[0] == '8' ||
		str[0] == '9' || isDelimiter(str[0]) == true)
		return (false);
	return (true);
}
bool isStringLiteral(char *str)
{
	int len = strlen(str);
	return (len >= 2 && str[0] == '"' && str[len - 1] == '"');
}

// Returns 'true' if the string is a KEYWORD.
bool isKeyword(char *str)
{
	if (!strcmp(str, "if") || !strcmp(str, "else") ||
		!strcmp(str, "while") || !strcmp(str, "do") ||
		!strcmp(str, "break") || !strcmp(str, "include") ||
		!strcmp(str, "stdio.h") || !strcmp(str, "printf") ||
		!strcmp(str, "continue") || !strcmp(str, "int") || !strcmp(str, "double") || !strcmp(str, "float") || !strcmp(str, "return") || !strcmp(str, "char") || !strcmp(str, "case") || !strcmp(str, "char") || !strcmp(str, "sizeof") || !strcmp(str, "long") || !strcmp(str, "short") || !strcmp(str, "typedef") || !strcmp(str, "switch") || !strcmp(str, "unsigned") || !strcmp(str, "void") || !strcmp(str, "static") || !strcmp(str, "struct") || !strcmp(str, "goto"))
		return (true);
	return (false);
}

// Returns 'true' if the string is an INTEGER.
bool isInteger(char *str)
{
	int i, len = strlen(str);

	if (len == 0)
		return (false);
	for (i = 0; i < len; i++)
	{
		if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' || (str[i] == '-' && i > 0))
			return (false);
	}
	return (true);
}
bool isFileName(char *str)
{
	// Simple check: if it contains a dot, consider it a file name (like stdio.h)
	return strchr(str, '.') != NULL;
}

// Returns 'true' if the string is a REAL NUMBER.
bool isRealNumber(char *str)
{
	int i, len = strlen(str);
	bool hasDecimal = false;

	if (len == 0)
		return (false);
	for (i = 0; i < len; i++)
	{
		if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' && str[i] != '.' ||
			(str[i] == '-' && i > 0))
			return (false);
		if (str[i] == '.')
			hasDecimal = true;
	}
	return (hasDecimal);
}

// Extracts the SUBSTRING.
char *subString(char *buffer, int left, int right)
{
	int i;
	char *subStr = (char *)malloc(
		sizeof(char) * (right - left + 2));

	for (i = left; i <= right; i++)
		subStr[i - left] = buffer[i];
	subStr[right - left + 1] = '\0';
	return (subStr);
}

void Tokenize(FILE *file)
{
	char buffer[MAX_LENGTH];
	struct Tokens token;

	while (fscanf(file, "%s", buffer) != EOF)
	{
		memset(&token, 0, sizeof(token)); // Reset token

		int left = 0, right = 0;
		int len = strlen(buffer);

		while (right <= len && left <= right)
		{
			if (!isDelimiter(buffer[right]) && !isSymbol(buffer[right]))
				right++;

			if ((isDelimiter(buffer[right]) || isSymbol(buffer[right])) && left == right)
			{
				// Check for operators and symbols, skip if neither
				if (isOperator(buffer[right]))
					printf("'%c' IS AN OPERATOR\n", buffer[right]);
				else if (isSymbol(buffer[right]))
					printf("'%c' IS A SYMBOL\n", buffer[right]);

				right++;
				left = right;
			}
			else if (isDelimiter(buffer[right]) || isSymbol(buffer[right]) || (right == len && left != right))
			{
				char *subStr = subString(buffer, left, right - 1);

				// Skip if subStr is empty
				if (strlen(subStr) > 0)
				{
					if (isKeyword(subStr))
						printf("'%s' IS A KEYWORD\n", subStr);
					else if (isStringLiteral(subStr))
						printf("'%s' IS A STRING LITERAL\n", subStr);
					else if (isInteger(subStr))
						printf("'%s' IS AN INTEGER\n", subStr);
					else if (isRealNumber(subStr))
						printf("'%s' IS A REAL NUMBER\n", subStr);
					else if (validIdentifier(subStr))
						printf("'%s' IS A VALID IDENTIFIER\n", subStr);
					else
						printf("'%s' IS NOT A VALID IDENTIFIER\n", subStr);
				}

				free(subStr);
				left = right;
			}
		}
	}
	fclose(file);
}

int main()
{
	char code[] = "for (int X = 0; X <= 100; X++) { Sum += X; }";
	char **tokens = tokenize(code);
	parse(tokens);
	free(tokens);
	return 0;
}
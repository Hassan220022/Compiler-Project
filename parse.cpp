#include "main.h"

// Function to create a new expression node
ExprNode *createExpressionNode(char operation, int value, const char *identifier)
{
	ExprNode *newNode = (ExprNode *)malloc(sizeof(ExprNode));
	if (newNode == NULL)
	{
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}

	newNode->operation = operation;
	newNode->value = value;
	newNode->identifier = (identifier != NULL) ? strdup(identifier) : NULL;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

// Function to check if a token is an identifier
int isIdentifier(const char *token)
{
	// Check if the token is not empty
	if (token == NULL || *token == '\0')
	{
		return 0; // Not an identifier
	}

	// Check the first character
	if (!isalpha(*token) && *token != '_')
	{
		return 0; // Doesn't start with a letter or underscore
	}

	// Check the remaining characters
	while (*(++token) != '\0')
	{
		if (!isalnum(*token) && *token != '_')
		{
			return 0; // Contains an invalid character
		}
	}

	return 1; // It's an identifier
}

// Function to check if a token is a constant
int isConstant(const char *token)
{
	// Check if the token is not empty
	if (token == NULL || *token == '\0')
	{
		return 0; // Not a constant
	}

	// Check for integer constant
	char *endptr;
	strtol(token, &endptr, 10);
	if (*endptr == '\0')
	{
		return 1; // It's an integer constant
	}

	// Check for floating-point constant
	char *endptrFloat;
	strtod(token, &endptrFloat);
	if (*endptrFloat == '\0')
	{
		return 1; // It's a floating-point constant
	}

	return 0; // Not a constant
}
// TODO: to implement with tonization
char *sourceCode = "myIdentifier = 123;"; // The source code to tokenize
char *currentPosition = sourceCode;		  // The current position in the source code

// Function to simulate getting the next token
char *getNextToken()
{
	static char token[256]; // Buffer to hold the token

	// Skip any whitespace
	while (isspace(*currentPosition))
		currentPosition++;

	// Check if we've reached the end of the source code
	if (*currentPosition == '\0')
		return NULL;

	// Start of the token
	char *start = currentPosition;

	// Find the end of the token
	while (!isspace(*currentPosition) && *currentPosition != '\0')
		currentPosition++;

	// Copy the token into the buffer
	strncpy(token, start, currentPosition - start);
	token[currentPosition - start] = '\0';

	return token;
}
// Function to parse an expression
ExprNode *parseExpression()
{
	char *token = getNextToken();

	// Expect a number
	if (token == NULL || !isNumber(token))
	{
		printf("Error: Expected number, got '%s'\n", token);
		return NULL;
	}

	// Create an expression node for the number
	ExprNode *exprNode = (ExprNode *)malloc(sizeof(ExprNode));
	exprNode->operation = '\0'; // No operation for a number
	exprNode->value = atoi(token);
	exprNode->identifier = NULL; // No identifier for a number
	exprNode->left = NULL;
	exprNode->right = NULL;

	token = getNextToken();

	// Check for an operator
	if (token != NULL && (strcmp(token, "+") == 0 || strcmp(token, "*") == 0))
	{
		// Create an expression node for the operator
		ExprNode *opNode = (ExprNode *)malloc(sizeof(ExprNode));
		opNode->operation = token[0]; // Assume token is a single char
		opNode->value = 0;			  // No value for an operator
		opNode->identifier = NULL;	  // No identifier for an operator
		opNode->left = exprNode;

		// Parse the right operand
		opNode->right = parseExpression();
		if (opNode->right == NULL)
		{
			free(opNode);
			return NULL;
		}

		return opNode;
	}
	else
	{
		// No operator, so unget the token and return the number
		ungetToken(token);
		return exprNode;
	}
}

// Function to parse a statement

StmtNode *parseStatement()
{
	// Get the next token
	char *token = getNextToken();

	// Check if the token is an identifier
	if (!isIdentifier(token))
	{
		printf("Error: Expected identifier, got %s\n", token);
		return NULL;
	}

	// Create a StmtNode
	StmtNode *stmtNode = (StmtNode *)malloc(sizeof(StmtNode));
	stmtNode->type = strdup("assignment");
	stmtNode->expression = (ExprNode *)malloc(sizeof(ExprNode));

	// Store the identifier in the expression node
	stmtNode->expression->identifier = strdup(token);

	// Get the next token
	token = getNextToken();

	// Check if the token is an equals sign
	if (strcmp(token, "=") != 0)
	{
		printf("Error: Expected =, got %s\n", token);
		return NULL;
	}

	// Parse the expression after the equals sign
	stmtNode->expression->left = parseExpression();

	// Get the next token
	token = getNextToken();

	// Check if the token is a semicolon
	if (strcmp(token, ";") != 0)
	{
		printf("Error: Expected ;, got %s\n", token);
		return NULL;
	}

	return stmtNode;
}

#define MAX_TOKENS 100

char *tokenStack[MAX_TOKENS];
int top = -1;

void ungetToken(char *token)
{
	if (top >= MAX_TOKENS - 1)
	{
		printf("Error: token stack overflow\n");
		return;
	}
	tokenStack[++top] = token;
}

// Function to parse the entire program
ProgramNode *parseProgram()
{
	printf("Parsing the entire program...\n");

	// Create a ProgramNode
	ProgramNode *programNode = (ProgramNode *)malloc(sizeof(ProgramNode));
	programNode->statements = NULL;
	programNode->numStatements = 0;

	// Parse statements until the end of the program
	while (1)
	{
		// Get the next token
		char *token = getNextToken();

		// Check if we've reached the end of the program
		if (token == NULL)
			break;

		// Unget the token so parseStatement can use it
		ungetToken(token);

		// Parse the next statement
		StmtNode *stmtNode = parseStatement();
		if (stmtNode == NULL)
			break;

		// Add the statement to the program
		programNode->statements = (StmtNode **)realloc(programNode->statements, sizeof(StmtNode *) * (programNode->numStatements + 1));
		programNode->statements[programNode->numStatements] = stmtNode;
		programNode->numStatements++;
	}

	return programNode;
}

// Function to free memory allocated for a program node
void freeProgram(struct ProgramNode *programNode)
{
	for (int i = 0; i < programNode->numStatements; i++)
	{
		free(programNode->statements[i]);
	}
	free(programNode->statements);
	free(programNode);
}
bool isNumber(const char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (!isdigit(str[i]))
		{
			return false;
		}
	}
	return true;
}

ExprNode *parseExpression()
{
	char *token = getNextToken();

	// Expect a number
	if (token == NULL || !isNumber(token))
	{
		printf("Error: Expected number, got '%s'\n", token);
		return NULL;
	}

	// Create an expression node for the number
	ExprNode *exprNode = (ExprNode *)malloc(sizeof(ExprNode));
	exprNode->operation = '\0'; // No operation for a number
	exprNode->value = atoi(token);
	exprNode->identifier = NULL; // No identifier for a number
	exprNode->left = NULL;
	exprNode->right = NULL;

	token = getNextToken();

	// Check for an operator
	if (token != NULL && (strcmp(token, "+") == 0 || strcmp(token, "*") == 0))
	{
		// Create an expression node for the operator
		ExprNode *opNode = (ExprNode *)malloc(sizeof(ExprNode));
		opNode->operation = token[0]; // Assume token is a single char
		opNode->value = 0;			  // No value for an operator
		opNode->identifier = NULL;	  // No identifier for an operator
		opNode->left = exprNode;

		// Parse the right operand
		opNode->right = parseExpression();
		if (opNode->right == NULL)
		{
			free(opNode);
			return NULL;
		}

		return opNode;
	}
	else
	{
		// No operator, so unget the token and return the number
		ungetToken(token);
		return exprNode;
	}
}

StmtNode *parseAssignmentStatement()
{
	char *token = getNextToken();

	// Expect a variable name
	if (token == NULL)
	{
		printf("Error: Expected variable name, got NULL\n");
		return NULL;
	}

	char *variableName = strdup(token);

	token = getNextToken();

	// Expect '='
	if (token == NULL || strcmp(token, "=") != 0)
	{
		printf("Error: Expected '=', got '%s'\n", token);
		free(variableName);
		return NULL;
	}

	// Parse the expression
	ExprNode *expression = parseExpression();
	if (expression == NULL)
	{
		free(variableName);
		return NULL;
	}

	// Create and return the assignment statement node
	StmtNode *stmtNode = (StmtNode *)malloc(sizeof(StmtNode));
	stmtNode->type = "assignment";
	stmtNode->expression = expression;

	return stmtNode;
}

StmtNode *parsePrintStatement()
{
	char *token = getNextToken();

	// Expect an identifier
	if (token == NULL || !isIdentifier(token))
	{
		printf("Error: Expected identifier, got '%s'\n", token);
		return NULL;
	}

	// Create an expression node for the identifier
	ExprNode *exprNode = (ExprNode *)malloc(sizeof(ExprNode));
	exprNode->operation = '\0';			  // No operation for an identifier
	exprNode->value = 0;				  // No value for an identifier
	exprNode->identifier = strdup(token); // Copy the identifier
	exprNode->left = NULL;
	exprNode->right = NULL;

	// Create and return the print statement node
	StmtNode *stmtNode = (StmtNode *)malloc(sizeof(StmtNode));
	stmtNode->type = "print";
	stmtNode->expression = exprNode;

	return stmtNode;
}

// Recursive descent parsing for statements
StmtNode *parseStatement()
{
	char *token = getNextToken();

	if (token == NULL)
	{
		// End of input
		return NULL;
	}
	else if (strcmp(token, "let") == 0)
	{
		// Assignment statement
		return parseAssignmentStatement();
	}
	else if (strcmp(token, "print") == 0)
	{
		// Print statement
		return parsePrintStatement();
	}
	else
	{
		printf("Error: Unexpected token '%s'\n", token);
		return NULL;
	}
}

// Recursive descent parsing for the entire program
ProgramNode *parseProgram()
{
	ProgramNode *programNode = (ProgramNode *)malloc(sizeof(ProgramNode));
	programNode->statements = (StmtNode **)malloc(sizeof(StmtNode *) * MAX_LENGTH);
	programNode->numStatements = 0;

	char *token = getNextToken();

	// Parse statements until the end of the program
	while (token != NULL)
	{
		ungetToken(token);

		// Parse the next statement
		StmtNode *stmtNode = parseStatement();
		if (stmtNode == NULL)
		{
			printf("Error: Failed to parse statement\n");
			free(programNode);
			return NULL;
		}

		// Add the statement to the program
		programNode->statements[programNode->numStatements] = stmtNode;
		programNode->numStatements++;

		token = getNextToken();
	}

	return programNode;
}

// Function to create a new expression node
ExprNode *createExprNode(char op, int val, const char *id)
{
	ExprNode *node = (ExprNode *)malloc(sizeof(ExprNode));
	node->operation = op;
	node->value = val;
	node->identifier = strdup(id);
	node->left = NULL;
	node->right = NULL;
	return node;
}

// Function to free the memory used by an expression tree
void freeExprTree(ExprNode *root)
{
	if (root != NULL)
	{
		freeExprTree(root->left);
		freeExprTree(root->right);
		free(root->identifier);
		free(root);
	}
}

// Function to parse an expression and build a parse tree
ExprNode *parseExpression()
{
	// Assume getNextToken() returns the next token in the input

	// Get the first token
	const char *token = getNextToken();

	// Check if the token is an identifier or constant
	if (isIdentifier(token))
	{
		return createExprNode('\0', 0, token);
	}
	else if (isConstant(token))
	{
		int value = atoi(token);
		return createExprNode('\0', value, NULL);
	}

	// If the token is an operator, create a node for the operator
	if (isOperator(token[0]))
	{
		return createExprNode(token[0], 0, NULL);
	}

	// Handle parentheses
	if (strcmp(token, "(") == 0)
	{
		ExprNode *node = parseExpression(); // Parse the expression within parentheses
		// Ensure the next token is ")"
		if (strcmp(getNextToken(), ")") != 0)
		{
			// Handle error: mismatched parentheses
			printf("Error: Mismatched parentheses\n");
			freeExprTree(node);
			return NULL;
		}
		return node;
	}

	// Handle other cases as needed based on your grammar

	// If none of the above cases match, there's an error
	printf("Error: Invalid expression\n");
	return NULL;
}


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

StmtNode *parseAssignmentStatement(SymbolTable *symbolTable)
{
	char *token = getNextToken();

	// Expect a variable name
	if (token == NULL)
	{
		printf("Error: Expected variable name, got NULL\n");
		return NULL;
	}

	char *variableName = strdup(token);

	// Check if the variable is already declared
	if (!checkVariableInSymbolTable(symbolTable, variableName))
	{
		printf("Error: Variable '%s' not declared.\n", variableName);
		free(variableName);
		return NULL;
	}

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

StmtNode *parsePrintStatement(SymbolTable *symbolTable)
{
	char *token = getNextToken();

	// Expect an identifier
	if (token == NULL || !isIdentifier(token))
	{
		printf("Error: Expected identifier, got '%s'\n", token);
		return NULL;
	}

	// Check if the variable is already declared
	if (!checkVariableInSymbolTable(symbolTable, token))
	{
		printf("Error: Variable '%s' not declared.\n", token);
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

StmtNode *parseFunctionDeclarationStatement(SymbolTable *symbolTable, char *returnType, char *identifier)
{
	// Check if the function is already declared
	if (checkVariableInSymbolTable(symbolTable, identifier))
	{
		printf("Error: Function '%s' is already declared.\n", identifier);
		return NULL;
	}

	// Add the function to the symbol table
	addToSymbolTable(symbolTable, identifier, returnType);

	// Create a StmtNode for the function declaration
	StmtNode *stmtNode = (StmtNode *)malloc(sizeof(StmtNode));
	stmtNode->type = strdup(returnType); // Store the return type of the function
	stmtNode->expression = (ExprNode *)malloc(sizeof(ExprNode));
	stmtNode->expression->identifier = strdup(identifier); // Store the name of the function

	return stmtNode;
}

// Recursive descent parsing for statements
StmtNode *parseStatement(SymbolTable *symbolTable)
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
		return parseAssignmentStatement(symbolTable);
	}
	else if (strcmp(token, "print") == 0)
	{
		// Print statement
		return parsePrintStatement(symbolTable);
	}
	else if (strcmp(token, "int") == 0 || strcmp(token, "float") == 0 || strcmp(token, "char") == 0)
	{
		// Variable declaration
		char *identifier = getNextToken();
		if (checkVariableInSymbolTable(symbolTable, identifier))
		{
			printf("Error: Variable '%s' is already declared.\n", identifier);
			exit(1);
		}
		addToSymbolTable(symbolTable, identifier, token);
		return parseFunctionDeclarationStatement(symbolTable, token, identifier);
	}
	else if (strcmp(token, "function") == 0)
	{
		// Function declaration
		char *returnType = getNextToken();
		char *identifier = getNextToken();
		if (checkFunctionInSymbolTable(symbolTable, identifier, returnType))
		{
			printf("Error: Function '%s' is already declared.\n", identifier);
			exit(1);
		}
		addToSymbolTable(symbolTable, identifier, returnType);
		return parseFunctionDeclarationStatement(symbolTable, returnType, identifier);
	}
	else
	{
		printf("Error: Unexpected token '%s'\n", token);
		return NULL;
	}
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

int main()
{
	// Test createExpressionNode
	ExprNode *node = createExpressionNode('+', 0, NULL);
	if (node == NULL || node->operation != '+')
	{
		printf("createExpressionNode failed\n");
	}
	else
	{
		printf("createExpressionNode passed\n");
	}
	free(node);

	// Test isIdentifier
	int result = isIdentifier("myVar");
	if (result != 1)
	{
		printf("isIdentifier failed\n");
	}
	else
	{
		printf("isIdentifier passed\n");
	}

	// Test isConstant
	result = isConstant("123");
	if (result != 1)
	{
		printf("isConstant failed\n");
	}
	else
	{
		printf("isConstant passed\n");
	}

	// Test getNextToken
	char *token = getNextToken();
	if (token == NULL)
	{
		printf("getNextToken failed\n");
	}
	else
	{
		printf("getNextToken passed\n");
	}
	free(token);

	// Test parseStatement
	StmtNode *stmt = parseStatement();
	if (stmt == NULL)
	{
		printf("parseStatement failed\n");
	}
	else
	{
		printf("parseStatement passed\n");
	}
	free(stmt);

	// Test ungetToken
	ungetToken("myToken");

	// Test parseProgram
	ProgramNode *program = parseProgram();
	if (program == NULL)
	{
		printf("parseProgram failed\n");
	}
	else
	{
		printf("parseProgram passed\n");
	}
	free(program);

	// Test createExpressionNode
	ExprNode *expr = createExpressionNode('*', 123, "myVar");
	if (expr == NULL || expr->operation != '*')
	{
		printf("createExpressionNode failed\n");
	}
	else
	{
		printf("createExpressionNode passed\n");
	}
	free(expr);

	// Test freeExprTree
	ExprNode *root = createExpressionNode('+', 0, NULL);
	root->left = createExpressionNode('\0', 123, "myVar");
	root->right = createExpressionNode('\0', 456, "myVar2");
	freeExprTree(root);

	// Test parseExpression
	ExprNode *parsedExpr = parseExpression();
	if (parsedExpr == NULL)
	{
		printf("parseExpression failed\n");
	}
	else
	{
		printf("parseExpression passed\n");
	}
	freeExprTree(parsedExpr);

	// Test parsePrintStatement
	StmtNode *printStmt = parsePrintStatement();
	if (printStmt == NULL)
	{
		printf("parsePrintStatement failed\n");
	}
	else
	{
		printf("parsePrintStatement passed\n");
	}
	free(printStmt);

	return 0;
}

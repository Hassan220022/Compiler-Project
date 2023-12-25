#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>

#define MAX_RULES 100
#define MAX_SYMBOLS 10
#define MAX_LINE_LENGTH 100
#define MAX_LENGTH 1024

// Structure to store tokens in lexical

typedef struct TokenArray
{
	char **tokens;
	int count;
};

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

// Structure to store a rule
typedef struct Rule
{
	char nonTerminal[MAX_SYMBOLS];
	char production[MAX_SYMBOLS][MAX_SYMBOLS];
	int numSymbols;
};

// Structure to represent the CFG
typedef struct CFG
{
	Rule rules[MAX_RULES];
	int numRules;
};

// parsing

// Define AST Node for expressions
typedef struct ExprNode
{
	char operation;	  // For binary operators like +, -, *, /
	int value;		  // For constants
	char *type;		  // For type of the expression (int, real, or bool)
	char *identifier; // For identifiers
	ExprNode *left;
	ExprNode *right;
};

// Define AST Node for statements
typedef struct StmtNode
{
	char *type;			  // For statement type (e.g., assignment, if, while)
	ExprNode *expression; // For expressions within statements
						  // Additional fields as needed
};

// Define AST Node for the entire program

typedef struct ProgramNode
{							 // Define the structure for the program node
	StmtNode **statements;	 // An array of pointers to statements
	int numStatements;		 // The number of statements in the program
	SymbolTable symbolTable; // Add the symbol table to the program node
};

// Structure to store information about functions
typedef struct FunctionInfo
{
	char name[MAX_SYMBOLS];
	char returnType[MAX_SYMBOLS];
} FunctionInfo;

// Structure to store information about the symbol table
typedef struct SymbolTable
{
	VariableInfo variables[MAX_SYMBOLS];
	int numVariables;
	FunctionInfo functions[MAX_SYMBOLS];
	int numFunctions;
} SymbolTable;

/*FUNCTIONS PROTOCAL*/

bool isDelimiter(char ch);
bool isSymbol(char ch);
bool isOperator(char ch);
bool validIdentifier(char *str);
bool isStringLiteral(char *str);
bool isKeyword(char *str);
bool isInteger(char *str);
bool isFileName(char *str);
bool isRealNumber(char *str);
char *subString(char *buffer, int left, int right);
TokenArray Tokenize(FILE *file);

void loadCFG(struct CFG *cfg, const char *filename);
int parseCFG(struct CFG *cfg, const char *input, const char *nonTerminal);

// Function to parse the entire program
// Function to create a new expression node
ExprNode *createExpressionNode(char operation, int value, const char *identifier);
// Function to check if a token is an identifier
int isIdentifier(const char *token);
// Function to check if a token is a constant
int isConstant(const char *token);

// TODO: to implement with tonization in line 76 to line 80
char *sourceCode = "myIdentifier = 123;"; // The source code to tokenize
char *currentPosition = sourceCode;		  // The current position in the source code
// Function to simulate getting the next token
char *getNextToken();

// Function to parse a statement
StmtNode *parseStatement();

// TODO: to implement with tonization in line 177o line 182
#define MAX_TOKENS 100

char *tokenStack[MAX_TOKENS];
int top = -1;

void ungetToken(char *token);
// Function to parse the entire program
ProgramNode *parseProgram();

// Function to free the memory used by an expression tree
void freeExprTree(ExprNode *root);
// Function to parse an expression
ExprNode *parseExpression();

// Function prototypes
StmtNode *parsePrintStatement();

// semantic_analyzer.cpp
//  Add the following prototypes to main.h
void initializeSymbolTable(SymbolTable *symbolTable);
void addToSymbolTable(SymbolTable *symbolTable, const char *name, const char *type);
int checkVariableInSymbolTable(SymbolTable *symbolTable, const char *name);
int checkFunctionInSymbolTable(SymbolTable *symbolTable, const char *name, const char *returnType);

// Modify parseStatement function in parse_tree.cpp to handle semantic analysis
StmtNode *parseStatement(SymbolTable *symbolTable);

// Function to initialize the symbol table
void initializeSymbolTable(SymbolTable *symbolTable);

// Function to add a variable or function to the symbol table
void addToSymbolTable(SymbolTable *symbolTable, const char *name, const char *type);

// Function to check if a variable is already in the symbol table
int checkVariableInSymbolTable(SymbolTable *symbolTable, const char *name);

// Function to check if a function is already in the symbol table
int checkFunctionInSymbolTable(SymbolTable *symbolTable, const char *name, const char *returnType);

// Function to perform semantic analysis on a statement
StmtNode *parseStatement(SymbolTable *symbolTable);

// semantic_analyzer.cpp
void initializeSymbolTable(SymbolTable *symbolTable);
void addToSymbolTable(SymbolTable *symbolTable, const char *name, const char *type);
int checkVariableInSymbolTable(SymbolTable *symbolTable, const char *name);
int checkFunctionInSymbolTable(SymbolTable *symbolTable, const char *name, const char *returnType);

#endif

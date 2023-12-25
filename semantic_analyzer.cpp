#include "main.h"

void initializeSymbolTable(SymbolTable *symbolTable)
{
	symbolTable->numVariables = 0;
	symbolTable->numFunctions = 0;
}

void addToSymbolTable(SymbolTable *symbolTable, const char *name, const char *type)
{
	if (strcmp(type, "function") == 0)
	{
		// Add function information to the symbol table
		strcpy(symbolTable->functions[symbolTable->numFunctions].name, name);
		strcpy(symbolTable->functions[symbolTable->numFunctions].returnType, type);
		symbolTable->numFunctions++;
	}
	else
	{
		// Add variable information to the symbol table
		strcpy(symbolTable->variables[symbolTable->numVariables].name, name);
		strcpy(symbolTable->variables[symbolTable->numVariables].type, type);
		symbolTable->numVariables++;
	}
}

int checkVariableInSymbolTable(SymbolTable *symbolTable, const char *name)
{
	for (int i = 0; i < symbolTable->numVariables; i++)
	{
		if (strcmp(symbolTable->variables[i].name, name) == 0)
		{
			return 1; // Variable found in the symbol table
		}
	}
	return 0; // Variable not found
}

int checkFunctionInSymbolTable(SymbolTable *symbolTable, const char *name, const char *returnType)
{
	for (int i = 0; i < symbolTable->numFunctions; i++)
	{
		if (strcmp(symbolTable->functions[i].name, name) == 0 &&
			strcmp(symbolTable->functions[i].returnType, returnType) == 0)
		{
			return 1; // Function found in the symbol table
		}
	}
	return 0; // Function not found
}

int main()
{
	// Initialize the symbol table
	SymbolTable symbolTable;
	initializeSymbolTable(&symbolTable);

	// Add a variable and a function to the symbol table
	addToSymbolTable(&symbolTable, "myVar", "variable");
	addToSymbolTable(&symbolTable, "myFunc", "function");

	// Check if the variable is in the symbol table
	if (checkVariableInSymbolTable(&symbolTable, "myVar"))
	{
		printf("Variable 'myVar' found in the symbol table.\n");
	}
	else
	{
		printf("Variable 'myVar' not found in the symbol table.\n");
	}

	// Check if the function is in the symbol table
	if (checkFunctionInSymbolTable(&symbolTable, "myFunc", "function"))
	{
		printf("Function 'myFunc' found in the symbol table.\n");
	}
	else
	{
		printf("Function 'myFunc' not found in the symbol table.\n");
	}

	return 0;
}

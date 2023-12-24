#include <stdio.h>
#include <stdlib.h>

// Define a structure to represent a symbol table entry
struct SymbolEntry {
    char name[50];
    char type[20];
    // Add more fields as needed
};

// Function to perform semantic analysis
void analyzeSemantic(struct SymbolEntry *symbolTable, int tableSize) {
    // Your semantic analysis logic goes here
    // Check for type compatibility, undeclared variables, etc.
}

// Function to add a symbol to the symbol table
void addToSymbolTable(struct SymbolEntry *symbolTable, int *tableSize, const char *name, const char *type) {
    // Your symbol table handling logic goes here
}

int main() {
    // Initialize the symbol table
    struct SymbolEntry symbolTable[100];
    int tableSize = 0;

    // Example: Add an entry to the symbol table
    addToSymbolTable(symbolTable, &tableSize, "X", "INT");

    // Example: Perform semantic analysis
    analyzeSemantic(symbolTable, tableSize);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Define a structure to represent a symbol table entry
struct SymbolEntry {
    char name[50];
    char type[20];
    // Add more fields as needed
};

// Token structure from the lexical analyzer
struct Token {
    enum TokenType type;
    char value[50];
};

// Function to perform semantic analysis
void analyzeSemantic(struct Token *tokens, int tokenCount) {
    struct SymbolEntry symbolTable[100];
    int tableSize = 0;

    for (int i = 0; i < tokenCount; ++i) {
        if (tokens[i].type == IDENTIFIER) {
            // Check if the identifier is declared
            if (!isDeclared(symbolTable, tableSize, tokens[i].value)) {
                printf("Error: Undeclared variable '%s'\n", tokens[i].value);
                // Handle error logic
            }
        }
        // Add more semantic checks based on your language's rules
    }

    // Perform additional semantic checks as needed

    printf("Semantic analysis completed.\n");
}

// Function to add a symbol to the symbol table
void addToSymbolTable(struct SymbolEntry *symbolTable, int *tableSize, const char *name, const char *type) {
    // Check if the symbol is already in the table
    if (!isDeclared(symbolTable, *tableSize, name)) {
        struct SymbolEntry entry;
        strncpy(entry.name, name, sizeof(entry.name));
        strncpy(entry.type, type, sizeof(entry.type));
        symbolTable[*tableSize] = entry;
        (*tableSize)++;
    } else {
        printf("Error: Redeclaration of variable '%s'\n", name);
        // Handle error logic
    }
}

// Function to check if a symbol is declared in the symbol table
bool isDeclared(struct SymbolEntry *symbolTable, int tableSize, const char *name) {
    for (int i = 0; i < tableSize; ++i) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return true;
        }
    }
    return false;
}

// Function to parse a declaration (variable_declaration or other_declaration)
void parseDeclaration(struct Token *tokens, int *index, struct SymbolEntry *symbolTable, int *tableSize) {
    // Your parsing logic for declarations goes here
    // Example: variable_declaration
    if (tokens[*index].type == INT || tokens[*index].type == FLOAT || tokens[*index].type == CHAR) {
        (*index)++;
        if (tokens[*index].type == IDENTIFIER) {
            addToSymbolTable(symbolTable, tableSize, tokens[*index].value, getTypeString(tokens[*index - 1].type));
            (*index)++;
            if (tokens[*index].type == ';') {
                (*index)++;
            } else {
                // Handle syntax error
                printf("Error: Expected ';' after variable declaration\n");
            }
        } else {
            // Handle syntax error
            printf("Error: Expected identifier after type in variable declaration\n");
        }
    } else {
        // Handle syntax error
        printf("Error: Expected type in declaration\n");
    }
}

// Function to convert TokenType to type string
const char *getTypeString(enum TokenType type) {
    switch (type) {
        case INT:
            return "INT";
        case FLOAT:
            return "FLOAT";
        case CHAR:
            return "CHAR";
        // Add more types as needed
        default:
            return "UNKNOWN";
    }
}

// Example main function
int main() {
    // Assume you have tokens from the lexical analyzer
    struct Token tokens[] = {
        {INT, "int"},
        {IDENTIFIER, "X"},
        {';', ";"},
        // Add more tokens as needed
    };

    int tokenCount = sizeof(tokens) / sizeof(tokens[0]);

    // Example: Initialize the symbol table
    struct SymbolEntry symbolTable[100];
    int tableSize = 0;

    // Example: Parse declarations and add to the symbol table
    for (int i = 0; i < tokenCount; ++i) {
        parseDeclaration(tokens, &i, symbolTable, &tableSize);
    }

    // Example: Perform semantic analysis
    analyzeSemantic(tokens, tokenCount);

    return 0;
}

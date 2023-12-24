#include <stdio.h>
#include <stdbool.h>

// Function prototypes
bool program(const char *input);
bool statement_list(const char *input);
bool statement(const char *input);
bool variable_declaration(const char *input);
bool type(const char *input);
bool assignment(const char *input);
bool expression(const char *input);
bool term(const char *input);
bool factor(const char *input);
bool for_loop(const char *input);
bool if_statement(const char *input);
bool condition(const char *input);
bool relational_operator(const char *input);
bool other_statement(const char *input);
bool identifier(const char *input);
bool number(const char *input);

// Function to print errors
void printError(const char *expected, const char *found)
{
    printf("Error: Expected %s, found %s\n", expected, found);
}

// Implementation of the CFG functions
bool program(const char *input)
{
    return statement_list(input);
}

bool statement_list(const char *input)
{
    return statement(input) && statement_list(input);
}

bool statement(const char *input)
{
    return variable_declaration(input) ||
           assignment(input) ||
           for_loop(input) ||
           if_statement(input) ||
           other_statement(input);
}

bool variable_declaration(const char *input)
{
    return type(input) && identifier(input) && (input[0] == ';');
}

bool type(const char *input)
{
    return (input[0] == 'i' && input[1] == 'n' && input[2] == 't') ||
           (input[0] == 'f' && input[1] == 'l' && input[2] == 'o' && input[3] == 'a' && input[4] == 't') ||
           (input[0] == 'c' && input[1] == 'h' && input[2] == 'a' && input[3] == 'r');
}

bool assignment(const char *input)
{
    return identifier(input) && (input[0] == '=') && expression(input) && (input[0] == ';');
}

bool expression(const char *input)
{
    return term(input) ||
           (term(input) && (input[0] == '+') && expression(input)) ||
           (term(input) && (input[0] == '-') && expression(input));
}

bool term(const char *input)
{
    return factor(input) ||
           (factor(input) && (input[0] == '*') && term(input)) ||
           (factor(input) && (input[0] == '/') && term(input));
}

bool factor(const char *input)
{
    return identifier(input) || number(input) || (input[0] == '(' && expression(input) && input[0] == ')');
}

bool for_loop(const char *input)
{
    return (input[0] == 'f' && input[1] == 'o' && input[2] == 'r') &&
           (input[0] == '(') && variable_declaration(input) && condition(input) &&
           (input[0] == ';') && assignment(input) && (input[0] == ')') && statement(input);
}

bool if_statement(const char *input)
{
    return (input[0] == 'i' && input[1] == 'f') &&
           (input[0] == '(') && condition(input) &&
           (input[0] == ')') && statement(input) && (input[0] == 'e' && input[1] == 'l' && input[2] == 's' && input[3] == 'e') && statement(input);
}

bool condition(const char *input)
{
    return expression(input) && relational_operator(input) && expression(input);
}

bool relational_operator(const char *input)
{
    return (input[0] == '<') || (input[0] == '>') ||
           (input[0] == '=' && input[1] == '=') || (input[0] == '!' && input[1] == '=');
}

bool other_statement(const char *input)
{
    // Implement other statements as needed
    return false;
}

bool identifier(const char *input)
{
    // Placeholder for identifier recognition
    // You may need to implement more sophisticated logic based on your language's rules
    return (input[0] >= 'a' && input[0] <= 'z') || (input[0] >= 'A' && input[0] <= 'Z');
}

bool number(const char *input)
{
    // Placeholder for number recognition
    // You may need to implement more sophisticated logic based on your language's rules
    return (input[0] >= '0' && input[0] <= '9');
}

int main()
{
    // Test your CFG functions with an example input
    const char *input = "for (int X = 0; X <= 100; X++) {Sum += X;}";
    if (program(input))
    {
        printf("Input is valid!\n");
    }
    else
    {
        printf("Input is invalid!\n");
    }

    return 0;
}

#include "main.h"

/* List of reserved words */
char *reserved_words[] = {"for", "int"};

/* Check if a word is a reserved word */
int is_reserved_word(char *word)
{
	int num_reserved_words = sizeof(reserved_words) / sizeof(char *);
	for (int i = 0; i < num_reserved_words; i++)
	{
		if (strcmp(word, reserved_words[i]) == 0)
		{
			return 1;
		}
	}
	return 0;
}

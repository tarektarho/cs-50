// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

unsigned int hash_value;
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{

    hash_value = hash(word);
    // Create cursor var
    node *cursor = table[hash_value];

    // Loop until the end of the linked list
    while (cursor != NULL || cursor != 0)
    {
        // Check if the words are the same
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        // Otherweise, move cursor to the next node
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Improve this hash function
    unsigned long total = 0;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        total += toupper(word[i]);
    }

    return total % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dic read from it
    FILE *file = fopen(dictionary, "r");

    // Check if value is NULL return false
    if (file == NULL)
    {
        printf("Unable to open %s\n", dictionary);
        return false;
    }

    // Read Strings form file one at a time
    char word[LENGTH + 1]; // We're doing + 1 since the last el is NULL
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for each word
        // Use malloc
        node *temp = malloc(sizeof(node));

        // Check if return value is NULL
        if (temp == NULL)
        {
            return false;
        }
        // Copy word into node using strcpy
        strcpy(temp->word, word);

        // Use the hash function
        hash_value = hash(word);

        // Check if the head is pointing to NULL
        if (table[hash_value] == NULL)
        {
            // Point temp to NULL
            temp->next = NULL;
        }
        else
        {
            // Otherwise, point temp to the first node of the linked list
            temp->next = table[hash_value];
        }
        // Point the header to temp
        table[hash_value] = temp;

        word_count++;
    }

    // Close the file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (word_count > 0)
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}

// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Custom function prototype
void initializeTable(int size);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1000;

// Hash table
node *table[N];

// Number of nodes in hash table
unsigned int word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Find which "bucket" the word should go into
    unsigned int trav = hash(word);

    // Set up a pointer to point to the "bucket" in the hash table
    node *cursor = table[trav];

    // Check each word if it matches the word in the node, keep moving cursor to the next
    while (cursor->next != NULL)
    {
        cursor = cursor->next;
        if (!strcasecmp(word, cursor->word))
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
// Variation of "https://cp-algorithms.com/string/string-hashing.html#toc-tgt-0"
unsigned int hash(const char *word)
{
    unsigned int sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        sum += (int) tolower(word[i]) * pow(13, i);
    }
    return sum % 1000;
    // return 0;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *word_bank = fopen(dictionary, "r");
    if (!word_bank) return false;

    // Create char array to store read word
    char str[LENGTH + 1];

    // Fill the hash table with nodes with NULL pointers
    initializeTable(N);

    // Read strings from file one at a time
    while ((fscanf(word_bank, "%s", str)) != EOF)
    {
        // Allocate memory for new node
        node *n = malloc(sizeof(node));
        if (!n) return false;

        // Create a new node for each word
        // Copy word from dictionary into word field of new node
        strcpy(n->word, str);

        // Set the new node's next field to NULL
        n->next = NULL;

        // Hash word to obtain a hash value
        unsigned int hashVal = hash(str);


        // Insert node into hash table at that location
        // If this is the first value to be added to 'bucket'
        if (table[hashVal]->next == NULL)
        {
            // Make the head point to the new node
            table[hashVal]->next = n;

            // Update word_count
            word_count++;
        }
        else
        {
            // Make the new node point to the node pointed to by the head
            n->next = table[hashVal]->next;

            // Make the head point to the new node
            table[hashVal]->next = n;

            // Update word_count
            word_count++;
        }
        // printf("%s\n", n->word);
    }
    fclose(word_bank);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // Create pointer to head node in hash table
        node *cursor = table[i]->next;

        // Create temp pointer to point to the same head node
        node *temp = table[i]->next;

        // If the cursor is on a node and the next field isn't NULL
        while (cursor != NULL && cursor->next != NULL)
        {
            // Make the cursor point to the next node
            cursor = cursor->next;

            // Free the node temp is pointing to
            free(temp);

            // Then make temp point to the same thing cursor is
            temp = cursor;
        }
        free(cursor);
        free(table[i]);
    }
    return true;
}

// Initialize hash table with number of pointers needed
void initializeTable(int size)
{
    // Initialize each head node in hash table with a next value of NULL
    for (int i = 0; i < size; i++)
    {
        // Allocate enough memory for a node
        node *temp = malloc(sizeof(node));

        // Set the 'next' field of the temp node to NULL
        temp->next = NULL;

        // Make each element in the hash table point to
        table[i] = temp;
    }
}

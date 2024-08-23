// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 1023;

// Hash table
node *table[N];

bool compare_two_strings(const char *word_1, const char *word_2)
{
    bool continueloop = true;
    int i = 0;
    char c1 = '\0', c2 = '\0';
    // Loop through 2 words to check their equality
    while (continueloop)
    {
        c1 = tolower(word_1[i]);
        c2 = tolower(word_2[i]);
        if (c1 != c2)
        {
            return false;
        }
        if (c1 == '\0' && c2 == '\0')
        {
            return true;
        }
        if (c1 == '\0' || c2 == '\0')
        {
            return false;
        }
        i++;
    }
    return false;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Loop through the list until NULL
    for (node *check_ptr = table[hash(word)]; check_ptr != NULL; check_ptr = check_ptr->next)
    {
        // Return when it finds the word
        if (compare_two_strings(check_ptr->word, word) == true)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // return toupper(word[0]) - 'A';
    // hash function
    int hash_code = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash_code += tolower(word[i]) * (N - i);
    }
    return hash_code % N;
}

// Add a word to a linked list
void append_to_linked_list(char *word, node **list)
{
    // Create a new node, add the word to that node, and set pointer next to NULL
    node *new_word = malloc(sizeof(node));
    if (new_word == NULL)
    {
        return;
    }
    int i = 0;
    // Add the word until hit the line break
    // MAKE SURE THERE IS NO LINE BREAK IN THE WORD IN THE MAIN CODE
    while (word[i] != '\0')
    {
        new_word->word[i] = word[i];
        i++;
    }
    new_word->word[i] = '\0';
    new_word->next = *list;
    *list = new_word;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Loop the the table and set pointers to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    // Read one character at a time and store to a string
    char buffer = 0;
    // Use to get the word one by one
    char word[LENGTH + 1];
    word[LENGTH] = '\0';
    int word_index = 0;
    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    // Loop through the dictionary and save that to the hash table
    while (fread(&buffer, sizeof(buffer), 1, file) == 1)
    {
        // Append each character to word
        word[word_index] = tolower(buffer);
        word_index++;
        if (buffer == '\n')
        {
            word[word_index - 1] = '\0';
            // Add the word to linked list
            // MAKE SURE THERE IS NO LINE BREAK IN THE WORD IN THE MAIN CODE
            append_to_linked_list(word, &table[hash(word)]);
            word_index = 0;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Looop through each hash code and count number of words
    node *ptr = NULL;
    unsigned int word_count = 0UL;
    for (int i = 0; i < N; i++)
    {
        ptr = table[i];
        while (ptr != NULL)
        {
            word_count++;
            // Go to next pointer
            ptr = ptr->next;
        }
    }
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Looop through each hash code and free the linked list
    node *ptr = NULL;
    node *free_ptr = NULL;
    for (int i = 0; i < N; i++)
    {
        ptr = table[i];
        while (ptr != NULL)
        {
            // Store current pointer
            free_ptr = ptr;
            // Go to next pointer
            ptr = ptr->next;
            // Free current pointer
            free(free_ptr);
        }
    }
    return true;
}

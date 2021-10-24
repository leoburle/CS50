// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#include <string.h>
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// 122 (ASCII z) x 27
const unsigned int N = 100000;
int numberWords = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary and false otherwise
bool check(const char *word)
{
    char wordAux [strlen(word)];

    // copies the word to an auxiliar variable
    strcpy(wordAux, word);

    int count = 0;
    while (word[count] != '\0')
    {
        if (isupper(word[count]))
        {
            wordAux[count] = tolower(word[count]);
            count++;
        }
        else
        {
            wordAux[count] = word[count];
            count++;
        }
    }

    int hashValue = hash(wordAux);

    if (table[hashValue] == NULL)
    {
        return false;
    }

    node *nodePointer = table[hashValue];

    while (nodePointer != NULL)
    {
        if (strcmp(nodePointer -> word, wordAux) == 0)
        {
            return true;
        }
        nodePointer = nodePointer -> next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // lose lose - adapted from http://www.cse.yorku.ca/~oz/hash.html, accessed in 06/14/2020
    // This hash function appeared in K&R (1st ed)
    unsigned int hash_value = 0;

    while (*word != '\0')
    {
        hash_value += *word;
        word++;
    }
    return hash_value;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r"); // Opens the file
    char word[LENGTH + 1]; // initializes the variable
    int hashValue = 0;
    node *wordNode = NULL;
    int count = 0;
    int i = 0;
    int count2 = 0;

    // Check if the file was open sucessfully
    if (file == NULL)
    {
        return false;
    }


    while (count2 < 1)
    {
        // reads word from the file
        if (fscanf(file, "%s", word) == EOF)
        {
            count2 ++;
        }

        numberWords ++;

        // Gets the hash number for the word;
        hashValue = hash(word);

        wordNode = malloc(sizeof(node));

        if (wordNode == NULL)
        {
            return false;
        }

        strcpy(wordNode->word, word);
        wordNode -> next = NULL;

        if (table[hashValue] != NULL)
        {
            wordNode->next = table[hashValue];
            table[hashValue] = wordNode;
        }
        else
        {
            table[hashValue] = wordNode;
        }
        count ++;
    }

    numberWords --;
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return numberWords;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *tmp = NULL;
    node *nodePointer = NULL;
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        tmp = table[i];
        nodePointer = table[i];

        while (nodePointer != NULL)
        {
            if (nodePointer -> next == NULL)
            {
                free(nodePointer);
                break;
            }
            nodePointer = nodePointer -> next;
            free(tmp);
            tmp = nodePointer;
        }
    }
    return true;
}

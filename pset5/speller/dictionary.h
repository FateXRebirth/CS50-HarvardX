/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// maximum size of children
#define MaxSize 27

// define tries
typedef struct node
{
    bool is_word ;
    struct node * children[MaxSize] ;
}node;

// a tries for use
node * tries;

// convert Char to Integer and put it to proper position
int selectPos(char c);

// check the pointer whether exist or not
bool isNull(node * node);

// check if any children exist
bool isExist(node * node);

// check if the word is in dictionary
bool find(node * node, char * c, int pos, int length);

// take a look dict for test
void takeLook(node * root);

// clean a whole dictionary
void deepClean(node * root);

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

#endif // DICTIONARY_H

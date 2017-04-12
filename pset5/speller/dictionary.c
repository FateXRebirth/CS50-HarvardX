/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// number of words in dictionary
unsigned int sizeDict = 0;

bool isNull(node * node)
{
    if(node == NULL)
        return true;
    return false;
}

bool isExist(node * node)
{
    // check if there is any children of this node
    struct node * temp ;
    for(int i = 0 ; i < MaxSize ; i++)
    {
        temp = node->children[i];
        if(!isNull(temp))
            return true;
    }
    return false;
}

int selectPos(char c)
{
    int i = (char)c;
    // lower case
    if( i>=97 && i<=122)
        return i-=97;
    // upper case
    else if( i>=65 && i<=90)
        return i-=65;
    // apostrophes
    else
        return i-=13;
}


void takeLook(node * root)
{
    // This function is used to check if the word is stored proper
    node * temp = NULL;
    for(int i = 0 ; i < MaxSize ; i++)
    {
        temp = root->children[i];
        if(!isNull(temp))
        {
            printf("%c",i+97);
            temp->is_word? printf(" True\n") : printf(" False\n");
            if(isExist(temp))
            {
                takeLook(temp);
            }
            else
            {
                printf("Deepest\n");
            }
        }
    }
    return;
}

void deepClean(node * root)
{
    // keep free memory until all memony is released
    for (int i = 0; i < MaxSize ; i++)
    {
        if( !isNull(root->children[i]) )
        {
            deepClean( root->children[i]); 
        }
    }
    free(root);
}

bool find(node * node, char c[], int pos, int length)
{
    // looking for a word in dictionary , if it exists , return true , otherwise , return false
    struct node * root = NULL;
    if(!isNull(node->children[selectPos(c[pos])]))
    {
        root = node->children[selectPos(c[pos])];
        if(root->is_word == true && pos == length-1)
            return true;
        else
        {
            if(find(root, c, pos+1, length))
                return true;
        }
    }
    return false;
}


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // copy the word
    int length = strlen(word);
    char c[length];
    strcpy(c,word);

    /*
    // check point
    printf("Now Check : ");
    for(int i = 0 ; i < length ; i++)
        printf("%c",c[i]);
    printf("\n");
    */
    
    node * root = tries;
    int pos = 0;
    
    if( find(root, c, pos, length))
        return true;
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // use calloc instead of malloc to eusure no any error will occur
    tries = calloc(1,sizeof(node));
    if(isNull(tries))
        return false;
    
    // check if dictionary exist
    FILE *dict = fopen(dictionary, "r");
    if(dict == NULL)
    {
        fprintf(stderr, "Dictionary doesn't exist.\n");
        return false;
    }
    int c;
    node * temp = NULL;
    node * root = NULL;
    
    // keep getting char until reach end of file
    while( (c = fgetc(dict)) != EOF)
    {
        // keep getting char until find space
        while(isalpha(c))  
        {   
            
            // if it exists in the hashtable
            if(tries->children[selectPos(c)]!=NULL)
                root = tries->children[selectPos(c)];
            // if it doesn't exist in the hashtable
            else
            {
                root = calloc(1,sizeof(node));
                if(isNull(root))
                    return false;
                root->is_word = false;
                tries->children[selectPos(c)] = root;
            }
            // check if it's behind of another char
            c = fgetc(dict);
            while(isalpha(c) || (c == '\''))
            {
                // if it exists in the children
                if(root->children[selectPos(c)]!=NULL)
                    temp = root->children[selectPos(c)];
                // if it doesn't exist in children
                else
                {
                    temp = calloc(1,sizeof(node));
                    if(isNull(temp))
                        return false;
                    temp->is_word = false;
                    root->children[selectPos(c)] = temp;
                }
                root = temp;
                // check next char
                c = fgetc(dict);
            }
        }
        // is end of the word
        root->is_word = true;
        // record how many words in the dictionary
        sizeDict++;
    }
    
    /*
    // check point
    printf("Dictionary owns %i words.\n", sizeDict);
    takeLook(tries);
    printf("\n");
    */
    
    fclose(dict);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return sizeDict;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    deepClean(tries);
    return true;
}


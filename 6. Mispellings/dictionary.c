/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include "dictionary.h"
#include <cs50.h>
#include <stdint.h>

// typedef a node for the linked list
typedef struct node
{
    char* value;
    struct node* next;
}
node;

// function prototypes
bool insert_word(char* value, int index);
uint32_t hash(const char *str);
char *strdup(const char *str);
char *strtolower(const char *word, int len);

// global variables
uint32_t indexsize = 12000;
uint32_t INDXSZ;
node* head = NULL;
node* lists[12000];
int dict_size = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // change the word to lowercase
    char* word_lower = strtolower(word, strlen(word));
    
    // prepare a node pointer and getting index
    int index = hash(word_lower);
    node* curr_node = lists[index];
    
    // check if the word is present in the hash table
    while (curr_node != NULL)
    {
        if (strcmp(curr_node->value, word_lower) == 0)
        {
            free(word_lower);
            return true;
        }
        curr_node = curr_node->next;
    }
    
    free(word_lower);
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // initialize the lists to NULL
    for (int i = 0; i < indexsize; i++)
    {
        lists[i] = NULL;
    }

    // prepare variables
    int index = 0;
    char word[LENGTH+1];
    FILE* fp = fopen(dictionary, "r");
    
    // get each word in text
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            word[index] = c;
            index++;

            // ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // consume remainder of alphabetical string
                while ((c = fgetc(fp)) != EOF && isalpha(c));

                // prepare for new word
                index = 0;
            }
        }

        // ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // consume remainder of alphanumeric string
            while ((c = fgetc(fp)) != EOF && isalnum(c));

            // prepare for new word
            index = 0;
        }

        // we must have found a whole word
        else if (index > 0)
        {
            // terminate current word
            word[index] = '\0';
            
            // adding the word in the hashtable
            insert_word(word, hash(word));

            // prepare for next word
            index = 0;
        }
    }
    
    // check whether there was an error
    if (ferror(fp))
    {
        fclose(fp);
        printf("Error reading %s.\n", dictionary);
        return false;
    }

    // close text
    fclose(fp);
   
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return dict_size;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // iterate through the lists array
    for (int index = 0; index < indexsize; index++)
    {
        if (lists[index] != NULL)
        {
            // prepare pointers
            node* curr_node = lists[index];
            node* next_node = NULL;
            
            // iterate through the current linked list and free every node
            while (curr_node != NULL)
            {
                //free(curr_node->value); - HAS TO BE FIXED
                next_node = curr_node->next;
                //free(curr_node); - HAS TO BE FIXED
                curr_node = next_node;
            }
        }
    }
    
    return true;
}

/**
 * Create a new node for a given value and insert it into as a head of the hashtable.
 */
bool insert_word(char* value, int index)
{
    // prepare a node pointer
    node* new_node = malloc(sizeof(node));
    new_node->value = strdup(value);
    new_node->next = NULL;
    
    // if current linked list is empty
    if (head == NULL)
    {
        head = new_node;
    }
    // if current linked list is not empty
    else
    {
        new_node->next = head;
        head = new_node;
    }
    
    // add the new value to the lists array and increment dict_size
    lists[index] = head;
    dict_size++;
    
    return true;
}

// hash function is taken from http://dm.8mydog.com/src/assignments/harvard/cs50/pset6/dictionary.c?view
uint32_t hash(const char *str) {
    INDXSZ = indexsize - 1;
    
    unsigned long hash = 5381;
    int c;
 
    while ((c = *str++))
        hash = ((hash << 5) + hash) + tolower(abs(c)); /* hash * 33 + c */
    //case sensitive:   hash = ((hash << 5) + hash) + abs(c); /* hash * 33 + c */
 
    return (uint32_t) hash & INDXSZ;
    /* faster than doing hash % indexsize + 1;
     * snipped from: http://forums.devshed.com/showpost.php?p=231648&postcount=3
     */
}

char* strdup(const char* str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup)
    {
        strcpy(dup, str);
    }
    return dup;
}

char* strtolower(const char* word, int len) {
    char* str = malloc(1 + (len* sizeof (char)));
    for (int i = 0; i < len; i++)
    {
        str[i] = tolower((int) word[i]);
    }
    str[len] = '\0';
    return str;
}

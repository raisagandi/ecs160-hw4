#include "dictionary.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Dictionary* dict_new() {
    Dictionary *dictionary = (Dictionary*)malloc(sizeof(Dictionary));
    assert(dictionary != NULL);
    dictionary->head = NULL;
    dictionary->tail = NULL;
    return dictionary;
}

void dict_add(Dictionary *dictionary, const char *key, int value) {
    
    // Remove existing key
    if(dict_has(dictionary, key) == 1)
        dict_remove(dictionary, key);

    // Adding a new key after existing head
    if(dictionary != NULL && dictionary->head != NULL) { 
        
        // Find an empty tail
        while(dictionary->tail) {
	    dictionary = dictionary->tail;
        } 
	// Now, create a new tail
	if(!dictionary->tail) {
	    dictionary->tail = dict_new();
	    dictionary = dictionary->tail;
	}
    }
    
    // Adding brand new key and value pair
    int key_length = strlen(key) + 1;
    dictionary->head = (KVPair*)malloc(sizeof(KVPair));
    assert(dictionary->head != NULL);
    dictionary->head->key = (char *)malloc(key_length * sizeof(char));
    assert(dictionary->head->key != NULL);
    strcpy(dictionary->head->key, key);
    dictionary->head->value = value;
}

int dict_has(Dictionary *dictionary, const char *key) {
    
    if (dictionary->head == NULL)
        return 0;

    while(dictionary) {
    
        if(strcmp(dictionary->head->key, key) == 0)
            return 1;
	dictionary = dictionary->tail;
    }
    
    return 0;
}

int dict_get(Dictionary *dictionary, const char *key) {
    
    if (dictionary->head == NULL)
        return 0;
    
    while(dictionary) {
    
        if(strcmp(dictionary->head->key, key) == 0)
            return dictionary->head->value;
        dictionary = dictionary->tail;
    }

    return 0;
}

void dict_remove(Dictionary *dictionary, const char *key) {
    
    if (dictionary->head == NULL)
        return;
    // Setting previous dictionary
    Dictionary *previous = NULL; 

    while(dictionary != NULL) {

	// Found matching key
    	if(strcmp(dictionary->head->key, key) == 0) {

    	    if(previous == NULL) {
                free(dictionary->head->key);
                dictionary->head->key = NULL;
		free(dictionary->head);
		dictionary->head = NULL;
                if(!dictionary->tail) // No tail
	            return;

		// Setting the new head and tail after deleting the old head
                if (dictionary->head == NULL && dictionary->tail != NULL) {                
		    Dictionary *toremove = dictionary->tail;
                    dictionary->head->key = toremove->head->key;
                    dictionary->tail = toremove->tail;
                    free(toremove->head);
                    free(toremove);
                    toremove->head = NULL;
		    toremove = NULL;
		    return;
                }
            }
            else { 
		// Setting the new dictionary's tail to the old tail 
                previous->tail = dictionary->tail;
            }
	    // Get rid of the old dictionary
            free(dictionary->head->key);
            free(dictionary->head);
            free(dictionary);
            return;
        }
        previous = dictionary;
        dictionary = dictionary->tail;
    }
}

void dict_free(Dictionary *dictionary) {
    if(dictionary == NULL)
        return;
    free(dictionary->head->key);
    free(dictionary->head);
    Dictionary *tail = dictionary->tail;
    free(dictionary);
    dict_free(tail);
}

/*
 * Debugging aid
 */
void dict_print(Dictionary *dictionary) {    

    if(!dictionary) {
        printf("Dictionary is empty\n");
        return;
    } 

    while(dictionary->head != NULL) {
        printf("key: %s, value: %d\n", dictionary->head->key, dictionary->head->value); 
	if(dictionary->tail)
	    dictionary = dictionary->tail;
        else
            return;
    }

}

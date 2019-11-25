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
    
    printf("\nIN DICT_ADD\n");
    printf("%s %d\n", key, value);

    if(dict_has(dictionary, key) == 1) // Remove existing key
        dict_remove(dictionary, key);
    
    if(dictionary != NULL && dictionary->head != NULL) { // Adding a new key after existing head
        
        printf("adding a new key after existing head! woohoo!\n");
        while(dictionary->tail) {
	    printf("go to the tail -- tailception lol\n");
	    dictionary = dictionary->tail;
            printf("dictionary is now the tail\n");
        } 
	if(!dictionary->tail) { // tail is null
	    printf("creating a new tail\n");
	    dictionary->tail = dict_new();
	    dictionary = dictionary->tail;
	}
    }
    
    printf("about to add new key and value\n");
    int key_length = strlen(key) + 1;
    dictionary->head = (KVPair*)malloc(sizeof(KVPair));
    assert(dictionary->head != NULL);
    dictionary->head->key = (char *)malloc(key_length * sizeof(char));
    assert(dictionary->head->key != NULL);
    strcpy(dictionary->head->key, key);
    dictionary->head->value = value;

    printf("dict head key: %s, value: %d\n", dictionary->head->key, dictionary->head->value);
}

int dict_has(Dictionary *dictionary, const char *key) {
    
    printf("\nIN DICT_HAS\n");

    if (dictionary->head == NULL)
        return 0;

    while(dictionary) {
	printf("dict head key: %s\n", dictionary->head->key);
        if(strcmp(dictionary->head->key, key) == 0)
            return 1;
        //if(!dictionary->tail) {
        //    printf("tail is NULL\n");
	//    break;
        //}	    

	dictionary = dictionary->tail;
        printf("dictionary tailing....\n\n");
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

// HAS A BUG! Not deleting properly.
void dict_remove(Dictionary *dictionary, const char *key) {
    if (dictionary->head == NULL)
        return;
    Dictionary *previous = NULL; 
    while(dictionary != NULL) {
        if(strcmp(dictionary->head->key, key) == 0) {
            if(previous == NULL) {
                free(dictionary->head->key);
                dictionary->head->key = NULL;
		free(dictionary->head);
		dictionary->head = NULL;
		printf("dict head key is now null\n");
                if(!dictionary->tail) {
	            return;
		}

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
                previous->tail = dictionary->tail;
            }
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

void dict_print(Dictionary *dictionary) {    

    printf("\nin DICT_PRINT\n");

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

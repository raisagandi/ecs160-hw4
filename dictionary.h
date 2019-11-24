#ifndef DICT_H
#define DICT_H

typedef struct {
    char *key;
    int value;
} KVPair;

typedef struct Dictionary_t {
    KVPair *head;
    struct Dictionary_t *tail;
} Dictionary;

Dictionary* dict_new();
void dict_add(Dictionary *dictionary, const char *key, int value);
int dict_has(Dictionary *dictionary, const char *key);
int dict_get(Dictionary *dictionary, const char *key);
void dict_remove(Dictionary *dictionary, const char *key);
void dict_free(Dictionary *dictionary);

#endif // DICT_H

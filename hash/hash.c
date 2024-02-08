#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

struct node {
    struct node *next;
    char *key;
    char *value;
};

#define HASHSIZE 101

// array of linked lists
static struct node *table[HASHSIZE];

static char *strduplicate(char *string) {
    char *new = (char *) malloc(strlen(string) + 1);
    if (new) {
        strcpy(new, string);
    }
    return new;
}

// the "hashing function" takes a string and returns an index into hash table
static unsigned hash(char *s) {
    // unsiged arithmetic ensures hash value is positive.
    unsigned hashval = 0;

    // logic for hash function
    for (; *s != '\0'; ++s) {
        hashval = *s + 31 * hashval;
    }

    return hashval % HASHSIZE;
}

// look for key (string) in hash table
struct node *get(char *key) {
    unsigned hashval = hash(key);

    struct node *node = table[hashval];

    // walk the linked list
    for (; node != NULL; node = node->next) {
        if (strcmp(key, node->key) == 0) {
            return node;
        }
    }

    return NULL;
}

// check if key already exists
// if yes, add
// if not, update
// return NULL in case node cannot be added (e.g., ran out of memory)
struct node *set(char *key, char *value) {
    struct node *node = get(key);

    // key not found
    if (node == NULL) {
        node = (struct node *) malloc(sizeof(*node));

        if (!node) {
            return NULL;
        }

        node->key = strduplicate(key);

        if (!node->key) {
            free(node);
            return NULL;
        }

        unsigned hashval = hash(key);
        node->next = table[hashval];
        table[hashval] = node;
    }
    // key found
    else {
        free(node->value);
    }

    node->value = strduplicate(value);

    if (!node->value) {
        return NULL;
    }

    return node;
}

static void print_node(struct node *node) {
    printf("key=%s, value=%s\n", node->key, node->value);
}

void print_hash() {
    for (unsigned i = 0; i < HASHSIZE; ++i) {
        struct node *node = table[i];
        for (; node != NULL; node = node->next) {
            print_node(node);
        }
    }
}

int main() {
    char *key1 = "love";
    char *val1 = "you";
    char *key2 = "hate";
    char *val2 = "noone";

    set(key1, val1);
    set(key2, val2);

    print_hash();
    set(key2, "noONE!");
    print_hash();
}

/*
Name: Boris Vukajlovic
Ssid:15225054

This program initializes and configures a resizing hash table,
that uses a linked list implemetation and its functions, which
include a cleanup, bucket resize, insert, lookup,load factor,
delete, cleanup function.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "hash_table.h"

struct table {
    struct node **array;
    unsigned long (*hash_func)(const unsigned char *);
    double max_load_factor;
    unsigned long capacity;
    unsigned long load;
};

struct node {
    char *key;
    struct array *value;
    struct node *next;
};

struct table *table_init(unsigned long capacity,
                         double max_load_factor,
                         unsigned long (*hash_func)(const unsigned char *)) {

    struct table *table = malloc(sizeof(struct table));
    if (table == NULL) {
        return NULL;
    }

    table->array = calloc(capacity, sizeof(struct node *));
    if (table->array == NULL) {
        free(table);
        return NULL;
    }

    table->capacity = capacity;
    table->max_load_factor = max_load_factor;
    table->hash_func = hash_func;
    table->load = 0;

    return table;
}

/*Function that resizes the hash bucket array, by doubling its capacity.

Input: struct table *t, the pointer to the hash table

Output: returns 1 if unsuccesfull, returns 0 if the function is succesfull

Side effect: resizes the bucket array of the hash table*/
int resize_bucket(struct table *t) {
    if (t == NULL) {
        return 1;
    }

    struct node *to_move = NULL;
    struct node *next = NULL;

    t->capacity *= 2;
    struct node **re_array = calloc(t->capacity, sizeof(struct node *));
    if (re_array == NULL) {
        return 1;
    }

    for (unsigned long int i = 0; i < (t->capacity / 2); i++) {
        to_move = t->array[i];

            while (to_move != NULL) {
                next = to_move->next;
                unsigned long new_index = (t->hash_func
                        ((const unsigned char *)to_move->key) % (t->capacity));

                to_move->next = re_array[new_index];
                re_array[new_index] = to_move;

                to_move = next;
            }
    }

    free(t->array);
    t->array = re_array;

    return 0;
}

int table_insert(struct table *t, const char *key, int value) {
    if (t == NULL || key == NULL) {
        return 1;
    }

    if (table_load_factor(t) >= t->max_load_factor) {
        if (resize_bucket(t) == 1) {
            return 1;
        }
    }

    unsigned long index = (t->hash_func
            ((const unsigned char *)key) % t->capacity);

    struct array *existing_array = table_lookup(t, key);
    if (existing_array != NULL) {
        array_append(existing_array, value);
        return 0;
    }

    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        return 1;
    }

    new_node->key = malloc(sizeof(const unsigned char *) * (strlen(key) + 1));
    if (new_node->key == NULL) {
        free(new_node);
        return 1;
    }

    strcpy(new_node->key, key);
    if (new_node->key == NULL) {
        free(new_node->key);
        free(new_node);
        return 1;
    }

    new_node->value = array_init(10);
    if (new_node->value == NULL) {
        free(new_node->key);
        free(new_node);
        return 1;
    }

    if (array_append(new_node->value, value) != 0) {
        free(new_node->key);
        array_cleanup(new_node->value);
        free(new_node);
        return 1;
    }

    new_node->next = t->array[index];
    t->array[index] = new_node;

    t->load++;
    return 0;
}

struct array *table_lookup(const struct table *t, const char *key) {
    if (t == NULL || key == NULL) {
        return NULL;
    }

    unsigned long index = (t->hash_func((const unsigned char *)key) % t->capacity);

    struct node *tmp = t->array[index];

    while (tmp != NULL) {
        if (strcmp(tmp->key, key) == 0) {
            return tmp->value;
        }
        tmp = tmp->next;
    }

    return NULL;
}

double table_load_factor(const struct table *t) {
    if (t == NULL || t->capacity == 0) {
        return -1.0;
    }

    return (double)t->load / (double)t->capacity;
}

int table_delete(struct table *t, const char *key) {
    if (t == NULL || key == NULL) {
        return -1;
    }

    struct node *current = NULL;
    struct node *previous = NULL;

    unsigned long index = (t->hash_func((const unsigned char *)key) % t->capacity);
    current = t->array[index];

    while (current != NULL) {
        if (!strcmp(current->key, key)) {
            t->load--;

            if (previous != NULL) {
                previous->next = current->next;
            } else {
                t->array[index] = current->next;
            }

            free(current->key);
            array_cleanup(current->value);
            free(current);
            return 0;
        }
        previous = current;
        current = current->next;
    }

    return 1;
}

void table_cleanup(struct table *t) {
    if (t == NULL) {
        return;
    }

    struct node *to_free = NULL;
    struct node *next = NULL;

    for (unsigned long int i = 0; i < (t->capacity); i++) {
    to_free = t->array[i];

        while (to_free != NULL) {
            next = to_free->next;

            free(to_free->key);
            array_cleanup(to_free->value);
            free(to_free);
            to_free = next;
        }
    }
    free(t->array);
    free(t);
}

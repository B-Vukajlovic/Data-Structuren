/*
Name: Boris Vukajlovic
Ssid:15225054

This program initializes and configure a resizing array and its
functions, which include a cleanup, delete, append, get and size
function.*/

#include <stdlib.h>
#include <limits.h>
#include "array.h"

#define ARRAY_ERROR ULONG_MAX

struct array {
    int *array;
    long unsigned int size;
    long unsigned int capacity;
};

struct array *array_init(unsigned long initial_capacity) {
    struct array *array = malloc(sizeof(struct array));
    if (array == NULL) {
        return NULL;
    }

    array->array = malloc(initial_capacity * (sizeof(int)));
    if (array->array == NULL) {
        free(array);
        return NULL;
    }

    array->size = 0;
    array->capacity = initial_capacity;

    return array;
}

void array_cleanup(struct array *a) {
    free(a->array);
    free(a);
}

int array_get(const struct array *a, unsigned long index) {
    if (a == NULL) {
        return -1;
    }

    if (index >= a->size) {
        return -1;
    }

    return a->array[index];
}

int array_append(struct array *a, int elem) {
    if (a == NULL) {
        return 1;
    }

    if (a->size == a->capacity) {

        if (a->capacity > 0) {
            a->capacity *= 2;
        } else {
            a->capacity = 1;
        }

        int *tmp = realloc(a->array, a->capacity * sizeof(int));
        if (tmp == NULL) {
            return 1;
        }

        a->array = tmp;
    }

    a->array[a->size] = elem;
    a->size++;
    return 0;
}

unsigned long array_size(const struct array *a) {
    if (a == NULL) {
        return ARRAY_ERROR;
    }

    return a->size;
}

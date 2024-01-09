/*
Naam: Boris Vukajlovic
SSID:

This program implements a priority queue and its functions which allows to user
to use and manipulate data stored in said queue. These functions include a prioq
initialize, prioq size, prioq cleanup, prioq insert, prioq pop.*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

#define ERROR __INT_MAX__
#define ARRSIZE 20

static struct heap *heap_init(int (*compare)(const void *, const void *)) {
    struct heap *h = malloc(sizeof(struct heap));
    if (h == NULL) {
        return NULL;
    }

    h->array = array_init(ARRSIZE);
    if (h->array == NULL) {
        free(h);
        return NULL;
    }

    h->compare = compare;
    return h;
}

prioq *prioq_init(int (*compare)(const void *, const void *)) {
    prioq *q = heap_init(compare);
    if (q == NULL) {
        return NULL;
    }

    return q;
}

long int prioq_size(const prioq *q) {
    if (q == NULL || q->array == NULL) {
        return -1;
    }

    return array_size(q->array);
}

static int heap_cleanup(struct heap *h, void free_func(void *)) {
    if (h == NULL || h->array == NULL) {
        return -1;
    }

    array_cleanup(h->array, free_func);
    free(h);
    return 0;
}

int prioq_cleanup(prioq *q, void free_func(void *)) {
    if (q == NULL) {
        return -1;
    }

    heap_cleanup(q, free_func);
    return 0;
}

static int heap_insert(struct heap *h, void *p) {
    if (h == NULL || h->array == NULL) {
        return -1;
    }

    if (array_append(h->array, p) != 0) {
        return -1;
    }

    long i = array_size(h->array) - 1;
    while (i > 0) {
        long parent_index = (i - 1) / 2;
        void *parent = array_get(h->array, parent_index);
        void *current = array_get(h->array, i);

        if (parent != NULL && current != NULL && h->compare(current, parent) < 0) {
            array_set(h->array, parent_index, current);
            array_set(h->array, i, parent);

            i = parent_index;
        } else {
            break;
        }
    }

    return 0;
}

int prioq_insert(prioq *q, void *p) {
    if (p == NULL || q == NULL) {
        return ERROR;
    }

    heap_insert(q, p);
    return 0;
}

static void *heap_pop(struct heap *h) {
    if (h == NULL || h->array == NULL || array_size(h->array) == 0) {
        return NULL;
    }

    void *top = array_get(h->array, 0);
    void *last = array_pop(h->array);

    if (array_size(h->array) > 0) {
        array_set(h->array, 0, last);

        long index = 0;
        while (1) {
            long left_index = 2 * index + 1;
            long right_index = 2 * index + 2;
            long smallest = index;

            void *current = array_get(h->array, index);
            void *left_child = array_get(h->array, left_index);
            void *right_child = array_get(h->array, right_index);

            if (left_index < array_size(h->array) &&
                    left_child != NULL && h->compare(current, left_child) > 0) {

                smallest = left_index;
            }

            if (right_index < array_size(h->array) &&
                    right_child != NULL && h->compare(right_child, array_get(h->array, smallest)) < 0) {

                smallest = right_index;
            }

            if (smallest != index) {
                void *temp = array_get(h->array, smallest);
                array_set(h->array, smallest, current);
                array_set(h->array, index, temp);
                index = smallest;

            } else {
                break;
            }
        }
    }
    return top;
}

void *prioq_pop(prioq *q) {
    if (q == NULL) {
        return NULL;
    }

    return heap_pop(q);
}

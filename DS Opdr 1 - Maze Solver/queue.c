#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue {
    int *queue_data;
    size_t queue_capacity;
    size_t current_size;
    size_t pop_count;
    size_t push_count;
    int max_elements;
};

/*Initialiseert een pointer naar een struct van type queue, en alloceert plek op de heap
voor de struct queue en de queue_data array.

size_t capacity : grote van de queue_data array binnen de gealloceerde
queue.

Side effect : Initialiseert waarden binnen de struct queue.*/
struct queue *queue_init(size_t capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    struct queue *q = malloc(sizeof(struct queue));
    if (q == NULL) {
        return NULL;
    }

    q->queue_data = malloc(sizeof(int) * capacity);
    if (q->queue_data == NULL) {
        free(q); 
        return NULL;
    }

    q->queue_capacity = capacity;
    q->max_elements = 0;
    q->pop_count = 0;
    q->push_count = 0;

    return q;
}

/*struct queue *s : pointer naar de queue struct op de heap

Side effect: Freed de queue_data array en de struct queue op de heap.*/
void queue_cleanup(struct queue *q) {
    if (q == NULL) {
        return;
    }
    free(q->queue_data);
    free(q);
}

/*const struct queue *s : pointer naar de queue struct op de heap

Side effect: print de push_count, pop_count en max_elements naar de stderr output stream*/
void queue_stats(const struct queue *q) {
    if (q == NULL) {
        return;
    }
    fprintf(stderr, "stats %zu %zu %d\n",
        q->push_count, q->pop_count, q->max_elements);
}

/*Returned een integer die aangeeft of een integer succesvol gepushed is, 1 indien niet, 0 indien wel

struct queue *s : pointer naar de queue struct op de heap
int c : de integer die gepushed wordt op de queue

Side effect: pushed een integer op de queue*/
int queue_push(struct queue *q, int e) {
    if (q == NULL) {
        return 1;
    }

    if (q->queue_capacity != queue_size(q)) {
        q->queue_data[q->push_count++ % q->queue_capacity] = e;
        if ((int)queue_size(q) > q->max_elements) {
            q->max_elements = (int)queue_size(q);
        }
        return 0;
    } else {
        return 1;
    }
}

/*Returned een integer die aangeeft of een integer succesvol gepopped is, of de gepoppte integer zelf

struct queue *s : pointer naar de queue struct op de heap

side effect: een integer wordt gepopped van de queue*/
int queue_pop(struct queue *q) {
    if (q == NULL || queue_size(q) <= 0) {
        return -1;
    }
    return q->queue_data[q->pop_count++ % q->queue_capacity];
}

/*Returned de integer op de top van de queue, of -1 indien dit niet gelukt is

const struct queue *s : pointer naar de queue struct op de heap

*/
int queue_peek(const struct queue *q) {
    if (q == NULL) {
        return -1;
    } else if (queue_size(q) <= 0) {
        return 1;
    }
    return q->queue_data[q->pop_count % q->queue_capacity];
}

/*Returned een -1 indien de struct pointer NULL is, 1 als de queue leeg is, 0 als de queue elementen bevat

struct queue *s : pointer naar de queue struct op de heap*/
int queue_empty(const struct queue *q) {
    if (q == NULL) {
        return -1;
    } else if (queue_size(q) <= 0) {
        return 1;
    } else {
        return 0;
    }
}

/*Returned een -1 indien de struct pointer NULL is, 1 als de queue leeg is, 0 als de queue elementen bevat

struct queue *s : pointer naar de queue struct op de heap*/
size_t queue_size(const struct queue *q) {
    if (q == NULL) {
        return 0;
    }
    return (q->push_count - q->pop_count);
}

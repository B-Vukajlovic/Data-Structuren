#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

#define EMPTY -1

typedef struct stack {
    int *stack_data;
    size_t stack_capacity;
    int stack_top;
    int pop_count;
    int push_count;
    int max_elements;
} stack_t;

/*Initialiseert een pointer naar een struct van type stack, en alloceert plek op de heap
voor de struct stack en de stack_data array.

size_t capacity : grote van de stack_data array binnen de gealloceerde
stack.

Side effect : Initialiseert waarden binnen de struct stack.*/
struct stack *stack_init(size_t capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    stack_t *s = (stack_t *)malloc(sizeof(stack_t));
    if (s == NULL) {
        return NULL;
    }

    s->stack_data = (int *)malloc(capacity * sizeof(size_t));
    if (s->stack_data == NULL) {
        free(s);
        return NULL;
    }

    s->stack_top = EMPTY;
    s->stack_capacity = capacity;
    s->pop_count = 0;
    s->push_count = 0;
    s->max_elements = 0;

    return s;
}

/*struct stack *s : pointer naar de stack struct op de heap

Side effect: Freed de stack_data array en de struct stack op de heap.*/
void stack_cleanup(struct stack *s) {
    if (s == NULL) {
        return;
    }
    free(s->stack_data);
    free(s);
}

/*const struct stack *s : pointer naar de stack struct op de heap

Side effect: print de push_count, pop_count en max_elements naar de stderr output stream*/
void stack_stats(const struct stack *s) {
    if (s == NULL) {
        return;
    }
    fprintf(stderr, "stats %d %d %d\n", s->push_count, s->pop_count, s->max_elements);
}

/*Returned een integer die aangeeft of de stack_data array succesvol is vergroot, 0 indien niet, 1 indien wel;

struct stack *s : pointer naar de stack struct op de heap

Side effect: Vergroot de size of de stack_data array op de*/
int resize_stack(struct stack *s) {
    int *tmp;
    tmp = (int*)realloc(s->stack_data, (s->stack_capacity * 2 + 1) * sizeof(int));
    s->stack_capacity = ((s->stack_capacity * 2 + 1));
    if (tmp == NULL) {
        return 0;
    }
    s->stack_data = tmp;
    return 1;
}

/*Returned een integer die aangeeft of een integer succesvol gepushed is, 1 indien niet, 0 indien wel

struct stack *s : pointer naar de stack struct op de heap
int c : de integer die gepushed wordt op de stack

Side effect: pushed een integer op de stack*/
int stack_push(struct stack *s, int c) {
    if (stack_empty(s) == -1) {
        return 1;
    }

    s->stack_top++;
    if (s->stack_top >= (int)s->stack_capacity) {
        resize_stack(s);
    }

    if ((int)stack_size(s) > s->max_elements) {
        s->max_elements = (int)stack_size(s);
    }
    s->push_count++;
    s->stack_data[s->stack_top] = c;
    return 0;
}

/*Returned een integer die aangeeft of een integer succesvol gepopped is, of de gepoppte integer zelf

struct stack *s : pointer naar de stack struct op de heap

side effect: een integer wordt gepopped van de stack*/
int stack_pop(struct stack *s) {
    if (stack_empty(s) == 1 || stack_empty(s) == -1) {
        return -1;
    }

    s->stack_top--;
    s->pop_count++;
    return s->stack_data[s->stack_top + 1];
}

/*Returned de integer op de top van de stack, of -1 indien dit niet gelukt

const struct stack *s : pointer naar de stack struct op de heap

*/
int stack_peek(const struct stack *s) {
    if (stack_empty(s) == -1) {
        return -1;
    }

    return s->stack_data[s->stack_top];
}

/*Returned een -1 indien de struct pointer NULL is, 1 als de stack leeg is, 0 als de stack elementen bevat

struct stack *s : pointer naar de stack struct op de heap*/
int stack_empty(const struct stack *s) {
    if (s == NULL) {
        return -1;
    } else if (s->stack_top == EMPTY) {
        return 1;
    } else {
        return 0;
    }

}

/*Returned de grote van de stack en 0 indien struct pointer gelijk is aan NULL

struct stack *s : pointer naar de stack struct op de heap*/
size_t stack_size(const struct stack *s) {
    if (s == NULL) {
        return 0;
    }
    return (size_t)s->stack_top + 1;
}

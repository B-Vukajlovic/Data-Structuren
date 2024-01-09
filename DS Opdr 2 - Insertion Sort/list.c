#include "list.h"

/*
Name: Boris Vukajlovic
SSID: 15225054
This program initializes a linked list and provides functions to use and manipulate it.
*/

typedef struct node {
    int value;
    struct node *next;
    struct node *previous;
} node_t;

typedef struct list {
    node_t *head;
    node_t *tail;
} list_t;


struct list *list_init(void) {
    list_t *list = malloc(sizeof(list_t));

    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;

    return list;
}

struct node *list_new_node(int num) {
    node_t *node = malloc(sizeof(node_t));

    if (node == NULL) {
        return NULL;
    }

    node->next = NULL;
    node->previous = NULL;
    node->value = num;

    return node;
}

struct node *list_head(const struct list *l) {
    if (l == NULL) {
        return NULL;
    }

    return l->head;
}

struct node *list_next(const struct node *n) {
    if (n == NULL || n->next == NULL) {
        return NULL;
    }

    return n->next;
}

int list_add_front(struct list *l, struct node *n) {
    if (n == NULL || l == NULL) {
        return 1;
    }

    if (l->tail == NULL || l->head == NULL) {
        l->head = n;
        l->tail = n;
        return 0;
    }

    n->next = l->head;
    l->head->previous = n;
    l->head = n;

    return 0;
}

struct node *list_tail(const struct list *l) {
    if (l == NULL) {
        return NULL;
    }

    return l->tail;
}

struct node *list_prev(const struct list *l, const struct node *n) {
    if (l == NULL || n == NULL || list_node_present(l, n) == 0 || n == l->head) {
        return NULL;
    }

    return n->previous;
}

int list_add_back(struct list *l, struct node *n) {
    if (n == NULL || l == NULL) {
        return 1;
    }

    if (l->tail == NULL || l->head == NULL) {
        l->head = n;
        l->tail = n;
        return 0;
    }

    n->previous = l->tail;
    l->tail->next = n;
    l->tail = n;

    return 0;
}

int list_node_get_value(const struct node *n) {
    if (n == NULL) {
        return 0;
    }

    return n->value;
}

int list_node_set_value(struct node *n, int value) {
    if (n == NULL || sizeof(value) != sizeof(int)) {
        return 1;
    }

    n->value = value;
    return 0;
}

int list_unlink_node(struct list *l, struct node *n) {
    if (n == NULL || l == NULL || list_node_present(l, n) == 0) {
        return 1;
    }

    node_t *tmp_prev = n->previous;
    node_t *tmp_next = n->next;

    if (l->head == n) {
        l->head = tmp_next;
        if (l->head != NULL) {
            l->head->previous = NULL;
        }
    }

    if (l->tail == n) {
        l->tail = tmp_prev;
        if (l->tail != NULL) {
            l->tail->next = NULL;
        }
    }

    if (tmp_prev != NULL) {
        tmp_prev->next = tmp_next;
    }
    if (tmp_next != NULL) {
        tmp_next->previous = tmp_prev;
    }

    n->next = NULL;
    n->previous = NULL;

    return 0;

}

void list_free_node(struct node *n) {
    if (n != NULL) {
        free(n);
    }
}

int list_cleanup(struct list *l) {
    if (l == NULL) {
        return 1;
    }

    node_t *tmp = l->head;

    while (tmp != NULL) {
        tmp = tmp->next;
        free(l->head);
        l->head = tmp;
    }

    free(l);
    return 0;
}

int list_node_present(const struct list *l, const struct node *n) {
    if (n == NULL || l == NULL) {
        return -1;
    }

    node_t *tmp = NULL;

    for (tmp = l->head; tmp != NULL; tmp = tmp->next) {
        if (tmp->next == n->next && tmp->previous == n->previous && tmp->value == n->value) {
            return 1;
        }
    }
    return 0;
}

int list_insert_after(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL) {
        return 1;
    }

    if (list_node_present(l, n) == 1 || list_node_present(l, m) == 0) {
        return 1;
    }

    node_t *tmp_next = m->next;

    if (l->tail == m) {
        m->next = n;
        n->previous = m;
        l->tail = n;
        return 0;
    }

    tmp_next->previous = n;
    m->next = n;

    n->previous = m;
    n->next = tmp_next;

    return 0;

}

int list_insert_before(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL) {
        return 1;
    }

    if (list_node_present(l, n) == 1 || list_node_present(l, m) == 0) {
        return 1;
    }

    node_t *tmp_prev = m->previous;

    if (l->head == m) {
        m->previous = n;
        n->next = m;
        l->head = n;
        return 0;
    }

    tmp_prev->next = n;
    m->previous = n;

    n->next = m;
    n->previous = tmp_prev;
    return 0;
}

size_t list_length(const struct list *l) {
    if (l == NULL) {
        return 0;
    }

    size_t size = 0;
    node_t *tmp = NULL;

    for (tmp = l->head; tmp != NULL; tmp = tmp->next) {
        size++;
    }
    return size;
}

struct node *list_get_ith(const struct list *l, size_t i) {
    if (l == NULL || i >= list_length(l)) {
        return NULL;
    }

    node_t *tmp = l->head;

    for (size_t j = 0; j < i; j++) {
        tmp = tmp->next;
    }
    return tmp;
}

struct list *list_cut_after(struct list *l, struct node *n) {
    if (l == NULL || n == NULL || list_node_present(l, n) == 0) {
        return NULL;
    }

    list_t *list_2 = malloc(sizeof(list_t));
    if (list_2 == NULL) {
        return NULL;
    }

    node_t *tmp_next = n->next;

    if (l->tail == n) {
        list_2->head = NULL;
        list_2->tail = NULL;
        return list_2;
    }

    n->next = NULL;
    tmp_next->previous = NULL;

    list_2->tail = l->tail;
    l->tail = n;

    list_2->head = tmp_next;

    return list_2;
}

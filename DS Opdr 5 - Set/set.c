/*
Naam: Boris
SSID: 15225054
Bsc Informatica

Dit programma implementeert de logica voor een set,
die gebruik maakt van functies gedeclareerd in tree.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "tree.h"

struct set {
    struct tree *tree_t;
};

struct set *set_init(int turbo) {
    struct set *set_t = (struct set *)malloc(sizeof(struct set));
    if (set_t == NULL) {
        return NULL;
    }

    set_t->tree_t = tree_init(turbo);
    if(set_t->tree_t == NULL) {
        free(set_t);
        return NULL;
    }

    return set_t;
}

int set_insert(struct set *s, int num) {
    if (s == NULL) {
        return -1;
    }

    if (tree_insert(s->tree_t, num) == 1) {
        return 1;
    }
    return 0;
}

int set_find(struct set *s, int num) {
    if (s == NULL) {
        return 0;
    }

    if (tree_find(s->tree_t, num) == 0) {
        return 0;
    }
    return 1;
}

int set_remove(struct set *s, int num) {
    if (s == NULL) {
        return 1;
    }

    if (tree_remove(s->tree_t, num) == 1) {
        return 1;
    }
    return 0;
}

void set_cleanup(struct set *s) {
    if (s == NULL && s->tree_t == NULL) {
        return;
    }
    tree_dot(s->tree_t, "tree.dot");
    tree_cleanup(s->tree_t);
    free(s);
}

void set_print(const struct set *s) {
    if (s == NULL && s->tree_t == NULL) {
        return;
    }

    tree_print(s->tree_t);
}

int set_verify(const struct set *s) {
    if (tree_check(s->tree_t)) {
        return 1;
    }
    return 0;
}

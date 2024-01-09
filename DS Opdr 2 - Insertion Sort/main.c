#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

/*
Name: Boris Vukajlovic
SSID: 15225054
This program implements an insertion sort algorithm, with a few variations.
*/

#define BUF_SIZE 1024

char buf[BUF_SIZE];

struct config {
    int descending_order;
    int combine;
    int remove_odd;
    int zip_alternating;
};

/* The function that sort a given linked list by using an insertion sort algortitm

Input:
struct list *list: the unsorted linked list

Side effect: sorts the unsorted linked list*/

void insertion_sort(struct list *list) {
    if (list == NULL || list_head(list) == NULL || list_next(list_head(list)) == NULL) {
        return;
    }

    struct node *current_node = list_next(list_head(list));

    while (current_node != NULL) {

        struct node *insertion_node = list_head(list);
        struct node *previous_node = NULL;

        while (list_node_get_value(insertion_node) <= list_node_get_value(current_node) && insertion_node != current_node) {
            previous_node = insertion_node;
            insertion_node = list_next(insertion_node);
        }

        if (insertion_node != current_node) {
            list_unlink_node(list, current_node);

            if (previous_node == NULL) {
                list_add_front(list, current_node);
            } else {
                list_insert_after(list, current_node, previous_node);
            }
        }
        current_node = list_next(current_node);
    }
}

/* The function that sort a given linked list by using an insertion sort algortitm in descending order

Input:
struct list *list: the unsorted linked list

Side effect: sorts the unsorted linked list */
void descending_order(struct list *list) {
    if (list == NULL || list_head(list) == NULL || list_next(list_head(list)) == NULL) {
        return;
    }

    struct node *current_node = list_next(list_head(list));

    while (current_node != NULL) {
        struct node *next_node = list_next(current_node);

        struct node *insertion_node = list_head(list);
        struct node *previous_node = NULL;

        while (insertion_node != current_node && list_node_get_value(insertion_node) > list_node_get_value(current_node)) {
            previous_node = insertion_node;
            insertion_node = list_next(insertion_node);
        }

        if (insertion_node != current_node) {
            list_unlink_node(list, current_node);

            if (previous_node == NULL) {
                list_add_front(list, current_node);
            } else {
                list_insert_after(list, current_node, previous_node);
            }
        }
        current_node = next_node;
    }
}

/* The function that only sorts the even values of a given linked list by using an insertion sort algortitm

Input:
struct list *list: the unsorted linked list

Side effect: sorts the even values of the unsorted linked list*/
void remove_odd(struct list *list) {
    if (list == NULL || list_head(list) == NULL || list_next(list_head(list)) == NULL) {
        return;
    }

    struct node *current_node = list_head(list);

    while (current_node != NULL) {

        struct node *next_node = list_next(current_node);

        if ((list_node_get_value(current_node) % 2) == 1) {
            list_unlink_node(list, current_node);
            list_free_node(current_node);
        }

        current_node = next_node;
    }
    descending_order(list);
}

/* The function that decides which sorting algorithm to use

Input:
struct config *cfg: the struct that represent the choice between the different solving algortihms
argc: the argument count
argv: the length of an argument

Output:
1 if a (valid) choice was made, 0 if not

Side effect: Updates the int value of one of the struct cfg members*/
int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "dcoz")) != -1) {
        switch (c) {
        case 'd':
            cfg->descending_order = 1;
            break;
        case 'c':
            cfg->combine = 1;
            break;
        case 'o':
            cfg->remove_odd = 1;
            break;
        case 'z':
            cfg->zip_alternating = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}

/*Function that prints a given linked list

Input:
struct list *list: the given linked list

Side effect:
prints the linked list in the stdout stream
*/
void print_list(struct list *list) {
    for (struct node* node = list_head(list); node != NULL; node = list_next(node)) {
        fprintf(stdout, "%d\n", list_node_get_value(node));
    }
}

int main(int argc, char *argv[]) {
    struct config cfg;

    if (parse_options(&cfg, argc, argv) != 0) {
        return 1;
    }

    struct list *list = list_init();
    if (list == NULL) {
        return 1;
        fprintf(stderr, "Error allocating memory");
    }

    char *nptr = NULL;
    char *endptr = NULL;

    while (fgets(buf, BUF_SIZE, stdin)) {
        nptr = buf;

            while (*nptr != '\0') {
                int value = (int)strtol(nptr, &endptr, 10);

                if (nptr != endptr) {
                    struct node *node = list_new_node(value);
                    if (node == NULL) {
                        return 1;
                        fprintf(stderr, "Error allocating memory");
                    }

                    if (list_add_back(list, node) == 1) {
                        return 1;
                    }
                } else {
                    break;
                }

                nptr = endptr;
            }
    }

    if (cfg.descending_order) {
        descending_order(list);

    } else if (cfg.remove_odd) {
        remove_odd(list);

    } else {
        insertion_sort(list);
    }

    print_list(list);

    if (list_cleanup(list) != 0) {
        return 1;
        fprintf(stderr, "Error freeing memory");
    }

    return 0;
}

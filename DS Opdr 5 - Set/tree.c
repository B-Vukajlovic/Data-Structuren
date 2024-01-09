/*
Naam: Boris
SSID: 15225054
Bsc Informatica

Dit programma implementeert de logica voor een standaard BST en
AVL tree aan de hand van de stand van de turbo parameter (1 voor
AVL, 0 voor BST), onder deze implementatie vallen functies zoals
de tree_insert, tree_delete, tree_print, tree_cleanup, tree_find
en balance_node functies waaronder hun helper functies.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

struct tree {
    struct node *root;
    int turbo;
};

struct node {
    int data;
    struct node *lhs;
    struct node *rhs;
    int height;
};

typedef struct node node;

static int global_node_counter = 0;

/*Function that allocates a node with given data on the heap
and returns a pointer to it.*/
static node *make_node(int data) {
    node *node_t = (node *)calloc(1, sizeof(node));
    if (node_t == NULL) {
        return NULL;
    }

    node_t->data = data;
    return node_t;
}

/*Helper function for the tree_dot function*/
static int print_tree_dot_r(node *root, FILE *dotf) {
    int left_id, right_id, my_id = global_node_counter++;

    if (root == NULL) {
        fprintf(dotf, "    %d [shape=point];\n", my_id);
        return my_id;
    }

    fprintf(dotf, "    %d [color=%s label=\"%d\"]\n", my_id, "black", root->data);

    left_id = print_tree_dot_r(root->lhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"l\"]\n", my_id, left_id);

    right_id = print_tree_dot_r(root->rhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"r\"]\n", my_id, right_id);

    return my_id;
}

void tree_dot(const struct tree *tree, const char *filename) {
    node *root = tree->root;
    global_node_counter = 0;
    FILE *dotf = fopen(filename, "w");
    if (!dotf) {
        printf("error opening file: %s\n", filename);
        exit(1);
    }
    fprintf(dotf, "digraph {\n");
    if (root) {
        print_tree_dot_r(root, dotf);
    }
    fprintf(dotf, "}\n");
    fclose(dotf);
}

/*Returns the height(depth) of a given node*/
int height(struct node *n) {
    if (n == NULL) {
        return 0;
    }

    return n->height;
}

/*Return the maximum of two given integers*/
int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

/*Calculates the balance(depth) of a node given its
children*/
int get_balance(node *n) {
    if (n == NULL)
        return 0;
    return height(n->lhs) - height(n->rhs);
}

/*Rotates a portion of a search tree to the right,
to keep AVL balance in the tree*/
node* right_rotate(node* n) {
    node* n_l = n->lhs;
    node* n_l_r = n_l->rhs;

    n_l->rhs = n;
    n->lhs = n_l_r;

    n->height = max(height(n->lhs), height(n->rhs)) + 1;
    n_l->height = max(height(n_l->lhs), height(n_l->rhs)) + 1;

    return n_l;
}

/*Rotates a portion of a search tree to the left,
to keep AVL balance in the tree*/
node *left_rotate(node *n) {
    node* n_r = n->rhs;
    node* n_r_l = n_r->lhs;

    n_r->lhs = n;
    n->rhs = n_r_l;

    n->height = max(height(n->lhs), height(n->rhs)) + 1;
    n_r->height = max(height(n_r->lhs), height(n_r->rhs)) + 1;

    return n_r;
}

int tree_check(const struct tree *tree) {
    return 0;
}

/*Balances the AVL tree*/
node* balance_node(node *root, int data) {
    int balance = get_balance(root);

    if (balance > 1 && data < root->lhs->data ) {
        return right_rotate(root);
    }

    if (balance < -1 && data > root->rhs->data) {
        return left_rotate(root);
    }

    if (balance > 1 && data > root->lhs->data) {
        root->lhs = left_rotate(root->lhs);
        return right_rotate(root);
    }

    if (balance < -1 && data < root->rhs->data) {
        root->rhs = right_rotate(root->rhs);
        return left_rotate(root);
    }

    return root;
}

struct tree *tree_init(int turbo) {
    struct tree *tree_t = (struct tree *)calloc(1, sizeof(struct tree));
    if (tree_t == NULL) {
        return NULL;
    }

    tree_t->turbo = turbo;
    return tree_t;
}

/*Helper function to insert a node into the tree recursively*/
node* insert_node(node *root, int data, int turbo) {
    if (root == NULL) {
        return make_node(data);
    }

    if (data < root->data) {
        root->lhs = insert_node(root->lhs, data, turbo);
    } else if (data > root->data) {
        root->rhs = insert_node(root->rhs, data, turbo);
    } else {
        return root;
    }

    root->height = 1 + max(height(root->lhs), height(root->rhs));

    if (turbo) {
        return balance_node(root, data);
    }

    return root;
}

int tree_insert(struct tree *tree, int data) {
    if (tree == NULL) {
        return -1;
    }

    if (tree_find(tree, data)) {
        return 1;
    }

    tree->root = insert_node(tree->root, data, tree->turbo);
    return 0;
}

int tree_find(struct tree *tree, int data) {
    node *curr = tree->root;

    while (curr != NULL) {
        if (curr->data == data) {
            return 1;
        }

        if (data > curr->data) {
            curr = curr->rhs;
        } else {
            curr = curr->lhs;
        }
    }

    return 0;
}

/* Helper Function to delete a given node from
the tree recursively*/
node* delete_node(node* root, int data, int turbo) {
    if (root == NULL) {
        return root;
    }

    if (data < root->data) {
        root->lhs = delete_node(root->lhs, data, turbo);
    }

    else if (data > root->data) {
        root->rhs = delete_node(root->rhs, data, turbo);
    }

    else {
        if ((root->lhs == NULL) || (root->rhs == NULL)) {
            node *temp = NULL;
            if (root->lhs != NULL) {
                temp = root->lhs;
            } else {
                temp = root->rhs;
            }

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }

            free(temp);
        } else {
            node* temp = root->rhs;
            while (temp->lhs != NULL ) {
                temp = temp->lhs;
            }
            root->data = temp->data;
            root->rhs = delete_node(root->rhs, temp->data, turbo);
        }
    }

    if (root == NULL) {
      return root;
    }

    root->height = 1 + max(height(root->lhs), height(root->rhs));

    if (turbo) {
        return balance_node(root, data);
    }

    return root;
}

int tree_remove(struct tree *tree, int data) {
    if (tree == NULL) {
        return 1;
    }

    if (tree_find(tree, data) == 0) {
        return 1;
    }

    tree->root = delete_node(tree->root, data, tree->turbo);
    return 0;
}

/*Prints a given node*/
void tree_node_print(struct node *node_t) {
    if (node_t != NULL) {
        tree_node_print(node_t->lhs);
        printf("%d\n", node_t->data);
        tree_node_print(node_t->rhs);
    }
}

void tree_print(const struct tree *tree) {
    if (tree != NULL && tree->root != NULL) {
        tree_node_print(tree->root);
    }
}

/*Cleans a given node*/
void tree_node_destroy(struct node *node_t) {
    if (node_t != NULL) {
        tree_node_destroy(node_t->lhs);
        tree_node_destroy(node_t->rhs);
        free(node_t);
    }
}

void tree_cleanup(struct tree *tree) {
    if (tree != NULL && tree->root != NULL) {
        tree_node_destroy(tree->root);
        tree->root = NULL;
    }
    free(tree);
}

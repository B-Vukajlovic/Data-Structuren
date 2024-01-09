/*
Naam: Boris
SSID: 15225054
Bsc Informatica

Dit programma implementeert functies die gegevns uit de terminal
lezen en deze gebruiken om een gemaakte set te beinvloeden.*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

#define BUF_SIZE 256
#define TURBO 1

void cleanup_and_fail(char *buf, struct set *s) {
    if (s) {
        set_cleanup(s);
    }
    free(buf);
    exit(EXIT_FAILURE);
}

int main(void) {
    char *buf = malloc(BUF_SIZE);
    if (!buf) {
        perror("Could not allocate input buffer");
        return EXIT_FAILURE;
    }
    struct set *s = set_init(TURBO);
    if (s == NULL) {
        fprintf(stderr, "Error intializing set");
    }

    while (fgets(buf, BUF_SIZE, stdin)) {
        char *endptr;
        char *command;
        char *num_str;
        int num;

        command = strtok(buf, " ");
        if (strchr("+-?", *command)) {
            num_str = strtok(NULL, "\n");
            if (!num_str) {
                printf("set operation '%c' requires integer operand\n", *command);
                cleanup_and_fail(buf, s);
            }
            num = (int) strtol(num_str, &endptr, 10);
            if (*endptr != '\0') {
                printf("error converting '%s' to an integer\n", num_str);
                cleanup_and_fail(buf, s);
            }
        }
        switch (*command) {
            case '+':
                set_insert(s, num);
                break;

            case '-':
                set_remove(s, num);
                break;

            case '?':
                if (set_find(s, num)) {
                    printf("found: %d\n", num);
                    break;
                }
                printf("not found: %d\n", num);
                break;

            case 'p':
                set_print(s);
                break;

            default:
                break;
        }
    }

    if (set_verify(s)) {
        fprintf(stderr, "Set implementation failed verification!\n");
    }
    free(buf);
    set_cleanup(s);
    return EXIT_SUCCESS;
}

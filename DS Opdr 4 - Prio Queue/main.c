/*
Name: Boris Vukajlovic
SSID: 15225054

This program implements a digital waiting room which adheres to certain
rulesets and makes use of a priority queue, which is based on the heap
datastructure.*/

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "prioq.h"

#define BUF_SIZE 1024

static char buf[BUF_SIZE];

struct config {
    int year;
};

static int parse_options(struct config *cfg, int argc, char *argv[]);

typedef struct {
    char *name;
    int age;
} patient_t;

static int compare_patient_name(const void *a, const void *b) {
    return strcmp(((const patient_t *) a)->name, ((const patient_t *) b)->name);
}

/*Compares two patient age values and finds the difference, which
is used in the heap datastructure to decide where it eneds to be positioned*/
static int compare_patient_age(const void *a, const void *b) {
    const patient_t *pa = (const patient_t *)a;
    const patient_t *pb = (const patient_t *)b;

    if (pa->age - pb->age == 0) {
        return compare_patient_name(a,b);
    }

    return (pa->age - pb->age);
}

/*Free's the allocated patient struct on the heap*/
void free_patient(void *p) {
    patient_t *patient = (patient_t *)p;
    free(patient->name);
    free(patient);
}

int main(int argc, char *argv[]) {
    char *token, *name_cpy;
    prioq *queue;
    struct config cfg;

    if (parse_options(&cfg, argc, argv) != 0) {
        return EXIT_FAILURE;
    }

    if (cfg.year) {
        queue = prioq_init(&compare_patient_age);
    } else {
        queue = prioq_init(&compare_patient_name);
    }

    if (queue == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (int iterations = 0;;) {
        while (1) {
            char *s = fgets(buf, BUF_SIZE, stdin);
            if (s == NULL) {
                fprintf(stderr, "Unexpected end of file. exiting\n");
                prioq_cleanup(queue, NULL);
                return EXIT_FAILURE;
            }

            if (strcmp(buf, ".\n") == 0) {
                patient_t *popped_patient = (patient_t *)prioq_pop(queue);
                if (popped_patient == NULL) {
                    break;
                }

                printf("%s\n", popped_patient->name);
                free(popped_patient->name);
                free(popped_patient);
                break;
            }

            patient_t *patient = malloc(sizeof(patient_t));
            if (patient == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                prioq_cleanup(queue, NULL);
                return EXIT_FAILURE;
            }

            name_cpy = strtok(buf, " ");

            patient->name = malloc(strlen(name_cpy) + 1);
            if (patient->name == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                prioq_cleanup(queue, free_patient);
                return EXIT_FAILURE;
            }
            strcpy(patient->name, name_cpy);

            token = strtok(NULL, " \n");

            if (token != NULL) {
                patient->age = atoi(token);
            }

            if (prioq_insert(queue, patient) != 0) {
                fprintf(stderr, "Error inserting data\n");
                prioq_cleanup(queue, free_patient);
                return EXIT_FAILURE;
            }
        }

        printf(".\n");

        if (++iterations == 10) {
            while (prioq_size(queue) > 0) {
                patient_t *popped_patient = (patient_t *)prioq_pop(queue);
                printf("%s\n", popped_patient->name);
                free(popped_patient->name);
                free(popped_patient);
            }
            prioq_cleanup(queue, free);
            break;
        }
    }
    return EXIT_SUCCESS;
}

int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "y")) != -1) {
        switch (c) {
        case 'y':
            cfg->year = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}

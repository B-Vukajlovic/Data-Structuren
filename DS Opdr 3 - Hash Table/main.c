/*
Name: Boris Vukajlovic
Ssid:15225054

This program fills an hash table with data, which has been read
from a file stream. It also includes numerous functions which
manipulate and test this hash table, which include cleanup input
string, calc_delim, creating a table from a file, using stdin
to look up certain keys and their corresponding values and a
performance benchmark.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "array.h"
#include "hash_func.h"
#include "hash_table.h"

#define LINE_LENGTH 256

#define TABLE_START_SIZE 65536
#define MAX_LOAD_FACTOR 1.0
#define HASH_FUNCTION hash_djb2

#define START_TESTS 2
#define MAX_TESTS 2
#define HASH_TESTS 3

/* Replace every non-ascii char with a space and lowercase every char. */
static void cleanup_string(char *line) {
    for (char *c = line; *c != '\0'; c++) {
        *c = (char) tolower(*c);
        if (!isalpha(*c)) {
            *c = ' ';
        }
    }
}

/* Return a pointer to a heap allocated string with all the characters we
 * use as word delimiters. Return NULL on failure. */
static char *calc_delim(void) {
    const size_t ascii_table_size = 128;
    char *res = malloc(sizeof(char) * ascii_table_size);
    if (!res) {
        return NULL;
    }

    int res_index = 0;
    for (unsigned char c = 1; c < ascii_table_size; c++) {
        if (!isalpha(c)) {   /* if it's not an alpha it's a delimiter char. */
            res[res_index++] = (char) c;
        }
    }
    res[res_index] = '\0';
    return res;
}

/* Creates a hash table with a word index for the specified file and
 * parameters. Return a pointer to hash table or NULL if an error occured.
 */
static struct table *create_from_file(char *filename,
                               unsigned long start_size,
                               double max_load,
                               unsigned long (*hash_func)(const unsigned char *)) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return NULL;
    }

    char *line = malloc(LINE_LENGTH * sizeof(char));
    if (!line) {
        fclose(fp);
        return NULL;
    }

    struct table *hash_table = table_init(start_size, max_load, hash_func);

    int line_number = 0;
    char *delim = (char *)calc_delim();
    while (fgets(line, LINE_LENGTH, fp)) {

        cleanup_string(line);
        char *word = strtok(line, delim);
        line_number++;

        while (word != NULL) {
            table_insert(hash_table, word, line_number);
            word = strtok(NULL, delim);
        }
    }

    free(delim);
    fclose(fp);
    free(line);

    return hash_table;
}

/* Reads words from stdin and prints line lookup results per word.
 * Return 0 if succesful and 1 on failure. */
static int stdin_lookup(struct table *hash_table) {
    char *line = malloc(LINE_LENGTH * sizeof(char));
    if (!line) {
        return 1;
    }

    char *delim = (char *)calc_delim();
    while (fgets(line, LINE_LENGTH, stdin)) {

        cleanup_string(line);
        char *word = strtok(line, delim);

            struct array *word_array = table_lookup(hash_table, word);
            printf("%s\n", word);
            for (unsigned long int i = 0; i < array_size(word_array); i++) {
                printf("* %d\n", array_get(word_array, i));
            }
            printf("\n");
    }

    free(delim);
    free(line);
    return 0;
}

/* Tests hash fucntions against eachother and provides information about the
fastest performing times, like the max load and the starting size.

Input: char *filename, the filename that will be read.

Side effect: prints the benchmark on the stdout stream.*/
static void timed_construction(char *filename) {

    unsigned long start_sizes[START_TESTS] = { 2, 65536 };
    double max_loads[MAX_TESTS] = { 0.2, 1.0 };
    unsigned long (*hash_funcs[HASH_TESTS])(const unsigned char *) = { hash_too_simple, hash_djb2, hash_k_and_r_v2 };

    for (int i = 0; i < START_TESTS; i++) {
        for (int j = 0; j < MAX_TESTS; j++) {
            for (int k = 0; k < HASH_TESTS; k++) {
                clock_t start = clock();
                struct table *hash_table =
                create_from_file(filename, start_sizes[i], max_loads[j], hash_funcs[k]);
                clock_t end = clock();

                printf("Start: %ld\tMax: %.1f\tHash: %d\t -> Time: %ld "
                       "microsecs\n",
                       start_sizes[i], max_loads[j], k, end - start);
                table_cleanup(hash_table);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s text_file [-t]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 3 && !strcmp(argv[2], "-t")) {
        timed_construction(argv[1]);
    } else {
        struct table *hash_table =
        create_from_file(argv[1], TABLE_START_SIZE, MAX_LOAD_FACTOR, HASH_FUNCTION);
        if (hash_table == NULL) {
            printf("An error occured creating the hash table, exiting..\n");
            return EXIT_FAILURE;
        }
        if (stdin_lookup(hash_table) != 0) {
            table_cleanup(hash_table);
            return EXIT_FAILURE;
        }
        table_cleanup(hash_table);
    }

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "queue.h"

#define NOT_FOUND -1
#define ERROR -2
#define STACK_SIZE 4000

/*Returned de lengte van het pad door het doolhof

Struct maze *m = de struct maze pointer m
int **visitarray = de 2d array die de indexen van de voorgaande
stappen in het doolhof bevat
int dest_r = de rij van het doelwit
int dest_c = de kolom van het doelwit

Side effect: De visist array wordt gevuld met indexen van punten 
in het doolhof*/
int backtrack(struct maze *m, int **visitarray, int dest_r, int dest_c) {
    int lengte = 0;
    int array_index = visitarray[dest_r][dest_c];

    maze_set(m, dest_r, dest_c, 'x');
    lengte++;

    while (!maze_at_start(m, maze_row(m, array_index), maze_col(m, array_index))) {
        int r = maze_row(m, array_index);
        int c = maze_col(m, array_index);

        maze_set(m, r, c, 'x');

        array_index = visitarray[r][c];
        lengte++;
    }
    return lengte;
}

/* int **visitarray = de 2d array die de indexen van de voorgaande stappen in 
het doolhof bevat
int total_size = de lengte of breedte van het doolhof

Side effect: Freed het doolhof
*/
void free_visitarray(int **visitarray, int total_size) {
    for (int i = 0; i < total_size; i++) {
        free(visitarray[i]);
    }
    free(visitarray);
}

/* Returned de gealloceerde en geinitialliseerde int** naar de 
visitarray op de heap of NULL als dat niet is gelukt.

int r: de lengte van het doolhof
int c: de breedte van het doolhof*/
int **create_visitarray(int r, int c) {
    size_t rows = (size_t)r;
    size_t cols = (size_t)c;

    int **visitarray = (int**)malloc(rows * (sizeof(int*)));
    if (visitarray == NULL) {
        return NULL;
    }

    for (int i = 0; i < r; i++) {
    visitarray[i] = (int*)malloc(cols * sizeof(int));
        if (visitarray[i] == NULL) {
            free(visitarray);
            return NULL;
        }
    }
    return visitarray;
}

/*Implementeert het bfs_solve algoritme, deze returned hierbij
de lengte van kortste pad.

struct maze *m = een struct maze pointer naar m

side effects: de queue wordt ge-cleanupd
*/
int bfs_solve(struct maze *m) {
    int total_size = 0;
    int dest_r, dest_c;
    maze_destination(m, &dest_r, &dest_c);
    int dest_index = maze_index(m, dest_r, dest_c);

    struct queue *queue = queue_init(STACK_SIZE);
        if (queue == NULL) {
            return ERROR;
        }

    total_size = maze_size(m);
    int **visitarray = create_visitarray(total_size, total_size);
        if (visitarray == NULL) {
            return ERROR; 
        }

    int r_start, c_start = 0;
    maze_start(m, &r_start, &c_start);
    queue_push(queue, maze_index(m, r_start, c_start));

    while (queue_empty(queue) == 0) {
        int current_index = queue_pop(queue);
        int current_r = maze_row(m, current_index);
        int current_c = maze_col(m, current_index);

        if (current_index == dest_index) {
            int lengte = backtrack(m, visitarray, dest_r, dest_c);
            free_visitarray(visitarray, total_size);
            queue_cleanup(queue);
            return lengte;
        }

        for(int i = 0; i < 4; i ++) {
            int r_new = current_r + m_offsets[i][0];
            int c_new = current_c + m_offsets[i][1];

            maze_set(m, current_r, current_c, VISITED);

            if (maze_get(m, r_new, c_new) != WALL && maze_get(m, r_new, c_new) != VISITED && maze_valid_move(m, r_new, c_new)) {
                queue_push(queue, maze_index(m, r_new, c_new));
                visitarray[r_new][c_new] = current_index;
                maze_set(m, r_new, c_new, TO_VISIT);
            }
        }
    }
    free_visitarray(visitarray, total_size);
    queue_cleanup(queue);
    return NOT_FOUND; 
}

int main(void) {
    /* read maze */
    struct maze *m = maze_read();
    if (!m) {
        printf("Error reading maze\n");
        return 1;
    }

    /* solve maze */
    int path_length = bfs_solve(m);
    if (path_length == ERROR) {
        printf("bfs failed\n");
        maze_cleanup(m);
        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("bfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");
    maze_cleanup(m);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "draw.h"
#define ERROR -1

int draw(char **array, int size) {  /* Function that depicts crossword's solution. */
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {    /* Print array's data. */
            if (array[i][j] == '#') {   /* Print black crossword's cell. */
                printf("###");
            }
            else {  /* Print characters. */
                printf(" %c ", array[i][j]);
            }
        }
        printf("\n");
    }
    return 0;
}
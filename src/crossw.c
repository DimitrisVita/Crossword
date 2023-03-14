#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helping.h"
#include "draw.h"
#include "dict.h"
#include "quicksort.h"
#include "solve.h"
#include "check.h"

#define ERROR -1

int main(int argc, char *argv[]) {
    int cnt, dimensions, row, column, en_check, en_draw, *length, i, j, count_words=0, diflen;
    char **crossword, *lexicon = "Words.txt";
    FILE *blank_crossw;
    en_check = 0;   /* Switch for check mode. */
    en_draw = 0;    /* Switch for draw mode. */
    cnt = 2;
    if (argc <= 1) {    /* Too few argument given. */
        perror("Wrong input. Please try again.");
        return ERROR;
    }
    else {
        if ((blank_crossw = fopen(argv[1], "r")) == NULL) { /* Check if blank crossword file can be opened. */
            perror("Can not open blank crossword file. Please try again.");
            return ERROR;
        }
        while (cnt < argc) {
            if (strcmp(argv[cnt], "-dict") == 0) {      /* Enable dict mode. */
                if ((cnt+1) < argc) {   /* Avoid segmentation fault. */
                    lexicon = argv[++cnt];
                }
                else {  /* No custom dictionary file. */
                    perror("Wrong input. Please try again.");
                    return ERROR;
                }
            }
            else if (strcmp(argv[cnt], "-check") == 0) {    /* Enable check mode. */
                en_check = 1;
            }
            else if (strcmp(argv[cnt], "-draw") == 0) { /* Enable draw mode. */
                en_draw = 1;
            }
            else {  /* Wrong input given. E.g. ./crossw junk -dict Morewords.txt */
                perror("Wrong input. Please try again.");
                return ERROR;
            }
            cnt++;
        }
        if(!fscanf(blank_crossw, " %d", &dimensions)) { /* Read the dimensions of the square crossword. */
            perror("Wrong input. Please try again.");
            return ERROR;
        }
        if ((crossword = malloc(dimensions * sizeof(char*))) == NULL) { /* Allocate memory for 2-dimensional crossword's array. */
            perror("Error while allocating memory.\n");
            return ERROR;
        }
        for (i=0; i<dimensions; i++) {
            if ((crossword[i] = malloc(dimensions * sizeof(char))) == NULL) {
                perror("Error while allocating memory.\n");
                return ERROR;
            }
        }
        for (i=0; i<dimensions; i++) {   /* Fill crossword's cells with '-'. */
            for (j=0; j<dimensions; j++) {
                crossword[i][j] = '-';
            }
        }
        while ((fscanf(blank_crossw, " %d %d", &row, &column)) != EOF) { /* Fill crossword's black cells. */
            crossword[row-1][column-1] = '#';
        }
        length = calloc(dimensions-1, sizeof(int)); /* Array which stores all the different crossword's words lengths. */

        diflen = find_length(length,dimensions,crossword,&count_words); /* How many the different legths of crossword's words are? */

        /* Array that stores the population of each word length. */
        int *counts = calloc(diflen,sizeof(int));
        if(counts == NULL){
            fprintf(stderr,"Error while allocating memory.\n");
            return 0;
        }
        quicksort_body(length, 0, diflen-1);    /* Sort length array. */

        /* Find blank crossword's words. */
        struct word *cr_words;
        if ((cr_words = find_words(dimensions, crossword, count_words)) == NULL) {  /* Array with each crossword's word information. */
            perror("Error while allocating memory.\n");
            return ERROR;
        }

        /* Array that stores lecicon's words. */
        char ***words = dict(length, lexicon, diflen, count_words, cr_words, dimensions, counts);

        /* Array that stores how many different words pass through each crossword's cell. */
        int **crash_m = crash(dimensions, count_words, cr_words);

        /* Array that stores how many inserted words pass through each crossword's cell. */
        int **crash_l;
        crash_l = malloc(dimensions * sizeof(int *));
        for(i = 0 ; i < dimensions ; i++){
            crash_l[i] = calloc(dimensions, sizeof(int));
        }

        /* Sort crossword words by their priority. */
        quicksort_by_priority(cr_words, 0, count_words - 1);

        /* Update neighbours of each word. */
        for (i=0; i<count_words; i++){
            cr_words[i].nei = neighbours_(cr_words[i], crash_m, cr_words, count_words);
        }
        if (en_check) { /* If check mode is enabled. */
            if (check(length, diflen, counts, words, cr_words, count_words, crash_l, crossword, dimensions) && en_draw) {
                /* Print solved crossword. */
                draw(crossword, dimensions);
            }
        }
        else if (en_draw) { /* If draw mode is enabled. */
            /* Attempt to solve the crossword. */
            solve(cr_words, count_words, crossword, crash_l, dimensions);
            /* Print solved crossword. */
            draw(crossword, dimensions);
        }
        else {
            /* Attempt to solve the crossword. */
            solve(cr_words, count_words, crossword, crash_l, dimensions);
            /* Sort words by their row/column and print them. */
            sort_words(cr_words, count_words);
        }
        

        /* Close opened files. */
        fclose(blank_crossw);

        /* Free allocated memory. */
        for (int i=0; i<count_words; i++) {
            free(cr_words[i].basic_constr);
            free(cr_words[i].flags_D);
            free(cr_words[i].nei);
            if (cr_words[i].chars != NULL)
                free(cr_words[i].chars);
        }
        free(cr_words);
        free(counts);
        free(length);
        for (int i=0; i<diflen; i++) {
            for (int j=0; j<counts[j]; j++) {
                free(words[i][j]);
            }
            free(words[i]);
        }
        free(words);
        for (int i=0; i<dimensions; i++) {
            free (crossword[i]);
            free (crash_l[i]);
            free (crash_m[i]);
        }
    }
    return 0;
}
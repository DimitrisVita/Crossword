#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helping.h"
#include "check.h"
#define ERROR -1


int check(int *length, int diflen, int *counts, char ***words, struct word *cr_words, int count_words, int **crash_l, char **crossword, int dimensions){
    int i, j, index, counthor = 0, countvert = 0, indexhor = 0, indexvert = 0, len;
    struct word *hor, *vert; 
    for(i = 0 ; i < count_words ; i++)
        if(cr_words[i].ishor)
            counthor++; /*Counting horizontal variables*/
        else
            countvert++; /*Counting vertical variables*/
    if((hor = malloc(counthor * sizeof(struct word))) == NULL || (vert = malloc(countvert * sizeof(struct word))) == NULL){
        fprintf(stderr, "Error while allocating memory\n");
        exit(ERROR);
    }
    for(i = 0 ; i < count_words ; i++) /*Seperating horizontal from vertical*/
        if(cr_words[i].ishor)
            hor[indexhor++] = cr_words[i];
        else
            vert[indexvert++] = cr_words[i];
    bubblesort_for_horizontal(hor, 1, counthor); /*Sorting horizontal by their row*/
    bubblesort_for_vertical(vert, 1, countvert); /*Sorting vertical by their column*/
    indexhor = 0;
    indexvert = 0;
    int ch;
    char *tempword = malloc(101 *sizeof(char)); /*Thera does not exists a word  with more than 100 characters!*/
    if(tempword == NULL){
        free(hor);
        free(vert);
        fprintf(stderr, "Error while allocating memory\n");
        exit(ERROR);
    }
    while(!feof(stdin)){
        tempword = fgets(tempword, 101, stdin); /*fgets reads (100 + 1) - 1 = 100 max characters*/
        len = strlen(tempword); 
        if(tempword[len - 1] == '\n'){ /*fgets reads also '\n' characters*/
            len--;
            tempword[len] = '\0'; /*we do not need \n from the tempword which has been read from fgets function*/
        }
        index = len_index(length, diflen, len); /*Finding corresponding index in the words array*/
        if(index == -1){ /*If index == -1 wrong length*/
            for(i = 0 ; i < dimensions ; i++)
                for(j = 0 ; j < dimensions ; j++)
                    if(crossword[i][j] != '#')
                        crossword[i][j] = '-';
            printf("Length of the word \"%s\" does not match, thus it cannot be placed.\n", tempword);
            free(tempword);
            free(hor);
            free(vert);
            return 0;
        }
        int exists = 0; /*Does the word exists in the dictionary?*/
        for(j = 0 ; j < counts[index] ; j++){ /*Searching the words of the dictionary with the corresponding length...*/
            if(!strcmp(words[index][j], tempword)){
                exists = 1; /*Found the word!*/
                break;
            }
        }
        if(!exists){ /*If the word does not exist there is no solution*/
            for(i = 0 ; i < dimensions ; i++)
                for(j = 0 ; j < dimensions ; j++)
                    if(crossword[i][j] != '#')
                        crossword[i][j] = '-';
            printf("Word \"%s\" not in dictionary.\n", tempword);
            free(tempword);
            free(hor);
            free(vert);
            return 0;
        }
        if(indexhor < counthor){ /*First we place horizontal words*/
            if(hor[indexhor].lengthw != len){ /*Checking the corresponding length that the word needs to be*/
                for(i = 0 ; i < dimensions ; i++)
                    for(j = 0 ; j < dimensions ; j++)
                        if(crossword[i][j] != '#')
                            crossword[i][j] = '-';
                printf("Word \"%s\" cannot be placed because it has wrong length.\n", tempword);
                free(tempword);
                free(hor);
                free(vert);
                return 0;
            }
            if((hor[indexhor].chars = malloc((hor[indexhor].lengthw + 1)* sizeof(char))) == NULL){
                free(tempword);
                free(hor);
                free(vert);
                fprintf(stderr, "Error while allocating memory\n");
                exit(ERROR);
            }
            strcpy(hor[indexhor].chars, tempword);
            insert_word(crossword, hor[indexhor], crash_l);
            indexhor++;
        }
        else if(indexvert < countvert){ /*After we place all the horizontal words we go for the words that need to be placed vertical*/
            if(vert[indexvert].lengthw != len){ /*Checking the corresponding length that the word needs to be*/
                for(i = 0 ; i < dimensions ; i++)
                    for(j = 0 ; j < dimensions ; j++)
                        if(crossword[i][j] != '#')
                            crossword[i][j] = '-';
                printf("Word \"%s\" cannot be placed because it has wrong length.\n", tempword);
                free(tempword);
                free(hor);
                free(vert);
                return 0;
            }
            if(basic_restrictions_for_vertical_variables(&vert[indexvert], crossword, tempword)){ /*Vertical words have further restrictions because we have already placed the horizontal words*/
                if((vert[indexvert].chars = malloc((vert[indexvert].lengthw + 1)* sizeof(char))) == NULL){
                    free(tempword);
                    free(hor);
                    free(vert);
                    fprintf(stderr, "Error while allocating memory\n");
                    exit(ERROR);
                }
                strcpy(vert[indexvert].chars, tempword);
                insert_word(crossword, vert[indexvert], crash_l);
            }
            indexvert++;
        }
        else{ /*If we have exceeded the number of variables that we can get, then the input does not lead to a solution*/
            free(tempword);
            free(hor);
            free(vert);
            for(i = 0 ; i < dimensions ; i++)
                for(j = 0 ; j < dimensions ; j++)
                    if(crossword[i][j] != '#')
                        crossword[i][j] = '-';
            printf("More words given!\n");
            return 0;
        }
        if((ch = getc(stdin)) == EOF){ /*If next character is EOF then we have examined all the words*/
            break;
        }
        else{
            ungetc(ch, stdin);
        }
    }
    if(indexvert < countvert){ /*If there are either vertical or horizontal variables that need to be field we cannot solve the crossword with the given words*/
        free(tempword);
        free(hor);
        free(vert);
        for(i = 0 ; i < dimensions ; i++)
            for(j = 0 ; j < dimensions ; j++)
                if(crossword[i][j] != '#')
                    crossword[i][j] = '-';
        printf("Not enough words to fill the crossword.\n");
        return 0;
    }
    free(tempword);
    free(hor);
    free(vert);
    printf("Solution found!\n");
    return 1; /*Solution found!*/
}
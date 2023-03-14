#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helping.h"
#include "dict.h"
#define ERROR -1

char ***dict(int *length,char *lexicon, int diflen, int count_words, struct word *cr_words, int dimensions, int *counts){
    FILE *fp;
    if((fp = fopen(lexicon, "r")) == NULL){
        fprintf(stderr,"Error while opening file %s\n", lexicon);
        return NULL;
    }
    char *tempword = malloc(100 * sizeof(char)); //tempword is the current word I am getting from the dictionary... of course, there cannot be a word of more t
    if(tempword == NULL){
        fprintf(stderr,"Error while allocating memory.\n");
        return NULL;
    }
    int i,len;
    while(!feof(fp)){ //reading all the words to calculate the counts we want...
        tempword = fgets(tempword, 100, fp);
        len = strlen(tempword) - 1; //fgets reads also '\n' characters
        for(i = 0 ; i < diflen ; i++){ //Do we need this word to fill the crossword?
            if(len == length[i]){ //Is the length of the word appropriate for our crossword?
                counts[i]++; //if it is count it!
                break;
            }
        }
    }
    char ***words;  /* words[word's length][index to specific word][letters of the word] */
    if((words = malloc(diflen * sizeof(char **))) == NULL){ //word is an array in which we store the words depending of their length
        fprintf(stderr,"Error while allocating memory.\n");
        return NULL;
    }
    for(i = 0 ; i < diflen ; i++){
        if((words[i] = malloc(counts[i] * sizeof(char *))) == NULL){
            fprintf(stderr,"Error while allocating memory.\n");
            return NULL;
        }
    }
    rewind(fp);
    int *counter = calloc(diflen,sizeof(int)); //counter array stores the indexes of the words of the array "words" for each length of word that we care about
    while(!feof(fp)){
        tempword = fgets(tempword, 100, fp);
        len = strlen(tempword) - 1; //fgets reads also '\n' characters
        tempword[len] = '\0'; //we do not need the '\n'
        int index = len_index(length, diflen, len); //index stores the position in the array words, in which we store words with length "len"
        if(index != -1){
            words[index][counter[index]] = malloc((len+ 1) * sizeof(char)); //allocating len + 1 cells for each word (the length of the word plus the character
            strcpy(words[index][counter[index]++], tempword); // After copying the word to the position of the array "words",  we increase the counter/index of the corresponding length by one.
        }
    }

    initialize_domains(cr_words, count_words, length, words, counts, diflen);   /* Initiallizing the domains of the variables in the array "cr_words" with all the words that have the same length with them. */
    
    free(tempword);
    free(counter);
    fclose(fp);
    return words;
}
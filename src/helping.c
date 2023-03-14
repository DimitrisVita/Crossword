#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helping.h"
#include "quicksort.h"
#define ERROR -1

int len_index(const int *length, const int diflen, const int len){ //Has the word an appropriate length so as to take it into consideration? If not, return -1.
    int i;
    for(i = 0 ; i < diflen ; i++)
        if(len == length[i])
            return i;
    return -1;
}

int find_length(int *length,int dimensions,char **crossword,int *count_words){ //function which stores in the array length all the lengths of the words I need to fill
    int k = 0,i,j; //k is the index of length array.
    int currentrowcount = 0,currentcolcount = 0;
    for(i = 0 ; i < dimensions ; i++){
        for(j = 0 ; j < dimensions ; j++){
            if(crossword[i][j] != '#'){ //Not '#' in the row i? I need to count this cell!
                currentrowcount++;
            }
            else{  //Found '#'?
                if(currentrowcount > 1){ //Does it make sense to have a word in this area of the row i?
                    (*count_words)++;
                    int p;
                    int exists = 0;
                    for(p = 0 ; length[p] != 0 ; p++){ //searching in the length array to examine if the current length exists
                        if(currentrowcount == length[p]){
                            exists = 1;
                            break;
                        }
                    }
                    if(!exists){
                        length[k++] = currentrowcount;
                    }
                }
                currentrowcount = 0;
            }
            if(crossword[j][i] != '#'){ //Not '#' in the column i? I need to count this cell!
                currentcolcount++;
            }
            else{  //Found '#'?
                if(currentcolcount > 1){ //Does it make sense to have a word in this area of the column i?
                    (*count_words)++;
                    int p;
                    int exists = 0;
                    for(p = 0 ; length[p] != 0 ; p++){ //searching in the length array to examine if the current length exists
                        if(currentcolcount == length[p]){
                            exists = 1;
                            break;
                        }
                    }
                    if(!exists){
                        length[k++] = currentcolcount;
                    }
                }
                currentcolcount = 0;
            }
        }
        if(currentrowcount > 1){ //the loop finishes! Have we got some characters that we need to fill the row i with?
            (*count_words)++;
            int exists = 0;
            int p;
            for(p = 0 ; length[p] != 0 ; p++){
                if(currentrowcount == length[p]){
                    exists = 1;
                    break;
                }
            }
            if(!exists)
                length[k++] = currentrowcount;
        }
        currentrowcount = 0; //preparing currentrowcount for next loop
        if(currentcolcount > 1){ //Have we got some characters that we need to fill the column with?
            (*count_words)++;
            int exists = 0;
            int p;
            for(p = 0 ; length[p] != 0 ; p++){
                if(currentcolcount == length[p]){
                    exists = 1;
                    break;
                }
            }
            if(!exists)
                length[k++] = currentcolcount;
        }
        currentcolcount = 0; //preparing currentcolcount for next loop
    }
    return k; //return the size of length array (without considering the positions which store 0)
}

/* Function that counts how many different words pass through each crossword's cell. */
int **crash(int dimensions, int cnt, struct word *cr_words) {
    int **crash_m;  /* Two-dimensional array parallel to crossword's array. */
    if ((crash_m = malloc(dimensions * sizeof(int *))) == NULL) {
        perror("Error while allocating memory.\n");
        return NULL;
    }
    for (int i = 0; i < dimensions; i++)
        if ((crash_m[i] = calloc(dimensions, sizeof(int))) == NULL) {
            perror("Error while allocating memory.\n");
            return NULL;
        }
    for (int i=0; i<cnt; i++)
        if (cr_words[i].ishor)  /* Add horizontal words. */
            for (int j=0; j<cr_words[i].lengthw; j++)
                crash_m[cr_words[i].row - 1][cr_words[i].col - 1 + j]++;
        else    /* Add vertical words. */
            for (int j=0; j<cr_words[i].lengthw; j++)
                crash_m[cr_words[i].row + j - 1][cr_words[i].col - 1]++;
    /* Calculate each word's priority and neighbours count. */
    for (int i=0; i<cnt; i++) {
        cr_words[i].pri = 0;
        cr_words[i].nei_cnt = 0;
        if (cr_words[i].ishor)  /* Process for horizontal words. */
            for (int j=0; j<cr_words[i].lengthw; j++) {
                cr_words[i].pri += crash_m[cr_words[i].row-1][cr_words[i].col-1 + j];
                if (crash_m[cr_words[i].row-1][cr_words[i].col-1 + j] == 2) {   /* If crash_m[x][y] is 2, two words pass from this specific crossword's cell. */
                    cr_words[i].nei_cnt++;
                }
            }
        else    /* Process for vertical words. */
            for (int j=0; j<cr_words[i].lengthw; j++) {
                cr_words[i].pri += crash_m[cr_words[i].row-1 + j][cr_words[i].col-1];
                if (crash_m[cr_words[i].row-1 + j][cr_words[i].col-1] == 2) {   /* If crash_m[x][y] is 2, two words pass from this specific crossword's cell. */
                    cr_words[i].nei_cnt++;
                }
            }
    }
    return crash_m;
}

/* Function that finds blank crossword's words. */ 
struct word *find_words(int dimensions, char **crossword,int count) {
    struct word *cr_words; /* Type word struct array with every crossword's word information. */
    if ((cr_words = malloc(count * sizeof(struct word))) == NULL) {
        perror("Wrong input. Please try again.");
        return NULL;
    }
    int cnt=0, len;
    for (int i=0; i<dimensions; i++) {
        for (int j=0; j<dimensions; j++) {  /* Find horizontal words. */
            len=0; /* Length of current word. */
            while ((j+len != dimensions) && (crossword[i][j+len] != '#'))   /* Increment word's length by 1, until we find '#' or crossword's border. */
                len++;
            if (len > 1) {
                cr_words[cnt].row = i+1;
                cr_words[cnt].col = j+1;
                cr_words[cnt].lengthw = len;
                cr_words[cnt].ishor = 1;
                cr_words[cnt].chars = NULL;
                cr_words[cnt].basic_constr = malloc(len * sizeof(int)); /* Array that stores word's letter restrictions positions. */
                if((cr_words[cnt].basic_constr) == NULL){
                    perror("Error while allocating memory.\n");
                    return NULL;
                }
                cnt++;
                j += len; /* Ignore crossword's cells we have already checked. */
            }
        }
        for (int k=0; k<dimensions; k++) { /* Repeating the same process for vertical words. */
            len=0;
            while ((k+len != dimensions) && (crossword[k+len][i] != '#'))
                len++;
            if (len > 1) {
                cr_words[cnt].row = k+1;
                cr_words[cnt].col = i+1;
                cr_words[cnt].lengthw = len;
                cr_words[cnt].ishor = 0;
                cr_words[cnt].chars = NULL;
                cr_words[cnt].basic_constr = malloc(len * sizeof(int));
                if((cr_words[cnt].basic_constr) == NULL){
                    perror("Error while allocating memory.\n");
                    return NULL;
                }
                cnt++;
                k += len;
            }
        }
    }
    return cr_words;
}




int initialize_domains(struct word *cr_words, int count_words, int *length, char ***words, int *counts, int diflen){
    int i, j, index;
    for(i = 0 ; i < count_words ; i++){
        index = len_index(length, diflen, cr_words[i].lengthw); /*First we find the corresponding index to the array words*/
        cr_words[i].domain_count = counts[index]; /*Initially the domain of the current varable consists of all the words which have the same length with it*/
        cr_words[i].domain = words[index];  /* The domain of the variable equals to the pointer to the words with the corresponding length. */
        if((cr_words[i].flags_D = malloc(cr_words[i].domain_count * sizeof(int))) == NULL){ /*Allocating memory for the flags (wether a word is appropriate for the variable or not)*/
            fprintf(stderr,"Error while allocating memory\n");
            return ERROR;
        }
        for(j = 0 ; j < cr_words[i].domain_count ; j++){    /* In the beginning, all words are available. */
            cr_words[i].flags_D[j] = 1;
        }
    }
    return 0;
}

/* Function that returns a pointer to a word with specific information. */
struct word *search_word(int x, int y, int cnt, struct word* cr_words, int hor) {
    for (int i=0; i<cnt; i++)   /* Search for suitable word and return it's address. */
        if ((cr_words[i].row == x) && (cr_words[i].col == y) && (cr_words[i].ishor != hor))
            return &cr_words[i];
    return NULL;
}

/* Function that finds each word's neighbours. */
struct word** neighbours_(struct word var, int **crash_m, struct word* cr_words, int count_words) {
    int i, j, cnt=0, k, l, last_pri;
    struct word *nei_var;
    if ((var.nei = malloc(var.nei_cnt * sizeof(struct word*))) == NULL) {   /* Array with word's neighbours. */
        perror("Error while allocating memory.\n");
        return NULL;
    }
    if (var.ishor) {    /* Find horizontal word's neighbours. */
        for (i=0; i<var.lengthw; i++) {
            if (crash_m[var.row-1][var.col-1+i] > 1) {  /* Search cells that two words pass through. */
                j=0;
                while ((var.row-1-j) >= 0 && (crash_m[var.row-1-j][var.col-1+i] != 0)) {    /* If true search next cell. */
                    j++;
                }
                nei_var = search_word(var.row-j+1, var.col+i, count_words, cr_words, 1);    /* Search for word that starts from these coordinates and is horizontal. */
                last_pri = 1;
                for (k=0; k < cnt; k++) {   /* Sorting word's neighbours by their priority. */
                    if (nei_var->pri > var.nei[k]->pri) {
                        last_pri = 0;
                        for (l = cnt; l >= k + 1; l--) {
                            var.nei[l] = var.nei[l - 1];
                        }
                        var.nei[k] = nei_var;
                        break;
                    }
                }
                if (last_pri) {
                    var.nei[cnt] = nei_var;
                }
                cnt++;
            }
        }
    }
    else {  /* Repeat same process for vertical words. */
        for (i=0; i<var.lengthw; i++) {
            if (crash_m[var.row-1+i][var.col-1] > 1) {
                j=0;
                while ((var.col-1-j) >= 0 && (crash_m[var.row-1+i][var.col-1-j] != 0)) {
                    j++;
                }
                last_pri = 1;
                nei_var =  search_word(var.row+i, var.col-j+1, count_words, cr_words, 0);
                for(k = 0 ; k < cnt ; k++){
                    if(nei_var->pri > var.nei[k]->pri){
                        last_pri = 0;
                        for(l = cnt; l >= k + 1 ; l--){
                            var.nei[l] = var.nei[l - 1];
                        }
                        var.nei[k] = nei_var;
                        break;
                    }
                }
                if(last_pri){
                    var.nei[cnt] = nei_var;
                }
                cnt++;
            }
        }
    }
    return var.nei; /* Return array with word's neighbours. */
}


void insert_word(char **crossword, struct word in_word, int **crash_l) {
    int i, j = 0;
    if(in_word.ishor){ //if the word is horizontal
        for(i = in_word.col - 1 ; i < in_word.col - 1 + in_word.lengthw ; i++){ //we cross all the cells of the array crossword where so as to store the word
            crash_l[in_word.row - 1][i]++; //we increase the corresponding cells in the array crash_l
            crossword[in_word.row - 1][i] = in_word.chars[j++];
        }
    }
    else{ //if the word is vertical
        for(i = in_word.row - 1 ; i < in_word.row - 1 + in_word.lengthw ; i++){ //we cross all the cells of the array crossword where so as to store the word
            crash_l[i][in_word.col - 1]++; //we dicrease the corresponding cells in the array crash_m
            crossword[i][in_word.col - 1] = in_word.chars[j++];
        }
    }
}


void export_word(int **crash_l, struct word ex_word, char **crossword){
    int i;
    if(ex_word.ishor){ //if the word is horizontal
        for(i = ex_word.col - 1 ; i < ex_word.col - 1 + ex_word.lengthw ; i++){ //we cross all the cells of the array crossword where we store the word
            if(!(--crash_l[ex_word.row - 1][i])){ //we decrease the corresponding cells in the array crash_l and if there does not exists any letter in there we initallize them into '-'
                crossword[ex_word.row - 1][i] = '-';
            }
        }
    }
    else { //if the word is vertical
        for(i = ex_word.row - 1 ; i < ex_word.row - 1 + ex_word.lengthw ; i++){ //we cross all the cells of the array crossword where we store the word
            if(!(--crash_l[i][ex_word.col - 1])){ //we decrease the corresponding cells in the array crash_m and if there does not exists any letter in there we initallize them into '-'
                crossword[i][ex_word.col - 1] = '-';
            }
        }
    }
}


struct word *find_word_to_export(struct word *var){ /* We remove the initialized neighbour of the variable "var" with the less priority. */
    int i;
    for(i = var->nei_cnt - 1 ; i >= 0 ; i--){   /* Neighbours are sorted by their priority. */
        if((var->nei[i])->chars != NULL){
            return var->nei[i];
        }
    }
    return NULL;
}


/* Function that checks if all crossword's variables got words and returns NULL. If not, returns a pointer to the available word with the max priority. */
struct word *availability(struct word **words_var, int n) {
    for (int i = 0 ; i < n ; i++)
        if(words_var[i]->chars == NULL)
            return words_var[i];
    return NULL;
}


void find_basic_constraints(struct word *var, char **crossword){
    int i;
    var->count_bc = 0;
    if(var->ishor){ //if the word is horizontal
        for(i = var->col - 1 ; i < var->col - 1 + var->lengthw ; i++){ //finding the positions of the letters which var.char must have
            if(crossword[var->row - 1][i] != '-'){
                var->basic_constr[var->count_bc++] = i - var->col + 1; //restrictions are the positons of the letters which constraint the searching of the word
            }
        }
    }
    else{ //if the word is vertical
        for(i = var->row - 1 ; i < var->row - 1 + var->lengthw ; i++){
            if(crossword[i][var->col - 1] != '-'){
                var->basic_constr[var->count_bc++] = i - var->row + 1; //restrictions are the positons of the letters which constraint the searching of the word
            }
        }
    }
}


char *searching(struct word *var, char **crossword){ //function which searches the appropriate value for the chars member of var variable and it returns the outcome
    find_basic_constraints(var, crossword);
    int i,j;
    for(i = 0 ; i < var->domain_count ; i++){ //searching the domain of the var
        if(var->flags_D[i]){ //if the coresponding flag is true
            for(j = 0 ; j < var->count_bc ; j++){ //we search the positions of the letters which constraint the variable
                if(var->domain[i][var->basic_constr[j]] != ((var->ishor) ?  crossword[var->row - 1][var->basic_constr[j] + var->col - 1] : crossword[var->basic_constr[j] + var->row - 1][var->col - 1])){
                    var->flags_D[i] = 0; //not appropriate word if the coresponding letters are different
                    break;
                }
            }
        }        
        if(var->flags_D[i]){ //If the word is appropriate (satisfies the requirments we want)
            var->flags_D[i] = 0; //we are trying this one, so if we have no solution and we backtrak we should not try this word again
            if((var->chars = malloc((var->lengthw + 1)* sizeof(char))) == NULL){
                fprintf(stderr, "Error while allocating memory\n");
                exit(ERROR);
            }
            strcpy(var->chars, var->domain[i]);
            return var->chars;
        }
    }
    return NULL; //We did not find a word to match
}


void bubblesort_for_horizontal(struct word x[], int y, int n){
    int i,k,flag;
    for(i = y ; i < n ; i++){
        flag = 0;
        for(k = n - 1 ; k >= i ; k--){
            if(x[k].row < x[k - 1].row){
                swapsw(&x[k], &x[k - 1]);
                flag = 1;
            }
            else if(x[k].row == x[k - 1].row){
                if(x[k].col < x[k - 1].col){
                    swapsw(&x[k], &x[k - 1]);
                    flag = 1;
                }
            }
        }
        if(!flag) break;
    }
}

void bubblesort_for_vertical(struct word x[], int y, int n){
    int i,k,flag;
    for(i = y ; i < n ; i++){
        flag = 0;
        for(k = n - 1 ; k >= i ; k--){
            if(x[k].col < x[k - 1].col){
                swapsw(&x[k], &x[k - 1]);
                flag = 1;
            }
            else if(x[k].col == x[k - 1].col){
                if(x[k].row < x[k - 1].row){
                    swapsw(&x[k], &x[k - 1]);
                    flag = 1;
                }
            }
        }
        if(!flag) break;
    }
}

int basic_restrictions_for_vertical_variables(struct word *var, char **crossword, char *tempword){
    int inx = 0, *x = calloc(var->lengthw, sizeof(int)); //inx stands for "index for x" while x is an array which stores the position of the letters which constraint the searching of the appropriate word
    if(x == NULL) {
        fprintf(stderr, "Error while allocating memory\n");
        exit(ERROR);
    }
    int i;
    for(i = var->row - 1 ; i < var->row - 1 + var->lengthw ; i++){
        if(crossword[i][var->col - 1] != '-'){
            x[inx++] = i - var->row + 1; //restrictions are the positons of the letters which constraint the searching of the word
        }
    }
    for(i = 0 ; i < inx ; i++){
        if(tempword[x[i]] != crossword[x[i] + var->row - 1][var->col - 1]){
            printf("No solution, found an error while tryng to insert %s\n", tempword);
            free(x);
            return 0;
        }
    }
    free(x);
    return 1;
}

/* Function that sorts words by their row/column and prints them. */
void sort_words(struct word *cr_words, int count_words) {
    int cnt1=0, cnt2=0, cnt=0;
    for (int i=0; i<count_words; i++) { /* Counter for horizontal words. */
        if (cr_words[i].ishor)
            cnt++;
    }
    for(int i=0; i<cnt; i++) {  /* Separate horizontal and vertical words. */
        while (cr_words[cnt1].ishor) {
            cnt1++;
        }
        while (!cr_words[cnt2].ishor) {
            cnt2++;
        }
        swapsw(&cr_words[cnt1], &cr_words[cnt2]);
    }
    
    bubblesort_for_vertical(cr_words, 1, cnt);  /* Sort vertical words. */
    bubblesort_for_horizontal(cr_words, cnt+1, count_words);    /* Sort horizontal words. */
    
    for (int i=cnt; i<count_words; i++) {   /* Print horizontal words. */
        printf("%s\n", cr_words[i].chars);
    }
    for (int i=0; i<cnt; i++) { /* Print vertical words. */
        printf("%s\n", cr_words[i].chars);
    }
}
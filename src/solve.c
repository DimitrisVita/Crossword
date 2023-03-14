#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helping.h"
#include "stack.h"
#include "solve.h"
#define ERROR -1

void solve(struct word *cr_words, int count_words, char **crossword, int **crash_l, int dimensions){
    int i;
    struct word *var;
    struct word **ptr_var;
    stack s = NULL;
    ptr_var = malloc(count_words * sizeof(struct word *));
    for(i = 0 ; i < count_words ; i++){
        ptr_var[i] = &cr_words[i];
    }
    while((var = availability(ptr_var, count_words)) != NULL){
        if(searching(var, crossword) != NULL){ /*searching the domain of the available variable*/
            insert_word(crossword, *var, crash_l); /*If we find an appropriate word for the variable we insert it in the crossword*/
            push(var, &s);
        }
        else{
            for(i = 0 ; i < var->domain_count ; i++){ /*If we do not find an appropriate word, then the domain of the variable should not have changed*/
                var->flags_D[i] = 1;
            }
            struct word *ex_var = find_word_to_export(var); /*Backjumping to the variable that causes the problem*/
            if(ex_var == NULL){ /*If there is not such variable there is no solution to the problem*/
                printf("No solution found\n");
                exit(0);
            }
            struct word *tempvar;
            while(s->in != ex_var){ /*Removing from the stack all the previous words until the ex_var (var which causes the problem)*/
                if(!pop(&s, &tempvar)){ /*If there is not ex_var in the stack then, the crossword has no solution with the given lexicon*/
                    printf("No solution found\n");
                    exit(0);
                }
                export_word(crash_l, *tempvar, crossword);
                free(tempvar->chars);
                tempvar->chars = NULL; /*Enable the possibility for every variable we export to get an other value*/
                tempvar->count_bc = 0; /*Constraints may be different next time*/
                for(i = 0 ; i < tempvar->domain_count ; i++){ /*All the words with the coresponding length are possible*/
                    tempvar->flags_D[i] = 1;
                }
            }
            if(!pop(&s, &ex_var)){ /*Doing the same for the ex_var except from changing the domain of it. We do not want to get the same value again!*/
                printf("No solution found\n");
                exit(0);
            }
            export_word(crash_l, *ex_var, crossword);
            free(ex_var->chars);
            ex_var->chars = NULL;
        }
    }
    freestack(s);
    free(ptr_var);
}
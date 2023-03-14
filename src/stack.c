#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helping.h"
#include "stack.h"
#define ERROR -1

int push(struct word *var, stack *s){
    stack temp = *s; /* Save current start of stack */
    if((*s = malloc(sizeof(struct node))) == NULL) {    /* Space for new node */
        perror("Error while allocating memory.\n");
        return 0;
    }
    (*s)->in = var; /* Put value */
    (*s)->next = temp; /* Next element is former first */
    return 1;
}

int pop(stack *s, struct word **var) {
    if(*s == NULL)
        return 0; /*Stack is empty*/
    *var = (*s)->in; /*Var is the variable name of the word we remove from the stack*/
    stack n;
    n = *s; /*Defining a new stack to store the existing one*/
    *s = (*s)->next; /*The stack pointer points to the next element, and thus the upper element is ignored*/
    free(n); /*We do not need the stack node that we vanished from the stack*/
    return 1;
}

void freestack(stack s) {
    while(s != NULL){ /*Crossing all the elements of the stack*/
        stack temp = s; /*Save it first*/
        s = s->next; /*Go to the next one*/
        free(temp); /*free the one we saved*/
    }
}
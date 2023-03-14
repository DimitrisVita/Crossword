#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helping.h"
#include "quicksort.h"
#define ERROR -1

void swapd(int *a, int *b) { /* Just exchange two doubles */
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}


void swapsw(struct word *a, struct word *b) /* Just exchange two doubles */
{
    struct word temp;
    temp = *a;
    *a = *b;
    *b = temp;
}


void quicksort_body(int *x, int up, int down)
{
    int start, end;
    start = up; /* Save start position of small elements */
    end = down; /* Save end position of large elements */
    while (up < down)
    {                                         /* Pivot element is at up position */
        while (x[down] >= x[up] && up < down) /* Let down elements */
            down--;                           /* larger than pivot stay where they are */
        if (up != down)
        {                            /* If pivot is not reached */
            swapd(&x[up], &x[down]); /* echange it with smaller element */
            up++;                    /* Pivot is at down position, move up a bit further */
        }
        while (x[up] <= x[down] && up < down) /* Let up elements */
            up++;                             /* smaller than pivot stay where they are */
        if (up != down)
        {                            /* If pivot is not reached */
            swapd(&x[up], &x[down]); /* exchange it with larger element */
            down--;                  /* Pivot is at up position, move down a bit further */
        }
    }                                     /* Now up = down is the position of the pivot element */
    if (start < up - 1)                   /* Is there at least one element left of pivot? */
        quicksort_body(x, start, up - 1); /* Quick(sort) smaller elements */
    if (end > down + 1)                   /* Is there at least one element right of pivot? */
        quicksort_body(x, down + 1, end); /* Quick(sort) larger elements */
}


int partition(struct word *arr, int low, int high) {
    int pivot = arr[high].pri;  /* Choose the last element as the pivot. */
    int i=(low-1);  /* Index of smaller element. */
    for (int j = low; j < high; j++) {  /* Move all elements with the smaller priority to the left side. */
        if (arr[j].pri >= pivot) {
            i++;
            swapsw(&arr[i], &arr[j]);
        }
    }
    swapsw(&arr[i+1], &arr[high]);    /* Move the pivot element to its correct position. */
    return i+1;  /* Return the index of the pivot element. */
}

/* Quicksort function. */
void quicksort_by_priority(struct word arr[], int low, int high) {
    if (low < high) {
        int pivot_index = partition(arr, low, high); /* Find the pivot index. */
        quicksort_by_priority(arr, low, pivot_index-1);  /* Sort left sub array. */
        quicksort_by_priority(arr, pivot_index+1, high); /* Sort right sub array. */
    }
}
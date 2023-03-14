# Crossword solver

## Creators:


> [Evangelos Argyropoulos]() AM: 1115202200010

**I have created the following parts of the project:**
* the stack and all the functions that have to do with it ('push', 'pop', 'freestack')
* 'len_index' and 'find_length' functions
* 'initiallize_domains' function
* 'insert_word' and 'export_word' functions
* the part of the neighbours function in which the neighbours get into order by priority
* the 'find_word_to_export' function
* the 'availability' function
* the 'find_basic_constraints' and the 'searching' functions
* the 'solve' function
* the 'dict' function
* the 'bubblesort_for_horizontal' and the 'bubblesort_for_vertical' functions
* the 'basic_restrictions_for_vertical_variables' function
* the 'check' function(https://cgi.di.uoa.gr/~ip/K04.pdf)


> [Dimitrios Vitantzakis](https://github.com/DimitrisVita) AM: 1115202200019

**I have created the following parts of the project:**
Functions:
* crossw
* crash
* find_words
* neighbours_ and search_word
* sort_words
* draw
* quicksort_by_priority and partition

Files:
* Makefile

Others:

* File organization.


## Compilation:

### How to run
```bash
 ~ make run
```
### How to change crossword 
```bash
 ~ ./crossw Crossword(X).txt
```
### Done? 
```bash
 ~ make clean
```
## Notes: 

1. The '-dict' question of the exersicise may not be perfect, it can solve for sure 'Crossword1.txt' and 'Crossword2.txt'.

2. There is a doubt if it can solve the other crosswords of the pronunciation. To tell the truth, it takes time and we have not waited that much to find out.

3. Quicksort_body function is based on the notes of the ["INTRODUCTION TO PROGRAMMING"](https://cgi.di.uoa.gr/~ip/K04.pdf) subject.
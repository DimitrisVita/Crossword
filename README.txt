/* File: README.txt */


Creators:


> Evangelos Argyropoulos AM: 1115202200010

I have created the following parts of the project:
+> the stack and all the functions that have to do with it ('push', 'pop', 'freestack')
+> 'len_index' and 'find_length' functions
+> 'initiallize_domains' function
+> 'insert_word' and 'export_word' functions
+> the part of the neighbours function in which the neighbours get into order by priority
+> the 'find_word_to_export' function
+> the 'availability' function
+> the 'find_basic_constraints' and the 'searching' functions
+> the 'solve' function
+> the 'dict' function
+> the 'bubblesort_for_horizontal' and the 'bubblesort_for_vertical' functions
+> the 'basic_restrictions_for_vertical_variables' function
+> the 'check' function


> Dimitrios Vitantzakis AM: 1115202200019

I have created the following parts of the project:
+ Functions:
=> crossw
=> crash
=> find_words
=> neighbours_ and search_word
=> sort_words
=> draw
=> quicksort_by_priority and partition

+ Files:
Makefile

+ Others:
Files organization.


Compilation:

Run "make" command and execute crossword solving program by typing for example: "./crossw Crossword1.txt" or "./crossw Crossword1.txt -dict EvenMoreWords.c" by following exercise's rules.
Run "make clean" command to to get rid of your object and executable files.


Note: The '-dict' question of the exersicise may not be perfect, it can solve for sure 'Crossword1.txt' and 'Crossword2.txt' in the following estimated times:
'Crossword1.txt' ->approximately 0.05 seconds in a computer of the linux lab
'Crossword2.txt' -> approximately 8.9 seconds in a computer of the linux lab
However, there is a doubt if it can solve the other crosswords of the pronunciation. To tell the truth, it takes time and we have not waited that much to find out.

Furthemore, quicksort_body function is based on the notes of the "INTRODUCTION TO PROGRAMMING" lesson and can be found at: https://cgi.di.uoa.gr/~ip/K04.pdf
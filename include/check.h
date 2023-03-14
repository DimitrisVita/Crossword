int check(int *length, int diflen, int *counts, char ***words, struct word *cr_words, int count_words, int **crash_l, char **crossword, int dimensions);

void bubblesort_for_horizontal(struct word x[], int y, int n);

void bubblesort_for_vertical(struct word x[], int y, int n);

int basic_restrictions_for_vertical_variables(struct word *var, char **crossword, char *tempword);
struct word {
    int row;    /* Row that word starts. */
    int col;    /* Column that word starts. */
    char *chars;    /* Word's characters. */
    int lengthw;    /* Word's length. */
    int ishor;  /* Flag if word is horizontal. */
    int pri;    /* Word's priority. */
    int nei_cnt;    /* Number of word's neighbours. */
    struct word **nei; /* Word's neighbours. */
    char **domain; /*Domain of word*/
    int domain_count; /*How many are the words of the domain of this variable?*/
    int *flags_D; /*For each word in the domain flags_D tells us wether it is appropriate for the variable or not*/
    int *basic_constr; /*Basic constraints which the variable should satisfy*/
    int count_bc; /*Counting basic constraints*/
};

int len_index(const int *length, const int diflen, const int len);

int find_length(int *length,int dimensions,char **crossword,int *count_words);

int **crash(int dimensions, int cnt, struct word *cr_words);

struct word *find_words(int dimensions, char **crossword,int count);

struct word *search_word(int x, int y, int cnt, struct word* cr_words, int hor);

int initialize_domains(struct word *cr_words, int count_words, int *length, char ***words, int *counts, int diflen);

struct word** neighbours_(struct word var, int **crash_m, struct word* cr_words, int count_words);

void insert_word(char **crossword, struct word in_word, int **crash_l);

void export_word(int **crash_l, struct word ex_word, char **crossword);

struct word *find_word_to_export(struct word *var);

struct word *availability(struct word **words_var, int n);

void find_basic_constraints(struct word *var, char **crossword);

char *searching(struct word *var, char **crossword);

void bubblesort_for_horizontal(struct word x[], int y, int n);

void bubblesort_for_vertical(struct word x[], int y, int n);

int basic_restrictions_for_vertical_variables(struct word *var, char **crossword, char *tempword);

void sort_words(struct word *cr_words, int count_words);
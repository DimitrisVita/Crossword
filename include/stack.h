typedef struct node *stack;

struct node{
    struct word *in;
    stack next;
};

int push(struct word *var, stack *s);

int pop(stack *s, struct word **var);

void freestack(stack s);
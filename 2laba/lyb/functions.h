#ifndef INC_2LABA_FUNCTIONS_H
#define INC_2LABA_FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef struct Data{
    char *name;
    char length;
}Data;

typedef struct Node{
    struct Data *data;
    struct Node *next;
}Node;

typedef struct Stack{
    struct Node *top;
}Stack;

//////////////////////////////STACK
Stack *create_stack();
void push(Stack *stack, Data *data);
Data *pop(Stack *stack);
Data *peek(Stack *stack);
int is_empty(Stack *stack);
int stack_size(Stack *stack);
void clear(Stack *stack);
void destroy(Stack *stack);


//////////////////////////////MEMORY
void free_node(Node *node);

/////////////////////////////HUI_Z
void copy_data(Data *dest_data, Data *source_data);

////////////////////////////FILES

#endif //INC_2LABA_FUNCTIONS_H
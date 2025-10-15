#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Stack {
  size_t count;    // number of elements in the stack
  size_t capacity; // number of elements the stack can hold before it needs to
                   // be resized in memory
  void **data;     // array of pointers to the elements stored in the stack
} stack_t;

stack_t *stack_new(size_t capacity);

int main() {
  stack_t *s = stack_new(3);
  printf("count: %zu ; capacity: %zu\n", s->count, s->capacity);
  return 0;
}

stack_t *stack_new(size_t capacity) {
  stack_t *new_stack =
      malloc(sizeof(stack_t)); // allocate memory for the new stack
  if (new_stack == NULL) {
    return NULL;
  }

  new_stack->count = 0;
  new_stack->capacity = capacity;
  // allocate memory for 'capacity' number of void* elements
  new_stack->data = malloc(capacity * sizeof(void *));

  if (new_stack->data == NULL) {
    free(new_stack);
    return NULL;
  }

  return new_stack;
}

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
void stack_push(stack_t *stack, void *obj);

int main() {
  // create new stack
  stack_t *s = stack_new(3);
  printf("count: %zu ; capacity: %zu\n", s->count, s->capacity);

  // push an integer to the stack
  int a = 42;
  printf("before stack push - count: %zu ; capacity: %zu\n", s->count,
         s->capacity);
  stack_push(s, &a);
  printf("after stack push - count: %zu ; capacity: %zu\n", s->count,
         s->capacity);
  printf("after stack push - data: %d\n", *(int *)s->data[0]);

  // push but exceed the capacity of the stack
  int b = 52;
  int c = 62;
  int d = 72;
  int e = 82;

  printf("before stack push - count: %zu ; capacity: %zu\n", s->count,
         s->capacity);
  stack_push(s, &b);
  printf("after one stack push - count: %zu ; capacity: %zu\n", s->count,
         s->capacity);
  stack_push(s, &c);
  printf("after two stack pushes - count: %zu ; capacity: %zu\n", s->count,
         s->capacity);
  stack_push(s, &d);
  printf("after three stack pushes - count: %zu ; capacity: %zu\n", s->count,
         s->capacity); // the capacity of the stack should double here
  stack_push(s, &e);
  printf("after four stack push - count: %zu ; capacity: %zu\n", s->count,
         s->capacity); // should continue to be double here as well

  free(s->data); // make sure to free the data pointers of the stack
  free(s);       // then also free the stack pointer itself
  return 0;
}

void stack_push(stack_t *stack, void *obj) {
  // should not operate on empty stack or object
  if (stack == NULL || obj == NULL) {
    fprintf(stderr,
            "stack_push was called with either an invalid stack or object\n");
    return;
  }

  // if we have reached our capacity, double the stack's capacity so that we
  // don't have to keep allocating small increments
  if (stack->count == stack->capacity) {
    size_t original_capacity = stack->capacity;
    stack->capacity *= 2;
    // we use a tmp_data here to make sure that if stack reallocation fails
    // we do not overwrite our original data with NULL
    void **tmp_data = realloc(stack->data, sizeof(void *) * stack->capacity);

    // if realloc has failed, revert back to the original capacity and just
    // return since the original data in the stack is still valid
    if (tmp_data == NULL) {
      stack->capacity = original_capacity;
      return;
    } else {
      // if realloc succeeded, update the stack->data to contain the new
      // resized data
      stack->data = tmp_data;
    }
  }

  // add the object to the stack data at index stack->count (essentially push
  // the new object to the end/top of the stack)
  stack->data[stack->count] = obj;
  stack->count++;
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

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
void *stack_pop(stack_t *stack);
void stack_free(stack_t *stack);
void dangerous_push(stack_t *stack);

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

  // pop an element from the stack
  int *popped = stack_pop(s);
  printf("after popping one element off the stack, the popped element is %d\n",
         *(int *)popped);
  printf("count of the stack after popping an element: %zu\n", s->count);
  if (s->count > 0) {
    printf("the last element in the stack data is now %d\n",
           *(int *)s->data[s->count - 1]); // the new top of the stack is now at
                                           // index count - 1
  } else {
    printf("the stack is now empty\n");
  }

  int f = 92;
  // we do one more push after we've popped an element from the stack to make
  // sure it keeps working
  stack_push(s, &f);
  printf("another stack push after we have used stack_pop in order to test - "
         "count: %zu ; capacity: %zu\n",
         s->count, s->capacity);
  printf("the last element in the stack data is now %d\n",
         *(int *)s->data[s->count - 1]);

  // here for testing invalid integer to pointer conversion examples, commented
  // out to prevent seg fault errors
  // dangerous_push(s);

  // free the memory used by the stack-> data and the stack itself
  stack_free(s);
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

void *stack_pop(stack_t *stack) {
  if (stack == NULL) {
    fprintf(stderr, "stack_pop was called with an invalid stack\n");
    return NULL;
  }

  // nothing to pop
  if (stack->count == 0) {
    fprintf(stderr, "stack_pop was called on an empty stack\n");
    return NULL;
  }

  stack->count--; // decrement the count of elements in the stack in order to
                  // make sure that stack size reflects that we have removed one
                  // element from it

  //  we pop the top element of the stack (the count-th element in the array).
  // because the stack is LIFO (last in, first out)
  void *popped_obj = stack->data[stack->count];

  // nullify the pointer to the popped element so that it is no longer
  // part of the stack.
  //
  // this step is optional since the stack's 'count'
  // already defines the active portion of the stack.
  //
  // however, it is
  // good practice to avoid leaving dangling pointers behind.
  stack->data[stack->count] = NULL;

  return popped_obj;
}

void stack_free(stack_t *stack) {
  if (stack == NULL) {
    fprintf(stderr,
            "stack_free was called with either an invalid stack or object\n");
    return;
  }

  // make sure to free the data pointers of the stack
  if (stack->data != NULL) {
    free(stack->data);
  }

  // then also free the stack pointer itself
  free(stack);
}

// just for test purposes
void dangerous_push(stack_t *stack) {
  // we try to cast an integer to void * and push it on the stack (this is not
  // valid)
  stack_push(stack, (void *)1337);
  // casting an integer like 1337 to a void * is not portable and invalid unless
  // you later cast it back to the exact same integer type, and even then,
  // dereferencing it (like *(int*)...) is completely illegal.

  // allocate 4 bytes for an integer
  int *b = malloc(sizeof(int));
  *b = 1024;
  // this is a valid pointer to an integer
  stack_push(stack, (void *)b);

  // the reason (void *)b is valid is because :
  // b has type int * → a pointer to an int.
  // b points to a memory location on the heap that can hold an integer.
  // Dereferencing it with *b is valid, and you can read/write the integer:
  // b → 0x1234 (heap address) → 1024
  //
  // (void *)b casts b from int * to void *.
  // void * is a generic pointer type — it can hold any object pointer.
  // The value of the pointer doesn’t change; it still points to the same memory
  // address (0x1234 in our example).

  // the result of these 2 stack pushes should be
  // Last element (1024) → points to real allocated memory (malloc) → safe.
  // Second-to-last (1337) → fabricated pointer (void*)1337 → unsafe (undefined
  // behaviour).

  printf("the last element in the stack data is now %d\n",
         *(int *)stack->data[stack->count - 1]);

  printf("the secont to last element in the stack data is now %d\n",
         *(int *)stack->data[stack->count - 2]);
  // this is basically trying to read from random memory.
  // stack->data[stack->count - 2] is (void *)1337.
  // So *(int *)1337 dereferences address 0x539 — illegal access →
  // segfault.
}

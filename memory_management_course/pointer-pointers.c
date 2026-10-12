#include "stdlib.h"
#include <stdio.h>
#include <stdlib.h>

void allocate_int(int **pointer_pointer, int value);

int main() {
  int x = 10;
  int *ptr = &x;

  printf("value of x: %d\n", x);
  printf("value of *ptr: %d\n", *ptr);

  int y = 15;
  allocate_int(
      &ptr,
      y); // here we pass the address of ptr via &ptr and since its address also
          // points to anotehr address ii becomes **pointer_pointer

  printf("value of x: %d (should not have changed)\n", x);
  printf("after updating ptr, value of *ptr: %d (should have changed)\n", *ptr);

  // don't forget to free memory that was previously allocated on the heap
  free(ptr);
}

void allocate_int(int **pointer_pointer, int value) {
  int *temp_ptr = malloc(sizeof(int));
  if (temp_ptr == NULL) {
    fprintf(stderr, "memory allocation failed\n");
    exit(1);
  }
  *pointer_pointer = temp_ptr;
  *temp_ptr = value;
}

#include <inttypes.h> // used to import PRIu64 in order to format and print integers of type uint64_t
#include <stdint.h> // used to provide uint64_t that is used inside the struct
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // imported for memcpy used in generic_swap()

void swap_ints(int *a, int *b);
void swap_strings(char **a, char **b);
void generic_swap(void *ptrA, void *ptrB, size_t size);

// struct used only to test the generic swap on a variety of types
typedef struct CoffeeShop {
  uint64_t quality;
  uint64_t taste;
  uint64_t branding;
} coffee_shop_t;

int main() {
  // INT SWAP TESTS
  int a = 5;
  int b = 6;

  printf("before swap: a: %d ; b: %d\n", a, b);
  swap_ints(&a, &b);
  printf("after swap: a: %d ; b: %d\n", a, b);
  // swap 'a' with itself, should result in 5 again

  a = 5; // reset a back to 5 before next test
  printf("before swap a: %d \n", a);
  swap_ints(&a, &a);
  printf("after swap a: %d \n", a);

  // STRING SWAP TESTS
  char *strA = "hello";
  char *strB = "world!";
  printf("before string swap strA: %s ; strB: %s\n", strA, strB);
  swap_strings(&strA, &strB);
  printf("after string swap strA: %s ; strB: %s\n", strA, strB);

  // reset back to what they were before first test
  strA = "hello";
  strB = "world!";
  printf("before string swap strA: %s ; strB: %s\n", strA, strB);
  swap_strings(&strA,
               &strA); // swap 'a' with itself, should result in no change
  printf("after string swap strA: %s ; strB: %s\n", strA, strB);

  // GENERIC SWAP TESTS
  // using INT
  int i1 = 1234;
  int i2 = 5678;

  printf("generic swap test using INT (before swap) i1: %d, i2: %d\n", i1, i2);
  generic_swap(&i1, &i2, sizeof(int));
  printf("generic swap test using INT (after swap) i1: %d, i2: %d\n", i1, i2);

  // using STRING
  char *s1 = "boris";
  char *s2 = "test";

  printf("generic swap test using STRING (before swap) s1: %s, s2: %s\n", s1,
         s2);
  generic_swap(&s1, &s2, sizeof(char *));
  printf("generic swap test using STRING (after swap) s1: %s, s2: %s\n", s1,
         s2);

  // using STRUCT
  coffee_shop_t sbucks = {2, 3, 5};
  coffee_shop_t dabov = {4, 5, 3};

  // PRIu64 is a macro defined in <inttypes.h> for printing uint64_t
  // trying to print a uint64_t using %d will not work, so we have to use this
  // macro
  printf(
      "generic swap test using a STRUCT (sbucks before swap) quality: %" PRIu64
      ", taste: %" PRIu64 ",branding : %" PRIu64 "d\n",
      sbucks.quality, sbucks.taste, sbucks.branding);
  generic_swap(&sbucks, &dabov, sizeof(coffee_shop_t));
  printf(
      "generic swap test using a STRUCT (sbucks after swap) quality: %" PRIu64
      ", taste: %" PRIu64 ",branding : %" PRIu64 "d\n ",
      sbucks.quality, sbucks.taste, sbucks.branding);
}

void swap_ints(int *a, int *b) {
  int tmp_int = *a; // temp int to hold value of 'a'
  *a = *b;          // assign 'b' to 'a'
  *b = tmp_int; // assign temp int that contains previous value of 'a' to 'b'
}

void swap_strings(char **a, char **b) {
  char *tmp_string = *a; // temp string to hold value of 'a'
  *a = *b;               // asssign 'b' to 'a'
  *b = tmp_string; // assign temp string that has previous value of 'a' to 'b'
}

void generic_swap(void *ptrA, void *ptrB, size_t size) {
  // allocate a temporary buffer on the heap to hold the data of size 'size'.
  // we need this because we want to swap arbitrary types and sizes of data.
  void *tmpPtr = malloc(size);
  if (tmpPtr == NULL) {
    printf("unable to allocate memory");
    return;
  }

  // copy 'a' to tmpPtr
  memcpy(tmpPtr, ptrA, size); // memcpy is used because we don;t know the type
                              // of data yet, memcpy copies raw bytes safely
  // copy 'b' to 'a'
  memcpy(ptrA, ptrB, size);
  // copy tmpPtr to 'b'
  memcpy(ptrB, tmpPtr, size);

  free(tmpPtr); // don't forget to free the temporary heap allocation we made
}

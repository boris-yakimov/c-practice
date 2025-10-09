#include <stdio.h>

void printMessageOne();
void printMessageTwo();
void printMessageThree();
void printStackPointerDiff();

int main() {
  printMessageOne();
  return 0;
}

void printMessageOne() {
  const char *message = "Dark mode?\n";
  printStackPointerDiff();
  printf("%s\n", message);
  printMessageTwo();
}

void printMessageTwo() {
  const char *message = "More like...\n";
  printStackPointerDiff();
  printf("%s\n", message);
  printMessageThree();
}

void printMessageThree() {
  const char *message = "dark roast.\n";
  printStackPointerDiff();
  printf("%s\n", message);
}

void printStackPointerDiff() {
  static void *last_sp = NULL;
  void *current_sp;
  current_sp = __builtin_frame_address(0);
  long diff = (char *)last_sp - (char *)current_sp;
  if (last_sp == NULL) {
    last_sp = current_sp;
    diff = 0;
  }
  printf("---------------------------------\n");
  printf("Stack pointer offset: %ld bytes\n", diff);
  printf("---------------------------------\n");
}

// output:
// ---------------------------------
// Stack pointer offset: 0 bytes
// ---------------------------------
// Dark mode?
//
// ---------------------------------
// Stack pointer offset: 32 bytes
// ---------------------------------
// More like...
//
// ---------------------------------
// Stack pointer offset: 64 bytes
// ---------------------------------
// dark roast.

// we can see the stack pointer offset is increasing becase we have nested
// function calls and each is still allocted in the stack until the function
// that it has called finishes its execution and its stack frame in memory on
// gets deallocated

// Each time a new function is called (printMessageOne → printMessageTwo →
// printMessageThree), a new stack frame is created.
//
// The stack pointer (SP) moves downward in memory (on most architectures like
// x86-64) to allocate space for that frame — local variables, return address,
// etc.
//
// That’s why the difference between stack pointer values increases (e.g., 0 →
// 32 → 64 bytes) — each nested call adds more stack usage.
//
// When a function returns, its stack frame is popped off, and the SP moves back
// up to the previous address.

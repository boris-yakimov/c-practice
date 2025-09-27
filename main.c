#include "prototypes.h" // include our headers file where we store function prototypes
// import standard I/O library
#include <stdbool.h> // C doesn't have a builtin bool type, in standard C bool is provided only if we include this
#include <stdio.h>

/*
this type of comment is also supported
and it is a multiline comment
*/
int main() {
  int age;
  age = 37;
  int height = 171;

  printf("%d years old\n", age);
  printf("%d cm tall\n", height);

  // char a = "a";           // should be an err becuase "a" is a char * not a
  char *a = "a";           // should work
  char single_thing = 'b'; // should work

  printf("%s\n", a);
  printf("%s\n", &single_thing);

  // change value of a var
  a = "no longer a";
  printf("%s\n", a);

  // const int x = 5;
  // x = 10; // will give a complier error because we cannot change a constant
  int x = 5;
  x = 10; // this however will work because it is a var

  float result =
      add(7, 36); // this will be a compiler error that add is not defined
                  // unless we create a function prortype before the main()
                  // function, because our add function is declared after main
                  // in our case we declare it in the prototypes.h file that is
                  // getting imported
  printf("%f\n", result);

  // run the function that shows how void works
  iDoNothing();

  // brackets are mandatory
  if (x > 15) {
    printf("we shoudn't hit this\n");
  } else if (x == 10) {
    printf("this is it\n");
  } else {
    printf("other cases\n");
  }

  // turnary operator (conditional) - similar to count in tf
  char *turnaryResult = x == 9 ? "t" : "f";
  // x is not equal to 9 so we should get the "falsey" value here
  printf("%s\n", turnaryResult);

  // %zu used for printing 'sizeof' results
  printf("sizeof(char) = %zu\n", sizeof(char));
  printf("sizeof(int) = %zu\n", sizeof(int));
  printf("sizeof(float) = %zu\n", sizeof(float));
  printf("sizeof(double) = %zu\n", sizeof(double));
  printf("sizeof(size_t) = %zu\n", sizeof(size_t));
  printf("sizeof(bool) = %zu\n", sizeof(bool));

  // for loops test
  printf("for loop tests\n");
  printNumbers(3);
  printNumbers(-2);
  printNumbers(0);

  // for while loop test
  printf("while loop test\n");
  printNumbersInReverse(10, 2);

  return 0;
}

float add(int x, int y) {
  return (float)(x + y); // we are type casting the result of integers x+y to
                         // become a float
}

// function returns an int
// in C return values are before the func name
int helloWorld(void) {
  printf("Hello, world!\n"); // statements in C end with a semicolon ;
  return 0;                  // we return exit code 0 to the OS
}

// void in C is the absence of value
void iDoNothing(void) {
  printf("func that takes no values and returns no values to provide an "
         "example for void\n\n");
}

void printNumbers(int x) {
  // initialization; conditional; final expression (runs after each iteration of
  // the loop)
  for (int i = 0; i < x; i++) {
    printf("%d\n", i);
  }
}

void printNumbersInReverse(int start, int end) {
  int i = start;
  while (i >= end) {
    printf("%d\n", i);
    i--;
  }
}

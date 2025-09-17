#include "prototypes.h" // include our headers file where we store function prototypes
// import standard I/O library
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
  printf("%f\n", result);

  // run the function that shows how void works
  iDoNothing();

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
         "example for void");
}

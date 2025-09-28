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

  // zero value initializer
  struct Coordinate zeroValueStructTest = {0};
  printf("print zero value initialized struct: %d %d %d\n",
         zeroValueStructTest.x, zeroValueStructTest.y, zeroValueStructTest.z);

  // poistional initialization
  struct Coordinate positionalValueStructTest = {
      32,
      45,
      55,
  };
  printf("print positional value initialized struct: %d %d %d\n",
         positionalValueStructTest.x, positionalValueStructTest.y,
         positionalValueStructTest.z);

  // designated initializion
  struct Coordinate designatedStructTest = {
      .x = 56,
      .y = 89,
      .z = 102,
  };
  printf("print designated value initialized struct: %d %d %d\n",
         designatedStructTest.x, designatedStructTest.y,
         designatedStructTest.z);

  // create a custom typedef for our strcut and test using it as a function
  // argument, return value, etc
  coordinate_t testForTypeDef = {.x = 105, .y = 205, .z = 305};
  coordinate_t scaledCoord = updateCoordinate(testForTypeDef, 405);
  printf("this ia test for a custom typedef on a struct, updated values are %d "
         "%d %d\n",
         scaledCoord.x, scaledCoord.y, scaledCoord.z);

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

// we cannot return multiple values from a function in C, so this will give an
// error
//
// int, char * become_older(int age, char *name) {
//   return age + 1, name;
// }
//
// we can however return structs that can have many fields
struct Coordinate newCoordinate(int x, int y, int z) {
  struct Coordinate coord = {.x = x, .y = y, .z = z};

  return coord;
}

// use the custom type definition we added to our struct
coordinate_t updateCoordinate(coordinate_t coord, int factor) {
  coordinate_t scaled = coord;
  scaled.x *= factor;
  scaled.y *= factor;
  scaled.z *= factor;

  return scaled;
}

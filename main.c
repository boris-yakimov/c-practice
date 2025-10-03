#include "prototypes.h" // include our headers file where we store function prototypes
// import standard I/O library
#include <stdbool.h> // C doesn't have a builtin bool type, in standard C bool is provided only if we include this
// to print offset and padding during struct memory layout examples
#include <stddef.h>
#include <stdio.h>
// to use strcat
#include <string.h>

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

  // sizeof can also be used on custom types
  printf("size of our custom type coordinate_t is %zu bytes\n",
         sizeof(coordinate_t));

  // see how a struct gets padding for some of its fields that make it have a
  // non-muliplicative alignment
  printf("sizeof(human_t) = %zu\n", sizeof(human_t));
  printf("offsetof(human_t, first_initial) = %zu\n",
         offsetof(human_t, first_initial));
  printf("offsetof(human_t, age) = %zu\n", offsetof(human_t, age));
  printf("offsetof(human_t, height) = %zu\n", offsetof(human_t, height));

  // print address in memory of a var via pointer
  printf("The memory address of age is: %p\n", &age);
  // update struct field via its pointer
  coordinate_t pointerTest = {10, 20, 30};
  printf("x before update via pointer is: %d\n", pointerTest.x);
  coordinate_t updatedViaPointer = updateCoordinateNewX(&pointerTest, 42);
  printf("updated x via its pointer: %d\n", updatedViaPointer.x);
  // update struct field via its pointer, but via a function that doesn't return
  // anything
  printf("z before update via pointer is: %d\n", pointerTest.z);
  updateCoordinateNewZ(&pointerTest, 17);
  printf("updated z via its pointer from a function that doesn't return "
         "anything, new value is %d\n",
         pointerTest.z);

  int yP = 5;       // yP is a normal int with value 5
  int *pTest = &yP; // &yP gives the address of yP → store it in a pointer pTest
  int xP = *pTest;  // *pTest dereferences pTest → gives the value at address
                    // (i.e. xP = 5)
  printf("pointer test, yP is: %d\n", yP);
  printf("pointer test, yTest address is: %p\n",
         pTest); // %p used to print pointer address
  printf("pointer test, yTest value is: %d\n",
         *pTest); // %d and than *pTest dereference the pointer to get its value
  printf("pointer test, xP is: %d\n", xP);

  yP = 10;
  printf("we've now update yP so print everything again\n");
  printf("pointer test, yP is: %d\n", yP);
  printf("pointer test, yTest address is: %p\n",
         pTest); // %p used to print pointer address
  printf("pointer test, yTest value is: %d\n",
         *pTest); // %d and than *pTest dereference the pointer to get its value
  printf("pointer test, xP is: %d\n",
         xP); // xP remains 5 because it was previously assign by dereferencing
              // the pointer pTest which at the time had the value of 5, so when
              // we update yP, xP doesn't change

  // arrays
  int testArray[5] = {1, 2, 3, 4, 5};
  testArray[1] = 20;
  testArray[3] = 40;
  for (int i = 0; i < 5; i++) {
    printf("%d", testArray[i]);
  }

  // array and pointers
  int testNumbers[5] = {1, 2, 3, 4, 5};
  printf(
      "\naccessing elements of an array using indexing %d\n", // access element
                                                              // via index
      testNumbers[2]);
  printf("accessing elements using pointers %d\n",
         *(testNumbers + 2)); // access the same element via pointer

  // pointer arithmetic
  int *ptrTestForNum = testNumbers;
  printf("pointer points to testNumbers[0]: %d\n", *ptrTestForNum); // output 1
  *ptrTestForNum += 2;
  printf("pointer points to testNumbers[2]: %d\n", *ptrTestForNum); // output 3

  // array of structs
  coordinate_t pointsArray[3] = {
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9},
  };
  // print out values using indexing
  printf(
      "pointsArray[1].x = %d, pointsArray[1].y = %d, pointsArray[1].z = %d\n",
      pointsArray[1].x, pointsArray[1].y, pointsArray[1].z);
  // do the same with pointers
  coordinate_t *ptrOfPoints = pointsArray;
  printf(
      "ptrOfPoints[1].x = %d, ptrOfPoints[1].y = %d, ptrOfPoints[1].z = %d\n",
      (ptrOfPoints + 1)->x, (ptrOfPoints + 1)->y,
      (ptrOfPoints + 1)->z); // here we use the -> operator to get the element
                             // of the struct when using a pointer to the struct

  // pointers are the same size no matter what data they point to because they
  // essentially contain addresses in memory
  int *intPtr;
  char *charPtr;
  double *doublePtr;
  printf("Size of int pointer: %zu bytes\n", sizeof(intPtr));
  printf("Size of char pointer: %zu bytes\n", sizeof(charPtr));
  printf("Size of double pointer: %zu bytes\n", sizeof(doublePtr));
  // this size depends on the architecture that you run this on - on a 32bit
  // system those will be 4 bytes each on a 64bit system they will be 8 bytes

  // demonstrate arrays decaying into pointers
  int core_utilization[] = {43, 67, 89, 92, 71, 43, 56, 12};
  int len = sizeof(core_utilization) / sizeof(core_utilization[0]);
  printf("sizeof core_utilization in main: %zu\n", sizeof(core_utilization));
  printf("len of core_utilization: %d\n", len);
  core_utils_func(core_utilization);

  // strings in C can be declared either with the array style or with the
  // pointer style (pointer style is preferred)
  char testStr1[] = "hello";
  char *testStr2 = "hello but pointer";
  printf("testStr1 from array: %s\n", testStr1);
  printf("testStr2 from pointer: %s\n", testStr2);

  // notice that we can allocate an array that is larger than what we use (the
  // full size is still allocated and uses memory)
  char firstStr[50] = "shorter than array size";
  char *secondStr = " and second string";
  // strcat uses the null terminator '\0' to determine where to start appending
  // the secondStr to the firstStr
  strcat(firstStr, secondStr);
  printf("the result is %s\n", firstStr);

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

// update a field of struct via its pointer
coordinate_t updateCoordinateNewX(coordinate_t *coord, int new_x) {
  coord->x = new_x; // use -> for strcut pointer
  return *coord;    // return updated struct (by value)
}

// update a field of a struct via its pointer without the function that does
// that returning anything
void updateCoordinateNewZ(coordinate_t *coord, int new_z) { coord->z = new_z; }

// test func to show arrays decaying into pointers
void core_utils_func(
    int core_utilization[]) { // this is not really an array parameter. The
                              // compiler treats it as if you had written:
                              // void core_utils_func(int *core_utilization);
  printf("sizeof core_utilization in core_utils_func: %zu\n",
         sizeof(core_utilization));
}

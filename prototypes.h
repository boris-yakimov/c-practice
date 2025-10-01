// if two files include the same header files C will try to actually declare the
// functions in the header files twice which will result in an error the
// solution to this is to add to the top of header files, this is there for the
// compiler just to avoid the issue above
#pragma once

// function prototypes; declare what function will be created later, for the
// compiler to be able to verify that correct arguments are used in function
// call before it needs to see the full function delcaration
float add(int x, int y);
void iDoNothing(void);
int helloWorld(void);

// we use this file as a sort of specification of all functions that we will
// later create, .h headers file than has to be imported with #include
// "prototypes.h" in our main function

void printNumbers(int x);
void printNumbersInReverse(int start, int end);

// struct declaration
// custom type definition
typedef struct Coordinate {
  int x;
  int y;
  int z;
} coordinate_t;

// test for a struct that has a custom typedef
coordinate_t updateCoordinate(coordinate_t coord, int factor);

// test to update struct fields via their pointer
coordinate_t updateCoordinateNewX(coordinate_t *coord, int new_x);
// the same but just update the value via the pointer wihtout the function
// returning anything
void updateCoordinateNewZ(coordinate_t *coord, int new_z);

// tests to understand struct memory layout and padding
typedef struct Human {
  char first_initial; // 1 byte
  int age;            // 4 bytes (on most systems)
  double height;      // 8 bytes (on most systems)
} human_t;

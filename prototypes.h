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

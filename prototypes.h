// included in order to use size_t as a type for field of a struct bellow
#include <string.h>
// for uint8/16/32/etc
#include <stdint.h>

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

// test func to show arrays decaying into pointers
void core_utils_func(int core_utilization[]);

typedef struct {
  size_t length;   // number of characters currently in use (not counting '\0')
  char buffer[64]; // the actual text data, always null-terminated
} TextBuffer;

int appendTxtToBuffer(TextBuffer *dest, const char *src);

// enums
typedef enum Color {
  RED = 55,
  GREEN = 176,
  BLUE = 38,
} color_t;
// color_t is a new type that can only have one of the values defined in the

typedef enum {
  EXIT_SUCCESS = 0,
  EXIT_FAILURE = 1,
  EXIT_COMMAND_NOT_FOUND = 127,
  EXIT_EXTREMELLY_LARGE_INT =
      1236347263412839714, // add a very large integer to test behaviour of
                           // sizeof() on enums
} ExitStatus;

// incrementing enums
typedef enum {
  first = 87,
  second, // becomes 88
  third,  // becomes 89
} IncrementedEnums;

// for http to str test function (also tests switch statements)
typedef enum {
  HTTP_BAD_REQUEST = 400,
  HTTP_UNAUTHORIZED = 401,
  HTTP_NOT_FOUND = 404,
  HTTP_TEAPOT = 418,
  HTTP_INTERNAL_SERVER_ERROR = 500
} http_error_code_t;

char *http_to_str(http_error_code_t code);

// unions
typedef enum SnekObjectKind {
  INTEGER, // 0  - for the urposes of identifying if an object in a union has
           // become an int or a string
  STRING,  // 1
} snek_object_kind_t;

typedef union SnekObjectData { // when we set snek_object_data_t.v_int it
                               // becomes an int stored in memory and when we
                               // set snek_object_data.v_string it becomes a
                               // string in memory
  int v_int;
  char *v_string;
} snek_object_data_t;

typedef struct SnekObject {
  snek_object_kind_t kind;
  snek_object_data_t data;
} snek_object_t;

snek_object_t new_integer(int);
snek_object_t new_string(char *str);
void format_object(snek_object_t obj, char *buffer);

// memory layout of unions
typedef union TcpPackerHeader {
  struct tcp_header {
    uint16_t src_port;  // 16 bits = 2 bytes
    uint16_t dest_port; // 16 bitts = 2 bytes
    uint32_t seq_num;   // 32 bits = 4 bytes
  };
  char raw[8]; // 64 bits = 8 bytes
  // uint8_t raw[8]; can also work here because it is uint8_t = 1 byte * [8]

  // uint8_t raw[8] expresses your intent clearly (“raw bytes”) and ensures
  // unsigned 0–255 values.

  // char raw[8] is technically more standard-compliant
  // for strict aliasing (since char can alias anything).

} packet_header_t; // the compiler allocates 8 bytes total here because the
                   // union has 2 potential options either a tcp_header struct
                   // or a raw field

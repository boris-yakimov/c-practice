#include <stdio.h>

typedef enum ObjectKind {
  INTEGER,
  FLOAT,
  BOOL,
} object_kind_t;

typedef struct TestInt {
  char *name;
  int value;
} test_int_t;

typedef struct TestFloat {
  char *name;
  float value;
} test_float_t;

typedef struct TestBool {
  char *name;
  unsigned int value;
} test_bool_t;

void test_zero_out(void *ptr, object_kind_t kind);

int main() {
  // test with an int
  test_int_t testInt1;
  testInt1.value = 42;
  printf("int value before update: %d\n", testInt1.value);
  test_zero_out(&testInt1, INTEGER);
  printf("int value after update: %d\n", testInt1.value);

  // test with a float
  test_float_t testFloat1;
  testFloat1.value = 62;
  printf("float value before update: %.2f\n", testFloat1.value);
  test_zero_out(&testFloat1, FLOAT);
  printf("float value after update: %.2f\n", testFloat1.value);

  // test with a bool
  test_bool_t testBool1;
  testBool1.value = 1;
  printf("bool value before update: %d\n", testBool1.value);
  test_zero_out(&testBool1, BOOL);
  printf("bool value after update: %d\n", testBool1.value);

  return 0;
}

void test_zero_out(void *ptr, object_kind_t kind) {
  switch (kind) {
  case INTEGER:
    //  ((some_struct_t*)ptr)->field means casting is applied to ptr and then
    //  the field is obtained.
    // (some_struct_t*)ptr->field means casting is applied to ptr->field.
    ((test_int_t *)ptr)->value = 0; // cast the void pointer to test_int_t type
    break; // don't forget to break on each case otehrwise we fall through to
           // the next one
  case FLOAT:
    ((test_float_t *)ptr)->value = 0; // cast void pointer to test_float_t type

    break;
  case BOOL: {
    // another way to do the same thing we did before but on multiple lines
    // is to first do the pointer casting
    test_bool_t *bool_obj = (test_bool_t *)ptr;
    // update the value of the pointer that is now cast to a test_bool_t type
    bool_obj->value = 0;
    break;
  } // the additional brackets here and after case BOOL: are to fix a warning
    // that the approach above works only in C23 and above but not in earlier C
    // standards, the reason this happens is becuase in the bool case we also
    // introduce a variable declaration, whereas in the integer and float cases
    // we do use only expressions
  }
}

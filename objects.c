#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum ObjectKind {
  INTEGER,
  FLOAT,
  STRING,
} object_kind_t;

typedef union ObjectData {
  int v_int;
  float v_float;
  char *v_string;
} object_data_t;

typedef struct Object {
  object_kind_t kind;
  object_data_t data;
} object_t;

object_t *new_snek_integer(int value);
object_t *new_snek_float(float value);
object_t *new_snek_string(
    const char *value); // we make this a const char * to make it clear we do
                        // not intend to modify the input

int main() {
  // int
  object_t *int_object = new_snek_integer(42);
  printf("%d\n", int_object->data.v_int);

  // float
  object_t *float_object = new_snek_float(3.14);
  printf("%.2f\n", float_object->data.v_float);

  object_t *string_object = new_snek_string("hello world");
  printf("%s\n", string_object->data.v_string);

  // don't forget to cleanup heap memory
  free(int_object);
  free(float_object);
  if (string_object->data.v_string != NULL) {
    free(string_object->data.v_string);
  }
  free(string_object);

  return 0;
}

object_t *new_snek_integer(int value) {
  // allocate memory on the heap for the object
  object_t *obj = malloc(sizeof(object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = INTEGER;
  obj->data.v_int = value;

  return obj;
}

object_t *new_snek_float(float value) {
  // allocate memory on the heap for the object
  object_t *obj = malloc(sizeof(object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = FLOAT;
  obj->data.v_float = value;

  return obj;
}

// we make this a const char * to make it clear we do not intend to modify the
// input
object_t *new_snek_string(const char *value) {
  // allocate memory on the heap for the object
  object_t *obj = malloc(sizeof(object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->kind =
      STRING; // set type before allocating string data so we can
              // correctly clean this object even if the next malloc fails.

  // allocate a second set of memory on the heap that will actually store the
  // value of the string (char * / array of char)
  obj->data.v_string = malloc(strlen(value) + 1);
  if (obj->data.v_string == NULL) {
    // we free the object here to make sure we don't leak memory if the
    // secondary heap allocation for the actual string contents fails
    free(obj);
    return NULL;
  }
  // copy value into newly allocated char * object (also copies the '\0')
  strcpy(obj->data.v_string, value);

  return obj;
}

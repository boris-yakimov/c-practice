#include <stdio.h>
#include <stdlib.h>

typedef enum ObjectKind {
  INTEGER,
} object_kind_t;

typedef union ObjectData {
  int v_int;
} object_data_t;

typedef struct Object {
  object_kind_t kind;
  object_data_t data;
} object_t;

object_t *new_snek_integer(int value);

int main() {
  object_t *int_object = new_snek_integer(42);
  printf("%d\n", int_object->data.v_int);

  return 0;
}

object_t *new_snek_integer(int value) {
  object_t *obj = malloc(sizeof(object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = INTEGER;
  obj->data.v_int = value;

  return obj;
}

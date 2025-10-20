#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Object object_t;

typedef struct Vector {
  object_t *x;
  object_t *y;
  object_t *z;
} vector_t;

typedef struct Array {
  size_t size;         // number of elements in array
  object_t **elements; // actual elements inside the array are pointers to other
                       // objects
} array_t;

typedef enum ObjectKind {
  INTEGER,
  FLOAT,
  STRING,
  VECTOR3,
  ARRAY,
} object_kind_t;

typedef union ObjectData {
  int v_int;
  float v_float;
  char *v_string;
  vector_t v_vector3; // 3 point integer
  array_t v_array;    // dynamic size array
} object_data_t;

typedef struct Object {
  object_kind_t kind; // the kind of the object
  object_data_t data; // type of data to be stored in object
} object_t;

object_t *new_snek_integer(int value);
object_t *new_snek_float(float value);
object_t *new_snek_string(
    const char *value); // we make this a const char * to make it clear we do
                        // not intend to modify the input
object_t *new_snek_vector3(object_t *x, object_t *y, object_t *z);
object_t *new_snek_array(size_t size);

int main() {
  // int
  object_t *int_object = new_snek_integer(42);
  printf("%d\n", int_object->data.v_int);

  // float
  object_t *float_object = new_snek_float(3.14);
  printf("%.2f\n", float_object->data.v_float);

  // string
  object_t *string_object = new_snek_string("hello world");
  printf("%s\n", string_object->data.v_string);

  // vector (3 point object)
  object_t *x = new_snek_integer(1);
  object_t *y = new_snek_integer(2);
  object_t *z = new_snek_integer(3);
  object_t *vector_object = new_snek_vector3(x, y, z);
  // there is a lot more nesting here because
  // vector_object contains 3 of object_t
  // than each object_t contains a data and a kind field
  // the data field is a union that can have different types
  printf("x:%d y:%d z:%d\n", vector_object->data.v_vector3.x->data.v_int,
         vector_object->data.v_vector3.y->data.v_int,
         vector_object->data.v_vector3.z->data.v_int);

  object_t *a = new_snek_integer(1);
  object_t *second_vector = new_snek_vector3(a, a, a);
  printf("x:%d y:%d z:%d\n", second_vector->data.v_vector3.x->data.v_int,
         second_vector->data.v_vector3.y->data.v_int,
         second_vector->data.v_vector3.z->data.v_int);

  object_t *array_object = new_snek_array(2);
  printf("size of the array: %zu\n", array_object->data.v_array.size);
  // TODO: for now we just allocate the array, no elements are put inside it yet

  // don't forget to cleanup heap memory
  free(int_object);
  free(float_object);
  // strings
  if (string_object->data.v_string != NULL) {
    free(string_object->data.v_string);
  }
  free(string_object);
  // vector
  free(vector_object);
  free(second_vector);
  // array
  if (array_object->data.v_array.elements != NULL) {
    free(array_object->data.v_array.elements);
  }
  free(array_object);

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

// a collection type object (similar to python's tuple that contains 3 elements)
object_t *new_snek_vector3(object_t *x, object_t *y, object_t *z) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  // allocate space on heap of the object
  object_t *obj = malloc(sizeof(object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = VECTOR3;

  // assign each object inside the fields of the vector struct
  obj->data.v_vector3.x = x;
  obj->data.v_vector3.y = y;
  obj->data.v_vector3.z = z;

  // can also be represented with a compound literal instead of assigning each
  // value individually
  // obj->data.v_vector3 = (vector_t){.x = x, .y = y, .z = z};

  return obj;
}

object_t *new_snek_array(size_t size) {
  // allocate space on heap for the object
  object_t *obj = malloc(sizeof(object_t));
  if (obj == NULL) {
    return NULL;
  }

  // allocate space for an array of pointers, each of size one object_t *
  // and the array itself of 'size' (from function argument)
  // use calloc to make sure they are initialized to zero values
  object_t **array_of_pointers = calloc(size, sizeof(object_t *));
  if (array_of_pointers == NULL) {
    free(obj);
    return NULL;
  }

  obj->kind = ARRAY;
  obj->data.v_array = (array_t){.size = size, .elements = array_of_pointers};
  // instead of using a compounding literal we can also break it down into
  // separate rows :
  //
  // obj->data.v_array.size = size;
  // obj->data.v_array.elements = array_of_pointers;

  return obj;
}

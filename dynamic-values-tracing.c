#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Lab tests implementing a tagged runtime object system — the skeleton of a
// dynamic language / interpreter value model, similar to how Python, Lisp, Lua,
// or a toy VM represents values.
// - object_t is a boxed value with a kind tag (INTEGER, FLOAT, STRING, etc.)
// - All primitive and composite types are heap-allocated objects
// - VECTOR3 is a product type — fixed tuple of 3 objects
// - ARRAY is a dynamically sized growable list of object pointers
// - All the new_snek_* functions are constructors in a managed heap
// - We are manually handling reference semantics + lifetime + dynamic typing
// - We are doing runtime type tagging + union dispatch

typedef struct Stack {
  size_t count;
  size_t capacity;
  void **data;
} stack_t;

typedef struct VirtualMachine {
  stack_t *frames;
  stack_t *objects;
} vm_t;

typedef struct StackFrame {
  stack_t *references;
} frame_t;

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
  bool is_marked;     // mark and sweep GC of objects
} object_t;

object_t *new_snek_integer(int value);
object_t *new_snek_float(float value);
object_t *new_snek_string(
    const char *value); // we make this a const char * to make it clear we do
                        // not intend to modify the input
object_t *new_snek_vector3(object_t *x, object_t *y, object_t *z);
object_t *new_snek_array(size_t size);
bool snek_array_set(object_t *obj, size_t index, object_t *value);
object_t *snek_array_get(object_t *obj, size_t index);
int snek_len(object_t *obj);
object_t *snek_add(object_t *a, object_t *b);
stack_t *stack_new(size_t capacity);
void stack_free(stack_t *stack);
void *vm_new();
void vm_free(vm_t *vm);
void stack_push(stack_t *stack, void *obj);
void *stack_pop(stack_t *stack);
void vm_frame_push(vm_t *vm, frame_t *frame);
frame_t *vm_new_frame(vm_t *vm);
void frame_free(frame_t *frame);
void frame_reference_object(frame_t *frame, object_t *obj);
void vm_track_object(vm_t *vm, object_t *obj);
void mark(vm_t *vm);
void stack_remove_nulls(stack_t *stack);
void sweep(vm_t *vm);
void trace_blacken_object(stack_t *gray_objects, object_t *obj);
void trace_mark_object(stack_t *gray_objects, object_t *obj);
void vm_collect_garbage(vm_t *vm);

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

  // arrays
  int test_array_size = 5;
  object_t *array_object = new_snek_array(test_array_size);
  printf("size of the array: %zu\n", array_object->data.v_array.size);
  object_t *new_int = new_snek_integer(3);
  // set a new value inside the array at index 0
  bool success = snek_array_set(array_object, 0, new_int);
  if (!success) {
    printf("failed to set %d in array at index %d\n", new_int->data.v_int, 0);
  }
  // get the value inside the array at index 0
  object_t *value_at_index_0 = snek_array_get(array_object, 0);
  printf("element in array at index %d is %d\n", 0,
         value_at_index_0->data.v_int);
  // get new integer object
  object_t *new_int_1 = new_snek_integer(15);
  // set the new integer inside the array at index 1
  bool success_set_1 = snek_array_set(array_object, 1, new_int_1);
  // get the element at index 1 from the array
  object_t *value_at_index_1 = snek_array_get(array_object, 1);
  // check what we got at th end
  printf("element in array at index %d is %d\n", 1,
         value_at_index_1->data.v_int);

  // get the length of an oject
  int len_of_int = snek_len(int_object);
  printf("len of int: %d\n", len_of_int);
  int len_of_float = snek_len(float_object);
  printf("len of float: %d\n", len_of_float);
  int len_of_string = snek_len(string_object);
  printf("len of string: %d\n", len_of_string);
  int len_of_vector3 = snek_len(vector_object);
  printf("len of vector: %d\n", len_of_vector3);
  int len_of_array = snek_len(array_object);
  printf("len of array: %d\n", len_of_array);

  //// dynamic add function
  // add integers
  object_t *int_one = new_snek_integer(1);
  object_t *int_three = new_snek_integer(3);
  object_t *int_four = snek_add(int_one, int_three);
  printf("result of snek_add(int_one, int_three): %d\n", int_four->data.v_int);

  // add floats
  object_t *float_one = new_snek_float(1.5);
  object_t *float_three = new_snek_float(3.5);
  object_t *float_five = snek_add(float_one, float_three);
  printf("result of snek_add(float_one, float_three): %.2f\n",
         float_five->data.v_float);

  // add strings
  object_t *string_one = new_snek_string("hello");
  object_t *string_two = new_snek_string(", world!");
  object_t *result_string = snek_add(string_one, string_two);
  printf("result of snek_add(string_one, string_two): %s\n",
         result_string->data.v_string);
  // what if we add the same string twice
  object_t *repeated_string = new_snek_string("hello ");
  object_t *result_after_repeated_string =
      snek_add(repeated_string, repeated_string);
  printf("result of snek_add(repeated_string, repeated_string): %s\n",
         result_after_repeated_string->data.v_string);

  // add vectors
  object_t *vector_int_one = new_snek_integer(1);
  object_t *vector_int_three = new_snek_integer(3);
  object_t *vectory_int_five = new_snek_integer(5);
  object_t *vector3_one =
      new_snek_vector3(vector_int_one, vector_int_three, vectory_int_five);
  object_t *result_vector_add = snek_add(vector3_one, vector3_one);
  printf("vector3_one has: x: %d, y: %d, z: %d\n", vector_int_one->data.v_int,
         vector_int_three->data.v_int, vectory_int_five->data.v_int);
  printf("result of repeated snek_add(vector3_one, vector3_one - x: %d y: %d "
         "z: %d\n",
         result_vector_add->data.v_vector3.x->data.v_int,
         result_vector_add->data.v_vector3.y->data.v_int,
         result_vector_add->data.v_vector3.z->data.v_int);

  // add arrays
  // array of 2 integers
  object_t *int_six = new_snek_integer(6);
  object_t *array_of_sixes = new_snek_array(2);
  assert(snek_array_set(array_of_sixes, 0, int_six));
  assert(snek_array_set(array_of_sixes, 1, int_six));

  // array of 3 strings
  object_t *hi = new_snek_string("hi");
  object_t *hellos = new_snek_array(3);
  assert(snek_array_set(hellos, 0, hi));
  assert(snek_array_set(hellos, 1, hi));
  assert(snek_array_set(hellos, 2, hi));

  // add the 2 together
  object_t *result_array_add = snek_add(array_of_sixes, hellos);
  printf("result of adding 2 arays, one of size 2 one of size 3: %zu\n",
         result_array_add->data.v_array.size);
  printf("print the second element: [%d] and fourth element: [%s] in "
         "the array to see that "
         "it is larger than both the 2 and 3 sized ones that were merged "
         "together, also second element is an int, fourth is a string\n ",
         result_array_add->data.v_array.elements[1]->data.v_int,
         result_array_add->data.v_array.elements[3]->data.v_string);

  // Mark and sweep
  vm_t *vm = vm_new();
  printf("frames capacity %zu\n", vm->frames->capacity);
  printf("objects capacity %zu\n", vm->objects->capacity);
  vm_free(vm);

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

  // custom dynamic objects
  free(int_one);
  free(int_three);
  free(int_four);
  free(float_one);
  free(float_three);
  free(float_five);
  free(string_one);
  free(string_two);
  free(result_string);
  free(repeated_string);
  free(result_after_repeated_string);
  free(vector_int_one);
  free(vector_int_three);
  free(vectory_int_five);
  free(vector3_one);
  free(result_vector_add);

  // Continue here for tracing GC tests
  // Test stack_push and stack_pop
  stack_t *stack = stack_new(2);
  assert(stack != NULL);
  int a_val = 10;
  int b_val = 20;
  stack_push(stack, &a_val);
  stack_push(stack, &b_val);
  assert(stack->count == 2);
  int *popped = stack_pop(stack);
  assert(*popped == 20);
  assert(stack->count == 1);
  printf("stack_push/stack_pop test passed, popped value = %d\n", *popped);

  // Test stack_remove_nulls
  stack->data[0] = NULL;
  stack->data[1] = NULL;
  stack->count = 2;
  stack_remove_nulls(stack);
  assert(stack->count == 0);
  printf("stack_remove_nulls test passed (count=%zu)\n", stack->count);
  stack_free(stack);

  // Test VM + frame creation
  vm_t *test_vm = vm_new();
  assert(test_vm != NULL);
  frame_t *test_frame = vm_new_frame(test_vm);
  assert(test_frame != NULL);
  assert(test_vm->frames->count == 1);
  printf("vm_new_frame test passed (frames=%zu)\n", test_vm->frames->count);

  // Test frame_reference_object
  object_t *ref_obj = new_snek_integer(123);
  frame_reference_object(test_frame, ref_obj);
  assert(test_frame->references->count == 1);
  printf("frame_reference_object test passed (count=%zu)\n",
         test_frame->references->count);

  // Test vm_track_object
  vm_track_object(test_vm, ref_obj);
  assert(test_vm->objects->count == 1);
  printf("vm_track_object test passed (objects=%zu)\n",
         test_vm->objects->count);

  // Test trace_mark_object
  stack_t *gray = stack_new(4);
  trace_mark_object(gray, ref_obj);
  assert(ref_obj->is_marked == true);
  assert(gray->count == 1);
  printf("trace_mark_object test passed\n");

  // Test trace_blacken_object for VECTOR3
  object_t *vx = new_snek_integer(1);
  object_t *vy = new_snek_integer(2);
  object_t *vz = new_snek_integer(3);
  object_t *vec = new_snek_vector3(vx, vy, vz);
  trace_blacken_object(gray, vec);
  assert(vx->is_marked && vy->is_marked && vz->is_marked);
  printf("trace_blacken_object (VECTOR3) test passed\n");

  // Test trace_blacken_object for ARRAY
  object_t *arr = new_snek_array(2);
  snek_array_set(arr, 0, vx);
  snek_array_set(arr, 1, vy);
  trace_blacken_object(gray, arr);
  assert(vx->is_marked && vy->is_marked);
  printf("trace_blacken_object (ARRAY) test passed\n");

  stack_free(gray);

  // Test sweep: add unmarked object and expect it removed
  object_t *will_be_collected = new_snek_integer(999);
  vm_track_object(test_vm, will_be_collected);
  assert(test_vm->objects->count == 2);
  sweep(test_vm);
  assert(test_vm->objects->count == 1); // unmarked object was removed
  printf("sweep test passed (remaining objects=%zu)\n",
         test_vm->objects->count);

  // Test full GC (mark + trace + sweep)
  vm_collect_garbage(test_vm);
  printf("vm_collect_garbage test ran successfully\n");

  vm_free(test_vm);

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

// set *value in an array (*obj) at index
bool snek_array_set(object_t *obj, size_t index, object_t *value) {
  if (obj == NULL || value == NULL) {
    return false;
  }

  if (obj->kind != ARRAY) {
    return false;
  }

  // check if index may be out of bounds depending on the size of the array
  // valid index range is ..size - 1 (because we start from 0)
  if (index >= obj->data.v_array.size) {
    return false;
  }

  // set the the new 'value' in the array 'element' at 'index'
  obj->data.v_array.elements[index] = value;

  return true;
}

// get the value inside array (*obj) at index
object_t *snek_array_get(object_t *obj, size_t index) {
  if (obj == NULL) {
    return NULL;
  }

  if (obj->kind != ARRAY) {
    return NULL;
  }

  // check if index may be out of bounds depending on the size of the array
  // valid index range is ..size - 1 (because we start from 0)
  if (index >= obj->data.v_array.size) {
    return NULL;
  }

  // set the the value in array element at index
  return obj->data.v_array.elements[index];
}

int snek_len(object_t *obj) {
  if (obj == NULL) {
    return -1;
  }

  switch (obj->kind) {
  case INTEGER:
    return 1;
  case FLOAT:
    return 1;
  case STRING:
    return strlen(obj->data.v_string);
  case VECTOR3:
    return 3;
  case ARRAY:
    return obj->data.v_array.size;
  default:
    fprintf(stderr, "invalid object type");
    return -1;
  }
}

// dynamically add 2 things together, works for integers, floats, strings,
// arrays, vector3
object_t *snek_add(object_t *a, object_t *b) {
  if (a == NULL || b == NULL) {
    return NULL;
  }

  switch (a->kind) {
  case INTEGER:
    if (b->kind == INTEGER) {
      return new_snek_integer(a->data.v_int + b->data.v_int);
    }

    // int + float = float
    if (b->kind == FLOAT) {
      // typecasting 'а' to a float here is optional since it will get promoted
      // to a float automatically, but I add it for visual clarity of what
      // happens
      return new_snek_float((float)a->data.v_int + b->data.v_float);
    }

    return NULL; // if netither int nor float return because invalid operation
                 // we can add only integers and floats together

  case FLOAT:
    // float + int = float
    if (b->kind == INTEGER) {
      // typecasting 'b' to a float here is optional since it will get promoted
      // to a float automatically, but I add it for visual clarity of what
      // happens
      return new_snek_float(a->data.v_float + (float)b->data.v_int);
    }

    if (b->kind == FLOAT) {
      return new_snek_float(a->data.v_float + b->data.v_float);
    }

    return NULL; // if netither int nor float return because invalid operation
                 // we can add only integers and floats together

  case STRING:
    if (b->kind != STRING) {
      return NULL; // only a string can be added to another string
    }

    // Calculate the length of the new string by combining the length of the two
    // strings (properly handling the null terminator)
    //
    // type should be size_t not int because we can hit issues with very large
    // strings and size_t is what strlen() returns as a valid size
    size_t len_of_combined_str =
        strlen(a->data.v_string) + strlen(b->data.v_string) + 1; // +1 for '\0'

    // Allocate memory for a new temporary string using calloc (to make sure it
    // is also initialized)
    char *temp_string = calloc(len_of_combined_str, sizeof(char));
    if (temp_string == NULL) {
      fprintf(stderr, "failed to allocate space for temp string");
      return NULL;
    }

    // combine data from both arrays to a new one
    //  we use strcpy the first time to just copy the first full aray into the
    //  new one
    //  than we use strcat to only append the contents of 'b' on top of
    //  what was already added from 'a'
    strcpy(temp_string, a->data.v_string);
    strcat(temp_string, b->data.v_string);

    // Create a new_snek_string and pass in the temporary string.
    object_t *combined_string = new_snek_string(temp_string);

    // Free the memory for the temporary string and return the new string
    // object.
    free(temp_string);

    return combined_string;

  case VECTOR3:
    if (b->kind != VECTOR3) {
      return NULL;
    }

    // Recursively call snek_add for each of the x, y, and z fields. For
    // example, a vector [1,2,3]+[4,5,6] should result in a new vector [5,7,9].
    object_t *result_of_x = snek_add(a->data.v_vector3.x, b->data.v_vector3.x);
    if (!result_of_x) {
      return NULL; // handle the case where recursive add may fail
    }
    object_t *result_of_y = snek_add(a->data.v_vector3.y, b->data.v_vector3.y);
    if (!result_of_y) {
      free(result_of_x); // if the second add fails for any reason we need to
                         // free the memory that was allocated by the first add
                         // otherwise we will leak it

      return NULL; // handle the case where recursive add may fail
    }
    object_t *result_of_z = snek_add(a->data.v_vector3.z, b->data.v_vector3.z);
    if (!result_of_z) {
      // and in this case we have aleady allocated 2 obejcts, so we need to free
      // both if the third allocation fails, so that we do not leak memory
      free(result_of_x);
      free(result_of_y);

      return NULL; // handle the case where recursive add may fail
    }
    object_t *new_vector =
        new_snek_vector3(result_of_x, result_of_y, result_of_z);
    return new_vector;

  case ARRAY:
    if (b->kind != ARRAY) {
      return NULL;
    }

    // Create a new_snek_array with the combined length of the two arrays.
    size_t len_of_combined_array = a->data.v_array.size + b->data.v_array.size;
    object_t *new_combined_array = new_snek_array(len_of_combined_array);

    // populate elements from array 'a' into the new array
    for (size_t i = 0; i < a->data.v_array.size; i++) {
      object_t *current_value_at_index_i = snek_array_get(a, i);
      bool succes =
          snek_array_set(new_combined_array, i, current_value_at_index_i);
      if (!succes) {
        fprintf(stderr, "failed to set value in array 'a' at index %zu", i);
      }
    }
    // offset is used to see how many elements have been addded so far from
    // array 'a' and continue at the next free index, if we don't do this we
    // sould start from i = 0 which will start overwriting the elements that
    // have already been populated by 'a'

    // populate elements from array 'b' into the new array
    size_t offset = a->data.v_array.size;
    for (size_t i = 0; i < b->data.v_array.size; i++) {
      object_t *current_value_at_index_i = snek_array_get(b, i);
      bool succes = snek_array_set(new_combined_array, offset + i,
                                   current_value_at_index_i);
      if (!succes) {
        fprintf(stderr, "failed to set value in array 'b' at index %zu", i);
      }
    }

    return new_combined_array;

  default:
    fprintf(stderr, "invalid operation");
    return NULL;
  }
}

object_t *_new_snek_object(vm_t *vm) {
  // allocate and initialize an object on the heap, e.g. snek_integer,
  // snek_string, snek_array, etc
  object_t *obj = calloc(1, sizeof(object_t));
  if (obj == NULL) {
    return NULL;
  }

  // track the object in the VM for garbage collection
  vm_track_object(vm, obj);
  obj->is_marked = false;

  return obj;
}

// free an object from heap memory, while checking what type it is, if it is a
// type that has nested objects, we would free those as well
void snek_object_free(object_t *obj) {
  switch (obj->kind) {
  // int and float are simple because they don't have anything nested
  // so we just have to call free(obj) on each of them
  case INTEGER:
    break;
  case FLOAT:
    break;
  // for string we have to also make sure that we first free the data inside
  // and only than can we free the obj
  case STRING:
    free(obj->data.v_string);
    break;
  // for refcount we had to free nested integer objects inside the vector but in
  // mark and sweep GC we don't, we can just free the main object and let mark
  // and sweep handle freeing the nested objects
  case VECTOR3:
    break;
  // for array we need to only free the elements array, mark and sweep GC will
  // handle freeing the nested objects inside each element
  case ARRAY:
    free(obj->data.v_array.elements);
  }

  free(obj);
}

stack_t *stack_new(size_t capacity) {
  stack_t *stack = malloc(sizeof(stack_t));
  if (stack == NULL) {
    return NULL;
  }

  stack->capacity = capacity;
  stack->count = 0;
  stack->data = malloc(stack->capacity * sizeof(void *));
  if (stack->data == NULL) {
    free(stack);
    return NULL;
  }

  return stack;
}

void stack_free(stack_t *stack) {
  // if its already freed do nothing
  if (stack == NULL) {
    return;
  }

  // free the memory allocated by the data inside the stack
  if (stack->data != NULL) {
    free(stack->data);
  }

  // than free the stack itself
  free(stack);
}

void stack_remove_nulls(stack_t *stack) {
  size_t new_count = 0;

  // iterate through the stack and compact non-NULL pointers.
  for (size_t i = 0; i < stack->count; ++i) {
    if (stack->data[i] != NULL) {
      stack->data[new_count++] = stack->data[i];
    }
  }

  // update the count to reflect the new number of elements.
  stack->count = new_count;

  // optionally, you might want to zero out the remaining slots.
  for (size_t i = new_count; i < stack->capacity; ++i) {
    stack->data[i] = NULL;
  }
}

void stack_push(stack_t *stack, void *obj) {
  if (stack->count == stack->capacity) {
    // double the stack capacity to avoid having to reallocate often
    stack->capacity *= 2;
    stack->data = realloc(stack->data, stack->capacity * sizeof(void *));
    if (stack->data == NULL) {
      exit(1); // we should see immediately if this fails
    }
  }

  stack->data[stack->count] = obj;
  stack->count++;
}

void *stack_pop(stack_t *stack) {
  if (stack == NULL) {
    fprintf(stderr, "stack_pop was called with an invalid stack\n");
    return NULL;
  }

  // nothing to pop
  if (stack->count == 0) {
    fprintf(stderr, "stack_pop was called on an empty stack\n");
    return NULL;
  }

  stack->count--; // decrement the count of elements in the stack in order to
                  // make sure that stack size reflects that we have removed one
                  // element from it

  //  we pop the top element of the stack (the count-th element in the array).
  // because the stack is LIFO (last in, first out)
  void *popped_obj = stack->data[stack->count];

  // nullify the pointer to the popped element so that it is no longer
  // part of the stack.
  //
  // this step is optional since the stack's 'count'
  // already defines the active portion of the stack.
  //
  // however, it is
  // good practice to avoid leaving dangling pointers behind.
  stack->data[stack->count] = NULL;

  return popped_obj;
}

void *vm_new() {
  vm_t *vm = malloc(sizeof(vm_t));
  if (vm == NULL) {
    return NULL;
  }

  vm->frames = stack_new(8);
  vm->objects = stack_new(8);

  return vm;
}

void vm_free(vm_t *vm) {
  if (vm == NULL) {
    return; // nothing to be freed
  }

  if (vm->frames != NULL) {
    // if there are frames inside the vm stack go over them and free each one
    for (int i = 0; i < vm->frames->count; i++) {
      // typecasting (frame_t *) is optional here is it will implictly be cast
      // by the C compiler from void * to frame_t* but it might be nice to
      // explicitly declare the type that we expect this to be for safety
      // also implicit converstions are not allowed in C++ so if we use C++
      // compiler it will fail
      frame_free((frame_t *)vm->frames->data[i]);
    }
    // than free the frames stack
    stack_free(vm->frames);
    // defensive programming: nullify the pointers in case they may accidentally
    // be referenced after they are freed
    vm->frames =
        NULL; // this prevents dangling pointers if vm is ever reused after free
  }

  if (vm->objects != NULL) {
    // if there are objects inside the vm stack go over them and free each one
    for (int i = 0; i < vm->objects->count; i++) {
      // typecasting (object_t *) is optional here is it will implictly be cast
      // by the C compiler from void * to frame_t* but it might be nice to
      // explicitly declare the type that we expect this to be for safety
      // also implicit converstions are not allowed in C++ so if we use C++
      // compiler it will fail
      snek_object_free((object_t *)vm->objects->data[i]);
    }
    stack_free(vm->objects);
    // defensive programming: nullify the pointers in case they may accidentally
    // be referenced after they are freed
    vm->objects = NULL;
  }

  free(vm);
}

void vm_frame_push(vm_t *vm, frame_t *frame) {
  if (vm == NULL || vm->frames == NULL) {
    return; // vm should not be empty
  }

  if (frame == NULL) {
    return; // frame should not be empty
  }

  // push a frame on the frames stack
  stack_push(vm->frames, frame);
}

frame_t *vm_new_frame(vm_t *vm) {
  if (vm == NULL) {
    return NULL; // vm should not be empty
  }
  // allocate space for a frame on the heap
  frame_t *frame = malloc(sizeof(frame_t));
  if (frame == NULL) {
    return NULL; // heap allocation should succeed
  }

  // create a new stack reference with capacity 8
  frame->references = stack_new(8);
  if (frame->references == NULL) {
    free(frame);
    return NULL; // assigning a new stack to the references should succeed
  }

  // push newly allocated frame to the stack, we use the helper function we
  // created above to make sure we don't forget to associate the frame with our
  // VM
  vm_frame_push(vm, frame);

  return frame;
}

void frame_free(frame_t *frame) {
  if (frame == NULL) {
    return; // already free
  }

  if (frame->references != NULL) {
    stack_free(frame->references);
  }

  free(frame);
}

void frame_reference_object(frame_t *frame, object_t *obj) {
  if (frame == NULL || obj == NULL) {
    return; // neither should be empty
  }
  stack_push(frame->references, (void *)obj);
}

void vm_track_object(vm_t *vm, object_t *obj) {
  if (vm == NULL || obj == NULL) {
    return; // neither should be empty
  }

  // push the object to the vm->objects stack, in order to
  // start tracking it for later garbage collection
  stack_push(vm->objects,
             obj); // optionally we can also cast the object here to (void
                   // *)obj, this is is not
  // required in C but may be required if we use a C++ compiler, since they may
  // not allow implicit conversions
}

void mark(vm_t *vm) {
  if (vm == NULL) {
    return; // vm should not be empty
  }

  // go over each frame
  for (int f = 0; f < vm->frames->count; f++) {
    frame_t *frame = (frame_t *)vm->frames->data[f];
    if (frame == NULL || frame->references == NULL) {
      continue;
    }

    // go over each reference in each frame
    for (int r = 0; r < frame->references->count; r++) {
      object_t *obj = (object_t *)frame->references->data[r];
      // continue only if object is not NULL to prevent seg fault errors when
      // ocassionally the VM may hold null references on the stack
      if (obj != NULL) {
        // set mark on each referenced object to true because we have those
        // objects references directly by the stack frames which means they
        // should not be cleaned up
        obj->is_marked = true;
      }
    }
  }
}

void trace_mark_object(stack_t *gray_objects, object_t *obj) {
  if (obj == NULL || obj->is_marked == true) {
    return; // obj should not be empty and we don't need to do anything to
            // objects that are already marked
  }

  // if object is not empty and not marked, we mark it and we push it on the
  // gray objects stack
  obj->is_marked = true;
  stack_push(gray_objects, obj);
}

// there are 3 sets of objects in Mark and Swee
// white - Unreachable, not yet visited → will be freed
// gray - Reachable, discovered but not fully processed
// black - Reachable and fully processed (all children marked)

// mark nested objects for GC
void trace_blacken_object(stack_t *gray_objects, object_t *obj) {
  switch (obj->kind) {
    // int, float,string don't contain any references to other objects
  case INTEGER:
    return;
  case FLOAT:
    return;
  case STRING:
    return;
  case VECTOR3:
    // mark the nested integer objects inside vector3
    trace_mark_object(gray_objects, obj->data.v_vector3.x);
    trace_mark_object(gray_objects, obj->data.v_vector3.y);
    trace_mark_object(gray_objects, obj->data.v_vector3.z);
    break;
  case ARRAY:
    // mark each nested element inside the array
    for (int i = 0; i < obj->data.v_array.size; i++) {
      object_t *elem = obj->data.v_array.elements[i];
      trace_mark_object(gray_objects, elem);
    }
    break;
  }
}

// trace all objects in the VM and mark them and their nested objects for GC
void trace(vm_t *vm) {
  stack_t *gray_objects = stack_new(8);
  if (gray_objects == NULL) {
    return; // the allocation of a new stack failed
  }

  // build gray stack
  // collect a list of all marked objects in the VM
  for (int i = 0; i < vm->objects->count; i++) {
    object_t *obj = (object_t *)vm->objects->data[i];
    if (obj->is_marked == true) {
      // push each object that is marked to the gray_objects stack
      stack_push(gray_objects, obj);
    }
  }

  // go through each marked object and mark all of its nested objects
  while (gray_objects->count > 0) {
    // pop one of them and move it to the backen objects stack
    object_t *popped_obj = stack_pop(gray_objects);
    trace_blacken_object(gray_objects, popped_obj);
  }

  // we free this stack because we used it only as a placeholder stack to keep
  // the gray objects that were marked and but their nested objects also needed
  // to be checked, after we dealt with marking both the gray objects and their
  // nested ones, we no longer need this placeholder
  stack_free(gray_objects);
}

void sweep(vm_t *vm) {
  if (vm == NULL) {
    return; // vm should not be empty
  }

  for (int i = 0; i < vm->objects->count; i++) {
    // go over each object in the stack
    object_t *obj = (object_t *)vm->objects->data[i];
    if (obj->is_marked == true) {
      // if it is marked as used remove the mark
      obj->is_marked = false;
      continue;
    } else {
      // otherwise free the oject
      snek_object_free(obj);
      // and set the previous position it occupied to NULL
      vm->objects->data[i] = NULL;
    }
  }

  // remove NULLs from the objects stack in the VM
  stack_remove_nulls(vm->objects);
}

void vm_collect_garbage(vm_t *vm) {
  if (vm == NULL) {
    return; // vm should not be empty
  }

  // mark objects that have no references for garbage collection
  mark(vm);

  // trace all the objects (and their nested objects) for garbage collection
  trace(vm);

  // sweep all of the objects that have no references
  sweep(vm);
}

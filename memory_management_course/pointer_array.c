#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Token {
  char *literal;
  int line;
  int column;
} token_t;

token_t **create_token_pointer_array(token_t *tokens, size_t count);

int main() {
  token_t tokens[3] = {
      {"foo", 1, 1},
      {"bar", 2, 5},
      {"baz", 3, 10},
  };
  token_t **result1 = create_token_pointer_array(tokens, 3);

  for (size_t i = 0; i < 3; i++) {
    printf("Token %zu: literal='%s', line=%d, column=%d\n", i,
           result1[i]->literal, result1[i]->line, result1[i]->column);
  }

  for (size_t i = 0; i < 3; i++) {
    free(result1[i]); // free each token pointer first
  }
  free(result1); // then free the array of token_pointers
}

token_t **create_token_pointer_array(token_t *tokens, size_t count) {
  token_t **token_pointers = malloc(count * sizeof(token_t *));
  if (token_pointers == NULL) {
    exit(1);
  }

  for (size_t i = 0; i < count; i++) {
    token_t *tmp_ptr =
        malloc(sizeof(token_t)); // allocate memory for a new token_t struct

    token_pointers[i] = tmp_ptr; // add our newly created token pointer to the
                                 // array at current index

    *token_pointers[i] = tokens[i]; // assign the same struct values that we
                                    // have at tokens[i] (current index) to
                                    // the new pointer that we just created
  }
  // this results in a structure like this :
  // token_pointers[0] = ptr1 -> token_t{.literal="foo", .line=1, .column=1}
  // token_pointers[1] = ptr2 -> token_t{.literal="bar", .line=2, .column=5}
  // token_pointers[2] = ptr3 -> token_t{.literal="baz", .line=3, .column=10}

  return token_pointers;
}

#include <stdio.h>

typedef struct {
  int x;
  int y;
} coord_t;

coord_t *new_coord(int x, int y) {
  coord_t c;
  c.x = x;
  c.y = y;
  return &c; // this is a problem because we are returing a pointer that is
  // in a
  //            // stack that is going to get deallocated after the function
  //            // returns
}

int main() {
  coord_t *c1 = new_coord(10, 20);
  coord_t *c2 = new_coord(30, 40);
  coord_t *c3 = new_coord(50, 60);

  printf("c1: %d, %d\n", c1->x, c1->y);
  printf("c2: %d, %d\n", c2->x, c2->y);
  printf("c3: %d, %d\n", c3->x, c3->y);
  return 0;

  // this doesn't even compile, we get :

  // stack-pointers-test.c: In function ‘new_coord’:
  // stack-pointers-test.c:12:10: warning: function returns address of local
  // variable [-Wreturn-local-addr] <-- this is the problem we are returning a
  // pointer to something that will be deallocated from the stack
}

// how this can be fixed
coord_t new_coord_fixed(int x, int y) {
  coord_t c;
  c.x = x;
  c.y = y;

  return c; // the fix is to return a struct instead of a pointer to a struct
}

int main_fixed() {
  coord_t c1 = new_coord_fixed(10, 20); // we update each of this to no longer
                                        // be a pointer to a struct coord_t *
  coord_t c2 = new_coord_fixed(30, 40);
  coord_t c3 = new_coord_fixed(50, 60);

  printf("c1: %d, %d\n", c1.x,
         c1.y); // we also update ach of these because we are now accessing a
                // field of struct (using '.') instead of a pointer to a struct
                // (using '->')
  printf("c2: %d, %d\n", c2.x, c2.y);
  printf("c3: %d, %d\n", c3.x, c3.y);
  return 0;
}

// output of the working example :
// c1: 10, 20
// c2 : 30, 40
// c3 : 50, 60

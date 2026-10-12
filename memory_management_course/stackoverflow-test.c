#include <stdio.h>

// Run the code in its current state. You should get a stack overflow which will
// cause a segmentation fault.
int main() {
  const int pool_size =
      1024 * 10; // working version that doesn't cause a stackoverflow
  // const int pool_size = 1024 * 1024 * 10; // too large pool size that causes
  // a stack overflow and segmentation fault
  char snek_pool[pool_size];
  snek_pool[0] = 's';
  snek_pool[1] = 'n';
  snek_pool[2] = 'e';
  snek_pool[3] = 'k';
  snek_pool[4] = '\0';

  printf("Size of pool: %d\n", pool_size);
  printf("Initial string: %s\n", snek_pool);
  return 0;
}

// output:
// [1]    3746 segmentation fault (core dumped)  ./stof

// it means the Linux kernel dumped the process memory into a core dump file for
// debugging. Where that file ends up depends on your system configuration.

// core dumps can be disabled on OS level so nothing gets generated
// check with :
// ulimit -c
// if the response is 0 , no core dump file will be generated

// import standard I/O library
#include <stdio.h>

int main() {
  int age;
  age = 37;
  int height = 171;

  printf("%d years old\n", age);
  printf("%d cm tall\n", height);

  return 0;
}

// function returns an int
// in C return values are before the func name
int helloWorld() {
  printf("Hello, world!\n"); // statements in C end with a semicolon ;
  return 0;                  // we return exit code 0 to the OS
}

/*
this type of comment is also supported
and it is a multiline comment
*/

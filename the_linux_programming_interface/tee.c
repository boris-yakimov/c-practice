#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// write all n bytes to fd. write() may write fewer bytes than asked
// (partial write), so loop until everything is written
static int write_all(int fd, const char *buf, size_t n) {
  while (n > 0) {
    ssize_t w = write(fd, buf, n);
    if (w < 0) {
      if (errno == EINTR) // interrupted by a signal before writing, retry
        continue;
      return -1;
    }
    buf += w;
    n -= w;
  }
  return 0;
}

// linux tee like behaviour
// usage: ./tee [-a] file
int main(int argc, char *argv[]) {
  // create file if it doesn't exist, open it for writing
  // default: truncate (delete old contents), -a: append at the end
  int flags = O_WRONLY | O_CREAT | O_TRUNC;

  int opt;
  while ((opt = getopt(argc, argv, "a")) != -1) {
    if (opt == 'a') {
      flags = O_WRONLY | O_CREAT | O_APPEND;
    } else {
      fprintf(stderr, "usage: %s [-a] file\n", argv[0]);
      return 1;
    }
  }
  // getopt leaves optind pointing at the first non-option arg (the file)
  if (optind != argc - 1) {
    fprintf(stderr, "usage: %s [-a] file\n", argv[0]);
    return 1;
  }
  const char *path = argv[optind];

  int fd = open(path, flags, 0644);
  if (fd < 0) { // syscall wrappers return -1 on error
    perror("open");
    return 1;
  }

  // no dup2 here: tee needs BOTH stdout (fd 1) and the file (fd).
  // dup2(fd, STDOUT_FILENO) would replace stdout with the file, so
  // nothing would reach the terminal anymore

  char buf[8192];
  ssize_t n; // signed: read() returns -1 on error, size_t can't hold that
  // read() returns as soon as some data is available (e.g. one line typed
  // in the terminal). fread() would wait to fill all 8192 bytes or EOF,
  // so nothing would show up until Ctrl-D
  while ((n = read(STDIN_FILENO, buf, sizeof buf)) != 0) { // 0 = EOF
    if (n < 0) {
      if (errno == EINTR)
        continue;
      perror("read");
      return 1;
    }
    // send the same bytes to both places (file and stdout)
    if (write_all(STDOUT_FILENO, buf, n) < 0) {
      perror("write stdout");
      return 1;
    }
    if (write_all(fd, buf, n) < 0) {
      perror("write file");
      return 1;
    }
  }

  if (close(fd) < 0) { // close file descriptor, can report delayed errors
    perror("close");
    return 1;
  }

  return 0;
}

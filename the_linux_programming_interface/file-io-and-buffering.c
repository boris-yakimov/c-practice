// 13-1.   Using the time built-in command of the shell, try timing the
// operation of the program in Listing 4-1 (copy.c) on your system.
//
// a) Experiment with different file and buffer sizes. You can set the buffer
// size using the –DBUF_SIZE=nbytes option when compiling the program.
//
// b) Modify the open() system call to include the O_SYNC flag. How much
// difference does this make to the speed for various buffer sizes?
//
// c) Try performing these timing tests on a range of file systems (e.g., ext3,
// XFS, Btrfs, and JFS). Are the results similar? Are the trends the same when
// going from small to large buffer sizes?
//
// 13-2.   Time the operation of the filebuff/write_bytes.c program (provided in
// the source code distribution for this book) for various buffer sizes and file
// systems.
//
// 13-3.   What is the effect of the following statements?
//
// fflush(fp);
// fsync(fileno(fp));
//
// 13-4.   Explain why the output of the following code differs depending on
// whether standard output is redirected to a terminal or to a disk file.
//
// printf("If I had more time, \n");
// write(STDOUT_FILENO, "I would have written you a shorter letter.\n", 43);
//
// 13-5.   The command tail [ –n num ] file prints the last num lines (ten by
// default) of the named file. Implement this command using I/O system calls
// (lseek(), read(), write(), and so on). Keep in mind the buffering issues
// described in this chapter, in order to make the implementation efficient.

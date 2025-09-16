CFLAGS=-Wall -g



clean:

	rm -f bin/main 

build:

	gcc main.c -o bin/main 

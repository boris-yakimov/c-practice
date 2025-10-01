CFLAGS=-Wall -g



clean:
	rm -f bin/main 

build:
	gcc main.c -o bin/main 

run:
	gcc main.c -o main && ./main && rm -f main

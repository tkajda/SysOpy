CC = gcc -Wall -std=c11 -ggdb

main: clean
	$(CC) main.c -o main
	./main

tests:
	./test ..
	./test ../..


clean:
	rm -f test *.o *.a *.so
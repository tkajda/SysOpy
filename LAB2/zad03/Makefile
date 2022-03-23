CC = gcc -Wall -std=c11 -ggdb -D_BSD_SOURCE



main: clean
	$(CC) main.c -o test
	make tests

tests: 
	./test ..
	./test ../..


clean:
	rm -f test *.o *.a *.so

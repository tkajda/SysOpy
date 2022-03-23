CC = gcc -Wall -std=c11 -ggdb


main: clean
	$(CC) main.c -o test
	make tests


tests: clean
	echo testing
	./test test1 test2
	./test test3 test4
	./test test5 test6



clean:
	rm -f main *.o *.a *.so
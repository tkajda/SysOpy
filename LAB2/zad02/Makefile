CC = gcc -Wall -std=c11 -ggdb


main: clean
	$(CC) main.c -o test
	make tests


tests: clean
	echo testing
	./test 'c' test1
	./test 'a' test2
	./test 'b' test3
	./test 'x' test4
	./test 'a' test5
	./test 'a' test6

clean:
	rm -f main *.o *.a *.so
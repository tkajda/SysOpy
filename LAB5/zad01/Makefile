CC = gcc -Wall -std=c11 -ggdb

main: clean
	$(CC) main.c -o main
	./main commands.txt "1 | 3"
	./main commands.txt "1 | 3 | 4 | 1 | 2 | 3 | 1 | 5"


clean:
	rm -f main *.o *.a *.so
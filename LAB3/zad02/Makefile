CC = gcc -Wall -std=c11 -ggdb

main: clean
	$(CC) main.c -o main -lm
	./main 0.1 10000

clean:
	rm -f main *.o *.a *.so
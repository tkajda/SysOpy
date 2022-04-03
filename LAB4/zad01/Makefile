CC = gcc -Wall -std=c11 -ggdb

main: clean
	$(CC) main.c -o main
	./main handle fork
	./main ignore fork
	./main mask fork
	./main pending fork
	./main ignore exec
	./main mask exec
	./main pending exec


clean:
	rm -f main *.o *.a *.so

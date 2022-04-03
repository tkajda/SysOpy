CC = gcc -Wall -std=c11 -ggdb

main: clean
	$(CC) main.c -o main
	$(CC) child.c -o child
	./main handle fork
	./main ignore fork
	./main mask fork
	./main pending fork
	./main ignore exec
	./main mask exec
	./main pending exec
	make cleana


clean:
	rm -f *.o *.a *.so
cleana:
	rm  main child
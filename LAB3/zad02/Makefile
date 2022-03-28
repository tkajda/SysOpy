CC = gcc -Wall -std=c11 -ggdb

main: clean
	$(CC) main.c -o main -lm
	./main 0.1 10

tests:
	./main 0.1 5000
	./main 0.1 15000
	./main 0.1 20000
	./main 0.1 25000
	make clean



clean:
	rm -f main *.o *.a *.so *.txt

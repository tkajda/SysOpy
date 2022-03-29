CC = gcc -Wall -std=c11 -ggdb -D_DEFAULT_SOURCE

main: clean
	$(CC) main.c -o main
	make tests

tests:
	./main . "15000" 1
	make cleanmain

clean:
	rm -f *.o *.a *.so *.txt

cleanmain:
	rm main
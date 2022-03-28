CC = gcc -Wall -std=gnu99 -ggdb

main: clean
	$(CC) main.c -o main -lm
	make tests

tests:
	./main 0.0002 5000
	make clean
	./main 0.00006666666 15000
	make clean
	./main 0.00005882352 17000
	make clean
	./main 0.00005681818 17600
	make clean
	make cleanmain



clean:
	rm -f *.o *.a *.so *.txt

cleanmain:
	rm main
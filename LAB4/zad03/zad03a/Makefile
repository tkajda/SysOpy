CC = gcc -Wall -std=c11 -ggdb

main: clean
	$(CC) catcher.c -o main
	$(CC) sender.c -o sender
	./main 10 sigqueue


clean:
	rm -f main *.o *.a *.so

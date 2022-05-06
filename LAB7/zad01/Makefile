CC = gcc -Wall -std=gnu99 -ggdb


main:
	$(CC) cook.c -o cook -lm
	$(CC) deliveryman.c -o deliveryman -lm
	$(CC) shared.c -o shared -lm
	./shared

clean:
	rm deliveryman shared cook
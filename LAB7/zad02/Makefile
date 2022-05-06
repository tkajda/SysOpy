CC = gcc
flags = -Wall -lpthread -lrt

main:
	$(CC) cook.c -o cook $(flags)
	$(CC) deliveryman.c -o deliveryman $(flags)
	$(CC) shared.c -o shared $(flags)
	./shared

clean:
	rm deliveryman shared cook
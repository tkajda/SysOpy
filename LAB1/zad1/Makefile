CC=gcc

static: clean
	$(CC) -c library.c
	ar crs liblibrary.a library.o
	$(CC) main.c -o main -L. -llibrary

dynamic: clean
	$(CC) -shared -fPIC library.c -o library.so
	$(CC) main.c -o main library.so -L. -Wl,-rpath=.

clean:
	rm -f main *.o *.a *.so


shared:
	$(CC) -c -fPIC library.c
	$(CC) -shared -fPIC -o library.so library.o



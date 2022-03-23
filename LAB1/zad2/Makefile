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


zad2_static:
	make static
	$(CC) -o test main.c liblibrary.a -I .
	make tests

zad2_shared:
	make shared
	$(CC) -o test main.c -L. library.so -I .
	make tests

tests:
	./test create_table test0
	./test wc_files test4
	./test remove_block test0 test1 test2 test3 test4
	./test create_table test0 test1 test2
	./test wc_files test4 test4 test4 test4 test4 test4 test4 test4 test4 test4
	./test remove_block test0 test1 test2 test3 test4 test5 test6 test7 test8 test9



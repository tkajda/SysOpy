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

shared-O0: library.c library.h
	$(CC) -c -fPIC -O0 library.c
	$(CC) -shared -fPIC -o -O0 library.so library.o


shared-O2: library.c library.h
	$(CC) -c -fPIC -O2 library.c
	$(CC) -shared -fPIC -o -O2 library.so library.o


shared-Os: library.c library.h
	$(CC) -c -fPIC -Os library.c
	$(CC) -shared -fPIC -o -Os library.so library.o




static-tests: static main.c
	make static
	$(CC) -o test main.c liblibrary.a -I .
	echo "static-----------------------------------------"
	./test create_table test0
	./test wc_files test4
	./test remove_block test0 test1 test2 test3 test4
	./test create_table test0 test1 test2
	./test wc_files test4 test4 test4 test4 test4 test4 test4 test4 test4 test4
	./test remove_block test0 test1 test2 test3 test4 test5 test6 test7 test8 test9


shared-tests-O0: shared main.c
	make shared-O0
	$(CC) -O0 -o test main.c -L. library.so -I .
	echo "shared-O0-----------------------------------------"
	./test create_table test0
	./test wc_files test4
	./test remove_block test0 test1 test2 test3 test4
	./test create_table test0 test1 test2
	./test wc_files test4 test4 test4 test4 test4 test4 test4 test4 test4 test4
	./test remove_block test0 test1 test2 test3 test4 test5 test6 test7 test8 test9


shared-tests-O2: shared main.c
	make shared-O2
	$(CC) -O2 -o test main.c -L. library.so -I .
	echo "shared-O2-----------------------------------------"
	./test create_table test0
	./test wc_files test4
	./test remove_block test0 test1 test2 test3 test4
	./test create_table test0 test1 test2
	./test wc_files test4 test4 test4 test4 test4 test4 test4 test4 test4 test4
	./test remove_block test0 test1 test2 test3 test4 test5 test6 test7 test8 test9


shared-tests-Os: shared main.c
	make shared-Os
	$(CC) -Os -o test main.c -L. library.so -I .
	echo "shared-Os-----------------------------------------"
	./test create_table test0
	./test wc_files test4
	./test remove_block test0 test1 test2 test3 test4
	./test create_table test0 test1 test2
	./test wc_files test4 test4 test4 test4 test4 test4 test4 test4 test4 test4
	./test remove_block test0 test1 test2 test3 test4 test5 test6 test7 test8 test9


dynamic-tests: dynamic main.c
	make dynamic
	echo "dynamic-----------------------------------------"
	./test create_table test0
	./test wc_files test4
	./test remove_block test0 test1 test2 test3 test4
	./test create_table test0 test1 test2
	./test wc_files test4 test4 test4 test4 test4 test4 test4 test4 test4 test4
	./test remove_block test0 test1 test2 test3 test4 test5 test6 test7 test8 test9




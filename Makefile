CC=gcc
CFLAGS= -g -std=c99 -pedantic -Wall

check: dstring_test
	./dstring_test

clean:
	rm dstring_test *.o

dstring_test: dstring_test.o dstring.o

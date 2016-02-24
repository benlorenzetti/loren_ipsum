default:	test

test: loren_ipsum.o test.o
	gcc loren_ipsum.o test.o -o test

test.o: test.c
	gcc test.c -c

loren_ipsum.o: loren_ipsum.c loren_ipsum.h
	gcc -c loren_ipsum.c

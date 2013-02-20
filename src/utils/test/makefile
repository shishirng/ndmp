
CFLAGS = -I../  -g 
CC = gcc

all: testq testhexdump

testq: ../queue.h testq.c mtestq.c ../queue.c ../locks.c
	$(CC) -c $(CFLAGS) ../queue.c
	$(CC) -c $(CFLAGS) ../locks.c
	$(CC) -c $(CFLAGS) testq.c 
	$(CC) -c $(CFLAGS) mtestq.c 
	$(CC) -g -o  testq testq.o queue.o locks.o
	$(CC) -g -o  mtestq mtestq.o queue.o locks.o -lpthread

testhexdump: testhexdump.c ../hexdump.h ../hexdump.c
	$(CC) -c $(CFLAGS) testhexdump.c 
	$(CC) -c $(CFLAGS) ../hexdump.c 
	$(CC) -g  -o testhexdump testhexdump.o hexdump.o 

clean:
	rm *.o testq mtestq testhexdump

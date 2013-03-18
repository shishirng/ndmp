CFLAGS = -g -I  .
all: queue.o  locks.o hexdump.o

queue.o: queue.h queue.c locks.c
	gcc $(CFLAGS) -c queue.c

locks.o: locks.h locks.c
	gcc $(CFLAGS) -c locks.c

hexdump.o: hexdump.c hexdump.h
	gcc $(CFLAGS) -c  hexdump.c

clean:
	rm *.o

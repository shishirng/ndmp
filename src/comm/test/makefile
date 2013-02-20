SRC = testhexdump.c ../hexdump.c
OBJS = testhexdump.o hexdump.o
CFLAGS = -I ../ -I../../utils

all: testhexdump comm 

comm:	testcomm.o hexdump.o tcp_comm.o
	gcc -o testcomm testcomm.o tcp_comm.o hexdump.o

testcomm.o : testcomm.c ../comm.h
	gcc -c $(CFLAGS) testcomm.c

hexdump.o: ../../utils/hexdump.h ../../utils/hexdump.c
	gcc -c $(CFLAGS) ../../utils/hexdump.c

tcp_comm.o: ../comm.h ../tcp_comm.c
	gcc -c $(CFLAGS) ../tcp_comm.c

clean:
	rm *.o testhexdump testcomm

CFLAGS = -g -I . -I ../../utils/src -DDEBUG

all: comm.o worker.o

comm.o: comm.c comm.h
	gcc $(CFLAGS) -c comm.c 

worker.o: worker.h worker.c comm.h
	gcc $(CFLAGS) -c worker.c 

clean:
	rm *.o

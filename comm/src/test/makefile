# Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
# This file is part of glfs-ndmp.
# This file is licensed to you under your choice of the GNU Lesser
# General Public License, version 3 or any later version (LGPLv3 or
# later), or the GNU General Public License, version 2 (GPLv2), in all
# cases as published by the Free Software Foundation.
 
# This file and its components written by Shrinidhi
# unless otherwise specified.

SRC = testhexdump.c ../hexdump.c
OBJS = testhexdump.o hexdump.o
CFLAGS = -g -I ../ -I../../../utils/src

all: comm 

comm:	testcomm.o comm.o queue.o locks.o worker.o hexdump.o
	gcc -o testcomm -g testcomm.o comm.o  queue.o locks.o worker.o hexdump.o -lpthread

testcomm.o : testcomm.c ../comm.h
	gcc -c $(CFLAGS) testcomm.c

comm.o: ../comm.h ../comm.c
	gcc -c $(CFLAGS) ../comm.c

worker.o: ../worker.h ../worker.c 
	gcc -c $(CFLAGS) ../worker.c

hexdump.o: ../../../utils/src/queue.h ../../../utils/src/queue.c
	gcc -c $(CFLAGS) ../../../utils/src/hexdump.c

queue.o: ../../../utils/src/queue.h  ../../../utils/src/queue.c
	gcc -c $(CFLAGS) ../../../utils/src/queue.c

locks.o: ../../../utils/src/locks.h  ../../../utils/src/locks.c
	gcc -c $(CFLAGS) ../../../utils/src/locks.c

clean:
	rm *.o testcomm

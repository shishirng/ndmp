 # Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 # This file is part of glfs-ndmp.
 # This file is licensed to you under your choice of the GNU Lesser
 # General Public License, version 3 or any later version (LGPLv3 or
 # later), or the GNU General Public License, version 2 (GPLv2), in all
 # cases as published by the Free Software Foundation.

 # This file and its components written by Shrinidhi
 # unless otherwise specified. 
 

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

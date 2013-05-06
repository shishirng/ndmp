 # Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 # This file is part of glfs-ndmp.
 # This file is licensed to you under your choice of the GNU Lesser
 # General Public License, version 3 or any later version (LGPLv3 or
 # later), or the GNU General Public License, version 2 (GPLv2), in all
 # cases as published by the Free Software Foundation.

 # This file and its components written by Shrinidhi
 # unless otherwise specified. 

CFLAGS = -g -I  .
all: queue.o  locks.o hexdump.o logs.o
	(cd test; make)

queue.o: queue.h queue.c locks.c
	gcc $(CFLAGS) -c queue.c

locks.o: locks.h locks.c
	gcc $(CFLAGS) -c locks.c

hexdump.o: hexdump.c hexdump.h
	gcc $(CFLAGS) -c  hexdump.c

logs.o: logs.c logs.h
	gcc $(CFLAGS) -c  logs.c

clean:
	rm *.o
	(cd test; make clean)

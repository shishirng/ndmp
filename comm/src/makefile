# Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
# This file is part of glfs-ndmp.
# This file is licensed to you under your choice of the GNU Lesser
# General Public License, version 3 or any later version (LGPLv3 or
# later), or the GNU General Public License, version 2 (GPLv2), in all
# cases as published by the Free Software Foundation.
 
# This file and its components written by Shrinidhi
# unless otherwise specified.

CFLAGS = -g -I . -I ../../utils/src -DDEBUG

all: comm.o worker.o
	(cd test; make);

comm.o: comm.c comm.h
	gcc $(CFLAGS) -c comm.c 

worker.o: worker.h worker.c comm.h
	gcc $(CFLAGS) -c worker.c 

clean:
	rm *.o
	(cd test; make clean);

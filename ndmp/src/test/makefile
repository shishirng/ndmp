 # Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 # This file is part of glfs-ndmp.
 # This file is licensed to you under your choice of the GNU Lesser
 # General Public License, version 3 or any later version (LGPLv3 or
 # later), or the GNU General Public License, version 2 (GPLv2), in all
 # cases as published by the Free Software Foundation.
 
 # This file and its components written by Shrinidhi
 # unless specified otherwise.
 

ROOT = ../../..
SRC = testndmp.c 
CFLAGS = -g -I $(ROOT)/comm/src -I $(ROOT)/ndmp/src -I $(ROOT)/utils/src -DDEBUG
LIBS = -lpthread
OBJS = $(ROOT)/ndmp/src/ndmp_connect.o \
	$(ROOT)/ndmp/src/ndmp_data.o \
	$(ROOT)/ndmp/src/ndmp_msg.o \
	$(ROOT)/ndmp/src/ndmp_config.o \
	$(ROOT)/ndmp/src/ndmp_xdr.o \
	$(ROOT)/comm/src/comm.o \
	$(ROOT)/comm/src/worker.o \
	$(ROOT)/utils/src/locks.o \
	$(ROOT)/utils/src/hexdump.o \
	$(ROOT)/utils/src/queue.o


testndmp: 
	gcc $(CFLAGS) -c testndmp.c 
	gcc $(CFLAGS) -o testndmp testndmp.o $(OBJS) $(LIBS)

clean:
	rm  *.o testndmp
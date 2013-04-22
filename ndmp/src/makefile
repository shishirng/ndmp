 # Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 # This file is part of glfs-ndmp.
 # This file is licensed to you under your choice of the GNU Lesser
 # General Public License, version 3 or any later version (LGPLv3 or
 # later), or the GNU General Public License, version 2 (GPLv2), in all
 # cases as published by the Free Software Foundation.
 
 # This file and its components written by Shrinidhi
 # unless specified otherwise.
 
ROOT=../..
COM=$(ROOT)/comm
UTILS=$(ROOT)/utils
CFLAGS = -g -I. -I$(COM)/src -I$(UTILS)/src -DDEBUG
HEADERS = ndmp.h ndmp_msg.h

all: ndmp_xdr.o ndmp_config.o ndmp_connect.o ndmp_data.o ndmp_msg.o 
	(cd test; make)

ndmp.h: ndmp.x
	rpcgen ndmp.x

ndmp_xdr.o: ndmp.h
	gcc $(CFLAGS) -c ndmp_xdr.c

ndmp_config.o: ndmp_config.c $(HEADERS)
	gcc $(CFLAGS) -c ndmp_config.c 

ndmp_connect.o: ndmp_connect.c $(HEADERS)
	gcc $(CFLAGS) -c ndmp_connect.c 
	
ndmp_data.o: ndmp_data.c $(HEADERS)
	gcc $(CFLAGS) -c ndmp_data.c

ndmp_msg.o: ndmp_msg.c $(HEADERS)
	gcc $(CFLAGS) -c ndmp_msg.c 

clean:
	rm *.o ndmp.h ndmp_xdr.c
	(cd test; make clean)

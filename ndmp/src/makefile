ROOT=../..
COM=$(ROOT)/comm
UTILS=$(ROOT)/utils
CFLAGS = -g -I. -I$(COM)/src -I$(UTILS)/src -DDEBUG
HEADERS = ndmp.h ndmp_msg.h

all: ndmp_xdr.o ndmp_config.o ndmp_connect.o ndmp_msg.o 

ndmp.h: ndmp.x
	rpcgen ndmp.x

ndmp_xdr.o: ndmp.h
	gcc $(CFLAGS) -c ndmp_xdr.c

ndmp_config.o: ndmp_config.c $(HEADERS)
	gcc $(CFLAGS) -c ndmp_config.c 

ndmp_connect.o: ndmp_connect.c $(HEADERS)
	gcc $(CFLAGS) -c ndmp_connect.c 

ndmp_msg.o: ndmp_msg.c $(HEADERS)
	gcc $(CFLAGS) -c ndmp_msg.c 

clean:
	rm *.o ndmp.h ndmp_xdr.c

/*
 * 
 *  Copyright place holder
 *
*/

#ifndef __H__COMM__
#define __H__COMM__

#define LISTEN_PORT 10000
#define NUM_SESSIONS 512
#define MAX_MESSAGE_SIZE 1024

#include <queue.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>


struct client_endpoint {
	struct sockaddr_in client;	
	int fd;
};

struct comm_message {
	int message_id;
	char message[MAX_MESSAGE_SIZE];
	int length;
	int whence;
};

struct session {
	int session_id;
	struct client_endpoint client_info;
};

struct client_txn {
	struct session client_session;
	struct comm_message request;
	struct comm_message response;
};

typedef void (*message_handler) (struct client_txn *);
struct comm_context {
	message_handler marshal_unmarshal;
	struct queue_hdr *sessions;
	struct queue_hdr *client_jobs;
	int maxfds;
};
	
struct comm_context* comm_context(); /* creates/returns comm_context struct */
void comm_listen(struct comm_context *); /* listens for messages */
#endif
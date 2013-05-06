/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

/*
 * This file and its components written by Shrinidhi
 * unless otherwise specified.
 */

#ifndef __H__COMM__
#define __H__COMM__

#define LISTEN_PORT 10000
#define NUM_SESSIONS 512
#define MAX_MESSAGE_SIZE 2048

#include <queue.h>
#include <logs.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>

struct client_endpoint {
        struct sockaddr_in client;
        int fd;
};

struct comm_message {
        int is_tcp_connect;
        char message[MAX_MESSAGE_SIZE];
        int length;
};

struct session {
        int session_id;
        struct client_endpoint client_info;
};

struct client_txn {
        struct session client_session;
        struct comm_message request;
        struct comm_message reply;
};

typedef void (*message_handler) (struct client_txn *);
typedef int (*session_callback)(struct client_txn  *); 
typedef void (*session_close)(int session_id);

struct comm_context {
        message_handler marshal_unmarshal;
        session_callback cleanup_session;
        session_close terminate_session;
        struct queue_hdr *sessions;
        struct queue_hdr *request_jobs;
        struct queue_hdr *reply_jobs;
        int maxfds;
};
        
struct comm_context* comm_context(); /* creates/returns comm_context struct */
void comm_listen(struct comm_context *); /* listens for messages */
#endif

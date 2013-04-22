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

#include <comm.h>
#include <worker.h>

/*
 *  comm_context:
 *  Creates a comm_context structure
 *  which is the anchor structure that
 *  holds the session structures for all
 *  active sessions
 */

int session_comparator(void *target, void *elem);
void handle_client_requests(fd_set *read_socks, struct comm_context *ctx);
void handle_a_client_request(int fd, struct comm_context *ctx);

struct comm_context* comm_context()
{
        static struct comm_context *retval = NULL;
        if (retval == NULL ) {
                retval = calloc(1, sizeof(struct comm_context));
                retval->sessions = init_queue();
                retval->request_jobs = init_queue();
                retval->reply_jobs = init_queue();
        }
        return retval;
}

/*
 *  create_listener
 *    creates a listener socket to listen on specified port and 
 *    returns the socket to the caller
 *   
 *   NOTE: THIS CREATES A IPV4 LISTENER
 *         FOR IPV6, a similar function 
 *         can be written for the caller
 * 
 *    
 */

int create_listener(int port) {

        int retval; /* return the socket the listener should listen on */
        struct sockaddr_in listener;
        int reuse = 1;

        listener.sin_family = AF_INET;
        listener.sin_addr.s_addr = INADDR_ANY;
        listener.sin_port = htons(port);

        /* now create a socket and bind to listener */
        retval = socket(AF_INET, SOCK_STREAM, 0);

        setsockopt(retval, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
       // fcntl(retval, F_SETFL, O_NONBLOCK);
        if (bind(retval, (struct sockaddr *) &listener,
                        sizeof(struct sockaddr_in)) == -1)
                errExit("Error:bind");

        return retval;

}

struct client_endpoint accept_connection(int listener)
{
        struct sockaddr_in client;
        socklen_t len;
        int sockfd;
        struct client_endpoint retval;

        retval.fd = -1;
        printf("Accepting new connection ..\n");
        sockfd = accept(listener, (struct sockaddr *) &client, &len);
        if (sockfd < 0) {
                printf("Oops! Cannot accept new connection :( \n");
                return retval;
        }

        fcntl(sockfd, F_SETFL, O_NONBLOCK);
        retval.client = client;
        retval.fd = sockfd;

        printf("Accepted new connection ..\n");
        return retval;
}

/*
 * set_fd_flags sets all the sockets
 * that we want to multiplex on fd_set
 */

void set_fd_flags(fd_set *fds, int listener, struct comm_context *ctx)
{
        struct session **active_sessions;
        int n, i;
        FD_ZERO(fds);
        FD_SET(listener, fds);

        active_sessions = (struct session **) get_all_elems(ctx->sessions);
        n = num_elems(ctx->sessions);
        for (i = 0; i < n; i++) {
                FD_SET(active_sessions[i]->session_id, fds);
        }
}

/*
 * comm_listen:
 * comm_listen listens on a socket at the address
 * specified by LISTEN_PORT
 * 
 * We separate creation of listener from listening
 * so that support for IPV6 can be provided by
 * creating a IPV6 listener and rest of logic should,
 * hopefully, be invariant.
 * 
 * When a connect request is received, a session is
 * created and the session is added to the list of 
 * active sessions.
 *
 */

void comm_listen(struct comm_context *ctx)
{
        fd_set read_socks;
        int listener = create_listener(LISTEN_PORT);
        struct client_endpoint endpoint;
        struct client_endpoint *temp;
        int ready;
        int new_connections;

        FD_ZERO(&read_socks);

        listener = create_listener(LISTEN_PORT);
        listen(listener, NUM_SESSIONS);

        printf("Listening on port %d....\n", LISTEN_PORT);
        /*
         * add listening socket to read_socks
         * for multiplexing listening as well
         */
        FD_SET(listener, &read_socks);
        ctx->maxfds = listener;

        /* Create Worker Threads */
        create_worker_threads(THREAD_POOL_SIZE);
        /* when the listener wakes up create a session */

        for (;;) {
                printf("Wait on select \n");
                set_fd_flags(&read_socks, listener, ctx);
                ready = select((ctx->maxfds) + 1, &read_socks, NULL, NULL,
                                NULL );
                printf("Woke up from  select \n");
                if (ready != -1) {
                        if (FD_ISSET(listener, &read_socks)) {
                                new_connections = accept_all_connections(
                                        listener, &read_socks, ctx);
                                printf("%d new connection(s) \n",
                                                new_connections);
                                } else {
                                /*
                                 * Some client session is active
                                 */
                                handle_client_requests(&read_socks, ctx);
                                }
                }

        }
}

int accept_all_connections(int listener, fd_set *read_socks,
                struct comm_context *ctx)
{
        struct client_endpoint endpoint;
        struct session* new_session;
        struct client_txn* txn;

        int num_new_sessions = 0;

        //      do {
        endpoint = accept_connection(listener);
        if (endpoint.fd != -1) {
                /*
                 * Add new connection to session queue
                 * in comm_context
                 */

                new_session = (struct session*) malloc(sizeof(struct session));
                new_session->session_id = endpoint.fd;
                new_session->client_info = endpoint;
                enqueue(ctx->sessions, new_session);
                /*
                 * add to select list of sockets
                 */
                FD_SET(endpoint.fd, read_socks);
                if (endpoint.fd > ctx->maxfds) {
                        ctx->maxfds = endpoint.fd;
                }

                printf("Connection request from %s at %d\n",
                                inet_ntoa(endpoint.client.sin_addr),
                                ntohs(endpoint.client.sin_port));

                txn = (struct client_txn *) malloc(sizeof(struct client_txn));

                /*
                 * Create a pseudo NDMP request for the tcp_connect request
                 * so that a NDMP_NOTIFY_CONNECTED request can be sent to client
                 */

                txn->client_session = *new_session;
                txn->request.is_tcp_connect = 1;
                enqueue(ctx->request_jobs, txn);
                num_new_sessions++;
        }
        //      } while (endpoint.fd != -1);

        return num_new_sessions;
}

void handle_client_requests(fd_set *read_socks, struct comm_context *ctx)
{
        int i;
        for (i = 0; i <= ctx->maxfds; i++) {
                printf("Checking for event on sock %d\n", i);
                if (FD_ISSET(i,read_socks)) {
                        handle_a_client_request(i, ctx);
                }
        }
}

/*
 * create a new transaction for the new request received
 */

void handle_a_client_request(int fd, struct comm_context *ctx)
{
        struct session tmp;
        tmp.session_id = fd;
        struct client_txn* txn;
        int n;

        txn = (struct client_txn *) malloc(sizeof(struct client_txn));
        tmp = *(struct session *) get_elem(ctx->sessions, &tmp,
                session_comparator);
        txn->client_session = tmp;
        txn->request.length = recv(fd, txn->request.message, MAX_MESSAGE_SIZE,
                        0);
        txn->request.is_tcp_connect = 0;
        if (txn->request.length == 0) {
                /*
                 * Event on this socket indicated socket closure
                 */
                close(fd);
                remove_elem(ctx->sessions, &tmp, session_comparator);
                ctx->terminate_session(fd);/* mark this session as terminated */
                free(txn);

                printf("Detected socket closure\n");
                return;
        }
        enqueue(ctx->request_jobs, txn);
        printf("\tcreated a new job on %d [%d bytes]\n", fd,
                        txn->request.length);
}

int session_comparator(void *target, void *elem)
{
        int target_id;
        int elem_id;

        target_id = ((struct session *) target)->session_id;
        elem_id = ((struct session *) elem)->session_id;

        return elem_id == target_id;
}

int errExit(char *s)
{
        fprintf(stderr, "%s\n", s);
        exit(-1);
}

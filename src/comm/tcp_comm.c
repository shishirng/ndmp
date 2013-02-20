/*
 * Copyright place holder
*/

#include "comm.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

struct client_info {
	struct sockaddr_in client;	
	int fd;
};

/*
 *  comm_int: 
 *  Creates a comm_context structure
 *  which is the anchor structure that
 *  holds the session structures for all
 *  active sessions
 */


struct comm_context* comm_init() {
	static struct comm_context *retval = NULL;
	if (retval == NULL)
		retval = calloc(1,sizeof(struct comm_context));
	return retval;
}


/*
 *
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

	setsockopt(retval, SOL_SOCKET,SO_REUSEADDR, &reuse, sizeof(int));
	if (bind(retval, (struct sockaddr *)&listener, 
                         sizeof(struct sockaddr_in)) == -1)
		errExit("Error:bind");
	
	return retval;

}

struct client_info accept_connection(int listener) {

	struct sockaddr_in client;
	socklen_t len;
	int sockfd;
        struct client_info retval;	

	sockfd = accept(listener, (struct sockaddr *)&client,
			&len);
	if (sockfd< 0)
		errExit("Error:accept");


	retval.client = client;
	retval.fd = sockfd;

	return retval;
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

void comm_listen(struct comm_context *ctx) {

	struct session *a_session;
	int listener = create_listener(LISTEN_PORT);
	
	struct client_info endpoint;
	struct client_info *temp;

	listen(listener, NUM_SESSIONS);

	printf ("Listening on port %d....\n", LISTEN_PORT);

	/* when the listener wakes up create a session */

	for (;;) {
		endpoint = accept_connection(listener); 
	
		printf("Got a connection request from %s at %d \n", 
			inet_ntoa(endpoint.client.sin_addr),
			ntohs(endpoint.client.sin_port));
		
		if (ctx->marshal_unmarshal != NULL) {
			a_session = calloc(1,sizeof(struct session));	
			temp = calloc(1,sizeof(struct client_info));
		        *temp = endpoint;
			a_session->client_info = temp; 
			ctx->marshal_unmarshal(a_session);
		}
	}
       	
}


int write_message(struct session *a_session) {}
int read_message(struct session *a_session) {

	char msg[512];

	int n;

	struct client_info endpoint = *((struct client_info *)
					a_session->client_info);
	n = read(endpoint.fd, msg,512);
	hexdump(msg,n);
}


int errExit(char *s) {

	fprintf(stderr,"%s\n", s);
	exit(-1);
}


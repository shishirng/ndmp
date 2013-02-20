/*
 * 
 *  Copyright place holder
 *
*/

#ifndef __H__COMM__
#define __H__COMM__

#define LISTEN_PORT 10000
#define NUM_SESSIONS 512

struct comm_message {
	char *message;
	int length;
};

struct session {
	void *client_info; /* client end point */ 
	struct comm_message read_message; /* inbound message */
	struct comm_message write_message; /* outbound message */
};

typedef void (*message_handler) (struct session *);
struct comm_context {
	message_handler marshal_unmarshal;
	struct session* sessions;	
};
	
struct comm_context* comm_init(); /* creates a comm_context structure */
void comm_listen(struct comm_context *); /* listens for messages */
int write_message(struct session *);
int read_message(struct session *);

#endif

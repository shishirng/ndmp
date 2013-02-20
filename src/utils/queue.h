#ifndef __H__QUEUE__
#define __H__QUEUE__
/*
 * The queue implementation is thread-safe FIFO 
 * queue. The user is responsbile for managing
 * the space allocated for queue element
 *
 */

struct queue_node {
	void *elem;
	struct queue_node *next;
}; 

struct queue_hdr {
	struct queue_node  *first;
	struct queue_node  *last;
	int num_elems;
};

struct queue_hdr* init_queue();
void enqueue(struct queue_hdr* hdr, void *elem);
void *dequeue(struct queue_hdr *hdr);

int num_elems(struct queue_hdr *hdr);
#endif

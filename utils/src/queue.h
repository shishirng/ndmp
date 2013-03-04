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

typedef int (*comparator) (void *target, void *elem); /* returns 1 if target matches
                                                * elem
						*/
struct queue_hdr* init_queue();
void enqueue(struct queue_hdr* hdr, void *elem);
void *dequeue(struct queue_hdr *hdr);

void* get_elem(struct queue_hdr *hdr, void* target, comparator cmp);
void remove_elem(struct queue_hdr *hdr, void* target, comparator cmp);

void** get_all_elems(struct queue_hdr *hdr);

int num_elems(struct queue_hdr *hdr);
#endif

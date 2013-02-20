#include <queue.h>
#include <locks.h>
#include <stdlib.h>

static struct lock *q_lock = NULL;
void enter_critical_section(struct lock* q_lock);
void exit_critical_section(struct lock *q_lock);

struct queue_hdr* init_queue() 
{

	if (q_lock == NULL)
		q_lock = get_lock();
	return (struct queue_hdr *) calloc (1,sizeof(struct queue_hdr));
}

void enqueue(struct queue_hdr* hdr, void *elem) 
{

	struct queue_node *node = (struct queue_node *)
                                   malloc(sizeof(struct queue_node));
	node->elem = elem;
	node->next = NULL;

	enter_critical_section(q_lock); 
	/*
	 * Queue can either be emoty or non-empty
         */

	if (hdr->first == NULL) {  // empty queue
	   hdr->first = node;
	   hdr->last  = node; 
	}
	else {                    // non-empty queue
		hdr->last->next = node;
		hdr->last = node;
	}
	hdr->num_elems++;
	exit_critical_section(q_lock);
}

void *dequeue(struct queue_hdr *hdr) 
{

	void* retval;
        struct queue_node *node;
	
        enter_critical_section(q_lock); 
	if (hdr->first == NULL) { 
	    exit_critical_section(q_lock); 
	    return NULL;
	}
	node = hdr->first;
	hdr->first = node->next; // dequeue elem

	retval = node->elem;
	free(node); // free what we created in enqueue
	hdr->num_elems--;
	exit_critical_section(q_lock);

	return retval;
}

int num_elems(struct queue_hdr *hdr) 
{

	int retval = 0;

	enter_critical_section(q_lock);
	retval = hdr->num_elems;
	exit_critical_section(q_lock);
	
	return retval;
}

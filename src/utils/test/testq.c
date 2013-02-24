#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <queue.h>


void set_up_job_queue(struct queue_hdr* queue, int num_jobs);
void check_job_queue(struct queue_hdr* queue, int num_jobs);
int queue_cmp(void *target, void *elem);
void check_get_from_queue(struct queue_hdr *queue, int n);
void check_remove_from_queue(struct queue_hdr *queue, int n);


int main() 
{

	struct queue_hdr *job_queue = init_queue();
	set_up_job_queue(job_queue,100);	
	check_get_from_queue(job_queue,100);
	printf ("Single-threaded 'get' test passed\n");
	check_job_queue(job_queue,100);
	printf ("Single-threaded 'dequeue' test passed\n");
	check_remove_from_queue(job_queue, 100);	
	printf("Single-threaded queue test passed\n");
	
}

void add_a_job(struct queue_hdr *job_queue, int i) 
{
	int *jobid = (int *) malloc(sizeof(int));
	*jobid = i;
	enqueue(job_queue, jobid);
}

int *get_job_from_queue(struct queue_hdr *job_queue)
{ 

	return dequeue(job_queue);
}

void set_up_job_queue(struct queue_hdr* queue, int n)
{
	int i;
	for (i=0; i<n; i++)
		add_a_job(queue, i);
}

void check_job_queue(struct queue_hdr *queue, int n) {

	int i, *j;
	for (i=0; i<n; i++) {
		j = dequeue(queue);
		assert( i == *j);
		free(j);	
	}
	assert(num_elems(queue) == 0);	
}

int queue_cmp(void *target, void *elem)
{
	return (*((int *)target) == *((int *)elem));
}  

void check_get_from_queue(struct queue_hdr *queue, int n)
{
	int i;	
	for (i=0; i<n; i++)
		assert(i == *(int *)get(queue,&i,queue_cmp));
}

void check_remove_from_queue(struct queue_hdr *queue, int n)
{
	int i;
	set_up_job_queue(queue,n);	
	for (i=0; i<n; i++) {
		remove_elem(queue,&i,queue_cmp);
		assert(get(queue,&i,queue_cmp) == NULL);
	}
}
	

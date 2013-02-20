#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <queue.h>


void set_up_job_queue(struct queue_hdr* queue, int num_jobs);
void check_job_queue(struct queue_hdr* queue, int num_jobs);

int main() 
{

	
	struct queue_hdr *job_queue = init_queue();
	set_up_job_queue(job_queue,100);
	check_job_queue(job_queue,100);
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
	printf("Single-threaded queue test passed \n");
		
}

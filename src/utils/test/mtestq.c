#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <queue.h>
#include <locks.h>

void set_up_job_queue(struct queue_hdr* queue, int num_jobs);
int check_job_queue(struct queue_hdr* queue, int num_jobs, int num_threads);
void *get_jobs(void *queue) ;

static struct lock *num_lock;
int queue_size;

int main() 
{

	
	struct queue_hdr *job_queue = init_queue();
	set_up_job_queue(job_queue,100);
	check_job_queue(job_queue,100,10);
}

void add_a_job(struct queue_hdr *job_queue, int i)
{
	int *jobid = (int *) malloc(sizeof(int));
	*jobid = i;
	enqueue(job_queue, jobid);
}

int *get_job_from_queue(struct queue_hdr *job_queue) {

	return dequeue(job_queue);
}

void set_up_job_queue(struct queue_hdr* queue, int n) {

	int i;
	for (i=0; i<n; i++)
		add_a_job(queue, i);
}

int check_job_queue(struct queue_hdr *queue, int num_jobs, int num_threads) {

	int i;
	queue_size = 0;
	pthread_t *threads = (pthread_t *)malloc(num_threads*sizeof(pthread_t));

	num_lock = get_lock();

	for(i=0; i<num_threads; i++) {
	   pthread_create(&threads[i], NULL, get_jobs, (void *)queue);
	}

	for(i=0; i<num_threads; i++) {
	   pthread_join(threads[i], NULL);
	}

	assert(queue_size == num_jobs);

	printf ("Multi-threaded queue test passed\n");

	
}


void *get_jobs(void *queue) {
	struct queue_hdr *job_queue = (struct queue_hdr *) queue;
	while (dequeue(job_queue) != NULL) {
		enter_critical_section(num_lock);
		queue_size++;
		exit_critical_section(num_lock);
	}
	pthread_exit(NULL);
}

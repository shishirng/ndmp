/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "queue.h"
#include <locks.h>

static struct lock *num_lock;
int queue_size;

struct thread_args {
	struct queue_hdr *queue;
	int num_jobs;
};

void set_up_job_queue(struct queue_hdr* queue, int num_jobs);
void check_job_queue(struct queue_hdr* queue, int num_jobs, int num_threads);
void *get_jobs(void *queue);
int queue_cmp(void *target, void *elem);
void *get_and_remove_jobs(void *args);



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
	for (i = 0; i < n; i++)
		add_a_job(queue, i);
}

void check_job_queue(struct queue_hdr *queue, int num_jobs, int num_threads)
{
	int i;
	queue_size = 0;
	pthread_t *threads = (pthread_t *) malloc(
			num_threads * sizeof(pthread_t));

	num_lock = get_lock();

	for (i = 0; i < num_threads; i++) {
		pthread_create(&threads[i], NULL, get_jobs, (void *) queue);
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL );
	}

	assert(queue_size == num_jobs);

}

void check_job_removal(struct queue_hdr *queue, int num_jobs, int num_threads)
{
	int i;

	struct thread_args args;
	args.queue = queue;
	args.num_jobs = num_jobs;
	pthread_t *threads = (pthread_t *) malloc(
			num_threads * sizeof(pthread_t));

	assert(num_elems(queue) == num_jobs);
	for (i = 0; i < num_threads; i++) {
		pthread_create(&threads[i], NULL, get_and_remove_jobs,
				(void *) &args);
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}
	assert(num_elems(queue) == 0);
}

void *get_jobs(void *queue) 
{
	struct queue_hdr *job_queue = (struct queue_hdr *) queue;
	while (dequeue(job_queue) != NULL) {
		enter_critical_section(num_lock);
		queue_size++;
		exit_critical_section(num_lock);
	}
	pthread_exit(NULL);
}

void *get_and_remove_jobs(void *args)
{
	struct queue_hdr *job_queue = ((struct thread_args *) args)->queue;
	int i, n;

	n = ((struct thread_args *) args)->num_jobs;

	for (i = 0; i < n; i++) {
		if (get_elem(job_queue, &i, queue_cmp) != NULL) {
			remove_elem(job_queue, &i, queue_cmp);
			assert(get_elem(job_queue,&i,queue_cmp) == NULL);
		}
	}
	return job_queue;
}

int queue_cmp(void *target, void *elem)
{
	return (*((int *) target) == *((int *) elem));
}

int main() 
{
	struct queue_hdr *job_queue = init_queue();
	set_up_job_queue(job_queue, 100);
	check_job_queue(job_queue, 100, 10);
	printf("Multi-threaded enqueue-dequeue test passed\n");
	set_up_job_queue(job_queue, 100);
	check_job_removal(job_queue, 100, 10);
	printf("Multi-threaded check_job_removal test passed\n");
}

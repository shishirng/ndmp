/*
 * Copyright place holder
 */

#include <worker.h>
#include <pthread.h>

static pthread_t* threads = NULL;
static int num_threads;
void* run_job(void *context);

void create_worker_threads(int thread_pool_size) 
{
	int i;
	if (threads == NULL) {
		threads = (pthread_t *)malloc(thread_pool_size*
					sizeof(pthread_t));
		for (i=0; i<thread_pool_size; i++) {
			pthread_create(&threads[i], NULL, run_job, 
				(void *)comm_context());
		}
		num_threads = thread_pool_size;
	}
}

void shutdown_worker_threads() 
{

	/*
	 * TBD: As part of shutdown, threads need to be
	 *      gracefully terminated. 
	 */

}

void* run_job(void *context) 
{

	struct comm_context *ctx = (struct comm_context *)context;
	struct client_txn *job;
	while (1) {

		/*
		 * Get the next job in queue and make
		 * an upcall on XDR service.
		 */
		job = (struct client_txn *)dequeue(ctx->client_jobs);
	 	
		if (job != NULL) {
		
			/*
		 	* Make the up call
		 	*/
		
			// upcall

			/*
		 	* Add response to response queue
		 	*/	
		
			// add response to response queue
		

			// Process next job
		}
		else {
			printf("Job Queue Empty: Will wait for job\n");
		}
	}
}

void * process_response(struct comm_context *context)
{


}

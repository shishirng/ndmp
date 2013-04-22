/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

/*
 * This file and its components written by Shrinidhi
 * unless otherwise specified.
 */

#include <worker.h>
#include <pthread.h>

static pthread_t* threads = NULL;
static int num_threads;
void* run_job(void *context);
void* process_response(void *context);
extern int session_comparator(void *target, void *elem);

void create_worker_threads(int thread_pool_size) 
{
        int i;
        if (threads == NULL) {
                threads = (pthread_t *)malloc((thread_pool_size+1)*
                                              sizeof(pthread_t));
                for (i=0; i<thread_pool_size; i++) {
                        pthread_create(&threads[i], NULL, run_job, 
                                       (void *)comm_context());
                }
                /* Now create one thread to process responses */
                pthread_create(&threads[thread_pool_size], NULL, 
                               process_response, (void *)comm_context());
                num_threads = thread_pool_size;
        }
        printf("Created %d worker threads \n", thread_pool_size);
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
                job = (struct client_txn *)dequeue(ctx->request_jobs);
                
                if (job != NULL) {
                
                        /*
                         * Make the up call
                         */
                
#ifdef DEBUG

                        if (job->request.is_tcp_connect != 1) {

                                printf("Request message: \n");

                                hexdump(job->request.message, job->request.length);                     
                        }
#endif
                        ctx->marshal_unmarshal(job);
                        printf("Processed a job \n");
                        // Process next job
                }
                else {
//                      printf("Job Queue Empty: Will wait for job\n");
                        pthread_yield(); // Let another thread run
                }
        }
        return ctx;
}

void *process_response(void *context)
{
        int write_result;       
        struct comm_context *ctx = (struct comm_context *)context;
        struct client_txn *job;
        struct session client_session;
        while (1) {
                
                /*
                 * Process the next job in response queue and send
                 * response to client
                 */
                job = (struct client_txn *)dequeue(ctx->reply_jobs);
                
                if (job != NULL) {
                        // Write message to client                      
                        client_session = job->client_session;
                        if (get_elem(ctx->sessions, &client_session,
                                     session_comparator) != NULL) {
                                /* session still valid */
                                write_result = write_message(job);
                                if (write_result == -1) 
                                        printf("Write message error\n");
                        }
                        free(job);
                }
                else 
                        pthread_yield();
                                                
        }
        return ctx;     
}

int write_message(struct client_txn *job)
{
        struct session tmp;
        struct comm_message reply;
        int fd, sent_bytes;
        int len;
        tmp = job->client_session;
        reply = job->reply;
        fd = tmp.session_id;
        if (reply.length > 0) {
#ifdef DEBUG
                printf("Reply message:\n");
                hexdump(reply.message,reply.length);
#endif
                sent_bytes = send(fd,reply.message,reply.length,0);
                
                printf("Sent a %d byte response to (%s,%d) on sock %d\n",
                       sent_bytes, inet_ntoa(tmp.client_info.client.sin_addr),
                       ntohs(tmp.client_info.client.sin_port), fd);
        }
        return -1 * (sent_bytes != reply.length);
}

/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

#include <ndmp_msg.h>
#include <ndmp.h>
#include <time.h>
#include <queue.h>

int session_info_cmp (void *id, void *session);
int job_cmp(void *target, void*elem);
void add_job_to_session(struct client_txn *txn);
struct queue_hdr* get_session_queue() ;

/*
 * xdr_decode_encode is the entry point
 * and callback method for the comm layer 
 * This is the first upcall made by a worker
 * thread. 
 *
 */

void xdr_decode_encode(struct client_txn *txn)
{
        /*
         *  1. Decode the message type from txn->request
         *  2. Depending on the message type call the appropriate
         *     NDMP message handler. When the handler returns,
         *     txn->response will hold the marshaled (encoded) NDMP response
         *  3. Enqueue the response into the response queue
         */

        XDR request_stream, stream_len;
        int message_type;
        struct ndmp_header header;
        char *buf, mesg_len[4];
        int len;
        struct comm_context *ctx = comm_context();
        write_client_log(txn->client_session.client_info.client, "Received request from client");

        if (txn->request.is_tcp_connect == 1) {
                header.message = 0xf001;
        }
        else {
                xdrmem_create(&request_stream, txn->request.message+4, txn->request.length-4,
                        XDR_DECODE);
                /*
                 * Read the ndmp header
                 */

                xdr_ndmp_header(&request_stream, &header);
                write_client_log(txn->client_session.client_info.client, "Decoded NDMP header from client request");

        }
        /*
         * Add job to session's job queue
         */

        add_job_to_session(txn);
        write_client_log(txn->client_session.client_info.client, "Dispatched client request to appropriate message handler");
        ndmp_dispatch(header.message)(txn, header, &request_stream);

        if (cleanup_session(txn) == 1 || txn->reply.length == 0) {
                free(txn); /* client terminated. Don't send response */
        }
        else {
                /* 
                 * It is possible that at this point in time
                 * or later before the response is sent the client
                 * terminates. comm layer needs to check again for
                 * client termination, before sending the response
                 */

                buf = (char *)malloc(txn->reply.length+4);
                memcpy(buf+4,txn->reply.message,txn->reply.length);
                len = txn->reply.length | (1<<31);
                xdrmem_create(&stream_len,mesg_len,4, XDR_ENCODE);
                xdr_int(&stream_len, &len);
                memcpy(buf,mesg_len,4);
                memcpy(txn->reply.message, buf, txn->reply.length+4);
                txn->reply.length +=4;
                enqueue(ctx->reply_jobs, txn);
                free(buf);
        }

}

ndmp_message_handler ndmp_dispatch(int message) {

        switch (message) {
        case 0xf001:
                return ndmp_accept_notify;
        case 0x900:
                return ndmp_connect_open;
        case 0x902:
                return ndmp_connect_close;
        case 0x100:
                return ndmp_config_get_host_info;
        case 0x102:
                return ndmp_config_get_connection_type;
        case 0x103:
                return ndmp_config_get_auth_attr;
        case 0x104:
                return ndmp_config_get_butype_info;
        case 0x105:
                return ndmp_config_get_fs_info;
        case 0x106:
                return ndmp_config_get_tape_info;
        case 0x107:
                return ndmp_config_get_scsi_info;
        case 0x108:
                return ndmp_config_get_server_info;
        case 0x400:
        	return ndmp_data_get_state;
        case 0x401:
        	return ndmp_data_start_backup;
        case 0x402:
        	return ndmp_data_start_recover;
        case 0x403:
        	return ndmp_data_abort;
        case 0x404:
        	return ndmp_data_get_env;
        case 0x407:
        	return ndmp_data_stop;
        case 0x409:
        	return ndmp_data_listen;
        case 0x40A:
        	return ndmp_data_connect;
        default:
                return ndmp_error_message;
        }
}

void ndmp_error_message(struct client_txn *txn, 
                        struct ndmp_header header, 
                        XDR* requeststream)
{

        struct ndmp_header reply_header;
        XDR reply_stream;
        set_header(header, &reply_header, NDMP_NOT_SUPPORTED_ERR);
        txn->reply.length = xdr_sizeof((xdrproc_t) 
                                       xdr_ndmp_header,
                                       &reply_header);
        xdrmem_create(&reply_stream, txn->reply.message, txn->reply.length,
                      XDR_ENCODE);
        xdr_ndmp_header(&reply_stream, &reply_header);
}

void ndmp_accept_notify(struct client_txn* txn, struct ndmp_header header, XDR* request_stream) 
{
        struct ndmp_notify_connected_request reply;
        struct ndmp_header reply_header;
        XDR reply_stream;

        reply.reason = NDMP_CONNECTED;
        reply.protocol_version = 3;
        reply.text_reason = "";
        
        reply_header.sequence = get_next_seq_number();
        reply_header.time_stamp = (u_long) time(NULL);
        reply_header.message_type = NDMP_MESSAGE_REQUEST;
        reply_header.message = NDMP_NOTIFY_CONNECTED;
        reply_header.reply_sequence = 0;
        reply_header.error = NDMP_NO_ERR;

        //set_header(header, &reply_header, NDMP_NO_ERR);
        txn->reply.length = xdr_sizeof((xdrproc_t) 
                                       xdr_ndmp_header,
                                       &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t) 
                                        xdr_ndmp_notify_connected_request,
                                        &reply);
        
        xdrmem_create(&reply_stream, txn->reply.message, txn->reply.length,
                      XDR_ENCODE);

        xdr_ndmp_header(&reply_stream, &reply_header);
        xdr_ndmp_notify_connected_request(&reply_stream, &reply);
        write_client_log(txn->client_session.client_info.client, "Sent CONNECT_NOTIFY to client");

}


int get_next_seq_number() {

        static int sequence_number = 1;

        return sequence_number++;
}

void set_header(ndmp_header req, ndmp_header *reply, ndmp_error err)
{

        reply->sequence = get_next_seq_number();
        reply->time_stamp = (u_long) time(NULL);
        reply->message_type = NDMP_MESSAGE_REPLY;
        reply->message = req.message;
        reply->reply_sequence = req.sequence;
        reply->error = err;
} 


/*
 * get_session_queue can be called from
 * different threads concurrently. 
 * Needs to be thread-safe. 
 */

struct queue_hdr* get_session_queue() 
{
        static struct queue_hdr *session_info_queue = NULL;
        struct lock *s_lock = get_lock();
        enter_critical_section(s_lock);

        if (session_info_queue == NULL) {       
                session_info_queue = init_queue();
        }
        exit_critical_section(s_lock);

        return session_info_queue;
}

struct ndmp_session_info* get_session_info(int session_id) {


        struct queue_hdr* session_queue;
        struct ndmp_session_info *retval;
        struct lock *s_lock = get_lock();

        /*
         * This function returns the ndmp_session_info
         * struct. The ndmp_session_info structs are in 
         * a queue, keyed by session_id. The states are 
         * owned and managed by the caller(s). The queue 
         * does not manage them. The state updates must
         * occur in a thread-safe manner. Hence the struct
         * has a lock.
         *
         */
        enter_critical_section(s_lock);
        
        session_queue = get_session_queue();
        
        /*
         * Since we create a new sessions 
         * in this function, it needs to be
         * thread-safe
         */

        retval = get_elem(session_queue, (void *)&session_id,
                          session_info_cmp);
        if (retval == NULL) {

                /*
                 * This must be a new session
                 * set the the session states to LISTEN
                 */

                retval = (struct ndmp_session_info*) malloc(
                        sizeof(struct ndmp_session_info));
                retval->session_id = session_id;
                retval->connection_state  = LISTEN;
                retval->data_state = IDLE;
                retval->mover_state = IDLE;
                retval->s_lock = get_lock();
                retval->jobs = init_queue();
                retval->is_terminated = 0;
                enqueue(session_queue, retval);
        }       
        exit_critical_section(s_lock);
        return retval;
}

int session_info_cmp (void *id, void *session) 
{
        int session_id = *(int *) id;
        int queue_elem_session_id =
                ((struct ndmp_session_info *)session)->session_id;
        
        return session_id == queue_elem_session_id;
}

/*
 * Each NDMP request (job) that is handled is added
 * to an outstanding jobs queue till it is processed
 * and completed.
 */

void add_job_to_session(struct client_txn *txn)
{
        struct ndmp_session_info *session;

        session = get_session_info(txn->client_session.session_id);
        enqueue(session->jobs, txn);
}               
/*
 * Each session will have outstanding jobs held in 
 * a queue; one queue element for each request in 
 * a thread. cleanup_session  is a callback method that
 * the comm layer will call back on the worker thread
 * after it has sent the response. 
 * We remove the client_txn (txn) from the session job
 * queue. 
 *
 * If all jobs are done and the session is terminated, 
 * we remove the session from the session queue.
 *
 */

int cleanup_session(struct client_txn *txn) 
{
        struct ndmp_session_info *session;
        struct queue_hdr *session_queue;

        session = get_session_info(txn->client_session.session_id);
        remove_elem(session->jobs, txn, job_cmp);
        if (session->is_terminated) {
                /*
                 * If all jobs are done, remove session
                 */
                if (num_elems(session->jobs) == 0) {
                        session_queue = get_session_queue();
                        remove_elem(session_queue,
                                    &session->session_id,
                                    session_info_cmp);
                        free(session);
                }
                return 1; /* session terminated */
        }
        return 0; /* session active */
}

int job_cmp(void *target, void *elem) 
{
        return target == elem;
}

/*
 * terminate_session is called to indicate the
 * termination of client - socket close event at comm layer
 * When a session is marked for termination, results/responses for
 * outstanding jobs are not sent back to comm layer. 
 */

void terminate_session(int session_id)
{
        struct ndmp_session_info *session;
        
        session = get_session_info(session_id);
        session->is_terminated = 1;
}

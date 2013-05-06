/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

/*
 * This file and its functions written by Shrinidhi
 * unless specified otherwise.
 */

#ifndef __H__NDMP_XDR__
#define __H__NDMP_XDR__

#include <comm.h>
#include <rpc/xdr.h>
#include <ndmp.h>
#include <locks.h>
#include <queue.h>
#include <logs.h>

enum ndmp_state {IDLE, LISTEN, CONNECTED, ACTIVE, HALTED};
struct ndmp_session_info {
        int session_id;
        enum ndmp_state connection_state;
        enum ndmp_state data_state;
        enum ndmp_state mover_state;    
        struct lock *s_lock; /* for thread-safe updates to session states */
        struct queue_hdr *jobs; /* outstanding client requests */
        int is_terminated;
};

/*
 * xdr_decode_encode is the entry point
 * and callback method for the comm layer 
 * This is the first upcall made by a worker
 * thread. 
 *
 */

void xdr_decode_encode(struct client_txn *txn);

/*
 * Each NDMP message will have a message handler that is
 * responsible for decoding(unmarshaling) the NDMP XDR request
 * and invoking the appropriate NDMP action 
 */

typedef void (*ndmp_message_handler)(struct client_txn *txn, 
                                     ndmp_header header,
                                     XDR* request_stream);

/*
 * ndmp_dispatch determines the message handler for a given
 * NDMP message type
 */

ndmp_message_handler ndmp_dispatch(int message_type);


/*
 * Each session will have outstanding jobs held in 
 * a queue; one queue element for each request in 
 * a thread. We remove the client_txn (txn) from 
 * the session job queue. 
 */

int cleanup_session(struct client_txn *txn);

/*
 * terminate_session is called to indicate the
 * termination of client - socket close event at comm layer
 * When a session is marked for termination, results/responses for
 * outstanding jobs are not sent back to comm layer. 
 */

void terminate_session(int session_id);

/*
 * get_next_seq_number returns the next number in
 * sequence for messages
 */


int get_next_seq_number();

/*
 * set_header sets up the fields in the header
 *            some fields of reply header depend 
 *            on request header 
 */
            
 void set_header(struct ndmp_header request, 
                 struct ndmp_header *reply, ndmp_error err);
/*
 *
 * struct_ndmp_session_info* get_session_info(int session_id) 
 * returns the ndmp_session_info* that holds the session information (states)
 * for session_id
 */

struct ndmp_session_info* get_session_info(int session_id);


/*
 *
 *  The various NDMP message handlers
 */
void ndmp_accept_notify(struct client_txn *txn, struct ndmp_header, XDR*);
void ndmp_connect_open(struct client_txn *txn, struct ndmp_header, XDR* request_stream);
void ndmp_connect_close(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_config_get_host_info(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_config_get_connection_type(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_config_get_auth_attr(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_config_get_butype_info(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_config_get_fs_info(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_config_get_tape_info(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_config_get_scsi_info(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_config_get_server_info(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_data_get_state(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_data_start_backup(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_data_start_recover(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_data_abort(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_data_get_env(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_data_stop(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_data_listen(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_data_connect(struct client_txn *, struct ndmp_header, XDR*);
void ndmp_error_message(struct client_txn *, struct ndmp_header, XDR *); // For wrong message type

#endif

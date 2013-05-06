/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

#include <ndmp.h>
#include <comm.h>
#include <ndmp_msg.h>

/*
 * Each of the functions below take a client_txn and an XDR request stream as
 * parameters. The client_txn structure is defined in comm.h and stores
 * encoded requests and replies.
 *
 * By the time control reaches any of the below functions the message type is
 * already known - the header is decoded in xdr_encode_decode in ndmp_xdr.c.
 * In each of the functions below we examine the structure that comes after
 * the NDMP header. We then create the appropriate full NDMP reply message
 * (including header) based on the client's request. Finally, we XDR encode
 * (marshal) this reply message into txn.
 *
 * In summary, each function in this layer has 3 parts:
 * 1. Decode rest of request message if present (apart from header)
 * 2. Create appropriate message reply (including header)
 * 3. Encode reply message and add to txn
 *
 */


void ndmp_connect_open(struct client_txn *txn, struct ndmp_header header, XDR* request_stream) 
{
        /* NDMP_CONNECT_OPEN
         *
         * struct ndmp_connect_open_request
         * {
         *      u_short protocol_version;
         * };
         *
         * struct ndmp_connect_open_reply
         * {
         *      ndmp_error error;
         * };
         *
         */
        
        struct ndmp_connect_open_request request;
        struct ndmp_header reply_header;
        struct ndmp_connect_open_reply reply;
        struct ndmp_session_info *session_info;
        XDR reply_stream;
        
        xdr_ndmp_connect_open_request(request_stream, &request);
        
        session_info = get_session_info(txn->client_session.session_id);
        
        /*
         * There could be concurrent sessions for the
         * same client. Since this fuction updates shared data, 
         * it needs to be thread-safe.
         */

        enter_critical_section(session_info->s_lock);

        if (session_info->connection_state != LISTEN) 
                reply.error = NDMP_ILLEGAL_STATE_ERR;
        else if (request.protocol_version >= 1 && request.protocol_version <= 3)
                reply.error = NDMP_NO_ERR;
        else if (request.protocol_version > 3)
                reply.error = NDMP_ILLEGAL_ARGS_ERR;
       
        set_header(header, &reply_header, reply.error);
        
        txn->reply.length = xdr_sizeof((xdrproc_t) 
                                       xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t) 
                                        xdr_ndmp_connect_open_reply, &reply);
        
        xdrmem_create(&reply_stream, 
                      txn->reply.message, txn->reply.length,XDR_ENCODE);
        
        xdr_ndmp_header(&reply_stream, &reply_header);
        if (reply.error == NDMP_NO_ERR)  {
                xdr_ndmp_connect_open_reply(&reply_stream, &reply);
                session_info->connection_state = CONNECTED;
                /* TBD: What about other states? */
        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t) 
                                        xdr_ndmp_connect_open_reply, &reply);
        write_client_log(txn->client_session.client_info.client, "Sent CONNECT_OPEN to client");

        exit_critical_section(session_info->s_lock);
}


void ndmp_connect_close(struct client_txn *txn, 
                        struct ndmp_header header,  
                        XDR* request_stream)
{
        /* NDMP_CONNECT_CLOSE */
        /* no request arguments */
        /* no reply message */
        
        struct ndmp_session_info *session_info;

        session_info = get_session_info(txn->client_session.session_id);
        
        enter_critical_section(session_info->s_lock);

        /*
         * States set as per Figure 8 in NDMP V3 spec.
         */

        session_info->connection_state = HALTED; // No state diagram for connect?
        session_info->data_state = HALTED;
        session_info->mover_state = HALTED;
        txn->reply.length = 0; // No reply to be sent

        write_client_log(txn->client_session.client_info.client, "Sent CONNECT_CLOSE to client");

        exit_critical_section(session_info->s_lock);
}

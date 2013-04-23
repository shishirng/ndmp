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
#include <stdbool.h>

struct environment
{
        u_int env_len;
        ndmp_pval *env_val;
};

struct environment* get_environment()
{
        static struct environment *retval = NULL;
        if (retval == NULL ) {
                retval = calloc(1, sizeof(struct environment));
                retval->env_len = 0;
                retval->env_val = NULL;
        }
        return retval;
}

struct environment* init_environment()
{
        struct environment *env = get_environment();
        env->env_len = 0;
        env->env_val = NULL;
        return env;
}

ndmp_data_state get_data_state(struct client_txn *txn )
{
     
}

void set_data_state(struct client_txn *txn , ndmp_data_state new_data_state )
{
       
}

void ndmp_data_get_state(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{

}

void ndmp_data_listen(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
      

}

void ndmp_data_connect(struct client_txn *txn, struct ndmp_header header, XDR* request_stream)
{
      
}

void ndmp_data_start_recover(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
          
}

void ndmp_data_abort(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
        /* NDMP_DATA_ABORT
         *
         * no request arguments
         *
         * struct ndmp_data_abort_reply
         * {
         *      ndmp_error error;
         * };
         *
         */

                struct ndmp_header reply_header;
                struct ndmp_data_abort_reply reply;
                struct ndmp_session_info *session_info;

                XDR reply_stream;

                session_info = get_session_info(txn->client_session.session_id);
                enter_critical_section(session_info->s_lock);

                if (session_info-> data_state == NDMP_DATA_STATE_HALTED || session_info-> data_state == NDMP_DATA_STATE_IDLE){
                        reply.error = NDMP_ILLEGAL_STATE_ERR;
                }
                else {
                        session_info->data_state = NDMP_DATA_STATE_HALTED;
                        reply.error = NDMP_NO_ERR;
                }

                set_header(header, &reply_header, reply.error);

                txn->reply.length = xdr_sizeof((xdrproc_t)
                                                       xdr_ndmp_header, &reply_header);
                txn->reply.length += xdr_sizeof((xdrproc_t)
                                                        xdr_ndmp_data_abort_reply, &reply);

                xdrmem_create(&reply_stream,
                                              txn->reply.message, txn->reply.length,XDR_ENCODE);

                xdr_ndmp_header(&reply_stream, &reply_header);

                if (reply.error == NDMP_NO_ERR)
                        xdr_ndmp_data_abort_reply(&reply_stream, &reply);
                else
                        txn->reply.length -= xdr_sizeof((xdrproc_t)
                                                xdr_ndmp_data_abort_reply, &reply);

                exit_critical_section(session_info->s_lock);
#ifdef DEBUG
                printf("Data state: %d\n",session_info->data_state);
#endif

}

void ndmp_data_stop(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
        /*  NDMP_DATA_STOP
         *
         * no request arguments
         *
         * struct ndmp_data_stop_reply
         * {
         *      ndmp_error error;
         * };
         *
         */

        struct ndmp_header reply_header;
        struct ndmp_data_stop_reply reply;
        struct ndmp_session_info *session_info;
        struct environment* data_env = get_environment();

        XDR reply_stream;

        session_info = get_session_info(txn->client_session.session_id);
        enter_critical_section(session_info->s_lock);

        if (session_info-> data_state != NDMP_DATA_STATE_HALTED){
                reply.error = NDMP_ILLEGAL_STATE_ERR;
        }
        else {
                session_info->data_state = NDMP_DATA_STATE_IDLE;
                data_env = init_environment();
                reply.error = NDMP_NO_ERR;
        }

        set_header(header, &reply_header, reply.error);

        txn->reply.length = xdr_sizeof((xdrproc_t)
                                               xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t)
                                                xdr_ndmp_data_stop_reply, &reply);

        xdrmem_create(&reply_stream,
                                      txn->reply.message, txn->reply.length,XDR_ENCODE);

        xdr_ndmp_header(&reply_stream, &reply_header);

        if (reply.error == NDMP_NO_ERR)
                xdr_ndmp_data_stop_reply(&reply_stream, &reply);
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_data_stop_reply, &reply);

        exit_critical_section(session_info->s_lock);
#ifdef DEBUG
        printf("Data state: %d\n",session_info->data_state);
#endif
}

void ndmp_data_get_env(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
        /* NDMP_DATA_GET_ENV
         *
         * no request arguments
         *
         * struct ndmp_data_get_env_reply
         * {
         * ndmp_error error;
         * ndmp_pval env<>;
         * };
         *
         */

        int i;
        struct ndmp_header reply_header;
        struct ndmp_data_get_env_reply reply;
        struct ndmp_session_info *session_info;
        struct environment *data_env = get_environment();
        XDR reply_stream;
        session_info = get_session_info(txn->client_session.session_id);

        enter_critical_section(session_info->s_lock);

        reply.error = NDMP_NO_ERR;
        reply.env.env_len = data_env->env_len;

        reply.env.env_val = (ndmp_pval*)malloc(8*sizeof(ndmp_pval) );

        if (session_info-> data_state != NDMP_DATA_STATE_ACTIVE || session_info-> data_state != NDMP_DATA_STATE_HALTED){
                reply.error = NDMP_NO_ERR;
                if (data_env->env_len != 0) {

                        for (i = 0; i < data_env->env_len; i++) {
                                reply.env.env_val[i].name = (char*) malloc(64*sizeof(char));
                                reply.env.env_val[i].value = (char*) malloc(64*sizeof(char));
                        }

                        for (i = 0; i < data_env->env_len; i++) {
                                reply.env.env_val[i].name = data_env->env_val[i].name;
                                reply.env.env_val[i].value = data_env->env_val[i].value;

                        printf("reply.env.env_val[i].name = %s\n reply.env.env_val[i].value = %s\n\n",
                                        reply.env.env_val[i].name, reply.env.env_val[i].value);
                        }
                }
                else
                        reply.env.env_val = NULL;
        }
        else
                reply.error = NDMP_ILLEGAL_STATE_ERR;

        set_header(header, &reply_header, reply.error);

        txn->reply.length = xdr_sizeof((xdrproc_t)
                                               xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t)
                                                xdr_ndmp_data_get_env_reply, &reply);

        xdrmem_create(&reply_stream,
                              txn->reply.message, txn->reply.length,XDR_ENCODE);

        xdr_ndmp_header(&reply_stream, &reply_header);

        if (reply.error == NDMP_NO_ERR)
                xdr_ndmp_data_get_env_reply(&reply_stream, &reply);
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_data_get_env_reply, &reply);

        exit_critical_section(session_info->s_lock);
}

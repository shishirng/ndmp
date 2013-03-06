/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

#ifndef __H__NDMP_XDR__
#define __H__NDMP_XDR__

#include <comm.h>
#include <rpc/xdr.h>

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
		XDR* request_stream);

/*
 * ndmp_dispatch determines the message handler for a given
 * NDMP message type
 */

ndmp_message_handler ndmp_dispatch(int message_type);

/*
 *
 *  The various NDMP message handlers
 */
void ndmp_connect_open(struct client_txn *txn, XDR* request_stream);
void ndmp_connect_close(struct client_txn *, XDR*);
void ndmp_config_get_host_info(struct client_txn *, XDR*);
void ndmp_config_get_connection_type(struct client_txn *, XDR*);
void ndmp_config_get_auth_attr(struct client_txn *, XDR*);
void ndmp_config_get_butype_info(struct client_txn *, XDR*);
void ndmp_config_get_fs_info(struct client_txn *, XDR*);
void ndmp_config_get_tape_info(struct client_txn *, XDR*);
void ndmp_config_get_scsi_info(struct client_txn *, XDR*);
void ndmp_config_get_server_info(struct client_txn *, XDR*);
void ndmp_error_message(struct client *, XDR *); // For wrong message type
#endif

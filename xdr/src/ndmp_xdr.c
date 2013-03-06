/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

#include <ndmp_xdr.h>
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
	 *     txn->response will hold the marshaled (encoded)NDMP response
	 *  3. Enqueue the response into the response queue
	 */

	XDR *request_stream = (XDR *) malloc(sizeof(XDR));
	int message_type;
	struct comm_context *ctx;

	xdrmem_create(request_stream, txn->request.message, txn->request.length,
			XDR_DECODE);
	/*
	 * Get the message type
	 */

	// message_type = ....;

	ndmp_dispatch(message_type)(txn, request_stream);

	enqueue(ctx->response_jobs, txn);

}

ndmp_message_handler ndmp_dispatch(int message_type) {

	switch (message_type) {
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
	default:
		return ndmp_error_message;
	}
}

void ndmp_error_message(struct client_txn *, XDR*)
{

}


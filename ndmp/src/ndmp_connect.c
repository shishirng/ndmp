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
       
}


void ndmp_connect_close(struct client_txn *txn, 
                        struct ndmp_header header,  
                        XDR* request_stream)
{
        
}

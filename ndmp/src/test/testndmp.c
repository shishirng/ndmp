/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

#include <comm.h>
#include <stdlib.h>
#include <stdio.h>
#include <ndmp_msg.h>

main()
{
	struct comm_context *ctx = comm_context();
	ctx->marshal_unmarshal = xdr_decode_encode;
	ctx->cleanup_session = cleanup_session;
	ctx->terminate_session = terminate_session;
	comm_listen(ctx);
}


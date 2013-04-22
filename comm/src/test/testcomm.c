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

#include <comm.h>
#include <stdlib.h>
#include <stdio.h>

void process_job(struct client_txn *job);

main()
{
	struct comm_context *ctx = comm_context();
	ctx->marshal_unmarshal = process_job;
	comm_listen(ctx);
}

void process_job(struct client_txn *job) 
{
	hexdump(job->request.message, job->request.length);
}

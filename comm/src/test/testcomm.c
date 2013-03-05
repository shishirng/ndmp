#include <comm.h>
#include <stdlib.h>
#include <stdio.h>


void process_job(struct client_txn *job);

main() {

	struct comm_context *ctx = comm_context();
	ctx->marshal_unmarshal = process_job;
	comm_listen(ctx);
}

void process_job(struct client_txn *job) 
{

	hexdump(job->request.message, job->request.length);
}

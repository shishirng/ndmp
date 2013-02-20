#include <comm.h>
#include <stdlib.h>
#include <stdio.h>

void marshal(struct session *a_session) {

	read_message(a_session);
	exit(0);
}

main() {

	struct comm_context *ctx = comm_init();
	ctx->marshal_unmarshal = marshal;
	comm_listen(ctx);
}



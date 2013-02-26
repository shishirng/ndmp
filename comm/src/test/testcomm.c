#include <comm.h>
#include <stdlib.h>
#include <stdio.h>


main() {

	struct comm_context *ctx = comm_context();
	comm_listen(ctx);
}



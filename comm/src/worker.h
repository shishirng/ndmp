/*
 * Copyright place holder
 */

#ifndef __H_WORKER__
#define __H_WORKER__

#include <comm.h>

#define THREAD_POOL_SIZE 5

void create_thread_pool(int n);
void shutdown_thread_pool();

#endif

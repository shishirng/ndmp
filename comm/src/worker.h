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

#ifndef __H_WORKER__
#define __H_WORKER__

#include <comm.h>
#include <string.h>

#define THREAD_POOL_SIZE 1

void create_thread_pool(int n);
void shutdown_thread_pool();

#endif

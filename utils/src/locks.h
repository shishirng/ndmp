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

#ifndef __H_LOCKS__
#define __H_LOCKS__
#include <pthread.h>

struct lock {
        pthread_mutex_t mutex;

};

struct lock* get_lock();
void enter_critical_section(struct lock* a_lock);
void exit_critical_section(struct lock* a_lock);

#endif

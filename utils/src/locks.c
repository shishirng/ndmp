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

#include <stdlib.h>
#include <locks.h>

struct lock* get_lock()
{
        struct lock* retval;
        pthread_mutexattr_t attr;

        retval = (struct lock *) malloc(sizeof(struct lock));
        /* create a recursive lock */
        pthread_mutex_init(&retval->mutex, NULL );
        return retval;
}

void enter_critical_section(struct lock* a_lock)
{
        pthread_mutex_lock(&a_lock->mutex);
}

void exit_critical_section(struct lock* a_lock)
{
        pthread_mutex_unlock(&a_lock->mutex);
}

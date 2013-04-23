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

#ifndef __H_HEXDUMP__
#define __H_HEXDUMP__

#include <ctype.h>
#include <stdio.h>

void dump_hex(unsigned char *buf, unsigned int whence, unsigned int size);
void dump_ascii(unsigned char *buf, unsigned int whence, unsigned int size);
void hexdump(void *buffer, unsigned int size);

#endif

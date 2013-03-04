/*
 * Copyright placeholder
 */

#ifndef __H_HEXDUMP__
#define __H_HEXDUMP__

#include <ctype.h>
#include <stdio.h>
void dump_hex(unsigned char *buf, unsigned int whence, unsigned int size);
void dump_ascii(unsigned char *buf, unsigned int whence, unsigned int size);
void hexdump(void *buffer, unsigned int size);

#endif

/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

#include "hexdump.h"
#include <stdio.h>
#include <string.h>

main() 
{
	char *c = "This is a \n \t  hexdump test";
	hexdump(c,strlen(c));

}

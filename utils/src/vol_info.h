/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */
#ifndef __H__vol_info__
#define __H__vol_info__


struct vol_fs_node{
        char* vol_name;
        char* filesystem; // it is in the form of IP/FILESYSTEM and uniquely identifies a node in a list
        char * user;
        struct vol_fs_node * next ;
};


void insert_node( struct vol_fs_node * , char * , char *  , char *); 

int init_vol_fs_list( struct vol_fs_node ** , char* );

void init_head ( struct vol_fs_node ** , char* , char* , char *);

void print_vol_fs_info( struct vol_fs_node * );

struct vol_fs_node * find( struct vol_fs_node * , char * filesystem );

void set_host_config(char* filename);


#endif

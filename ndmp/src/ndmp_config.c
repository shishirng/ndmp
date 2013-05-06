/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

/*
 * This file and its functions written by Shrinidhi
 * unless otherwise specified.
 */

#include <ndmp.h>
#include <comm.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <ndmp_msg.h>
#include <stdio.h>
#include <string.h>
#include <sys/statvfs.h>

char *get_name(char *value, char *curr, char** saveptr);
char *get_value(char *value, char *curr, char** saveptr, int one_word);
struct ndmp_config_get_scsi_info_reply* alloc_scsi_info();
void get_scsi_info(struct ndmp_config_get_scsi_info_reply* scsi_info);
void free_scsi_info(struct ndmp_config_get_scsi_info_reply* scsi_info);

static char *fs_mount = "/"; /* change later */

/*
 * Each of the functions below take a client_txn and an XDR request stream as
 * parameters. The client_txn structure is defined in comm.h and stores
 * encoded requests and replies.
 *
 * By the time control reaches any of the below functions the message type is
 * already known - the header is decoded in xdr_encode_decode in ndmp_xdr.c.
 * In each of the functions below we examine the structure that comes after
 * the NDMP header. We then create the appropriate full NDMP reply message
 * (including header) based on the client's request. Finally, we XDR encode
 * (marshal) this reply message into txn.
 *
 * In summary, each function in this layer has 3 parts:
 * 1. Decode rest of request message if present (apart from header)
 * 2. Create appropriate message reply (including header)
 * 3. Encode reply message and add to txn
 *
 */

void ndmp_config_get_host_info(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
        /*  NDMP_CONFIG_GET_HOST_INFO
         *
         *  no request arguments
         *
         * struct ndmp_config_get_host_info_reply
         * {
         *      ndmp_error      error;
         *      string          hostname<>;
         *      string          os_type<>;
         *      string          os_vers<>;
         *      string          hostid<>;
         * };
         */
        
        struct ndmp_header reply_header;
        struct ndmp_config_get_host_info_reply reply;
        char hostname[32];
        struct utsname os_info;
        XDR reply_stream;

        reply.error = NDMP_NO_ERR;

        if (gethostname(hostname,32) != -1)
                reply.hostname = hostname;
        else
                reply.error = NDMP_NOT_SUPPORTED_ERR;

        if (uname(&os_info) != -1){
                reply.os_type = os_info.sysname;
                reply.os_vers = os_info.version;
        }
        else     
                reply.error = NDMP_NOT_SUPPORTED_ERR;

        reply.hostid = "1"; 
        
        set_header(header, &reply_header, reply.error);
        
        txn->reply.length = xdr_sizeof((xdrproc_t)
                                       xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_config_get_host_info_reply,
                                        &reply);
        
        xdrmem_create(&reply_stream,
                      txn->reply.message, txn->reply.length,XDR_ENCODE);
        
        xdr_ndmp_header(&reply_stream, &reply_header);
        if (reply.error == NDMP_NO_ERR)  {
                xdr_ndmp_config_get_host_info_reply(&reply_stream, &reply);
                write_client_log(txn->client_session.client_info.client, "Sent HOST INFO to client");

        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                                xdr_ndmp_config_get_host_info_reply, &reply);
}

void ndmp_config_get_connection_type(struct client_txn *txn, struct ndmp_header header, XDR* request_stream) 
{
        /*  NDMP_CONFIG_GET_CONNECTION_TYPE
         *
         *  no request arguments
         *
         * enum ndmp_addr_type
         * {
         *      NDMP_ADDR_LOCAL,
         *      NDMP_ADDR_TCP,
         *      NDMP_ADDR_FC,
         *      NDMP_ADDR_IPC
         * };
         *
         * struct ndmp_config_get_connection_type_reply
         * {
         *      ndmp_error      error;
         *      ndmp_addr_type  addr_types<>;
         * };
         *
         */

        struct ndmp_header reply_header;
        struct ndmp_config_get_connection_type_reply reply;
        enum ndmp_addr_type addr_types[2] = {NDMP_ADDR_LOCAL, NDMP_ADDR_TCP};
        XDR reply_stream;

        reply.addr_types.addr_types_len = 2;
        reply.addr_types.addr_types_val = addr_types;
        reply.error = NDMP_NO_ERR;

        set_header(header, &reply_header, reply.error);
        
        txn->reply.length = xdr_sizeof((xdrproc_t)
                                       xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_config_get_connection_type_reply, 
                                        &reply);
        
        xdrmem_create(&reply_stream,
                      txn->reply.message, txn->reply.length,XDR_ENCODE);
        
        xdr_ndmp_header(&reply_stream, &reply_header);
        if (reply.error == NDMP_NO_ERR)  {
                xdr_ndmp_config_get_connection_type_reply(&reply_stream, &reply);
        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                                xdr_ndmp_config_get_connection_type_reply, 
                                                &reply);
        write_client_log(txn->client_session.client_info.client, "Sent CONNECTION TYPES to client");

}

void ndmp_config_get_auth_attr(struct client_txn *txn, struct ndmp_header header, XDR* request_stream) 
{
        /* /* NDMP_CONFIG_GET_AUTH_ATTR
         *
         * struct ndmp_config_get_auth_attr_request
         * {
         *      ndmp_auth_type auth_type;
         * };
         *
         * struct ndmp_config_get_auth_attr_reply
         * {
         *      ndmp_error error;
         *      ndmp_auth_attr server_attr;
         * };
         */
        
        struct ndmp_header reply_header;
        struct ndmp_config_get_auth_attr_request request;
        struct ndmp_config_get_auth_attr_reply reply;
        XDR reply_stream;
        

        xdr_ndmp_config_get_auth_attr_request(request_stream, &request);
        
        switch (request.auth_type) {
        case NDMP_AUTH_NONE:
                reply.server_attr.auth_type = NDMP_AUTH_NONE;
                strcpy(reply.server_attr.ndmp_auth_attr_u.challenge, 
                       ""); /* No challenge */
                reply.error = NDMP_NO_ERR;
                break;
        case NDMP_AUTH_TEXT:
        case NDMP_AUTH_MD5:
        default: 
                reply.error = NDMP_NOT_SUPPORTED_ERR; 
        }

        set_header(header, &reply_header, reply.error);
        
        txn->reply.length = xdr_sizeof((xdrproc_t)
                                       xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_config_get_auth_attr_reply, 
                                        &reply);
        
        xdrmem_create(&reply_stream,
                      txn->reply.message, txn->reply.length,XDR_ENCODE);
        
        xdr_ndmp_header(&reply_stream, &reply_header);
        if (reply.error == NDMP_NO_ERR)  {
                xdr_ndmp_config_get_auth_attr_reply(&reply_stream, &reply);
                write_client_log(txn->client_session.client_info.client, "Sent authentication attributes to client");

        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                                xdr_ndmp_config_get_auth_attr_reply, 
                                                &reply);


}

void ndmp_config_get_butype_info(struct client_txn *txn, 
                                 struct ndmp_header header, XDR* request_stream) 
{
        /* NDMP_CONFIG_GET_BUTYPE_INFO
         *
         * No request arguments
         *
         * backup type attributes
         * const NDMP_BTYPE_BACKUP_FILE_HISTORY         = 0x0001;
         * const NDMP_BUTYPE_BACKUP_FILELIST            = 0x0002;
         * const NDMP_BUTYPE_RECOVER_FILELIST           = 0x0004;
         * const NDMP_BUTYPE_BACKUP_DIRECT              = 0x0008;
         * const NDMP_BUTYPE_RECOVER_DIRECT             = 0x0010;
         * const NDMP_BUTYPE_BACKUP_INCREMENTAL         = 0x0020;
         * const NDMP_BUTYPE_RECOVER_INCREMENTAL        = 0x0040;
         * const NDMP_BUTYPE_BACKUP_UTF8                = 0x0080;
         * const NDMP_BUTYPE_RECOVER_UTF8               = 0x0100;
         *
         * struct ndmp_butype_info
         * {
         *      string          butype_name <>;
         *      ndmp_pval       default_env <>;
         *      u_long          attrs;
         * };
         * struct ndmp_config_get_butype_attr_reply
         * {
         *      ndmp_error              error;
         *      ndmp_butype_info        butype_info <>;
         * };
         */

        struct ndmp_config_get_butype_info_reply reply;
        struct ndmp_butype_info *bu_type_info;
        ndmp_header reply_header;
        XDR reply_stream;
        int i;
        int bu_types[] = {NDMP_BUTYPE_BACKUP_FILELIST, NDMP_BUTYPE_BACKUP_DIRECT,
                          NDMP_BUTYPE_BACKUP_INCREMENTAL, NDMP_BUTYPE_BACKUP_UTF8};
        int num_attrs = 4;
        reply.error = NDMP_NO_ERR;
        reply.butype_info.butype_info_len = 3*num_attrs;
        bu_type_info  = (struct ndmp_butype_info *)
                malloc(reply.butype_info.butype_info_len*sizeof(struct ndmp_butype_info));
       
        for (i=0; i<3*num_attrs; i+=3) {
                bu_type_info[i].butype_name = "dump";
                bu_type_info[i].attrs = bu_types[i/3];
                bu_type_info[i].default_env.default_env_len = 0;
                bu_type_info[i+1].butype_name = "tar";
                bu_type_info[i+1].attrs = bu_types[i/3];
                bu_type_info[i+1].default_env.default_env_len = 0;
                bu_type_info[i+2].butype_name = "cpio";
                bu_type_info[i+2].attrs = bu_types[i/3];
                bu_type_info[i+2].default_env.default_env_len = 0;
        }
        reply.butype_info.butype_info_val = bu_type_info;
        set_header(header, &reply_header, reply.error);
        
        txn->reply.length = xdr_sizeof((xdrproc_t)
                                       xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_config_get_butype_info_reply, 
                                        &reply);
        
        xdrmem_create(&reply_stream,
                      txn->reply.message, txn->reply.length,XDR_ENCODE);
        
        xdr_ndmp_header(&reply_stream, &reply_header);
        xdr_ndmp_config_get_butype_info_reply(&reply_stream, &reply);
        write_client_log(txn->client_session.client_info.client, "Sent BACKUP TYPE INFO to client");

        free(bu_type_info); 

        
}

void ndmp_config_get_fs_info(struct client_txn *txn, struct ndmp_header header,  XDR* request_stream) 
{
        /* /* NDMP_CONFIG_GET_FS_INFO
         *
         * No request arguments
         *
         * const NDMP_FS_INFO_TOTAL_SIZE_INVALID        = 0x00000001;
         * const NDMP_FS_INFO_USED_SIZE_INVALID         = 0x00000002;
         * const NDMP_FS_INFO_AVAIL_SIZE_INVALID        = 0x00000004;
         * const NDMP_FS_INFO_TOTOAL_INODES_INVALID     = 0x00000008;
         * const NDMP_FS_INFO_USED_INNODES_INVALID      = 0x00000010;
         *
         * struct ndmp_fs_info
         * {
         *      u_long          invalid; /* invalid bit
         *      string          fs_type <>;
         *      string          fs_logical_device <>;
         *      string          fs_physical_device <>;
         *      ndmp_u_quad     total_size;
         *      ndmp_u_quad     used_size;
         *      ndmp_u_quad     avail_size;
         *      ndmp_u_quad     total_inodes;
         *      ndmp_u_quad     used_inodes;
         *      ndmp_pval       fs_env <>;
         *      string          fs_status<>;
         * };
         *
         * struct ndmp_config_get_fs_info_reply
         * {
         *      ndmp_error      error;
         *      ndmp_fs_info    fs_info <>;
         * };
         */
        

        struct statvfs statvfsbuf;
        struct ndmp_header reply_header;
        struct ndmp_config_get_fs_info_reply reply;
        struct ndmp_fs_info *fs_info;
        int rc;
        XDR reply_stream;

        rc = statvfs(fs_mount, &statvfsbuf);
        if (rc == -1) 
                reply.error = NDMP_NOT_SUPPORTED_ERR;
        else
                reply.error = NDMP_NO_ERR;

        set_header(header, &reply_header, reply.error);

        if (rc != -1) {
                reply.fs_info.fs_info_len = 1;
                reply.fs_info.fs_info_val = (struct ndmp_fs_info *)
                        malloc(sizeof(struct ndmp_fs_info));
                fs_info = reply.fs_info.fs_info_val;
                fs_info->invalid = 0;
                fs_info->fs_type = "glusterfs";
                fs_info->fs_logical_device = 
                        (char *)malloc(strlen(fs_mount)*sizeof(char));
                strcpy(fs_info->fs_logical_device, fs_mount);
                fs_info->fs_physical_device = 
                        (char *)malloc(strlen(fs_mount)*sizeof(char));
                strcpy(fs_info->fs_physical_device, fs_mount);
                
                fs_info->total_size.low = 
                                           statvfsbuf.f_blocks*statvfsbuf.f_frsize/1024;
                fs_info->total_size.high = 0;
                fs_info->avail_size.low = 
                                           statvfsbuf.f_bavail*statvfsbuf.f_frsize/1024;
                fs_info->avail_size.high = 0;
                fs_info->used_size.low =  fs_info->total_size.low -
                        statvfsbuf.f_bfree*statvfsbuf.f_frsize/1024;
                fs_info->used_size.high = 0;
                fs_info->total_inodes.low = statvfsbuf.f_files;
                fs_info->total_inodes.high = 0;
                fs_info->used_inodes.low = statvfsbuf.f_files - 
                                                                       statvfsbuf.f_ffree;
                
                fs_info->used_inodes.high = 0;
                fs_info->fs_env.fs_env_len = 0;
                fs_info->fs_status = "up";              
        }

        txn->reply.length = xdr_sizeof((xdrproc_t)
                                       xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_config_get_fs_info_reply, 
                                        &reply);
        
        xdrmem_create(&reply_stream,
                      txn->reply.message, txn->reply.length,XDR_ENCODE);
        
        xdr_ndmp_header(&reply_stream, &reply_header);
        if (reply.error == NDMP_NO_ERR)  {
                xdr_ndmp_config_get_fs_info_reply(&reply_stream, &reply);
        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                                xdr_ndmp_config_get_fs_info_reply, 
                                                &reply);
        write_client_log(txn->client_session.client_info.client, "Sent FILE SYSTEM INFO to client");

        free(fs_info->fs_logical_device);
        free(fs_info->fs_physical_device);
}

void ndmp_config_get_tape_info(struct client_txn *txn, struct ndmp_header header,  XDR* request_stream) 
{
        /* NDMP_CONFIG_GET_TAPE_INFO
         *
         * tape attributes
         *
         * const NDMP_TAPE_ATTR_REWIND  = 0x00000001;
         * const NDMP_TAPE_ATTR_UNLOAD  = 0x00000002;
         *
         * No request arguments
         *
         * struct ndmp_device_capability
         * {
         *      string          device <>;
         *      u_long          attr;
         *      ndmp_pval       capability <>;
         * };
         *
         * struct ndmp_device_info
         * {
         *      string                  model <>;
         *      ndmp_device_capability  caplist <>;
         * };
         *
         * struct ndmp_config_get_tape_info_reply
         * {
         *      ndmp_error       error;
         *      ndmp_device_info tape_info <>;
         * };
         */

        struct ndmp_header reply_header;
        struct ndmp_config_get_tape_info_reply reply;
        XDR reply_stream;

        ndmp_device_info tapeInfo;
        ndmp_device_capability devCap; 
        ndmp_pval pval;
        
        reply.error           = NDMP_NO_ERR;
        reply.tape_info.tape_info_len = 1;
        reply.tape_info.tape_info_val = &tapeInfo;

        tapeInfo.model = "HP MSL6000 Tape";
        tapeInfo.caplist.caplist_len = 1;
        tapeInfo.caplist.caplist_val = &devCap;
    
        devCap.device = "/dev/tape0";
        devCap.attr   = NDMP_TAPE_ATTR_REWIND;
        devCap.capability.capability_len = 1;
        devCap.capability.capability_val = &pval;

        pval.name  = "TAPE_SIZE";
        pval.value = "1GB";


        set_header(header, &reply_header, reply.error);
        
        txn->reply.length = xdr_sizeof((xdrproc_t)
                                       xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_config_get_tape_info_reply, &reply);

        xdrmem_create(&reply_stream,
                      txn->reply.message, txn->reply.length,XDR_ENCODE);

        xdr_ndmp_header(&reply_stream, &reply_header);
        if (reply.error == NDMP_NO_ERR) {
                xdr_ndmp_config_get_tape_info_reply(&reply_stream, &reply);
                write_client_log(txn->client_session.client_info.client, "Sent TAPE INFO to client");

        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t) xdr_ndmp_config_get_tape_info_reply,
                                                &reply);
}

void ndmp_config_get_scsi_info(struct client_txn *txn, 
                               struct ndmp_header header, 
                               XDR* request_stream) 
{
        /* NDMP_CONFIG_GET_SCSI_INFO
         *
         * No request arguments
         *
         * no SCSI attributes
         *
         * struct ndmp_config_get_scsi_info_reply
         * {
         * ndmp_error           error;
         * ndmp_device_info     scsi_info <>;
         * };
         */
        XDR reply_stream;
        struct ndmp_header reply_header;
        struct ndmp_config_get_scsi_info_reply reply;
        struct ndmp_config_get_scsi_info_reply *scsi_info =
                alloc_scsi_info();


        get_scsi_info(scsi_info);
        
        /*
         * Check if there are any scsi devices
         */
        if (scsi_info->scsi_info.scsi_info_len > 0) {
                /*
                 * There is device info to be sent 
                 */
                reply.error = NDMP_NO_ERR;
                reply.scsi_info.scsi_info_len = 
                        scsi_info->scsi_info.scsi_info_len;
                reply.scsi_info.scsi_info_val =
                        scsi_info->scsi_info.scsi_info_val;

        }       
        else
                reply.error = NDMP_NOT_SUPPORTED_ERR;
        set_header(header, &reply_header, reply.error);
        txn->reply.length = xdr_sizeof((xdrproc_t)
                                       xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_config_get_scsi_info_reply, 
                                        &reply);
        
        xdrmem_create(&reply_stream,
                      txn->reply.message, txn->reply.length,XDR_ENCODE);
        
        xdr_ndmp_header(&reply_stream, &reply_header);
        if (reply.error == NDMP_NO_ERR)  {
                xdr_ndmp_config_get_scsi_info_reply(&reply_stream, &reply);
                write_client_log(txn->client_session.client_info.client, "Sent SCSI INFO to client");

        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                                xdr_ndmp_config_get_scsi_info_reply, 
                                                &reply);


        free_scsi_info(scsi_info);
}

void ndmp_config_get_server_info(struct client_txn *txn, struct ndmp_header header, XDR* request_stream) 
{
        /* NDMP_CONFIG_GET_SERVER_INFO
         *
         * no request arguments
         *
         * struct ndmp_config_get_server_info_reply
         * {
         * ndmp_error error;
         * string vendor_name<>;
         * string product_name<>;
         * string revision_number<>;
         * ndmp_auth_type auth_type<>;
         * };
         */

        struct ndmp_header reply_header;
        struct ndmp_config_get_server_info_reply reply;
        enum ndmp_auth_type auth_types[1] = {NDMP_AUTH_NONE};
        XDR reply_stream;

        reply.vendor_name = "Red Hat, Inc.";
        reply.product_name = "NDMP Server";
        reply.revision_number = "0.1";
        reply.auth_type.auth_type_len = 1;
        reply.auth_type.auth_type_val = auth_types;
        reply.error = NDMP_NO_ERR;

        set_header(header, &reply_header, reply.error);
        
        txn->reply.length = xdr_sizeof((xdrproc_t)
                                       xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_config_get_server_info_reply, 
                                        &reply);
        
        xdrmem_create(&reply_stream,
                      txn->reply.message, txn->reply.length,XDR_ENCODE);
        
        xdr_ndmp_header(&reply_stream, &reply_header);
        if (reply.error == NDMP_NO_ERR)  {
                xdr_ndmp_config_get_server_info_reply(&reply_stream, &reply);
                write_client_log(txn->client_session.client_info.client, "Sent server info to client");

        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                                xdr_ndmp_config_get_server_info_reply, 
                                                &reply);
}

void get_scsi_info(struct ndmp_config_get_scsi_info_reply* scsi_info )
{
        /*
         * This function reads scsi information from /proc/scsi/scsi
         */

        FILE *scsi = fopen("/proc/scsi/scsi", "r");
        char *data;
        char *s;
        char *name;
        char *value;
        char *saveptr;
        int i, j,k;
        struct ndmp_device_info *device_info;
        struct ndmp_device_capability *device_capability;
        struct ndmp_pval *pval;
        char *model, *device;
        data = (char *)malloc(80*sizeof(char));
        name = (char *)malloc(80*sizeof(char));
        value = (char *)malloc(80*sizeof(char));
        device = (char *) malloc(80*sizeof(char));

        fgets(data, 80, scsi); // read and ignore first line
        if (scsi != NULL) {
                i = -1;
                j = 0;
                while(fgets(data,80, scsi) != NULL) {
                        // strtok_r is reentrant
                        s = strtok_r(data, " ", &saveptr);
                        if (strcmp(s,"Host:") == 0) {
                                /*
                                 * New scsi device 
                                 */
                                i++;
                                scsi_info->scsi_info.scsi_info_len = i+1;       /* number of devices */
                                device_info = scsi_info->scsi_info.scsi_info_val+i;
                                j = 0;
                        }
                        do {
                                strcpy(name, "");
                                strcpy(value,"");
                                s = get_name(name, s, &saveptr);
                                
                                if (strcmp(s, "Type:") == 0) {
                                        s = get_value(value, s, &saveptr,1);
                                }
                                else { 
                                        s = get_value(value, s, &saveptr,0);
                                }

                                if (value[strlen(value)-1] == '\n')
                                        value[strlen(value)-1] = '\0';
                                
                                pval = device_info->caplist.caplist_val->capability.capability_val+j;
                                strcpy(pval->name, name); 
                                strcpy(pval->value, value);
                                if (strcmp(name, "Model:") == 0)
                                        strcpy(device_info->model, value);
                                if (strcmp(name, "Type:")==0) { 
                                        strcpy(device_info->caplist.caplist_val->device, value);
                                }
                                device_info->caplist.caplist_val->attr = j;
                                j++;
                                device_info->caplist.caplist_val->capability.capability_len = j;
                                device_info->caplist.caplist_len = 1;
                        }while(s!=NULL);
                        printf ("\n");
                }
        }
        free(data);
        free(name);
        free(value);
        free(device);
}


char * get_name(char*  name, char *curr, char** saveptr) 
{
        
        while(curr!= NULL && curr[strlen(curr)-1] != ':') {
                if(strcmp(name,"") == 0)
                        strcpy(name, curr);
                else
                        sprintf(name,"%s %s", name,  curr);
                curr = strtok_r(NULL, " ", saveptr);
        }
        if (strcmp(name, "") == 0)
                strcpy(name, curr);
        else
                sprintf(name, "%s %s", name, curr);
        return name;
}

char *get_value(char *value, char *curr, char** saveptr, int one_word) 
{
        
        curr = strtok_r(NULL, " ", saveptr);
        if (!one_word) {
                while(curr!=NULL && curr[strlen(curr)-1] != ':') {
                        if(strcmp(value, "") == 0)
                                strcpy(value,curr);
                        else
                                sprintf(value,"%s %s",value, curr);
                        curr = strtok_r(NULL, " ", saveptr);
                }
        }
        else   {
                strcpy(value,curr);
                curr = strtok_r(NULL, " ", saveptr);
        }
        return curr;
}

struct ndmp_config_get_scsi_info_reply* alloc_scsi_info() {
        /*
         * Allocation approach:
         * ndmp_config_get_scsi_info_reply has many nmpd_device_info structs
         * one for each device
         * Each ndmp_device_info has a ndmp_device_capabilty struct, which
         * has a number of properties ndmp_pval structs.
         *
         * Assumptions: We will assume a max of 32 scsi devices
         * which is plenty.
         * We will assume max of 16 name-value properties for each
         * device, which should also be plenty. 
         * These assumptions simplify getting scsi information
         * and setting them in struct ndmp_config_get_scsi_info_reply
         * especially having to set info_len attribute and capability_len
         * attributes. It would require a two pass traversal of scsi device
         * info, otherwise.
         */

        int max_number_of_devices = 32;
        int max_number_of_properties = 16;
        int str_size = 32;
        int i,j;

        struct ndmp_config_get_scsi_info_reply* retval;
        struct ndmp_device_capability device_capability;
        struct ndmp_device_info *device;
        struct ndmp_pval *pval;
        retval = malloc(sizeof(struct ndmp_config_get_scsi_info_reply));
        
        retval->scsi_info.scsi_info_val = (struct ndmp_device_info *) 
                malloc(max_number_of_devices*
                       sizeof(struct ndmp_device_info));
        
        for (i=0; i<max_number_of_devices; i++) {
                device = retval->scsi_info.scsi_info_val+i;
                device->model = malloc(str_size*sizeof(char));
                device->caplist.caplist_val =  (struct ndmp_device_capability *)
                        malloc(sizeof(struct ndmp_device_capability));
                device->caplist.caplist_val->capability.capability_val = (struct ndmp_pval *)
                        malloc(max_number_of_properties*sizeof(struct ndmp_pval));
                device->caplist.caplist_val->device = (char *)
                        malloc(str_size*sizeof(char));
                               
                for (j=0; j<max_number_of_properties; j++) {
                        pval = device->caplist.caplist_val->capability.capability_val+j;
                        pval->name = (char *) malloc(str_size*sizeof(char));
                        pval->value = (char *)malloc(str_size*sizeof(char));
                }
        }
        return retval;
}

void free_scsi_info(struct ndmp_config_get_scsi_info_reply* scsi_info) {
        /*
         * Frees space held in scsi_info as per assumption
         * in alloc_scsi_info 
         */

        int max_number_of_devices = 32;
        int max_number_of_properties = 16;
        int i,j;

        struct ndmp_device_capability *device_capability;
        struct ndmp_device_info *device;
        struct ndmp_pval *pval;
                
        for (i=0; i<max_number_of_devices; i++) {
                device = scsi_info->scsi_info.scsi_info_val+i;
                free(device->model);
                device_capability  = device->caplist.caplist_val;
                for (j=0; j<max_number_of_properties; j++) {
                        free(device_capability->capability.capability_val[j].name);
                        free(device_capability->capability.capability_val[j].value);
                }
                free(device_capability->device);
                free(device_capability);
        }
        free(scsi_info->scsi_info.scsi_info_val);
        free(scsi_info);
}

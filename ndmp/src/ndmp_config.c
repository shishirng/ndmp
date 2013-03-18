/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

#include <ndmp.h>
#include <comm.h>

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

void ndmp_config_get_host_info(struct client_txn *txn, struct ndmp_header header, XDR* request_stream)
{
	/*  NDMP_CONFIG_GET_HOST_INFO
	 *
	 *  no request arguments
	 *
	 * struct ndmp_config_get_host_info_reply
	 * {
	 *	ndmp_error 	error;
	 *	string 		hostname<>;
	 *	string 		os_type<>;
	 *	string 		os_vers<>;
	 *	string 		hostid<>;
	 * };
	 */
}

void ndmp_config_get_connection_type(struct client_txn *txn, struct ndmp_header header, XDR* request_stream) 
{
	/*  NDMP_CONFIG_GET_CONNECTION_TYPE
	 *
	 *  no request arguments
	 *
	 * enum ndmp_addr_type
	 * {
	 * 	NDMP_ADDR_LOCAL,
	 * 	NDMP_ADDR_TCP,
	 * 	NDMP_ADDR_FC,
	 * 	NDMP_ADDR_IPC
	 * };
	 *
	 * struct ndmp_config_get_connection_type_reply
	 * {
	 * 	ndmp_error 	error;
	 * 	ndmp_addr_type 	addr_types<>;
	 * };
	 *
	 */
}

void ndmp_config_get_auth_attr(struct client_txn *txn, struct ndmp_header header, XDR* request_stream) 
{
	/* /* NDMP_CONFIG_GET_AUTH_ATTR
	 *
	 * struct ndmp_config_get_auth_attr_request
	 * {
	 * 	ndmp_auth_type auth_type;
	 * };
	 *
	 * struct ndmp_config_get_auth_attr_reply
	 * {
	 * 	ndmp_error error;
	 * 	ndmp_auth_attr server_attr;
	 * };
	 */

}

void ndmp_config_get_butype_info(struct client_txn *txn, struct ndmp_header header, XDR* request_stream) 
{
	/* NDMP_CONFIG_GET_BUTYPE_INFO
	 *
	 * No request arguments
	 *
	 * backup type attributes
	 * const NDMP_BTYPE_BACKUP_FILE_HISTORY 	= 0x0001;
	 * const NDMP_BUTYPE_BACKUP_FILELIST 		= 0x0002;
	 * const NDMP_BUTYPE_RECOVER_FILELIST 		= 0x0004;
	 * const NDMP_BUTYPE_BACKUP_DIRECT 		= 0x0008;
	 * const NDMP_BUTYPE_RECOVER_DIRECT 		= 0x0010;
	 * const NDMP_BUTYPE_BACKUP_INCREMENTAL 	= 0x0020;
	 * const NDMP_BUTYPE_RECOVER_INCREMENTAL 	= 0x0040;
	 * const NDMP_BUTYPE_BACKUP_UTF8 		= 0x0080;
	 * const NDMP_BUTYPE_RECOVER_UTF8 		= 0x0100;
	 *
	 * struct ndmp_butype_info
	 * {
	 * 	string 		butype_name <>;
	 *	ndmp_pval 	default_env <>;
	 *	u_long 		attrs;
	 * };
	 * struct ndmp_config_get_butype_attr_reply
	 * {
	 *	ndmp_error 		error;
	 *	ndmp_butype_info 	butype_info <>;
	 * };
	 */

}

void ndmp_config_get_fs_info(struct client_txn *txn, struct ndmp_header header,  XDR* request_stream) 
{
	/* /* NDMP_CONFIG_GET_FS_INFO
	 *
	 * No request arguments
	 *
	 * const NDMP_FS_INFO_TOTAL_SIZE_INVALID 	= 0x00000001;
	 * const NDMP_FS_INFO_USED_SIZE_INVALID		= 0x00000002;
	 * const NDMP_FS_INFO_AVAIL_SIZE_INVALID 	= 0x00000004;
	 * const NDMP_FS_INFO_TOTOAL_INODES_INVALID 	= 0x00000008;
	 * const NDMP_FS_INFO_USED_INNODES_INVALID 	= 0x00000010;
	 *
	 * struct ndmp_fs_info
	 * {
	 * 	u_long 		invalid; /* invalid bit
	 * 	string 		fs_type <>;
	 *	string 		fs_logical_device <>;
	 * 	string 		fs_physical_device <>;
	 *	ndmp_u_quad 	total_size;
	 *	ndmp_u_quad 	used_size;
	 * 	ndmp_u_quad 	avail_size;
 	 *	ndmp_u_quad 	total_inodes;
	 *	ndmp_u_quad 	used_inodes;
	 *	ndmp_pval 	fs_env <>;
	 *	string		fs_status<>;
 	 * };
	 *
	 * struct ndmp_config_get_fs_info_reply
	 * {
	 * 	ndmp_error 	error;
	 * 	ndmp_fs_info 	fs_info <>;
	 * };
	 */

}

void ndmp_config_get_tape_info(struct client_txn *txn, struct ndmp_header header,  XDR* request_stream) 
{
	/* NDMP_CONFIG_GET_TAPE_INFO
	 *
	 * tape attributes
	 *
	 * const NDMP_TAPE_ATTR_REWIND	= 0x00000001;
	 * const NDMP_TAPE_ATTR_UNLOAD	= 0x00000002;
	 *
	 * No request arguments
	 *
	 * struct ndmp_device_capability
	 * {
	 *	string 		device <>;
	 * 	u_long 		attr;
	 * 	ndmp_pval 	capability <>;
	 * };
	 *
	 * struct ndmp_device_info
	 * {
	 * 	string 			model <>;
	 * 	ndmp_device_capability 	caplist <>;
	 * };
	 *
	 * struct ndmp_config_get_tape_info_reply
	 * {
	 * 	ndmp_error	 error;
	 * 	ndmp_device_info tape_info <>;
	 * };
	 */

}

void ndmp_config_get_scsi_info(struct client_txn *txn, struct ndmp_header header, XDR* request_stream) 
{
	/* NDMP_CONFIG_GET_SCSI_INFO
	 *
	 * No request arguments
	 *
	 * no SCSI attributes
	 *
	 * struct ndmp_config_get_scsi_info_reply
	 * {
	 * ndmp_error		error;
	 * ndmp_device_info	scsi_info <>;
	 * };
	 */

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
}

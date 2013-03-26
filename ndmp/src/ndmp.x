/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

/*                                                         -*- Mode: C -*- 
 * ndmp.x
 * 
 * Description   : NDMP protocol rpcgen file.
 * 
 * Copyright (c) 1999 Intelliguard Software, Network Appliance. All Rights Reserved.
 *
 * $Id: ndmp.x,v 1.11 1998/05/26 03:52:12 tim Exp $
 */


#define VER 3
const NDMPVER = VER;
const NDMPPORT = 10000;

struct ndmp_u_quad
{
        u_long  high;
        u_long  low;
};

struct ndmp_pval
{
        string  name<>;
        string  value<>;
};

enum ndmp_error
{
        NDMP_NO_ERR,                            /* No error */
        NDMP_NOT_SUPPORTED_ERR,         /* Call is not supported */
        NDMP_DEVICE_BUSY_ERR,           /* The device is in use */
        NDMP_DEVICE_OPENED_ERR,         /* Another tape or scsi device  
                                                                 *is already open */
        NDMP_NOT_AUTHORIZED_ERR,        /* connection has not been authorized */
        NDMP_PERMISSION_ERR,            /* some sort of permission problem */
        NDMP_DEV_NOT_OPEN_ERR,          /* SCSI device is not open */
        NDMP_IO_ERR,                            /* I/O error */
        NDMP_TIMEOUT_ERR,                       /* command timed out */
        NDMP_ILLEGAL_ARGS_ERR,          /* illegal arguments in request */
        NDMP_NO_TAPE_LOADED_ERR,        /* Cannot open because there is 
                                                                 * no tape loaded */
        NDMP_WRITE_PROTECT_ERR,         /* tape cannot be open for write */
        NDMP_EOF_ERR,                           /* Command encountered EOF */
        NDMP_EOM_ERR,                           /* Command encountered EOM */
        NDMP_FILE_NOT_FOUND_ERR,        /* File not found during restore */
        NDMP_BAD_FILE_ERR,                      /* The file descriptor is invalid */
        NDMP_NO_DEVICE_ERR,                     /* The device is not at that target */
        NDMP_NO_BUS_ERR,                        /* Invalid controller */
        NDMP_XDR_DECODE_ERR,            /* Can't decode the request argument */
        NDMP_ILLEGAL_STATE_ERR,         /* Call can't be performed at this state */
        NDMP_UNDEFINED_ERR,                     /* Undefined Error */
        NDMP_XDR_ENCODE_ERR,            /* Can't encode the reply argument */
        NDMP_NO_MEM_ERR,                        /* no memory */
        NDMP_CONNECT_ERR                        /* Error connecting to another NDMP server */

};

enum ndmp_header_message_type
{
        NDMP_MESSAGE_REQUEST,
        NDMP_MESSAGE_REPLY
};

enum ndmp_message
{
        NDMP_CONNECT_OPEN = 0x900,                      /* CONNECT INTERFACE */
        NDMP_CONNECT_CLIENT_AUTH = 0x901,
        NDMP_CONNECT_CLOSE = 0x902,
        NDMP_CONNECT_SERVER_AUTH = 0x903,

        NDMP_CONFIG_GET_HOST_INFO = 0x100,              /* CONFIG INTERFACE */
        NDMP_CONFIG_GET_CONNECTION_TYPE = 0x102,        /* NDMP_CONFIG_GET_MOVER_TYPE on v2 */
        NDMP_CONFIG_GET_AUTH_ATTR = 0x103,
        NDMP_CONFIG_GET_BUTYPE_INFO = 0x104,    /* new from v3 */
        NDMP_CONFIG_GET_FS_INFO = 0x105,                /* new from v3 */
        NDMP_CONFIG_GET_TAPE_INFO = 0x106,              /* new from v3 */
        NDMP_CONFIG_GET_SCSI_INFO = 0x107,              /* new from v3 */
        NDMP_CONFIG_GET_SERVER_INFO =0x108,             /* new from v3 */

        NDMP_SCSI_OPEN = 0x200,                         /* SCSI INTERFACE */
        NDMP_SCSI_CLOSE = 0x201,
        NDMP_SCSI_GET_STATE = 0x202,
        NDMP_SCSI_SET_TARGET = 0x203,
        NDMP_SCSI_RESET_DEVICE = 0x204,
        NDMP_SCSI_RESET_BUS = 0x205,
        NDMP_SCSI_EXECUTE_CDB = 0x206,

        NDMP_TAPE_OPEN = 0x300,                         /* TAPE INTERFACE */
        NDMP_TAPE_CLOSE = 0x301,
        NDMP_TAPE_GET_STATE = 0x302,
        NDMP_TAPE_MTIO = 0x303,
        NDMP_TAPE_WRITE = 0x304,
        NDMP_TAPE_READ = 0x305,
        NDMP_TAPE_EXECUTE_CDB = 0x307,

        NDMP_DATA_GET_STATE = 0x400,            /* DATA INTERFACE */
        NDMP_DATA_START_BACKUP = 0x401,
        NDMP_DATA_START_RECOVER = 0x402,
        NDMP_DATA_ABORT = 0x403,
        NDMP_DATA_GET_ENV = 0x404,
        NDMP_DATA_STOP = 0x407,
        NDMP_DATA_LISTEN = 0x409,                       /* new from V3 */
        NDMP_DATA_CONNECT = 0x40a,                      /* new from V3 */

        NDMP_NOTIFY_DATA_HALTED =0x501,         /* NOTIFY INTERFACE */
        NDMP_NOTIFY_CONNECTED = 0x502,
        NDMP_NOTIFY_MOVER_HALTED = 0x503,
        NDMP_NOTIFY_MOVER_PAUSED = 0x504,
        NDMP_NOTIFY_DATA_READ =0x505,

        NDMP_LOG_FILE = 0x602,                          /* LOGGING INTERFACE */
        NDMP_LOG_MESSAGE = 0x603,                       /* new from v3 */

        NDMP_FH_ADD_FILE = 0x703,                       /* FILE HISTORY INTERFACE */
        NDMP_FH_ADD_DIR = 0x704,
        NDMP_FH_ADD_NODE = 0x705,

        NDMP_MOVER_GET_STATE = 0xa00,           /* MOVER INTERFACE */
        NDMP_MOVER_LISTEN = 0xa01,
        NDMP_MOVER_CONTINUE = 0xa02,
        NDMP_MOVER_ABORT = 0xa03,
        NDMP_MOVER_STOP = 0xa04,
        NDMP_MOVER_SET_WINDOW = 0xa05,
        NDMP_MOVER_READ = 0xa06,
        NDMP_MOVER_CLOSE =0xa07,
        NDMP_MOVER_SET_RECORD_SIZE =0xa08,
        NDMP_MOVER_CONNECT =0xa09,                      /* new from V3 */


        NDMP_VENDORS_BASE = 0xf000,                     /* Reserved for the vendor 
                                                                                 * specific usage
                                                                                 * from 0xf000 to 0xfeff */

        NDMP_RESERVED_BASE = 0xff00                     /* Reserved for prototyping 
                                                                                 * from 0xff00 to 0xffff */
};

struct ndmp_header
{
        u_long                                          sequence;               /* monotonically increasing number */
        u_long                                          time_stamp;             /* time stamp of message */
        ndmp_header_message_type        message_type;   /* what type of message */
        enum ndmp_message                       message;                /* message number */
        u_long                                          reply_sequence; /* reply is in response to */
        ndmp_error                                      error;                  /* communications errors */
};

/**********************/
/*  CONNECT INTERFACE */
/**********************/

/* NDMP_CONNECT_OPEN */
struct ndmp_connect_open_request
{
        u_short protocol_version;       /* the version of protocol supported */
};

struct ndmp_connect_open_reply
{
        ndmp_error      error;
};

/* NDMP_CONNECT_CLIENT_AUTH */
enum ndmp_auth_type
{
        NDMP_AUTH_NONE,         /* no password is required */
        NDMP_AUTH_TEXT,         /* the clear text password */
        NDMP_AUTH_MD5           /* md5 */
};

struct ndmp_auth_text
{
        string  auth_id<>;
        string  auth_password<>;

};

struct ndmp_auth_md5
{
        string  auth_id<>;
        opaque  auth_digest[16];
};

union ndmp_auth_data switch (enum ndmp_auth_type auth_type)
{
        case NDMP_AUTH_NONE:
                void;
        case NDMP_AUTH_TEXT:
                struct ndmp_auth_text   auth_text;
        case NDMP_AUTH_MD5:
                struct ndmp_auth_md5    auth_md5;
};

struct ndmp_connect_client_auth_request
{
        ndmp_auth_data  auth_data;
};

struct ndmp_connect_client_auth_reply
{
        ndmp_error      error;
};


/* NDMP_CONNECT_CLOSE */
/* no request arguments */
/* no reply arguments */

/* NDMP_CONNECT_SERVER_AUTH */
union ndmp_auth_attr switch (enum ndmp_auth_type auth_type)
{
        case NDMP_AUTH_NONE:
                void;
        case NDMP_AUTH_TEXT:
                void;
        case NDMP_AUTH_MD5:
                opaque  challenge[64];
};

struct ndmp_connect_server_auth_request
{
        ndmp_auth_attr  client_attr;
};

struct ndmp_connect_server_auth_reply
{
        ndmp_error              error;
        ndmp_auth_data  server_result;
};


/********************/
/* CONFIG INTERFACE */
/********************/

/* NDMP_CONFIG_GET_HOST_INFO */
/* no request arguments */
struct ndmp_config_get_host_info_reply
{
        ndmp_error              error;
        string                  hostname<>;     /* host name */
        string                  os_type<>;      /* The operating system type (i.e.
                                                                 * SOLARIS) */
        string                  os_vers<>;      /* The version number of the OS (i.e.
                                                                 * 2.5) */
        string                  hostid<>;
};

enum ndmp_addr_type
{
        NDMP_ADDR_LOCAL,
        NDMP_ADDR_TCP,
        NDMP_ADDR_FC,
        NDMP_ADDR_IPC
};

/* NDMP_CONFIG_GET_CONNECTION_TYPE */
/* no request arguments */
struct ndmp_config_get_connection_type_reply
{
        ndmp_error              error;
        ndmp_addr_type  addr_types<>;
};

/* NDMP_CONFIG_GET_AUTH_ATTR */
struct ndmp_config_get_auth_attr_request
{
        ndmp_auth_type  auth_type;
};

struct ndmp_config_get_auth_attr_reply
{
        ndmp_error              error;
        ndmp_auth_attr  server_attr;
};

/* NDMP_CONFIG_GET_SERVER_INFO */
/* no requset arguments */
struct ndmp_config_get_server_info_reply
{
        ndmp_error              error;
        string                  vendor_name<>;
        string                  product_name<>;
        string                  revision_number<>;
        ndmp_auth_type  auth_type<>;
};

/* backup type attributes */
const NDMP_BUTYPE_BACKUP_FILE_HISTORY   = 0x0001;
const NDMP_BUTYPE_BACKUP_FILELIST               = 0x0002;
const NDMP_BUTYPE_RECOVER_FILELIST              = 0x0004;
const NDMP_BUTYPE_BACKUP_DIRECT                 = 0x0008;
const NDMP_BUTYPE_RECOVER_DIRECT                = 0x0010;
const NDMP_BUTYPE_BACKUP_INCREMENTAL    = 0x0020;
const NDMP_BUTYPE_RECOVER_INCREMENTAL   = 0x0040;
const NDMP_BUTYPE_BACKUP_UTF8                   = 0x0080;
const NDMP_BUTYPE_RECOVER_UTF8                  = 0x0100;
 
struct ndmp_butype_info
{
        string          butype_name<>;
        ndmp_pval       default_env<>;
        u_long          attrs;
};

/* NDMP_CONFIG_GET_BUTYPE_INFO */
/* no request arguments */
struct ndmp_config_get_butype_info_reply 
{
        ndmp_error                      error;
        ndmp_butype_info        butype_info<>;
};

/* invalid bit */
const   NDMP_FS_INFO_TOTAL_SIZE_INVALID         = 0x00000001;
const   NDMP_FS_INFO_USED_SIZE_INVALID          = 0x00000002;
const   NDMP_FS_INFO_AVAIL_SIZE_INVALID         = 0x00000004;
const   NDMP_FS_INFO_TOTAL_INODES_INVALID       = 0x00000008;
const   NDMP_FS_INFO_USED_INODES_INVALID        = 0x00000010;

struct ndmp_fs_info
{
        u_long          invalid;
        string          fs_type<>;
        string          fs_logical_device<>;
        string          fs_physical_device<>;
        ndmp_u_quad     total_size;
        ndmp_u_quad     used_size;
        ndmp_u_quad     avail_size;
        ndmp_u_quad     total_inodes;
        ndmp_u_quad     used_inodes;
        ndmp_pval       fs_env<>;
        string          fs_status<>;
};

/* NDMP_CONFIG_GET_FS_INFO */
/* no request arguments */
struct ndmp_config_get_fs_info_reply
{
        ndmp_error              error;
        ndmp_fs_info    fs_info<>;
};

/* NDMP_CONFIG_GET_TAPE_INFO */
/* no request arguments */
/* tape attributes */
const NDMP_TAPE_ATTR_REWIND             = 0x00000001;
const NDMP_TAPE_ATTR_UNLOAD             = 0x00000002;   

struct ndmp_device_capability
{
        string          device<>;
        u_long          attr;
        ndmp_pval       capability<>;
};

struct ndmp_device_info
{
        string                                  model<>;
        ndmp_device_capability  caplist<>;

};
struct ndmp_config_get_tape_info_reply 
{
        ndmp_error                      error;
        ndmp_device_info        tape_info<>;

};

/* NDMP_CONFIG_GET_SCSI_INFO */
/* jukebox attributes */
struct ndmp_config_get_scsi_info_reply
{
        ndmp_error                      error;
        ndmp_device_info        scsi_info<>;
};

/******************/
/* SCSI INTERFACE */
/******************/

/* NDMP_SCSI_OPEN */
struct ndmp_scsi_open_request
{
        string  device<>;
};

struct ndmp_scsi_open_reply
{
        ndmp_error      error;
};

/* NDMP_SCSI_CLOSE */
/* no request arguments */
struct ndmp_scsi_close_reply
{
        ndmp_error      error;
};

/* NDMP_SCSI_GET_STATE */
/* no request arguments */
struct ndmp_scsi_get_state_reply
{
        ndmp_error      error;
        short           target_controller;
        short           target_id;
        short           target_lun;
};

/* NDMP_SCSI_SET_TARGET */
struct ndmp_scsi_set_target_request
{
        string          device<>;
        u_short         target_controller;
        u_short         target_id;
        u_short         target_lun;
};

struct ndmp_scsi_set_target_reply
{
        ndmp_error      error;
};

/* NDMP_SCSI_RESET_DEVICE */
/* no request arguments */
struct ndmp_scsi_reset_device_reply
{
        ndmp_error      error;
};

/* NDMP_SCSI_RESET_BUS */
/* no request arguments */
struct ndmp_scsi_reset_bus_reply
{
        ndmp_error      error;
};

/* NDMP_SCSI_EXECUTE_CDB */
const NDMP_SCSI_DATA_IN         = 0x00000001;   /* Expect data from SCSI device */
const NDMP_SCSI_DATA_OUT        = 0x00000002;   /* Transfer data to SCSI device */

struct ndmp_execute_cdb_request
{
        u_long  flags;
        u_long  timeout;
        u_long  datain_len;             /* Set for expected datain */
        opaque  cdb<>;
        opaque  dataout<>;
};

struct ndmp_execute_cdb_reply
{
        ndmp_error      error;
        u_char          status;                 /* SCSI status bytes */
        u_long          dataout_len;
        opaque          datain<>;               /* SCSI datain */
        opaque          ext_sense<>;    /* Extended sense data */
};

/******************/
/* TAPE INTERFACE */
/******************/
/* NDMP_TAPE_OPEN */
enum ndmp_tape_open_mode
{
        NDMP_TAPE_READ_MODE,
        NDMP_TAPE_RDWR_MODE
};

struct ndmp_tape_open_request
{
        string  device<>;
        ndmp_tape_open_mode     mode;
};

struct ndmp_tape_open_reply
{
        ndmp_error      error;
};

/* NDMP_TAPE_CLOSE */
/* no request arguments */
struct ndmp_tape_close_reply
{
        ndmp_error      error;
};

/*NDMP_TAPE_GET_STATE */
/* no request arguments */
const NDMP_TAPE_STATE_NOREWIND  = 0x0008;       /* non-rewind device */
const NDMP_TAPE_STATE_WR_PROT   = 0x0010;       /* write-protected */
const NDMP_TAPE_STATE_ERROR             = 0x0020;       /* media error */
const NDMP_TAPE_STATE_UNLOAD    = 0x0040;       /* tape will be unloaded when
                                                                                         * the device is closed */

/* invalid bit */
const NDMP_TAPE_STATE_FILE_NUM_INVALID          = 0x00000001;
const NDMP_TAPE_STATE_SOFT_ERRORS_INVALID       = 0x00000002;
const NDMP_TAPE_STATE_BLOCK_SIZE_INVALID        = 0x00000004;
const NDMP_TAPE_STATE_BLOCKNO_INVALID           = 0x00000008;
const NDMP_TAPE_STATE_TOTAL_SPACE_INVALID       = 0x00000010;
const NDMP_TAPE_STATE_SPACE_REMAIN_INVALID      = 0x00000020;
const NDMP_TAPE_STATE_PARTITION_INVALID         = 0x00000040;

struct ndmp_tape_get_state_reply
{
        u_long          invalid;
        ndmp_error      error;
        u_long          flags;
        u_long          file_num;
        u_long          soft_errors;
        u_long          block_size;
        u_long          blockno;
        ndmp_u_quad     total_space;
        ndmp_u_quad     space_remain;
        u_long          partition;
};

/* NDMP_TAPE_MTIO */
enum ndmp_tape_mtio_op
{
        NDMP_MTIO_FSF,
        NDMP_MTIO_BSF,
        NDMP_MTIO_FSR,
        NDMP_MTIO_BSR,
        NDMP_MTIO_REW,
        NDMP_MTIO_EOF,
        NDMP_MTIO_OFF
};

struct ndmp_tape_mtio_request
{
        ndmp_tape_mtio_op       tape_op;
        u_long                          count;
};

struct ndmp_tape_mtio_reply
{
        ndmp_error      error;
        u_long          resid_count;
};

/* NDMP_TAPE_WRITE */
struct ndmp_tape_write_request
{
        opaque  data_out<>;
};

struct ndmp_tape_write_reply
{
        ndmp_error      error;
        u_long          count;
};

/* NDMP_TAPE_READ */
struct ndmp_tape_read_request
{
        u_long  count;
};

struct ndmp_tape_read_reply
{
        ndmp_error      error;
        opaque          data_in<>;
};


/* NDMP_TAPE_EXECUTE_CDB */
typedef ndmp_execute_cdb_request        ndmp_tape_execute_cdb_request;
typedef ndmp_execute_cdb_reply          ndmp_tape_execute_cdb_reply;


/********************************/
/* MOVER INTERFACE              */
/********************************/
/* NDMP_MOVER_GET_STATE */
enum ndmp_mover_state
{
        NDMP_MOVER_STATE_IDLE,
        NDMP_MOVER_STATE_LISTEN,
        NDMP_MOVER_STATE_ACTIVE,
        NDMP_MOVER_STATE_PAUSED,
        NDMP_MOVER_STATE_HALTED
};

enum ndmp_mover_pause_reason
{
        NDMP_MOVER_PAUSE_NA,
        NDMP_MOVER_PAUSE_EOM,
        NDMP_MOVER_PAUSE_EOF,
        NDMP_MOVER_PAUSE_SEEK,
        NDMP_MOVER_PAUSE_MEDIA_ERROR,
        NDMP_MOVER_PAUSE_EOW
};

enum ndmp_mover_halt_reason
{
        NDMP_MOVER_HALT_NA,
        NDMP_MOVER_HALT_CONNECT_CLOSED,
        NDMP_MOVER_HALT_ABORTED,
        NDMP_MOVER_HALT_INTERNAL_ERROR,
        NDMP_MOVER_HALT_CONNECT_ERROR
};

/* mover address */
enum ndmp_mover_mode
{
        NDMP_MOVER_MODE_READ,   /* read from data connection; write to tape */
        NDMP_MOVER_MODE_WRITE   /* write to data connection; read from tape */
};

struct ndmp_tcp_addr
{
        u_long  ip_addr;
        u_short port;
};

struct ndmp_fc_addr
{
        u_long  loop_id;
};

struct ndmp_ipc_addr
{
        opaque  comm_data<>;
};
union ndmp_addr switch (ndmp_addr_type addr_type)
{
        case NDMP_ADDR_LOCAL:
                void;
        case NDMP_ADDR_TCP:
                ndmp_tcp_addr   tcp_addr;
        case NDMP_ADDR_FC:
                ndmp_fc_addr    fc_addr;
        case NDMP_ADDR_IPC:
                ndmp_ipc_addr   ipc_addr;
        
};

/* no request arguments */
struct ndmp_mover_get_state_reply
{
        ndmp_error                              error;
        ndmp_mover_state                state;
        ndmp_mover_pause_reason pause_reason;
        ndmp_mover_halt_reason  halt_reason;
        u_long                                  record_size;
        u_long                                  record_num;
        ndmp_u_quad                             data_written;
        ndmp_u_quad                             seek_position;
        ndmp_u_quad                             bytes_left_to_read;
        ndmp_u_quad                             window_offset;
        ndmp_u_quad                             window_length;
        ndmp_addr                               data_connection_addr;
};

/* NDMP_MOVER_LISTEN */
struct ndmp_mover_listen_request
{
        ndmp_mover_mode         mode;
        ndmp_addr_type  addr_type;
};

struct ndmp_mover_listen_reply
{
        ndmp_error      error;
        ndmp_addr       data_connection_addr;
};

/* NDMP_MOVER_CONNECT */
struct ndmp_mover_connect_request
{
        ndmp_mover_mode         mode;
        ndmp_addr               addr;
};

struct ndmp_mover_connect_reply
{
        ndmp_error      error;
};
/* NDMP_MOVER_SET_RECORD_SIZE */
struct ndmp_mover_set_record_size_request
{
        u_long  len;
};

struct ndmp_mover_set_record_size_reply
{
        ndmp_error      error;
};

/* NDMP_MOVER_SET_WINDOW */
struct ndmp_mover_set_window_request
{
        ndmp_u_quad     offset;
        ndmp_u_quad     length;
};

struct ndmp_mover_set_window_reply
{
        ndmp_error      error;
};

/* NDMP_MOVER_CONTINUE */
/* no request arguments */
struct ndmp_mover_continue_reply
{
        ndmp_error      error;
};


/* NDMP_MOVER_ABORT */
/* no request arguments */
struct ndmp_mover_abort_reply
{
        ndmp_error      error;
};

/* NDMP_MOVER_STOP */
/* no request arguments */
struct ndmp_mover_stop_reply
{
        ndmp_error      error;
};

/* NDMP_MOVER_READ */
struct ndmp_mover_read_request
{
        ndmp_u_quad     offset;
        ndmp_u_quad     length;
};

struct ndmp_mover_read_reply
{
        ndmp_error      error;
};

/* NDMP_MOVER_CLOSE */
/* no request arguments */
struct ndmp_mover_close_reply
{
        ndmp_error      error;
};

/********************************/
/* DATA INTERFACE                           */
/********************************/
/* NDMP_DATA_GET_STATE */
/* no request arguments */
enum ndmp_data_operation
{
        NDMP_DATA_OP_NOACTION,
        NDMP_DATA_OP_BACKUP,
        NDMP_DATA_OP_RESTORE
};

enum ndmp_data_state
{
        NDMP_DATA_STATE_IDLE,
        NDMP_DATA_STATE_ACTIVE,
        NDMP_DATA_STATE_HALTED,
        NDMP_DATA_STATE_LISTEN,
        NDMP_DATA_STATE_CONNECTED
};

enum ndmp_data_halt_reason
{
        NDMP_DATA_HALT_NA,
        NDMP_DATA_HALT_SUCCESSFUL,
        NDMP_DATA_HALT_ABORTED,
        NDMP_DATA_HALT_INTERNAL_ERROR,
        NDMP_DATA_HALT_CONNECT_ERROR
};

/* invalid bit */
const NDMP_DATA_STATE_EST_BYTES_REMAIN_INVALID  = 0x00000001;
const NDMP_DATA_STATE_EST_TIME_REMAIN_INVALID   = 0x00000002;

struct ndmp_data_get_state_reply
{
        u_long                                  invalid;
        ndmp_error                              error;
        ndmp_data_operation             operation;
        ndmp_data_state                 state;
        ndmp_data_halt_reason   halt_reason;
        ndmp_u_quad                             bytes_processed;
        ndmp_u_quad                             est_bytes_remain;
        u_long                                  est_time_remain;
        ndmp_addr                               data_connection_addr;
        ndmp_u_quad                             read_offset;
        ndmp_u_quad                             read_length;
};

/* NDMP_DATA_START_BACKUP */
struct ndmp_data_start_backup_request
{
        string          bu_type<>;      /* backup method to use */
        ndmp_pval       env<>;          /* Parameters that may modify backup */
};

struct ndmp_data_start_backup_reply
{
        ndmp_error      error;
};

/* NDMP_DATA_START_RECOVER */
struct ndmp_name
{
        string          original_path<>;
        string          destination_dir<>;
        string          new_name<>;     /* Direct access restore only */
        string          other_name<>;   /* Direct access restore only */
        ndmp_u_quad     node;           /* Direct access restore only */
        ndmp_u_quad     fh_info;        /* Direct access restore only */
};

struct ndmp_data_start_recover_request
{
        ndmp_pval       env<>;
        ndmp_name       nlist<>;
        string          bu_type<>;
};

struct ndmp_data_start_recover_reply
{
        ndmp_error      error;
};

/* NDMP_DATA_ABORT */
/* no request arguments */
struct ndmp_data_abort_reply
{
        ndmp_error      error;
};

/* NDMP_DATA_STOP */
/* no request arguments */
struct ndmp_data_stop_reply
{
        ndmp_error      error;
};

/* NDMP_DATA_GET_ENV */
/* no request arguments */
struct ndmp_data_get_env_reply
{
        ndmp_error      error;
        ndmp_pval       env<>;
};

/* NDMP_DATA_LISTEN */
struct ndmp_data_listen_request
{
        ndmp_addr_type  addr_type;
};

struct ndmp_data_listen_reply
{
        ndmp_error      error;
        ndmp_addr       data_connection_addr;
};

/* NDMP_DATA_CONNECT */
struct ndmp_data_connect_request
{
        ndmp_addr       addr;
};
struct ndmp_data_connect_reply
{
        ndmp_error      error;
};

/****************************************/
/* NOTIFY INTERFACE                     */
/****************************************/
/* NDMP_NOTIFY_DATA_HALTED */
struct ndmp_notify_data_halted_request
{
        ndmp_data_halt_reason   reason;
        string                                  text_reason<>;
};

/* NDMP_NOTIFY_CONNECTED */
enum ndmp_connect_reason
{
        NDMP_CONNECTED,         /* Connect sucessfully */
        NDMP_SHUTDOWN,          /* Connection shutdown */
        NDMP_REFUSED            /* reach the maximum number of connections */
};

struct ndmp_notify_connected_request
{
        ndmp_connect_reason     reason;
        u_short                         protocol_version;
        string                          text_reason<>;
};

/* NDMP_NOTIFY_MOVER_PAUSED */
struct ndmp_notify_mover_paused_request
{
        ndmp_mover_pause_reason reason;
        ndmp_u_quad                             seek_position;
};
/* No reply */

/* NDMP_NOTIFY_MOVER_HALTED */
struct ndmp_notify_mover_halted_request
{
        ndmp_mover_halt_reason  reason;
        string                                  text_reason<>;
};
/* No reply */

/* NDMP_NOTIFY_DATA_READ */
struct ndmp_notify_data_read_request
{
        ndmp_u_quad     offset;
        ndmp_u_quad     length;
};
/* No reply */

/********************************/
/* LOG INTERFACE                                */
/********************************/
/* NDMP_LOG_MESSAGE */
enum ndmp_log_type
{
        NDMP_LOG_NORMAL,
        NDMP_LOG_DEBUG,
        NDMP_LOG_ERROR,
        NDMP_LOG_WARNING
};


struct ndmp_log_message_request
{
        ndmp_log_type   log_type;
        u_long                  message_id;
        string                  entry<>;
};
/* No reply */

/* NDMP_LOG_FILE */
struct ndmp_log_file_request
{
        string          name<>;
        ndmp_error      error;
};
/* No reply */


/********************************/
/* File History INTERFACE           */
/********************************/
/* NDMP_FH_ADD_FILE */
enum ndmp_fs_type
{
        NDMP_FS_UNIX,   /* UNIX */
        NDMP_FS_NT,     /* NT */
        NDMP_FS_OTHER
};

typedef string ndmp_path<>;
struct ndmp_nt_path
{
        ndmp_path       nt_path;
        ndmp_path       dos_path;
};
 
union ndmp_file_name switch (ndmp_fs_type fs_type)
{
        case NDMP_FS_UNIX:
                ndmp_path               unix_name;
        case NDMP_FS_NT:
                ndmp_nt_path    nt_name;
        default:
                ndmp_path               other_name;
};

enum ndmp_file_type
{
        NDMP_FILE_DIR,
        NDMP_FILE_FIFO,
        NDMP_FILE_CSPEC,
        NDMP_FILE_BSPEC,
        NDMP_FILE_REG,
        NDMP_FILE_SLINK,
        NDMP_FILE_SOCK,
        NDMP_FILE_REGISTRY,
        NDMP_FILE_OTHER
};

/* invalid bit */
const NDMP_FILE_STAT_ATIME_INVALID      = 0x00000001;
const NDMP_FILE_STAT_CTIME_INVALID      = 0x00000002;
const NDMP_FILE_STAT_GROUP_INVALID      = 0x00000004;

struct ndmp_file_stat
{
        u_long                  invalid;
        ndmp_fs_type    fs_type;
        ndmp_file_type  ftype;
        u_long                  mtime;
        u_long                  atime;
        u_long                  ctime;
        u_long                  owner; /* uid for UNIX, owner for NT */
        u_long                  group; /* gid for UNIX, NA for NT */
        u_long                  fattr; /* mode for UNIX, fattr for NT */
        ndmp_u_quad             size;
        u_long                  links;
};


/* one file could have both UNIX and NT name and attributes */
struct ndmp_file
{
        ndmp_file_name  names<>;
        ndmp_file_stat  stats<>;
        ndmp_u_quad             node;           /* used for the direct access */
        ndmp_u_quad             fh_info;        /* used for the direct access */
};

struct ndmp_fh_add_file_request
{
        ndmp_file               files<>;
};

/* No reply */
/* NDMP_FH_ADD_DIR */

struct ndmp_dir
{
        ndmp_file_name  names<>;
        ndmp_u_quad             node;
        ndmp_u_quad             parent;
};
 
struct ndmp_fh_add_dir_request
{
        ndmp_dir        dirs<>;
};
/* No reply */
 
/* NDMP_FH_ADD_NODE */
struct ndmp_node
{
        ndmp_file_stat  stats<>;
        ndmp_u_quad             node;
        ndmp_u_quad             fh_info;
};
 
struct ndmp_fh_add_node_request
{
        ndmp_node       nodes<>;
};
/* No reply */


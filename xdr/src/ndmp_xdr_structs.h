/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

#ifndef __H_NDMP__
#define __H_NDMP__

/*
 * Type definitions based on NDMP V3 spec
 */

enum ndmp_error {
	NDMP_NO_ERR = 0,
	NDMP_NOT_SUPPORTED_ERR = 1,
	NDMP_DEVICE_BUSY_ERR = 2,
	NDMP_DEVICE_OPENED_ERR = 3,
	NDMP_NOT_AUTHORIZED_ERR = 4,
	NDMP_PERMISSION_ERR = 5,
	NDMP_DEV_NOT_OPEN_ERR = 6,
	NDMP_IO_ERR = 7,
	NDMP_TIMEOUT_ERR = 8,
	NDMP_ILLEGAL_ARGS_ERR = 9,
	NDMP_NO_TAPE_LOADED_ERR = 10,
	NDMP_WRITE_PROTECT_ERR = 11,
	NDMP_EOF_ERR = 12,
	NDMP_EOM_ERR = 13,
	NDMP_FILE_NOT_FOUND_ERR = 14,
	NDMP_BAD_FILE_ERR = 15,
	NDMP_NO_DEVICE_ERR = 16,
	NDMP_NO_BUS_ERR = 17,
	NDMP_XDR_DECODE_ERR = 18,
	NDMP_ILLEGAL_STATE_ERR = 19,
	NDMP_UNDEFINED_ERR = 20,
	NDMP_XDR_ENCODE_ERR = 21,
	NDMP_NO_MEM_ERR = 22,
	NDMP_CONNECT_ERR = 23,
};

typedef enum ndmp_error ndmp_error;

enum ndmp_header_message_type {
	NDMP_MESSAGE_REQUEST = 0, NDMP_MESSAGE_REPLY = 1,
};
typedef enum ndmp_header_message_type ndmp_header_message_type;

enum ndmp_message {
	NDMP_CONNECT_OPEN = 0x900,
	NDMP_CONNECT_CLIENT_AUTH = 0x901,
	NDMP_CONNECT_CLOSE = 0x902,
	NDMP_CONNECT_SERVER_AUTH = 0x903,
	NDMP_CONFIG_GET_HOST_INFO = 0x100,
	NDMP_CONFIG_GET_CONNECTION_TYPE = 0x102,
	NDMP_CONFIG_GET_AUTH_ATTR = 0x103,
	NDMP_CONFIG_GET_BUTYPE_INFO = 0x104,
	NDMP_CONFIG_GET_FS_INFO = 0x105,
	NDMP_CONFIG_GET_TAPE_INFO = 0x106,
	NDMP_CONFIG_GET_SCSI_INFO = 0x107,
	NDMP_CONFIG_GET_SERVER_INFO = 0x108,
	NDMP_SCSI_OPEN = 0x200,
	NDMP_SCSI_CLOSE = 0x201,
	NDMP_SCSI_GET_STATE = 0x202,
	NDMP_SCSI_SET_TARGET = 0x203,
	NDMP_SCSI_RESET_DEVICE = 0x204,
	NDMP_SCSI_RESET_BUS = 0x205,
	NDMP_SCSI_EXECUTE_CDB = 0x206,
	NDMP_TAPE_OPEN = 0x300,
	NDMP_TAPE_CLOSE = 0x301,
	NDMP_TAPE_GET_STATE = 0x302,
	NDMP_TAPE_MTIO = 0x303,
	NDMP_TAPE_WRITE = 0x304,
	NDMP_TAPE_READ = 0x305,
	NDMP_TAPE_EXECUTE_CDB = 0x307,
	NDMP_DATA_GET_STATE = 0x400,
	NDMP_DATA_START_BACKUP = 0x401,
	NDMP_DATA_START_RECOVER = 0x402,
	NDMP_DATA_ABORT = 0x403,
	NDMP_DATA_GET_ENV = 0x404,
	NDMP_DATA_STOP = 0x407,
	NDMP_DATA_LISTEN = 0x409,
	NDMP_DATA_CONNECT = 0x40a,
	NDMP_NOTIFY_DATA_HALTED = 0x501,
	NDMP_NOTIFY_CONNECTED = 0x502,
	NDMP_NOTIFY_MOVER_HALTED = 0x503,
	NDMP_NOTIFY_MOVER_PAUSED = 0x504,
	NDMP_NOTIFY_DATA_READ = 0x505,
	NDMP_LOG_FILE = 0x602,
	NDMP_LOG_MESSAGE = 0x603,
	NDMP_FH_ADD_FILE = 0x703,
	NDMP_FH_ADD_DIR = 0x704,
	NDMP_FH_ADD_NODE = 0x705,
	NDMP_MOVER_GET_STATE = 0xa00,
	NDMP_MOVER_LISTEN = 0xa01,
	NDMP_MOVER_CONTINUE = 0xa02,
	NDMP_MOVER_ABORT = 0xa03,
	NDMP_MOVER_STOP = 0xa04,
	NDMP_MOVER_SET_WINDOW = 0xa05,
	NDMP_MOVER_READ = 0xa06,
	NDMP_MOVER_CLOSE = 0xa07,
	NDMP_MOVER_SET_RECORD_SIZE = 0xa08,
	NDMP_MOVER_CONNECT = 0xa09,
	NDMP_VENDORS_BASE = 0xf000,
	NDMP_RESERVED_BASE = 0xff00,
};

typedef enum ndmp_message ndmp_message;

struct ndmp_header {
	u_long sequence;
	u_long time_stamp;
	ndmp_header_message_type message_type;
	enum ndmp_message message;
	u_long reply_sequence;
	ndmp_error error;
};

typedef struct ndmp_header ndmp_header;

struct ndmp_connect_open_request {
	u_short protocol_version;
};
typedef struct ndmp_connect_open_request ndmp_connect_open_request;

struct ndmp_connect_open_reply {
	ndmp_error error;
};
typedef struct ndmp_connect_open_reply ndmp_connect_open_reply;

enum ndmp_auth_type {
	NDMP_AUTH_NONE = 0, NDMP_AUTH_TEXT = 1, NDMP_AUTH_MD5 = 2,
};
typedef enum ndmp_auth_type ndmp_auth_type;

struct ndmp_auth_text {
	char *auth_id;
	char *auth_password;
};
typedef struct ndmp_auth_text ndmp_auth_text;

struct ndmp_auth_md5 {
	char *auth_id;
	char auth_digest[16];
};
typedef struct ndmp_auth_md5 ndmp_auth_md5;

struct ndmp_auth_data {
	ndmp_auth_type auth_type;
	union {
		struct ndmp_auth_text auth_text;
		struct ndmp_auth_md5 auth_md5;
	} ndmp_auth_data_u;
};

typedef struct ndmp_auth_data ndmp_auth_data;

struct ndmp_connect_client_auth_request {
	ndmp_auth_data auth_data;
};

typedef struct ndmp_connect_client_auth_request ndmp_connect_client_auth_request;

struct ndmp_connect_client_auth_reply {
	ndmp_error error;
};

typedef struct ndmp_connect_client_auth_reply ndmp_connect_client_auth_reply;

struct ndmp_auth_attr {
	ndmp_auth_type auth_type;
	union {
		char challenge[64];
	} ndmp_auth_attr_u;
};

typedef struct ndmp_auth_attr ndmp_auth_attr;

struct ndmp_connect_server_auth_request {
	ndmp_auth_attr client_attr;
};

typedef struct ndmp_connect_server_auth_request ndmp_connect_server_auth_request;

struct ndmp_connect_server_auth_reply {
	ndmp_error error;
	ndmp_auth_data server_result;
};

typedef struct ndmp_connect_server_auth_reply ndmp_connect_server_auth_reply;

struct ndmp_config_get_host_info_reply {
	ndmp_error error;
	char *hostname;
	char *os_type;
	char *os_vers;
	char *hostid;
};
typedef struct ndmp_config_get_host_info_reply ndmp_config_get_host_info_reply;

enum ndmp_addr_type {
	NDMP_ADDR_LOCAL = 0,
	NDMP_ADDR_TCP = 1,
	NDMP_ADDR_FC = 2,
	NDMP_ADDR_IPC = 3,
};
typedef enum ndmp_addr_type ndmp_addr_type;

struct ndmp_config_get_connection_type_reply {
	ndmp_error error;
	struct {
		u_int addr_types_len;
		ndmp_addr_type *addr_types_val;
	} addr_types;
};
typedef struct ndmp_config_get_connection_type_reply ndmp_config_get_connection_type_reply;

struct ndmp_config_get_auth_attr_request {
	ndmp_auth_type auth_type;
};

typedef struct ndmp_config_get_auth_attr_request ndmp_config_get_auth_attr_request;

struct ndmp_config_get_auth_attr_reply {
	ndmp_error error;
	ndmp_auth_attr server_attr;
};

typedef struct ndmp_config_get_auth_attr_reply ndmp_config_get_auth_attr_reply;

struct ndmp_config_get_server_info_reply {
	ndmp_error error;
	char *vendor_name;
	char *product_name;
	char *revision_number;
	struct {
		u_int auth_type_len;
		ndmp_auth_type *auth_type_val;
	} auth_type;
};

typedef struct ndmp_config_get_server_info_reply ndmp_config_get_server_info_reply;

#define NDMP_BUTYPE_BACKUP_FILE_HISTORY 0x0001
#define NDMP_BUTYPE_BACKUP_FILELIST 0x0002
#define NDMP_BUTYPE_RECOVER_FILELIST 0x0004
#define NDMP_BUTYPE_BACKUP_DIRECT 0x0008
#define NDMP_BUTYPE_RECOVER_DIRECT 0x0010
#define NDMP_BUTYPE_BACKUP_INCREMENTAL 0x0020
#define NDMP_BUTYPE_RECOVER_INCREMENTAL 0x0040
#define NDMP_BUTYPE_BACKUP_UTF8 0x0080
#define NDMP_BUTYPE_RECOVER_UTF8 0x0100

struct ndmp_butype_info {
	char *butype_name;
	struct {
		u_int default_env_len;
		ndmp_pval *default_env_val;
	} default_env;
	u_long attrs;
};
typedef struct ndmp_butype_info ndmp_butype_info;

struct ndmp_config_get_butype_info_reply {
	ndmp_error error;
	struct {
		u_int butype_info_len;
		ndmp_butype_info *butype_info_val;
	} butype_info;
};
typedef struct ndmp_config_get_butype_info_reply ndmp_config_get_butype_info_reply;
#define NDMP_FS_INFO_TOTAL_SIZE_INVALID 0x00000001
#define NDMP_FS_INFO_USED_SIZE_INVALID 0x00000002
#define NDMP_FS_INFO_AVAIL_SIZE_INVALID 0x00000004
#define NDMP_FS_INFO_TOTAL_INODES_INVALID 0x00000008
#define NDMP_FS_INFO_USED_INODES_INVALID 0x00000010

struct ndmp_fs_info {
	u_long invalid;
	char *fs_type;
	char *fs_logical_device;
	char *fs_physical_device;
	ndmp_u_quad total_size;
	ndmp_u_quad used_size;
	ndmp_u_quad avail_size;
	ndmp_u_quad total_inodes;
	ndmp_u_quad used_inodes;
	struct {
		u_int fs_env_len;
		ndmp_pval *fs_env_val;
	} fs_env;
	char *fs_status;
};
typedef struct ndmp_fs_info ndmp_fs_info;

struct ndmp_config_get_fs_info_reply {
	ndmp_error error;
	struct {
		u_int fs_info_len;
		ndmp_fs_info *fs_info_val;
	} fs_info;
};
typedef struct ndmp_config_get_fs_info_reply ndmp_config_get_fs_info_reply;
#define NDMP_TAPE_ATTR_REWIND 0x00000001
#define NDMP_TAPE_ATTR_UNLOAD 0x00000002

struct ndmp_device_capability {
	char *device;
	u_long attr;
	struct {
		u_int capability_len;
		ndmp_pval *capability_val;
	} capability;
};
typedef struct ndmp_device_capability ndmp_device_capability;

struct ndmp_device_info {
	char *model;
	struct {
		u_int caplist_len;
		ndmp_device_capability *caplist_val;
	} caplist;
};
typedef struct ndmp_device_info ndmp_device_info;

struct ndmp_config_get_tape_info_reply {
	ndmp_error error;
	struct {
		u_int tape_info_len;
		ndmp_device_info *tape_info_val;
	} tape_info;
};
typedef struct ndmp_config_get_tape_info_reply ndmp_config_get_tape_info_reply;

struct ndmp_config_get_scsi_info_reply {
	ndmp_error error;
	struct {
		u_int scsi_info_len;
		ndmp_device_info *scsi_info_val;
	} scsi_info;
};
typedef struct ndmp_config_get_scsi_info_reply ndmp_config_get_scsi_info_reply;

struct ndmp_scsi_open_request {
	char *device;
};
typedef struct ndmp_scsi_open_request ndmp_scsi_open_request;

struct ndmp_scsi_open_reply {
	ndmp_error error;
};
typedef struct ndmp_scsi_open_reply ndmp_scsi_open_reply;

struct ndmp_scsi_close_reply {
	ndmp_error error;
};
typedef struct ndmp_scsi_close_reply ndmp_scsi_close_reply;

struct ndmp_scsi_get_state_reply {
	ndmp_error error;
	short target_controller;
	short target_id;
	short target_lun;
};
typedef struct ndmp_scsi_get_state_reply ndmp_scsi_get_state_reply;

struct ndmp_scsi_set_target_request {
	char *device;
	u_short target_controller;
	u_short target_id;
	u_short target_lun;
};
typedef struct ndmp_scsi_set_target_request ndmp_scsi_set_target_request;

struct ndmp_scsi_set_target_reply {
	ndmp_error error;
};
typedef struct ndmp_scsi_set_target_reply ndmp_scsi_set_target_reply;

struct ndmp_scsi_reset_device_reply {
	ndmp_error error;
};
typedef struct ndmp_scsi_reset_device_reply ndmp_scsi_reset_device_reply;

struct ndmp_scsi_reset_bus_reply {
	ndmp_error error;
};
typedef struct ndmp_scsi_reset_bus_reply ndmp_scsi_reset_bus_reply;
#define NDMP_SCSI_DATA_IN 0x00000001
#define NDMP_SCSI_DATA_OUT 0x00000002

struct ndmp_execute_cdb_request {
	u_long flags;
	u_long timeout;
	u_long datain_len;
	struct {
		u_int cdb_len;
		char *cdb_val;
	} cdb;
	struct {
		u_int dataout_len;
		char *dataout_val;
	} dataout;
};
typedef struct ndmp_execute_cdb_request ndmp_execute_cdb_request;

struct ndmp_execute_cdb_reply {
	ndmp_error error;
	u_char status;
	u_long dataout_len;
	struct {
		u_int datain_len;
		char *datain_val;
	} datain;
	struct {
		u_int ext_sense_len;
		char *ext_sense_val;
	} ext_sense;
};
typedef struct ndmp_execute_cdb_reply ndmp_execute_cdb_reply;

enum ndmp_tape_open_mode {
	NDMP_TAPE_READ_MODE = 0, NDMP_TAPE_RDWR_MODE = 1,
};
typedef enum ndmp_tape_open_mode ndmp_tape_open_mode;

struct ndmp_tape_open_request {
	char *device;
	ndmp_tape_open_mode mode;
};
typedef struct ndmp_tape_open_request ndmp_tape_open_request;

struct ndmp_tape_open_reply {
	ndmp_error error;
};
typedef struct ndmp_tape_open_reply ndmp_tape_open_reply;

struct ndmp_tape_close_reply {
	ndmp_error error;
};
typedef struct ndmp_tape_close_reply ndmp_tape_close_reply;
#define NDMP_TAPE_STATE_NOREWIND 0x0008
#define NDMP_TAPE_STATE_WR_PROT 0x0010
#define NDMP_TAPE_STATE_ERROR 0x0020
#define NDMP_TAPE_STATE_UNLOAD 0x0040
#define NDMP_TAPE_STATE_FILE_NUM_INVALID 0x00000001
#define NDMP_TAPE_STATE_SOFT_ERRORS_INVALID 0x00000002
#define NDMP_TAPE_STATE_BLOCK_SIZE_INVALID 0x00000004
#define NDMP_TAPE_STATE_BLOCKNO_INVALID 0x00000008
#define NDMP_TAPE_STATE_TOTAL_SPACE_INVALID 0x00000010
#define NDMP_TAPE_STATE_SPACE_REMAIN_INVALID 0x00000020
#define NDMP_TAPE_STATE_PARTITION_INVALID 0x00000040

struct ndmp_tape_get_state_reply {
	u_long invalid;
	ndmp_error error;
	u_long flags;
	u_long file_num;
	u_long soft_errors;
	u_long block_size;
	u_long blockno;
	ndmp_u_quad total_space;
	ndmp_u_quad space_remain;
	u_long partition;
};
typedef struct ndmp_tape_get_state_reply ndmp_tape_get_state_reply;

enum ndmp_tape_mtio_op {
	NDMP_MTIO_FSF = 0,
	NDMP_MTIO_BSF = 1,
	NDMP_MTIO_FSR = 2,
	NDMP_MTIO_BSR = 3,
	NDMP_MTIO_REW = 4,
	NDMP_MTIO_EOF = 5,
	NDMP_MTIO_OFF = 6,
};
typedef enum ndmp_tape_mtio_op ndmp_tape_mtio_op;

struct ndmp_tape_mtio_request {
	ndmp_tape_mtio_op tape_op;
	u_long count;
};
typedef struct ndmp_tape_mtio_request ndmp_tape_mtio_request;

struct ndmp_tape_mtio_reply {
	ndmp_error error;
	u_long resid_count;
};
typedef struct ndmp_tape_mtio_reply ndmp_tape_mtio_reply;

struct ndmp_tape_write_request {
	struct {
		u_int data_out_len;
		char *data_out_val;
	} data_out;
};
typedef struct ndmp_tape_write_request ndmp_tape_write_request;

struct ndmp_tape_write_reply {
	ndmp_error error;
	u_long count;
};
typedef struct ndmp_tape_write_reply ndmp_tape_write_reply;

struct ndmp_tape_read_request {
	u_long count;
};
typedef struct ndmp_tape_read_request ndmp_tape_read_request;

struct ndmp_tape_read_reply {
	ndmp_error error;
	struct {
		u_int data_in_len;
		char *data_in_val;
	} data_in;
};
typedef struct ndmp_tape_read_reply ndmp_tape_read_reply;

typedef ndmp_execute_cdb_request ndmp_tape_execute_cdb_request;

typedef ndmp_execute_cdb_reply ndmp_tape_execute_cdb_reply;

enum ndmp_mover_state {
	NDMP_MOVER_STATE_IDLE = 0,
	NDMP_MOVER_STATE_LISTEN = 1,
	NDMP_MOVER_STATE_ACTIVE = 2,
	NDMP_MOVER_STATE_PAUSED = 3,
	NDMP_MOVER_STATE_HALTED = 4,
};
typedef enum ndmp_mover_state ndmp_mover_state;

enum ndmp_mover_pause_reason {
	NDMP_MOVER_PAUSE_NA = 0,
	NDMP_MOVER_PAUSE_EOM = 1,
	NDMP_MOVER_PAUSE_EOF = 2,
	NDMP_MOVER_PAUSE_SEEK = 3,
	NDMP_MOVER_PAUSE_MEDIA_ERROR = 4,
	NDMP_MOVER_PAUSE_EOW = 5,
};
typedef enum ndmp_mover_pause_reason ndmp_mover_pause_reason;

enum ndmp_mover_halt_reason {
	NDMP_MOVER_HALT_NA = 0,
	NDMP_MOVER_HALT_CONNECT_CLOSED = 1,
	NDMP_MOVER_HALT_ABORTED = 2,
	NDMP_MOVER_HALT_INTERNAL_ERROR = 3,
	NDMP_MOVER_HALT_CONNECT_ERROR = 4,
};
typedef enum ndmp_mover_halt_reason ndmp_mover_halt_reason;

enum ndmp_mover_mode {
	NDMP_MOVER_MODE_READ = 0, NDMP_MOVER_MODE_WRITE = 1,
};
typedef enum ndmp_mover_mode ndmp_mover_mode;

struct ndmp_tcp_addr {
	u_long ip_addr;
	u_short port;
};
typedef struct ndmp_tcp_addr ndmp_tcp_addr;

struct ndmp_fc_addr {
	u_long loop_id;
};
typedef struct ndmp_fc_addr ndmp_fc_addr;

struct ndmp_ipc_addr {
	struct {
		u_int comm_data_len;
		char *comm_data_val;
	} comm_data;
};
typedef struct ndmp_ipc_addr ndmp_ipc_addr;

struct ndmp_addr {
	ndmp_addr_type addr_type;
	union {
		ndmp_tcp_addr tcp_addr;
		ndmp_fc_addr fc_addr;
		ndmp_ipc_addr ipc_addr;
	} ndmp_addr_u;
};

typedef struct ndmp_addr ndmp_addr;

struct ndmp_mover_get_state_reply {
	ndmp_error error;
	ndmp_mover_state state;
	ndmp_mover_pause_reason pause_reason;
	ndmp_mover_halt_reason halt_reason;
	u_long record_size;
	u_long record_num;
	ndmp_u_quad data_written;
	ndmp_u_quad seek_position;
	ndmp_u_quad bytes_left_to_read;
	ndmp_u_quad window_offset;
	ndmp_u_quad window_length;
	ndmp_addr data_connection_addr;
};

typedef struct ndmp_mover_get_state_reply ndmp_mover_get_state_reply;

struct ndmp_mover_listen_request {
	ndmp_mover_mode mode;
	ndmp_addr_type addr_type;
};

typedef struct ndmp_mover_listen_request ndmp_mover_listen_request;

struct ndmp_mover_listen_reply {
	ndmp_error error;
	ndmp_addr data_connection_addr;
};

typedef struct ndmp_mover_listen_reply ndmp_mover_listen_reply;

struct ndmp_mover_connect_request {
	ndmp_mover_mode mode;
	ndmp_addr addr;
};

typedef struct ndmp_mover_connect_request ndmp_mover_connect_request;

struct ndmp_mover_connect_reply {
	ndmp_error error;
};

typedef struct ndmp_mover_connect_reply ndmp_mover_connect_reply;

struct ndmp_mover_set_record_size_request {
	u_long len;
};

typedef struct ndmp_mover_set_record_size_request ndmp_mover_set_record_size_request;

struct ndmp_mover_set_record_size_reply {
	ndmp_error error;
};

typedef struct ndmp_mover_set_record_size_reply ndmp_mover_set_record_size_reply;

struct ndmp_mover_set_window_request {
	ndmp_u_quad offset;
	ndmp_u_quad length;
};

typedef struct ndmp_mover_set_window_request ndmp_mover_set_window_request;

struct ndmp_mover_set_window_reply {
	ndmp_error error;
};

typedef struct ndmp_mover_set_window_reply ndmp_mover_set_window_reply;

struct ndmp_mover_continue_reply {
	ndmp_error error;
};
typedef struct ndmp_mover_continue_reply ndmp_mover_continue_reply;

struct ndmp_mover_abort_reply {
	ndmp_error error;
};
typedef struct ndmp_mover_abort_reply ndmp_mover_abort_reply;

struct ndmp_mover_stop_reply {
	ndmp_error error;
};
typedef struct ndmp_mover_stop_reply ndmp_mover_stop_reply;

struct ndmp_mover_read_request {
	ndmp_u_quad offset;
	ndmp_u_quad length;
};
typedef struct ndmp_mover_read_request ndmp_mover_read_request;

struct ndmp_mover_read_reply {
	ndmp_error error;
};
typedef struct ndmp_mover_read_reply ndmp_mover_read_reply;

struct ndmp_mover_close_reply {
	ndmp_error error;
};
typedef struct ndmp_mover_close_reply ndmp_mover_close_reply;

enum ndmp_data_operation {
	NDMP_DATA_OP_NOACTION = 0,
	NDMP_DATA_OP_BACKUP = 1,
	NDMP_DATA_OP_RESTORE = 2,
};
typedef enum ndmp_data_operation ndmp_data_operation;

enum ndmp_data_state {
	NDMP_DATA_STATE_IDLE = 0,
	NDMP_DATA_STATE_ACTIVE = 1,
	NDMP_DATA_STATE_HALTED = 2,
	NDMP_DATA_STATE_LISTEN = 3,
	NDMP_DATA_STATE_CONNECTED = 4,
};
typedef enum ndmp_data_state ndmp_data_state;

enum ndmp_data_halt_reason {
	NDMP_DATA_HALT_NA = 0,
	NDMP_DATA_HALT_SUCCESSFUL = 1,
	NDMP_DATA_HALT_ABORTED = 2,
	NDMP_DATA_HALT_INTERNAL_ERROR = 3,
	NDMP_DATA_HALT_CONNECT_ERROR = 4,
};
typedef enum ndmp_data_halt_reason ndmp_data_halt_reason;
#define NDMP_DATA_STATE_EST_BYTES_REMAIN_INVALID 0x00000001
#define NDMP_DATA_STATE_EST_TIME_REMAIN_INVALID 0x00000002

struct ndmp_data_get_state_reply {
	u_long invalid;
	ndmp_error error;
	ndmp_data_operation operation;
	ndmp_data_state state;
	ndmp_data_halt_reason halt_reason;
	ndmp_u_quad bytes_processed;
	ndmp_u_quad est_bytes_remain;
	u_long est_time_remain;
	ndmp_addr data_connection_addr;
	ndmp_u_quad read_offset;
	ndmp_u_quad read_length;
};
typedef struct ndmp_data_get_state_reply ndmp_data_get_state_reply;

struct ndmp_data_start_backup_request {
	char *bu_type;
	struct {
		u_int env_len;
		ndmp_pval *env_val;
	} env;
};
typedef struct ndmp_data_start_backup_request ndmp_data_start_backup_request;

struct ndmp_data_start_backup_reply {
	ndmp_error error;
};
typedef struct ndmp_data_start_backup_reply ndmp_data_start_backup_reply;

struct ndmp_name {
	char *original_path;
	char *destination_dir;
	char *new_name;
	char *other_name;
	ndmp_u_quad node;
	ndmp_u_quad fh_info;
};
typedef struct ndmp_name ndmp_name;

struct ndmp_data_start_recover_request {
	struct {
		u_int env_len;
		ndmp_pval *env_val;
	} env;
	struct {
		u_int nlist_len;
		ndmp_name *nlist_val;
	} nlist;
	char *bu_type;
};
typedef struct ndmp_data_start_recover_request ndmp_data_start_recover_request;

struct ndmp_data_start_recover_reply {
	ndmp_error error;
};
typedef struct ndmp_data_start_recover_reply ndmp_data_start_recover_reply;

struct ndmp_data_abort_reply {
	ndmp_error error;
};
typedef struct ndmp_data_abort_reply ndmp_data_abort_reply;

struct ndmp_data_stop_reply {
	ndmp_error error;
};
typedef struct ndmp_data_stop_reply ndmp_data_stop_reply;

struct ndmp_data_get_env_reply {
	ndmp_error error;
	struct {
		u_int env_len;
		ndmp_pval *env_val;
	} env;
};
typedef struct ndmp_data_get_env_reply ndmp_data_get_env_reply;

struct ndmp_data_listen_request {
	ndmp_addr_type addr_type;
};
typedef struct ndmp_data_listen_request ndmp_data_listen_request;

struct ndmp_data_listen_reply {
	ndmp_error error;
	ndmp_addr data_connection_addr;
};
typedef struct ndmp_data_listen_reply ndmp_data_listen_reply;

struct ndmp_data_connect_request {
	ndmp_addr addr;
};
typedef struct ndmp_data_connect_request ndmp_data_connect_request;

struct ndmp_data_connect_reply {
	ndmp_error error;
};
typedef struct ndmp_data_connect_reply ndmp_data_connect_reply;

struct ndmp_notify_data_halted_request {
	ndmp_data_halt_reason reason;
	char *text_reason;
};
typedef struct ndmp_notify_data_halted_request ndmp_notify_data_halted_request;

enum ndmp_connect_reason {
	NDMP_CONNECTED = 0, NDMP_SHUTDOWN = 1, NDMP_REFUSED = 2,
};
typedef enum ndmp_connect_reason ndmp_connect_reason;

struct ndmp_notify_connected_request {
	ndmp_connect_reason reason;
	u_short protocol_version;
	char *text_reason;
};
typedef struct ndmp_notify_connected_request ndmp_notify_connected_request;

struct ndmp_notify_mover_paused_request {
	ndmp_mover_pause_reason reason;
	ndmp_u_quad seek_position;
};
typedef struct ndmp_notify_mover_paused_request ndmp_notify_mover_paused_request;

struct ndmp_notify_mover_halted_request {
	ndmp_mover_halt_reason reason;
	char *text_reason;
};
typedef struct ndmp_notify_mover_halted_request ndmp_notify_mover_halted_request;

struct ndmp_notify_data_read_request {
	ndmp_u_quad offset;
	ndmp_u_quad length;
};
typedef struct ndmp_notify_data_read_request ndmp_notify_data_read_request;

enum ndmp_log_type {
	NDMP_LOG_NORMAL = 0,
	NDMP_LOG_DEBUG = 1,
	NDMP_LOG_ERROR = 2,
	NDMP_LOG_WARNING = 3,
};
typedef enum ndmp_log_type ndmp_log_type;

struct ndmp_log_message_request {
	ndmp_log_type log_type;
	u_long message_id;
	char *entry;
};
typedef struct ndmp_log_message_request ndmp_log_message_request;

struct ndmp_log_file_request {
	char *name;
	ndmp_error error;
};
typedef struct ndmp_log_file_request ndmp_log_file_request;

enum ndmp_fs_type {
	NDMP_FS_UNIX = 0, NDMP_FS_NT = 1, NDMP_FS_OTHER = 2,
};
typedef enum ndmp_fs_type ndmp_fs_type;

typedef char *ndmp_path;

struct ndmp_nt_path {
	ndmp_path nt_path;
	ndmp_path dos_path;
};
typedef struct ndmp_nt_path ndmp_nt_path;

struct ndmp_file_name {
	ndmp_fs_type fs_type;
	union {
		ndmp_path unix_name;
		ndmp_nt_path nt_name;
		ndmp_path other_name;
	} ndmp_file_name_u;
};
typedef struct ndmp_file_name ndmp_file_name;

enum ndmp_file_type {
	NDMP_FILE_DIR = 0,
	NDMP_FILE_FIFO = 1,
	NDMP_FILE_CSPEC = 2,
	NDMP_FILE_BSPEC = 3,
	NDMP_FILE_REG = 4,
	NDMP_FILE_SLINK = 5,
	NDMP_FILE_SOCK = 6,
	NDMP_FILE_REGISTRY = 7,
	NDMP_FILE_OTHER = 8,
};
typedef enum ndmp_file_type ndmp_file_type;
#define NDMP_FILE_STAT_ATIME_INVALID 0x00000001
#define NDMP_FILE_STAT_CTIME_INVALID 0x00000002
#define NDMP_FILE_STAT_GROUP_INVALID 0x00000004

struct ndmp_file_stat {
	u_long invalid;
	ndmp_fs_type fs_type;
	ndmp_file_type ftype;
	u_long mtime;
	u_long atime;
	u_long ctime;
	u_long owner;
	u_long group;
	u_long fattr;
	ndmp_u_quad size;
	u_long links;
};
typedef struct ndmp_file_stat ndmp_file_stat;

struct ndmp_file {
	struct {
		u_int names_len;
		ndmp_file_name *names_val;
	} names;
	struct {
		u_int stats_len;
		ndmp_file_stat *stats_val;
	} stats;
	ndmp_u_quad node;
	ndmp_u_quad fh_info;
};
typedef struct ndmp_file ndmp_file;

struct ndmp_fh_add_file_request {
	struct {
		u_int files_len;
		ndmp_file *files_val;
	} files;
};
typedef struct ndmp_fh_add_file_request ndmp_fh_add_file_request;

struct ndmp_dir {
	struct {
		u_int names_len;
		ndmp_file_name *names_val;
	} names;
	ndmp_u_quad node;
	ndmp_u_quad parent;
};
typedef struct ndmp_dir ndmp_dir;

struct ndmp_fh_add_dir_request {
	struct {
		u_int dirs_len;
		ndmp_dir *dirs_val;
	} dirs;
};
typedef struct ndmp_fh_add_dir_request ndmp_fh_add_dir_request;

struct ndmp_node {
	struct {
		u_int stats_len;
		ndmp_file_stat *stats_val;
	} stats;
	ndmp_u_quad node;
	ndmp_u_quad fh_info;
};
typedef struct ndmp_node ndmp_node;

struct ndmp_fh_add_node_request {
	struct {
		u_int nodes_len;
		ndmp_node *nodes_val;
	} nodes;
};
typedef struct ndmp_fh_add_node_request ndmp_fh_add_node_request;

#endif

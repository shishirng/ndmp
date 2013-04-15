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
#include <ndmp_msg.h>
#include <stdbool.h>

struct environment
{
	u_int env_len;
	ndmp_pval *env_val;
};

struct environment* get_environment()
{
	static struct environment *retval = NULL;
	if (retval == NULL ) {
	        retval = calloc(1, sizeof(struct environment));
	        retval->env_len = 0;
	        retval->env_val = NULL;
	}
        return retval;
}

struct environment* init_environment()
{
	struct environment *env = get_environment();
	env->env_len = 0;
	env->env_val = NULL;
	return env;
}

ndmp_data_state get_data_state(struct client_txn *txn )
{
        struct ndmp_session_info *session_info;
        session_info = get_session_info(txn->client_session.session_id);
        return session_info->data_state;
}

void set_data_state(struct client_txn *txn , ndmp_data_state new_data_state )
{
        struct ndmp_session_info *session_info;
        session_info = get_session_info(txn->client_session.session_id);
        session_info->data_state = new_data_state;
}

void ndmp_data_get_state(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{

}

void ndmp_data_start_backup(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
	  /* struct ndmp_data_start_backup_request
	         * {
	         *        string          bu_type<>;
	         *        ndmp_pval       env<>;
	         * };
	         *
	         * struct ndmp_data_start_backup_reply
	         * {
	         *        ndmp_error      error;
	         * };
	         *
	         */
#ifdef DEBUG
	        printf("Printing from the data Start backup \n");
#endif
		static bool mount_done = FALSE ;
	        struct ndmp_data_start_backup_request request;
	        struct ndmp_header reply_header;
	        struct ndmp_data_start_backup_reply reply;
	        struct ndmp_session_info *session_info;
	        XDR reply_stream;
	        struct environment *data_env = get_environment();
	        session_info = get_session_info(txn->client_session.session_id);
	        int i;

	        /* We assume that the backup type and env key-value pairs have a
	         * maximum length of 8 characters, and that the keys and values
	         * themselves have a maximum length of 64 characters.
	         */

	        request.bu_type = (char*) malloc(8*sizeof(char));
	        request.env.env_len = 8;
	        request.env.env_val = (ndmp_pval*)malloc(8*sizeof(ndmp_pval) );
		data_env->env_val = (ndmp_pval*)malloc(8*sizeof(ndmp_pval) );


		        for (i = 0; i < request.env.env_len; i++) {
		                request.env.env_val[i].name = (char*) malloc(64*sizeof(char));
		                request.env.env_val[i].value = (char*) malloc(64*sizeof(char));
		                data_env->env_val[i].name = (char*) malloc(64*sizeof(char));
				data_env->env_val[i].value = (char*) malloc(64*sizeof(char));
		        }
#ifdef DEBUG
	        printf("before xdr decoding\n ");
#endif
	        xdr_ndmp_data_start_backup_request(request_stream, &request);
#ifdef DEBUG
	        printf("after xdr decoding \n");
#endif
	        enter_critical_section(session_info->s_lock);
	        reply.error = NDMP_NO_ERR;

	        // checking state
	        if(session_info -> data_state != NDMP_DATA_STATE_CONNECTED){
	                reply.error = NDMP_ILLEGAL_STATE_ERR;
	        }
	        else {
	                //changing state
	                session_info->data_state = NDMP_DATA_STATE_ACTIVE;
	        }

	        char full_ip[30]="";
	        strcpy(full_ip,inet_ntoa(txn->client_session.client_info.client.sin_addr));
#ifdef DEBUG
	        printf("printing full ip : %s \n",full_ip);

	        printf("after xdr decoding : Printing from the data Start backup \n");

	        printf("Butype : %s \n",request.bu_type );
	        printf("ENV variables \n");
#endif

	        ndmp_pval* env_val = request.env.env_val;
	        u_long env_len = request.env.env_len;

		 data_env->env_len = request.env.env_len;

		        for (i = 0; i < env_len; i++){
		        		strcpy(data_env->env_val[i].name,request.env.env_val[i].name);
		        		strcpy(data_env->env_val[i].value,request.env.env_val[i].value);
		        }

	        char* name_buf;
	        char* val_buf;

	        char *client_path;
	        char *user_name;

	        for( i=0;i<env_len ; i++){
	                name_buf = (char*)malloc(strlen(env_val[i].name)+1);
	                val_buf = (char*)malloc(strlen(env_val[i].value)+1);
	                strcpy(name_buf,env_val[i].name);
	                strcpy(val_buf,env_val[i].value);

	                printf("\nname : %s \nvalue : %s \n\n",name_buf,val_buf);

	                if(strcmp(env_val[i].name,"FILESYSTEM") ==0 ) {
	                        client_path = (char*) malloc (strlen(env_val[i].value) +1 );
	                        strcpy(client_path,env_val[i].value);
	                }
	                if(strcmp(env_val[i].name,"USER")==0){
	                        user_name = (char*) malloc (strlen( env_val[i].value ) +1 );
	                        strcpy(user_name,env_val[i].value);
	                }


	        }

	        // get volume name, assuming volume name less than 20
	        char vol_name[20];
	        printf("enter the volume name : ");
	        scanf("%s",vol_name );
#ifdef DEBUG
	        printf("printing volume name 11 : %s \n", vol_name);
	        // create mount point dir
	        printf("creating the mounting dir \n");
#endif
	       char *mount_point;
	       mount_point = (char*) malloc(16 * sizeof(char));
	       // char mount_point[300];
	        strcpy(mount_point,"/mnt/backedup2/");
	        mount_point = (char*) realloc ( mount_point,strlen(mount_point) + strlen(full_ip) + strlen(client_path) +1 );
	        strcat(mount_point,full_ip);
	        strcat(mount_point,client_path);
	        // mount point does not has / at the end

	        char *dir_instr;
	        dir_instr = (char*) malloc( 11 * sizeof(char));
	        //char dir_instr[400];
	        strcpy(dir_instr,"mkdir -p ");
	        dir_instr = (char*) realloc(dir_instr,strlen(dir_instr) + strlen(mount_point)+1);
	        strcat(dir_instr,mount_point);
#ifdef DEBUG
	        printf("\nprinting mount instr $ %s \n",dir_instr);
#endif
	        if(system(dir_instr) <0){
	               printf("mount point creation failed or already exists \n");
	        }

	        char *mount_instr;
	        mount_instr=(char*) malloc( 21 * sizeof(char ) );
	        //char mount_instr[400];
	        strcpy(mount_instr,"mount -t glusterfs ");
	        mount_instr = (char*) realloc( mount_instr , strlen(mount_instr) + 17+ strlen(vol_name) + strlen(mount_point));
	        strcat(mount_instr,"10.70.1.93:/");	//where to mount, currently ajeet's laptop
	        strcat(mount_instr,vol_name);
	        strcat(mount_instr," ");
	        strcat(mount_instr,mount_point);
#ifdef DEBUG
	        printf("printing volume name 1 : %s \n ",vol_name );
	        printf("\nprinting mount instr $ %s \n",mount_instr);
#endif
		if(mount_done == FALSE ){
	        	if(system(mount_instr) <0){
	                	printf("failed : mount volume \n");
	                	return;
	        	}
			mount_done = TRUE;
		}

	        char *rsync_instr;
	        rsync_instr = (char*) malloc ( 12 * sizeof(char ));
	        //char rsync_instr[400];
	        strcpy(rsync_instr,"rsync -av ");
	        rsync_instr = (char*) realloc(rsync_instr,strlen(rsync_instr) + 6 + strlen(user_name) +\
	                        strlen(full_ip)+strlen(client_path) + strlen(mount_point) );
	        strcat(rsync_instr,user_name);
	        strcat(rsync_instr,"@");
	        strcat(rsync_instr,full_ip);
	        strcat(rsync_instr,":");
	        strcat(rsync_instr, client_path);
	        strcat(rsync_instr,"/ ");
	        strcat(rsync_instr,mount_point);
#ifdef DEBUG
	        printf("printing rsync instr $ %s \n",rsync_instr);
#endif
	        if(system(rsync_instr) <0){
	                printf("failed : rsync \n");
	                return;
	        }

	        set_header(header, &reply_header, reply.error);
	        txn->reply.length = xdr_sizeof((xdrproc_t) xdr_ndmp_header, &reply_header);
	        txn->reply.length += xdr_sizeof((xdrproc_t)xdr_ndmp_data_start_backup_reply, &reply);

	        xdrmem_create(&reply_stream, txn->reply.message, txn->reply.length,XDR_ENCODE);
	        xdr_ndmp_header(&reply_stream, &reply_header);
	        if (reply.error == NDMP_NO_ERR)
	                xdr_ndmp_data_start_backup_reply(&reply_stream, &reply);
	        else
	                txn->reply.length -= xdr_sizeof((xdrproc_t) xdr_ndmp_data_start_backup_reply, &reply);

		session_info->data_state = NDMP_DATA_STATE_HALTED;

	        exit_critical_section(session_info->s_lock);
#ifdef DEBUG
	        printf("Exiting data start backup \n ");
#endif
}

void ndmp_data_start_recover(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
	   /* struct ndmp_name
	         *      {
	         *              string original_path<>;
	         *              string destination_dir<>;
	         *              string new_name <>;
	         *              string other_name <>;
	         *              ndmp_u_quad node;
	         *              ndmp_u_quad fh_info;
	         *      };
	         *
	         * struct ndmp_data_start_recover_request
	         *      {
	         *              ndmp_pval env<>;
	         *              ndmp_name nlist<>;
	         *              string bu_type<>;
	         *      };
	         * struct ndmp_data_start_recover_reply
	         *      {
	         *              ndmp_error error;
	         *      };
	         */
#ifdef DEBUG
	        printf("Printing from the data Start Recovery \n");
#endif
	        struct ndmp_data_start_recover_request request;
	        struct ndmp_header reply_header;
	        struct ndmp_data_start_recover_reply reply;
	        struct ndmp_session_info *session_info;
	        XDR reply_stream;

	        session_info = get_session_info(txn->client_session.session_id);
	        int i;

	        /* We assume that the backup type and env key-value pairs have a
	         * maximum length of 8 characters, and that the keys and values
	         * themselves have a maximum length of 64 characters.
	         */

	        request.bu_type = (char*) malloc(8*sizeof(char));
	        request.env.env_len = 8;
	        request.env.env_val = (ndmp_pval*)malloc(8*sizeof(ndmp_pval) );

	        for (i = 0; i < request.env.env_len; i++) {
	                request.env.env_val[i].name = (char*) malloc(64*sizeof(char));
	                request.env.env_val[i].value = (char*) malloc(64*sizeof(char));
	        }

	        request.nlist.nlist_len = 5;
	        request.nlist.nlist_val = (ndmp_name*)malloc(5*sizeof(ndmp_name) );

	        for (i = 0; i < request.nlist.nlist_len; i++) {
	                request.nlist.nlist_val[i].original_path = (char*) malloc(64*sizeof(char));
	                request.nlist.nlist_val[i].destination_dir = (char*) malloc(64*sizeof(char));
	                request.nlist.nlist_val[i].new_name = (char*) malloc(64*sizeof(char));
	                request.nlist.nlist_val[i].other_name = (char*) malloc(64*sizeof(char));
	        }
#ifdef DEBUG
	        printf("before xdr decoding\n ");
#endif
	        xdr_ndmp_data_start_recover_request(request_stream, &request);
#ifdef DEBUG
	        printf("after xdr decoding \n");
#endif
	        enter_critical_section(session_info->s_lock);
	        reply.error = NDMP_NO_ERR;

	        if(session_info -> data_state != NDMP_DATA_STATE_CONNECTED){
	                reply.error = NDMP_ILLEGAL_STATE_ERR;
	        }
	        else {
	                //changing state
	                session_info->data_state = NDMP_DATA_STATE_ACTIVE;
	        }

	        char full_ip[20]="";
	        strcpy(full_ip,inet_ntoa(txn->client_session.client_info.client.sin_addr));

	        ndmp_pval* env_val = request.env.env_val;
	        u_long env_len = request.env.env_len;

	        char* name_buf;
	        char* val_buf;

	        char *client_path;
	        char *user_name;

	        for( i=0;i<env_len ; i++){
	                name_buf = (char*)malloc(strlen(env_val[i].name)+1);
	                val_buf = (char*)malloc(strlen(env_val[i].value)+1);
	                strcpy(name_buf,env_val[i].name);
	                strcpy(val_buf,env_val[i].value);
#ifdef DEBUG
	                printf("\nname : %s \nvalue : %s \n\n",name_buf,val_buf);
#endif
	                if(strcmp(env_val[i].name,"FILESYSTEM") ==0 ) {
	                        client_path = (char*) malloc (strlen(env_val[i].value) +1 );
	                        strcpy(client_path,env_val[i].value);
	                }
	                if(strcmp(env_val[i].name,"USER")==0){
	                        user_name = (char*) malloc (strlen( env_val[i].value ) +1 );
	                        strcpy(user_name,env_val[i].value);
	                }

	        }
#ifdef DEBUG
	        // obtaining mount point dir
	        printf("obtaining the dir/file to recover \n");
#endif
	        char *mount_point;
	        mount_point = (char*) malloc(16*sizeof(char) );
	       //char mount_point[300];
	        strcpy(mount_point,"/mnt/backedup2/");
	         mount_point = (char*) realloc(mount_point,strlen(mount_point) + strlen(full_ip) + \
	                         strlen(client_path ) + strlen(request.nlist.nlist_val[0].original_path ) );
	        strcat(mount_point,full_ip);
	        strcat(mount_point,client_path);
	        // assumes the the orignal path always starts with "/"
	        strcat(mount_point,request.nlist.nlist_val[0].original_path);

	        printf("creating rsync instr \n");
	       char *rsync_instr;
	       rsync_instr = (char*) malloc( 13 * sizeof(char));
	       // char rsync_instr[400];
	        strcpy(rsync_instr,"rsync -av ");
	        rsync_instr = (char*)realloc(rsync_instr ,strlen(rsync_instr) +  7 + strlen(mount_point)+strlen(user_name) + strlen(full_ip) \
	                        + strlen(request.nlist.nlist_val[0].destination_dir ) ) ;
	        strcat(rsync_instr,mount_point);
	        //strcat(rsync_instr,"/ "); // considering the recovery of exact file/dir

	        strcat(rsync_instr,user_name);
	        strcat(rsync_instr,"@");
	        strcat(rsync_instr,full_ip);
	        strcat(rsync_instr,":");
	        strcat(rsync_instr, request.nlist.nlist_val[0].destination_dir);
	        //strcat(rsync_instr,"/");
#ifdef DEBUG
	        printf("printing rsync instr $ %s \n",rsync_instr);
#endif
	        if(system(rsync_instr) <0){
	                printf("failed : rsync \n");
	                return;
	        }


	        set_header(header, &reply_header, reply.error);
	        txn->reply.length = xdr_sizeof((xdrproc_t) xdr_ndmp_header, &reply_header);
	        txn->reply.length += xdr_sizeof((xdrproc_t)xdr_ndmp_data_start_recover_reply, &reply);

	        xdrmem_create(&reply_stream, txn->reply.message, txn->reply.length,XDR_ENCODE);
	        xdr_ndmp_header(&reply_stream, &reply_header);
	        if (reply.error == NDMP_NO_ERR)
	                xdr_ndmp_data_start_recover_reply(&reply_stream, &reply);
	        else
	                txn->reply.length -= xdr_sizeof((xdrproc_t) xdr_ndmp_data_start_recover_reply, &reply);

		 session_info->data_state = NDMP_DATA_STATE_HALTED;


	        exit_critical_section(session_info->s_lock);
#ifdef DEBUG
	        printf("Exiting data start recover \n ");
#endif
}

void ndmp_data_abort(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
	/* NDMP_DATA_ABORT
	 *
	 * no request arguments
	 *
	 * struct ndmp_data_abort_reply
	 * {
	 * 	ndmp_error error;
	 * };
	 *
	 */

		struct ndmp_header reply_header;
		struct ndmp_data_abort_reply reply;
		struct ndmp_session_info *session_info;

		XDR reply_stream;

		session_info = get_session_info(txn->client_session.session_id);
		enter_critical_section(session_info->s_lock);

		if (session_info-> data_state == NDMP_DATA_STATE_HALTED || session_info-> data_state == NDMP_DATA_STATE_IDLE){
		        reply.error = NDMP_ILLEGAL_STATE_ERR;
		}
		else {
			session_info->data_state = NDMP_DATA_STATE_HALTED;
			reply.error = NDMP_NO_ERR;
		}

		set_header(header, &reply_header, reply.error);

		txn->reply.length = xdr_sizeof((xdrproc_t)
		                                       xdr_ndmp_header, &reply_header);
		txn->reply.length += xdr_sizeof((xdrproc_t)
		                                        xdr_ndmp_data_abort_reply, &reply);

		xdrmem_create(&reply_stream,
			                      txn->reply.message, txn->reply.length,XDR_ENCODE);

		xdr_ndmp_header(&reply_stream, &reply_header);

		if (reply.error == NDMP_NO_ERR)
		        xdr_ndmp_data_abort_reply(&reply_stream, &reply);
	        else
	                txn->reply.length -= xdr_sizeof((xdrproc_t)
	                                        xdr_ndmp_data_abort_reply, &reply);

		exit_critical_section(session_info->s_lock);
#ifdef DEBUG
		printf("Data state: %d\n",session_info->data_state);
#endif

}

void ndmp_data_get_env(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
	/* NDMP_DATA_GET_ENV
	 *
	 * no request arguments
	 *
	 * struct ndmp_data_get_env_reply
	 * {
	 * ndmp_error error;
	 * ndmp_pval env<>;
	 * };
	 *
	 */

	int i;
	struct ndmp_header reply_header;
	struct ndmp_data_get_env_reply reply;
	struct ndmp_session_info *session_info;
	struct environment *data_env = get_environment();
	XDR reply_stream;
	session_info = get_session_info(txn->client_session.session_id);

	enter_critical_section(session_info->s_lock);

	reply.error = NDMP_NO_ERR;
	reply.env.env_len = data_env->env_len;

	reply.env.env_val = (ndmp_pval*)malloc(8*sizeof(ndmp_pval) );

	if (session_info-> data_state != NDMP_DATA_STATE_ACTIVE || session_info-> data_state != NDMP_DATA_STATE_HALTED){
		reply.error = NDMP_NO_ERR;
		if (data_env->env_len != 0) {

			for (i = 0; i < data_env->env_len; i++) {
				reply.env.env_val[i].name = (char*) malloc(64*sizeof(char));
				reply.env.env_val[i].value = (char*) malloc(64*sizeof(char));
			}

			for (i = 0; i < data_env->env_len; i++) {
				reply.env.env_val[i].name = data_env->env_val[i].name;
				reply.env.env_val[i].value = data_env->env_val[i].value;

			printf("reply.env.env_val[i].name = %s\n reply.env.env_val[i].value = %s\n\n",
					reply.env.env_val[i].name, reply.env.env_val[i].value);
			}
		}
		else
			reply.env.env_val = NULL;
	}
	else
		reply.error = NDMP_ILLEGAL_STATE_ERR;

	set_header(header, &reply_header, reply.error);

	txn->reply.length = xdr_sizeof((xdrproc_t)
	                                       xdr_ndmp_header, &reply_header);
	txn->reply.length += xdr_sizeof((xdrproc_t)
	                                        xdr_ndmp_data_get_env_reply, &reply);

	xdrmem_create(&reply_stream,
	                      txn->reply.message, txn->reply.length,XDR_ENCODE);

	xdr_ndmp_header(&reply_stream, &reply_header);

	if (reply.error == NDMP_NO_ERR)
                xdr_ndmp_data_get_env_reply(&reply_stream, &reply);
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_data_get_env_reply, &reply);

	exit_critical_section(session_info->s_lock);
}

void ndmp_data_stop(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
	/*  NDMP_DATA_STOP
	 *
	 * no request arguments
	 *
	 * struct ndmp_data_stop_reply
	 * {
	 * 	ndmp_error error;
	 * };
	 *
	 */

	struct ndmp_header reply_header;
	struct ndmp_data_stop_reply reply;
	struct ndmp_session_info *session_info;
	struct environment* data_env = get_environment();

	XDR reply_stream;

	session_info = get_session_info(txn->client_session.session_id);
	enter_critical_section(session_info->s_lock);

	if (session_info-> data_state != NDMP_DATA_STATE_HALTED){
	        reply.error = NDMP_ILLEGAL_STATE_ERR;
	}
	else {
		session_info->data_state = NDMP_DATA_STATE_IDLE;
		data_env = init_environment();
		reply.error = NDMP_NO_ERR;
	}

	set_header(header, &reply_header, reply.error);

	txn->reply.length = xdr_sizeof((xdrproc_t)
	                                       xdr_ndmp_header, &reply_header);
	txn->reply.length += xdr_sizeof((xdrproc_t)
	                                        xdr_ndmp_data_stop_reply, &reply);

	xdrmem_create(&reply_stream,
		                      txn->reply.message, txn->reply.length,XDR_ENCODE);

	xdr_ndmp_header(&reply_stream, &reply_header);

	if (reply.error == NDMP_NO_ERR)
	        xdr_ndmp_data_stop_reply(&reply_stream, &reply);
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_data_stop_reply, &reply);

	exit_critical_section(session_info->s_lock);
#ifdef DEBUG
	printf("Data state: %d\n",session_info->data_state);
#endif
}

void ndmp_data_listen(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
	/* NDMP_DATA_LISTEN
	 *
	 * struct ndmp_tcp_addr
	 * {
	 *  	u_long	ip_addr;
	 *	u_short	port;
	 * };
	 *
	 * struct ndmp_fc_addr
	 * {
	 *	u_long	loop_id;
	 * };
	 *
	 * struct ndmp_ipc_addr
	 * {
	 * 	opaque comm_data<>;
	 * };
	 *
	 * union ndmp_addr switch (ndmp_addr_type addr_type)
	 * {
	 * 	case NDMP_ADDR_LOCAL:
	 *		void;
	 * 	case NDMP_ADDR_TCP:
	 *		ndmp_tcp_addr	tcp_addr;
	 * 	case NDMP_ADDR_FC:
	 *		ndmp_fc_addr 	fc_addr;
	 * 	case NDMP_ADDR_IPC:
	 * 		ndmp_ipc_addr 	ipc_addr;
	 * };
	 *
	 * struct ndmp_data_listen_request
	 * {
	 *	ndmp_addr_type	addr_type;
	 * };
	 *
	 * struct ndmp_data_listen_reply
	 * {
	 *	ndmp_error	error;
	 *	ndmp_addr	data_connection_addr;
	 * };
	 */

	struct ndmp_data_listen_request request;
	struct ndmp_header reply_header;
	struct ndmp_data_listen_reply reply;
	struct ndmp_session_info *session_info;
	XDR reply_stream;

	xdr_ndmp_data_listen_request(request_stream, &request);
	session_info = get_session_info(txn->client_session.session_id);

	enter_critical_section(session_info->s_lock);
	reply.error = NDMP_NO_ERR;

	if (session_info-> data_state != NDMP_DATA_STATE_IDLE){
	        reply.error = NDMP_ILLEGAL_STATE_ERR;
	}
	else{
		if(request.addr_type== NDMP_ADDR_LOCAL ){
	                session_info->data_state = NDMP_DATA_STATE_LISTEN;
	                // change the state to listen
	                reply.data_connection_addr.addr_type = NDMP_ADDR_LOCAL;
	               // reply.data_connection_addr.ndmp_addr_u = NULL;
	        }
                else{
	                // send illegal argument err
	                reply.error = NDMP_ILLEGAL_ARGS_ERR ;
	        }
	}

	set_header(header, &reply_header, reply.error);

	txn->reply.length = xdr_sizeof((xdrproc_t) xdr_ndmp_header, &reply_header);
	txn->reply.length += xdr_sizeof((xdrproc_t)xdr_ndmp_data_listen_reply, &reply);

	xdrmem_create(&reply_stream, txn->reply.message, txn->reply.length,XDR_ENCODE);
	xdr_ndmp_header(&reply_stream, &reply_header);
	if (reply.error == NDMP_NO_ERR)
		xdr_ndmp_data_listen_reply(&reply_stream, &reply);
	else
	        txn->reply.length -= xdr_sizeof((xdrproc_t) xdr_ndmp_data_listen_reply, &reply);

	exit_critical_section(session_info->s_lock);

}

void ndmp_data_connect(struct client_txn *txn, struct ndmp_header header, XDR* request_stream)
{
	/* NDMP_DATA_CONNECT
	 * struct ndmp_data_connect_request
         * {
         * 	ndmp_addr   addr;
         * };
         *
         * struct ndmp_data_connect_reply
         * {
         *      ndmp_error  error;
         * };
	 */

	struct ndmp_data_connect_request request;
        struct ndmp_header reply_header;
        struct ndmp_data_connect_reply reply;
        struct ndmp_session_info *session_info;
        XDR reply_stream;

        xdr_ndmp_data_connect_request(request_stream, &request);
#ifdef DEBUG
	printf("Printing from the data connect open \n");
	printf("Request message received : \
		addr_type : %d \n ",request.addr.addr_type);
#endif
        session_info = get_session_info(txn->client_session.session_id);

        enter_critical_section(session_info->s_lock);
        reply.error = NDMP_NO_ERR;

        if (session_info-> data_state != NDMP_DATA_STATE_IDLE ){
                reply.error = NDMP_ILLEGAL_STATE_ERR;
                }
        else{
                if(request.addr.addr_type== NDMP_ADDR_LOCAL ){
                        session_info->data_state = NDMP_DATA_STATE_CONNECTED;
                        // change the state to connected
                        }

                else{
                        // send illegal argument err
                        reply.error = NDMP_ILLEGAL_ARGS_ERR ;
                 }
        }

        set_header(header, &reply_header, reply.error);

        txn->reply.length = xdr_sizeof((xdrproc_t) xdr_ndmp_header, &reply_header);
        txn->reply.length += xdr_sizeof((xdrproc_t)xdr_ndmp_data_connect_reply, &reply);

        xdrmem_create(&reply_stream, txn->reply.message, txn->reply.length,XDR_ENCODE);
        xdr_ndmp_header(&reply_stream, &reply_header);
        if (reply.error == NDMP_NO_ERR)
                xdr_ndmp_data_connect_reply(&reply_stream, &reply);
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t) xdr_ndmp_data_connect_reply, &reply);

        exit_critical_section(session_info->s_lock);
}

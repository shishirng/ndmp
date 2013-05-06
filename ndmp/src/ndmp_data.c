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

void ndmp_data_listen(struct client_txn *txn,
                               struct ndmp_header header, XDR* request_stream)
{
        /* NDMP_DATA_LISTEN
         *
         * struct ndmp_tcp_addr
         * {
         *      u_long  ip_addr;
         *      u_short port;
         * };
         *
         * struct ndmp_fc_addr
         * {
         *      u_long  loop_id;
         * };
         *
         * struct ndmp_ipc_addr
         * {
         *      opaque comm_data<>;
         * };
         *
         * union ndmp_addr switch (ndmp_addr_type addr_type)
         * {
         *      case NDMP_ADDR_LOCAL:
         *              void;
         *      case NDMP_ADDR_TCP:
         *              ndmp_tcp_addr   tcp_addr;
         *      case NDMP_ADDR_FC:
         *              ndmp_fc_addr    fc_addr;
         *      case NDMP_ADDR_IPC:
         *              ndmp_ipc_addr   ipc_addr;
         * };
         *
         * struct ndmp_data_listen_request
         * {
         *      ndmp_addr_type  addr_type;
         * };
         *
         * struct ndmp_data_listen_reply
         * {
         *      ndmp_error      error;
         *      ndmp_addr       data_connection_addr;
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

        if (reply.error == NDMP_NO_ERR) {
                xdr_ndmp_data_listen_reply(&reply_stream, &reply);
                write_client_log(txn->client_session.client_info.client, "Listening for backup/recover requests");
        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t) xdr_ndmp_data_listen_reply, &reply);

        exit_critical_section(session_info->s_lock);

}

void ndmp_data_connect(struct client_txn *txn, struct ndmp_header header, XDR* request_stream)
{
        /* NDMP_DATA_CONNECT
         * struct ndmp_data_connect_request
         * {
         *      ndmp_addr   addr;
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

        if (reply.error == NDMP_NO_ERR) {
                xdr_ndmp_data_connect_reply(&reply_stream, &reply);
                write_client_log(txn->client_session.client_info.client, "Connected for backup/recover operations");
        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t) xdr_ndmp_data_connect_reply, &reply);

        exit_critical_section(session_info->s_lock);
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

                //static bool mount_done = FALSE ;
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

                xdr_ndmp_data_start_backup_request(request_stream, &request);
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

                char client_ip[30]="";
                strcpy(client_ip,inet_ntoa(txn->client_session.client_info.client.sin_addr));

#ifdef DEBUG
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
                
                struct vol_fs_node * list_head = NULL;
                int head_init ;
                head_init = init_vol_fs_list(&list_head ,"gluster_vol_usage") ;

                // synchronising with config file
                
                char host_ip[20];
                char vol_name[20];
                char bu_point[200];
                int is_it_new = 0;

                set_host_config("gluster_vol_usage");
                
                FILE * temp_fp = fopen("gluster_vol_usage","a+");
#ifdef DEBUG

                if(temp_fp == NULL ) {
                        printf("couldn't open vol_usage file in backup function \n");
                        write_client_log(txn->client_session.client_info.client, "couldn't open vol_usage file in backup function");
                }
                else printf("vol_usage file opened successfully :) \n");
#endif  
        
                fscanf(temp_fp,"%s %s",host_ip,bu_point );
                fclose(temp_fp);
                        
                char* client_ip_fs = (char*)malloc( ( strlen(client_ip) +1 + strlen(client_path) +1 ) * sizeof(char) );
                strcpy(client_ip_fs,client_ip);
                strcat(client_ip_fs,client_path);
                
                struct vol_fs_node * temp_node = find ( list_head , client_ip_fs  ) ;
                if(  temp_node  != NULL ) {
                        write_client_log(txn->client_session.client_info.client, "Client info was found in the config file");
                        printf("Client info was found in the config file \nusing volume $ %s $ for backup\n", temp_node->vol_name);
                        strcpy(vol_name,temp_node->vol_name);

                } 
                else {
                        is_it_new = 1;
                        write_client_log(txn->client_session.client_info.client, "Client info was NOT found in the config file");
                        printf("Client info was NOT found in the config file \nenter the volume to be used : ");
                        scanf("%s",vol_name);
                        char* temp_fs = (char*) malloc( ( strlen(client_ip) + strlen(client_path) +3 ) * sizeof(char));
                        strcpy(temp_fs,client_ip);
                        strcat(temp_fs,client_path);
                        printf("updating the config file \n");
                        FILE * temp_fp = fopen("gluster_vol_usage","a+");
                        fprintf(temp_fp,"%s %s %s\n",vol_name,temp_fs, user_name);
                        fclose(temp_fp);
                        free(temp_fs);

                }
                
               char* mount_point;
               mount_point = (char*) malloc((strlen(bu_point) +1 ) * sizeof(char));
               strcpy(mount_point,bu_point);
                

                // get mount_point using config file
                
                mount_point = (char*) realloc(mount_point,strlen(mount_point) + strlen(vol_name) + 3);
                strcat(mount_point,"/");
                strcat(mount_point,vol_name);

                
                /* *** creation of directory and mounting of volume not required ***
                
                char * dir_inst_1;
                dir_inst_1 = (char*) malloc (11 * sizeof(char));
                strcpy(dir_inst_1,"mkdir -p ");
                dir_inst_1 = (char*) realloc( dir_inst_1, strlen(dir_inst_1)+strlen(mount_point) );;
                strcat(dir_inst_1,mount_point);
                
                printf("creating directory for volume mount $ %s ",dir_inst_1);
                system(dir_inst_1);
                
                */
                // backup_point does not end with '/'
                if(is_it_new){
                        char *mount_instr;
                        mount_instr=(char*) malloc( 21 * sizeof(char ) );
                        strcpy(mount_instr,"mount -t glusterfs ");
                        mount_instr = (char*) realloc( mount_instr ,strlen(mount_instr) + 17+ strlen(vol_name) + strlen(mount_point));
                
                
                        /* do not need
                        FILE* fp;
                        fp=popen("ip addr show scope global | grep inet | cut -d' ' -f6 | cut -d/ -f1","r");
                        char ip[30];
                        fscanf(fp,"%s",host_ip);
                        pclose(fp);
        
                        */
                        
                        strcat(mount_instr,host_ip);
                        strcat(mount_instr,":/");
                        strcat(mount_instr,vol_name);
                        strcat(mount_instr," ");
                        strcat(mount_instr,mount_point);
#ifdef DEBUG
                        printf("\nprinting mount instr $ %s \n",mount_instr);
                        write_client_log(txn->client_session.client_info.client,mount_instr);
#endif

                        int ret_mount_instr = system(mount_instr);

                        if(ret_mount_instr  != 0){
                                printf("failed : mount volume \n");
                                reply.error = NDMP_ILLEGAL_STATE_ERR;
                                session_info->data_state = NDMP_DATA_STATE_HALTED;
                                goto SEND ;
                        }

                }
        
                char* backup_location;

                backup_location = (char*) malloc (strlen(mount_point) + strlen(client_ip) + strlen(client_path) +3 );
                
                strcpy(backup_location,mount_point);            
                strcat(backup_location,"/");
                strcat(backup_location ,client_ip);
                strcat(backup_location ,client_path);

                char *dir_instr;
                dir_instr = (char*) malloc( 11 * sizeof(char));
                strcpy(dir_instr,"mkdir -p ");
                dir_instr = (char*) realloc(dir_instr,strlen(dir_instr) + strlen(backup_location)+1);
                strcat(dir_instr,backup_location);


                int ret_dir_instr = system(dir_instr);


                if(ret_dir_instr != 0){
                       printf("mount point directory creation failed or already exists \n");
                }

                
                char *rsync_instr;
                rsync_instr = (char*) malloc ( 12 * sizeof(char ));
                strcpy(rsync_instr,"rsync -av ");
                rsync_instr = (char*) realloc(rsync_instr,strlen(rsync_instr) + 6 + strlen(user_name) +\
                                strlen(client_ip)+strlen(client_path) + strlen(backup_location) );
                strcat(rsync_instr,user_name);
                strcat(rsync_instr,"@");
                strcat(rsync_instr,client_ip);
                strcat(rsync_instr,":");
                strcat(rsync_instr, client_path);
                strcat(rsync_instr,"/ ");
                strcat(rsync_instr,backup_location);
#ifdef DEBUG
                printf("printing rsync instr $ %s \n",rsync_instr);
                write_client_log(txn->client_session.client_info.client,rsync_instr);
#endif
                int ret_rsync_instr = system(rsync_instr);
                //printf("return value of rsync instr = %d \n", ret_rsync_instr);

                if(ret_rsync_instr != 0){
                        // return after replying with proper error code tot the client
                        printf("failed : rsync \n");
                        reply.error = NDMP_ILLEGAL_STATE_ERR;
                        session_info->data_state = NDMP_DATA_STATE_HALTED;
                        goto SEND ;
                }


        SEND:   set_header(header, &reply_header, reply.error);
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

                xdr_ndmp_data_start_recover_request(request_stream, &request);
                enter_critical_section(session_info->s_lock);
                reply.error = NDMP_NO_ERR;

                if(session_info -> data_state != NDMP_DATA_STATE_CONNECTED){
                        reply.error = NDMP_ILLEGAL_STATE_ERR;
                }
                else {
                        //changing state
                        session_info->data_state = NDMP_DATA_STATE_ACTIVE;
                }

                char client_ip[20]="";
                strcpy(client_ip,inet_ntoa(txn->client_session.client_info.client.sin_addr));

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

                char vol_name[20];

                // extracting mount_point and vol_name
                FILE * temp_fp = fopen("gluster_vol_usage","a+");
#ifdef DEBUG

                if(temp_fp == NULL ) {
                        printf("couldn't open vol_usage file in backup function \n");
                        write_client_log(txn->client_session.client_info.client, "couldn't open vol_usage file in backup function");
                        }

                else {
                        printf("vol_usage file opened successfully \n");
                        write_client_log(txn->client_session.client_info.client, "vol_usage file opened successfully");
                }
#endif  
                char host_ip[20];
                char bu_point[100];
                fscanf(temp_fp,"%s %s",host_ip,bu_point );
                
                char temp_vol_name[20];
                char temp_filesystem[100];
                char temp_user[30];
                
                char * ip_fs;
                ip_fs = (char*) malloc( sizeof(char)* ( strlen ( client_ip ) + strlen ( client_path ) + 3) );
                strcpy(ip_fs , client_ip);
                strcat(ip_fs,client_path);
                
                while(fscanf(temp_fp,"%s %s %s",temp_vol_name,temp_filesystem,temp_user ) > 0 ){
                        if(strcmp(ip_fs , temp_filesystem  ) ==0 )
                                strcpy(vol_name,temp_vol_name);

                        }
                free(ip_fs);
                fclose(temp_fp);
        
                char *mount_point;
                 mount_point = (char*) realloc(mount_point,strlen(bu_point) + strlen(vol_name) + strlen(client_ip) + \
                                 strlen(client_path ) + strlen(request.nlist.nlist_val[0].original_path ) + 3);
                strcpy(mount_point,bu_point);
                strcat(mount_point,"/");                
                strcat(mount_point,vol_name);
                strcat(mount_point,"/");
                strcat(mount_point,client_ip);
                strcat(mount_point,client_path);
                // assumes the the orignal path always starts with "/"
                strcat(mount_point,request.nlist.nlist_val[0].original_path);

                char *rsync_instr;
                rsync_instr = (char*) malloc( 13 * sizeof(char));
                strcpy(rsync_instr,"rsync -av ");
                rsync_instr = (char*)realloc(rsync_instr ,strlen(rsync_instr) +  7 + strlen(mount_point)+strlen(user_name) + strlen(client_ip) \
                                + strlen(request.nlist.nlist_val[0].destination_dir ) ) ;
                strcat(rsync_instr,mount_point);
                strcat(rsync_instr," "); 

                strcat(rsync_instr,user_name);
                strcat(rsync_instr,"@");
                strcat(rsync_instr,client_ip);
                strcat(rsync_instr,":");
                strcat(rsync_instr, request.nlist.nlist_val[0].destination_dir);
#ifdef DEBUG
                printf("printing rsync instr $ %s \n",rsync_instr);
                write_client_log(txn->client_session.client_info.client,rsync_instr);
#endif
                int ret_rsync_instr = system(rsync_instr);


                if( ret_rsync_instr != 0){

                        // return after replying with proper error code to the client
                        printf("failed : rsync \n");
                        reply.error = NDMP_ILLEGAL_STATE_ERR;
                        session_info->data_state = NDMP_DATA_STATE_HALTED;
                        goto SEND ;
                }



         SEND : set_header(header, &reply_header, reply.error);
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
         *      ndmp_error error;
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

                if (reply.error == NDMP_NO_ERR) {
                        xdr_ndmp_data_abort_reply(&reply_stream, &reply);
                        write_client_log(txn->client_session.client_info.client, "Backup/recover operation aborted");
                }
                else
                        txn->reply.length -= xdr_sizeof((xdrproc_t)
                                                xdr_ndmp_data_abort_reply, &reply);

                exit_critical_section(session_info->s_lock);
#ifdef DEBUG
                printf("Data state: %d\n",session_info->data_state);
#endif

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
         *      ndmp_error error;
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

        if (reply.error == NDMP_NO_ERR) {
                xdr_ndmp_data_stop_reply(&reply_stream, &reply);
                write_client_log(txn->client_session.client_info.client, "Backup/recover operation stopped");
        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_data_stop_reply, &reply);

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

        if (reply.error == NDMP_NO_ERR) {
                xdr_ndmp_data_get_env_reply(&reply_stream, &reply);
                write_client_log(txn->client_session.client_info.client, "Sent backup ENV to client");
        }
        else
                txn->reply.length -= xdr_sizeof((xdrproc_t)
                                        xdr_ndmp_data_get_env_reply, &reply);

        exit_critical_section(session_info->s_lock);
}

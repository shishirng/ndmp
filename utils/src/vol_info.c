/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vol_info.h>
#include <stdbool.h>

void
insert_node( struct vol_fs_node * head , char * vol_name , char * filesystem , char* user ) {

        struct vol_fs_node * iter = head;

        while(iter->next != NULL ) iter = iter->next;

        struct vol_fs_node *temp = (struct vol_fs_node *) malloc(sizeof(struct vol_fs_node ));
        temp-> vol_name = (char*) malloc( (strlen(vol_name ) +1 ) * sizeof(char ));
        temp->filesystem = (char*) malloc ( ( strlen(filesystem) +1 )* sizeof(char));
        temp->user = (char*) malloc ( ( strlen ( user ) +1 ) * sizeof(char) );

        strcpy(temp->vol_name,vol_name);
        strcpy(temp->filesystem,filesystem);
        strcpy(temp->user,user);
        
        iter->next = temp;
        temp->next = NULL;

}


// returns -1 if there was nothing in file and list is still not initializes
// else 0 ( list successfully initialised )
int
init_vol_fs_list( struct vol_fs_node ** head_ref , char* filename ){
        
        static bool mount_done = false;
        FILE * fp_vol_fs = fopen(filename,"a+");
        
        printf("file opened : %s \n",filename );

        char temp_vol_name[20];
        char temp_filesystem[100];
        char temp_user[30];
        
        char host_ip[20];
        char bu_point[100];     
        int flag = 0;

        printf("printing info : \n");
        
        
        fscanf(fp_vol_fs,"%s %s",host_ip,bu_point );
        
        while(fscanf(fp_vol_fs,"%s %s %s",temp_vol_name,temp_filesystem,temp_user ) > 0 ) {
                
                if(mount_done == false ){
                        // mounting existing volumes 
                        /*
                        // resolving gluster bug ( though volume is started mount fails )
                        char* vol_start_instr = (char*) malloc( ( 23 + strlen(temp_vol_name ) )* sizeof(char) );
                        strcpy(vol_start_instr,"gluster volume start ");
                        strcat(vol_start_instr,temp_vol_name);
                        strcat(vol_start_instr," force");
                        
                        printf("executing force start instr $ %s \n",vol_start_instr);
                        system(vol_start_instr );
                        */
                        char* mount_point = (char*) malloc( ( strlen(bu_point) + strlen(temp_vol_name) +3 ) * sizeof(char) );
                        strcpy(mount_point,bu_point);
                        strcat(mount_point,"/");
                        strcat(mount_point,temp_vol_name);
                        
                        char *mount_instr;
                        mount_instr=(char*) malloc( 21 * sizeof(char ) );
                        strcpy(mount_instr,"mount -t glusterfs ");
                        mount_instr = (char*) realloc( mount_instr ,strlen(mount_instr) + strlen(host_ip) + strlen(temp_vol_name) + strlen(mount_point) + 7);
                

                        strcat(mount_instr,host_ip);
                        strcat(mount_instr,":/");
                        strcat(mount_instr,temp_vol_name);
                        strcat(mount_instr," ");
                        strcat(mount_instr,mount_point);
                        printf("\nexecuting mount instr $ %s \n",mount_instr);

                        int ret_mount_instr = system(mount_instr);

                        if(ret_mount_instr  != 0){
                                printf("failed : mount volume \n");
                        }
                        
                
                }               

                // mounting ends
                if(flag==0){
                        (*head_ref)= (struct vol_fs_node * )malloc( sizeof( struct vol_fs_node ));
                        (*head_ref)->vol_name = (char*) malloc( (strlen(temp_vol_name) +1 ) * (sizeof(char ) ) );
                        (*head_ref)-> filesystem = (char*) malloc( ( strlen(temp_filesystem ) + 1 ) * (sizeof(char )));
                        (*head_ref)-> user = (char*) malloc( ( strlen(temp_user ) + 1 ) * (sizeof(char )));
                        
                        strcpy( (*head_ref)->vol_name, temp_vol_name);
                        strcpy( (*head_ref)->filesystem , temp_filesystem);
                        strcpy( (*head_ref)->user , temp_user);
                        (*head_ref)->next = NULL;

                        flag = 1;
                }
                else {
                        printf("inside else : %s %s\n",temp_vol_name,temp_filesystem);
                         char * vol_name = (char*) malloc( (strlen(temp_vol_name) +1 ) * (sizeof(char ) ) );
                        char *filesystem = (char*) malloc( ( strlen(temp_filesystem ) + 1 ) * (sizeof(char )));
                        char *user = (char*) malloc( ( strlen(temp_user ) + 1 ) * (sizeof(char )));

                        strcpy( vol_name, temp_vol_name);
                        strcpy( filesystem , temp_filesystem);
                        strcpy( user, temp_user );
                
                        insert_node( (*head_ref ) ,  vol_name, filesystem, user );
                }

        }

        if(flag == 0) return -1;
        else return 0;
        fclose(fp_vol_fs );
        mount_done = true;

}

void 
init_head ( struct vol_fs_node ** head_ref , char* vol_name, char* filesystem , char* user){
        (*head_ref) = (struct vol_fs_node *) malloc( sizeof(struct vol_fs_node ) ) ;
        (*head_ref)->vol_name = vol_name;
        (*head_ref)->filesystem = filesystem;
        (*head_ref)->user = user;
        (*head_ref)->next = NULL;
}


void
print_vol_fs_info( struct vol_fs_node * head ){

        struct vol_fs_node * iter = head;

        while(iter != NULL ){
                printf("%s  %s %s \n",iter->vol_name,iter->filesystem,iter->user);
                iter=iter->next;
        }

}

void
print_vol_info( struct vol_fs_node * head ){

        struct vol_fs_node * iter = head;

        while(iter != NULL ){
                printf("%s  \t",iter->vol_name);
                iter=iter->next;
        }
        printf("\n\n");

}


struct vol_fs_node *
find( struct vol_fs_node * head , char* filesystem ){

        struct vol_fs_node* iter = head;
        
        while(iter != NULL ){

                if(strcmp(iter->filesystem, filesystem ) == 0 )
                                return iter;

                iter = iter->next;
        }
        return NULL;
}

void set_host_config(char* filename){
        FILE * fp_vol_fs = fopen(filename,"a+");
        
        printf("file opened : %s \n",filename );
        
        char host_ip[17]="";
        char bu_point[200]="";
        int flag = fscanf(fp_vol_fs,"%s %s",host_ip,bu_point);

        printf("from host config : %d \n",flag);

        printf("ip : %s  \tbu_point : %s \n",host_ip,bu_point);
        
        if( flag == 2 ){
                printf("backup point already set \n" );
                }
        else {
                printf("configure your ip and backup path : \n");
                printf("enter the ip : ");
                scanf("%s",host_ip);
                printf("enter the backup absolute path : ");
                scanf("%s",bu_point);
                fprintf(fp_vol_fs,"%s %s\n",host_ip,bu_point);
        }
        fclose(fp_vol_fs);
}

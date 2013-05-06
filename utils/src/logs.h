/*
 * logs.h
 *
 *  Created on: Apr 23, 2013
 *      Author: test
 */




#ifndef LOGS_H_
#define LOGS_H_

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void get_timestamp(char *buf);
void write_client_log(struct sockaddr_in socket, char* message);
void write_server_log(char* message);

#endif /* LOGS_H_ */

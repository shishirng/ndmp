/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

#include <logs.h>

void get_timestamp(char timebuf[])
{
	struct timeval tv;
	time_t nowtime;
	struct tm *nowtm;
	char tmbuf[64], buf[64];
	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
	strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
	snprintf(buf, sizeof buf, "%s.%06d", tmbuf, tv.tv_usec);
	strcpy(timebuf, buf);
}

/*For logging to work properly please ensure the ndmp server is placed in / */

void write_client_log(struct sockaddr_in socket, char* message)
{
	char log_file_name[64];
	char write_buf[256];
	int log_file_len, write_buf_len, log_fd;
	char timestamp[64];
	char filename[64];

	log_file_len = sprintf(log_file_name, "/ndmp_server/log/%s.txt", inet_ntoa(socket.sin_addr));
	log_fd = open(log_file_name, O_WRONLY | O_CREAT | O_APPEND);

	get_timestamp(timestamp);

#ifdef DEBUG
        printf("length of timestamp: %d\n", strlen(timestamp));
        printf("%s\n", timestamp);
#endif

	write_buf_len = sprintf(write_buf, "[%s] [Client (%s:%d)]: %s\n",
	        		timestamp, inet_ntoa(socket.sin_addr),
	        		           ntohs(socket.sin_port), message);

	write(log_fd, write_buf, write_buf_len);
	close(log_fd);
}

void write_server_log(char* message)
{
	char log_file_name[64];
	char write_buf[256];
	int log_file_len, write_buf_len, log_fd;
	char timestamp[64];

	log_file_len = sprintf(log_file_name, "/ndmp_server/log/server.txt");
	log_fd = open(log_file_name, O_WRONLY | O_CREAT | O_APPEND);

	get_timestamp(timestamp);

#ifdef DEBUG
        printf("length of timestamp: %d\n", strlen(timestamp));
        printf("%s\n", timestamp);
#endif

	write_buf_len = sprintf(write_buf, "[%s] [Server]: %s\n",
	        		timestamp, message);

	write(log_fd, write_buf, write_buf_len);
	close(log_fd);
}

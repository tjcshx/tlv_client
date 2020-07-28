/********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  socket.h
 *    Description:  This head file socket functiona head file.
 *
 *        Version:  1.0.0(06/04/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "06/04/2020 09:52:10 PM"
 *                 
 ********************************************************************************/
#ifndef  _SOCKET_H_
#define  _SOCKET_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>
#include "sqlite3.h"
#include<stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include "init_db.h"
#include "tlv_pack.h"
#include "log.h"
#include "tlv_dns.h"
#include "tlv_unpack.h"
typedef struct socket_s
{
    int                sockfd;
    unsigned char      conn_status;
    char               servip[32];
    unsigned short     port;
    struct sockaddr_in       servaddr;
}socket_t;

enum 
{
    CONNECTED,
    DISCONNECTED
};

int socket_init(socket_t *st_socket, char *servip, unsigned short port);
int socket_connect(socket_t *st_socket);
int socket_send(socket_t *st_socket, void* send_data);
void socket_close(socket_t* st_socket);
int sock_rcve(socket_t *st_socket);
#endif   /* ----- #ifndef _SOCKET_H_  ----- */


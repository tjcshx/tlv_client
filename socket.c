/*********************************************************************************
 * 
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  socket.c
 *    Description:  This file socket function.
 *                 
 *        Version:  1.0.0(06/04/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "06/04/2020 07:37:34 PM"
 *                 
 ********************************************************************************/
#include "socket.h"

int socket_init(socket_t *st_socket, char *servip, unsigned short port)
{
    if (NULL == st_socket || NULL == servip || port <= 0 )
    {
        log_error("Error input arguemwnts", NULL, __FILE__, __LINE__);

        return -1;
    }

    char servip_buf[32] = {0};

    /* 域名解析 */
    sock_getnameinfo(servip,servip_buf, 32);

    memset(st_socket, 0, sizeof(st_socket));
    memcpy(&st_socket->servip, servip_buf, 32); 
    st_socket->port   = port;
    st_socket->conn_status =DISCONNECTED;
    st_socket->sockfd = -1;

    return 0;
}

int socket_connect(socket_t *st_socket)
{
    if (NULL == st_socket)
    {    
        log_error("Error input arguemwnts.\n");

        return -1;
    }
    int   rv = 0;

    st_socket->sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(st_socket->sockfd < 0)
    {
        log_error("socket faliure: %s.\n", strerror(errno));
        return -1;
    }

    memset(&st_socket->servaddr,0,sizeof(st_socket->servaddr));
    st_socket->servaddr.sin_family = AF_INET;
    st_socket->servaddr.sin_port = htons(st_socket->port);
    inet_pton( AF_INET,st_socket->servip,&st_socket->servaddr.sin_addr);

    if ( (rv = connect(st_socket->sockfd,(struct sockaddr *)&st_socket->servaddr,sizeof(st_socket->servaddr))) < 0)
    {

        return -1;
    }

    st_socket->conn_status = CONNECTED;
    return 0;
}

int socket_send(socket_t *st_socket, void* send_data)
{
    if (NULL == st_socket || NULL == send_data)
    {    
        log_error("Error input arguemwnts.\n");

        return -1;
    }
    tlv_buf_t *da_tlv = (tlv_buf_t*)send_data;
    int  send_bytes = da_tlv->len;
    int  rv = 0;
    int  i = 0;

    /*  向客户端发送采样好的数据 */
    if(st_socket->conn_status != CONNECTED)
    {
        return -1;

    }
    while (send_bytes > 0)
    {
        rv = write(st_socket->sockfd,&da_tlv->buf[i],da_tlv->len);
        if(rv <0)
        {
            return -1;
        }
        else if (send_bytes == rv)
        {
            da_tlv->db_flag = SEAVED_DB;
            return rv;
            break;
        }
        else
        {
            i+= rv;
            send_bytes -= rv;
            continue;
        }
    }

    return 0;
}

int sock_rcve(socket_t *st_socket)
{
    if (NULL == st_socket)
    {    
        log_error("Error input arguemwnts.\n");

        return -1;
    }
    char    buf[32] = {0};
    int     rv  = -1;
    int     i  = 0;

    if (st_socket->conn_status != CONNECTED)
    {
        return -1;
    }

    rv = read(st_socket->sockfd,buf,64);
    if(rv < 0)
    {
        log_error("Socket read error.\n");
        return -2;
    }

    if(rv == 0)
    {

        log_error("connect get disconnected.\n");
        socket_close(st_socket);
        return -1;
    }

    if(rv >0)
    {
        if ( (rv = tlv_unpack(buf, rv)) < 0)
        {

            log_error("server unpack data failure.\n");
            return -1;
        }
        else 
        {
            return 0;
        }
    }
}
void socket_close(socket_t* st_socket)
{
    if (NULL == st_socket)
    {    
        log_error("Error input arguemwnts.\n");

        return ;
    }
    st_socket->conn_status =DISCONNECTED; 
    close(st_socket->sockfd);
}


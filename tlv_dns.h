/********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  tlv_dns.h
 *    Description:  This head file DNS head file.
 *
 *        Version:  1.0.0(06/05/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "06/05/2020 10:21:11 AM"
 *                 
 ********************************************************************************/

#ifndef  _TLV_DNS_H_
#define  _TLV_DNS_H_
#include <sys/types.h>      
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include<stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <getopt.h>
#include "log.h"
/*  将域名解析为ip地址 */
extern int sock_getnameinfo(char * h_name, char *buf, int len);
#endif   /*  ----- #ifndef _TLV_DNS_H_  ----- */


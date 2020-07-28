/*********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  tlv_dns.c
 *    Description:  This file is for soocket dns and return ip address.
 *                 
 *        Version:  1.0.0(06/05/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "06/05/2020 10:19:38 AM"
 *                 
 ********************************************************************************/
/*
 *  * 描述：本函数是为socket通信实现域名解析。
 *   * 参数: char* h_name, 为所需要解析的域名
 *    *
 *     */


#include "tlv_dns.h"

int sock_getnameinfo(char * h_name, char *buf, int len)
{
    if (NULL == h_name || len < 0)
    {
                             
        log_error("Error input arguemwnts.\n");
        return -1;
    }

    struct addrinfo          *result, *result_pointer;
    int                      ret;
    int                      ch;
    char                     hostname[32] = {0};
    struct addrinfo          hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_protocol =0;   

    ret = getaddrinfo(h_name, NULL, &hints, &result);
    if (ret != 0)
    {
        log_error("dns failure: %s.\n", gai_strerror(ret));
        return -1;;
    }

    for (result_pointer = result; result_pointer != NULL; result_pointer = result_pointer->ai_next)
    {

        ret = getnameinfo(result_pointer->ai_addr, result_pointer->ai_addrlen, hostname, sizeof(hostname), NULL, 0, NI_NUMERICHOST);
        if (ret != 0)  
        {
            log_error("dns failure: %s.\n",gai_strerror(ret));
            continue;   
        }
        else
        {          
            memset(buf, 0, len);
            memcpy(buf, hostname, len);
            log_info("dns get ipaddr:%s\n", hostname);
        }
    }

    return 0;
}


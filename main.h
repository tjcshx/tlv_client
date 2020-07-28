/********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  main.h
 *    Description:  This head file main head file.
 *
 *        Version:  1.0.0(06/05/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "06/05/2020 10:18:25 AM"
 *                 
 ********************************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__

#include <string.h>
#include <time.h>
#include "sqlite3.h"
#include<stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include "time.h"
#include "init_db.h"
#include "tlv_pack.h"
#include "log.h"
#include "tlv_dns.h"
#include "tlv_unpack.h"
#include "socket.h"
#include "get_temperature.h"

#define BUF_SIZE           1024  /*  所使用的buf大小            */
#define ACK                1     /*  服务器端接收数据并解包成功 */
#define NAK                0     /*  服务器端接收数据或解包失败 */
#define SEND_MAX_TIMES     3
#define SN_SIZE            64

enum 
{
    SEND_SAMPLE,
    DISSEND_SAMPLE
};

/*   获取温湿度 */
int   get_temperature(float *temp);

/*   获取产品序列号 */
int   get_sn(char *sn, int len);

/*   安装信号 */
void  install_signal();

/*   信号处理函数 */
int   sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
void  sig_handler(int signum);


/*   功能描述：socket发送数据 
 *   参数：
 *      1、socket fd 
 *      2、tlv_buf_t tlv 若不为空，则发送采样数据，若为空则无采样数据要发送
 *      3、 s_db_data* db_data 若不为空，则发送数据库数据，若为空则数据库无需要数据要发送
 *
 * */
int   socket_send_tlv_data(void *st_socket, void *send_data, int error_send_times);

#endif 


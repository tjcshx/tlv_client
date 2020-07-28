/********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  log.h
 *    Description:  This head file is for log function head file.
 *
 *        Version:  1.0.0(05/10/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "05/10/2020 09:14:00 PM"
 *                 
 ********************************************************************************/


#ifndef  _LOG_H_
#define  _LOG_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<time.h>
#include <stdarg.h>


enum en_log_level
{
    ERROR,
    INFO 
};

#define LOG_BUFFSIZE        256
#define MAX_ROLL_LOG_SIZE   (10*1024)
#define LOGFILENAME  "tlv_message.log"
#define log_info(fmt,...) log_write(__FILE__,__func__,__LINE__,INFO,fmt,##__VA_ARGS__) 
#define log_error(fmt,...)log_write(__FILE__,__func__,__LINE__,ERROR,fmt,##__VA_ARGS__) 

extern FILE *g_file;
extern FILE* log_open();
extern int log_close();
extern int log_print(char *log_msg);
extern int log_roll_back();
extern int log_write(char* file, const char *func, int line, enum en_log_level, const char *fmt, ...);

#endif   /* ----- #ifndef _LOG_H_  ----- */

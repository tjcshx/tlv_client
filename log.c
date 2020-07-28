/*********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  log.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(05/10/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "05/10/2020 09:13:28 PM"
 *                 
 ********************************************************************************/
/* 
 *函数功能描述：主要记录tlv_client的错误信息
 *参数：err_msg 传入需要打印的信息
 * 
 * */

#include"log.h"

FILE * g_file = NULL;
static const char* s_loginfo[] =
{
    "ERROR","INFO"
};

/* 创建文件记录error和warn信息*/
FILE* log_open()
{
    g_file = fopen(LOGFILENAME,"a");
    if(NULL == g_file)
        return NULL;
}
int log_write(char* file, const char *func, int line, enum
        en_log_level level, const char *fmt, ...)
{
    char           buf[LOG_BUFFSIZE];
    char           log_buf[LOG_BUFFSIZE];
    va_list        arg_list;
    int            millisec;
    off_t          filesize;

    memset(buf,0,sizeof(buf));
    va_start(arg_list,fmt);
    vsnprintf(buf,sizeof(buf),fmt,arg_list);
    va_end(arg_list);

    if(level > INFO)
    {
        return -1;
    }


    snprintf(log_buf,sizeof(buf),"%s,File: %s, Func: %s, Line :%d,information is: %s",s_loginfo[level],file, func, line, buf);

    log_print(log_buf);
}

int log_print(char *log_msg)
{
    if (NULL == log_msg)
    {                                                     
        log_error("Error input arguemwnts");
        return -1;                                         
    }    

    char log_buf[LOG_BUFFSIZE];
    snprintf(log_buf,LOG_BUFFSIZE, "tlv_client log message:\n %s \n",log_msg);
    fwrite(log_buf,strlen(log_buf),1,g_file);
    log_roll_back(LOGFILENAME);

    return 0;

}


/* 简单日志回滚 */
int log_roll_back()
{
    FILE *fp;
    if( (fp = fopen(LOGFILENAME,"r")) == NULL)
    {
        return -1;
    }
    fseek(fp,0L,SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    if(size >= MAX_ROLL_LOG_SIZE)
    {
        char name_str[32]={0};
        strcpy(name_str,LOGFILENAME);
        strcat(name_str,(char *)".bak");
        remove(name_str);
        if( !rename(LOGFILENAME,name_str));
        {
            return -1;
        }
    }
    return 0;
}

/* 程序结束后调用，关闭日志文件*/
int log_close()
{
    fclose(g_file);
}



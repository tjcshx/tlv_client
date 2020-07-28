/*********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  get_sn.c
 *    Description:  This file is get sn.
 *                 
 *        Version:  1.0.0(06/19/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "06/19/2020 05:22:14 PM"
 *                 
 ********************************************************************************/
#include "get_sn.h"
#include "log.h"

int get_sn(char *sn, int len)
{
    int             fd = -1;
    int             rv =  0;
    char            buf[BUF_SIZE];
    char            chip[CHIP_SIZE];
    char            DIR_PATH[DIR_PATH_SIZE] ="/sys/bus/w1/devices/";
    char            *ptr = NULL;
    int             find = 0;
    DIR             *dirp = NULL;
    struct dirent   *direntp = NULL;

    if((dirp = opendir(DIR_PATH))==NULL)
    {
        log_error("open %s failure:%s.\n",DIR_PATH,strerror(errno));
        return -1;
    }
    while((direntp= readdir(dirp))!=NULL)
    {
        if(strstr(direntp->d_name,"28-"))
        {
            memset(sn,0,len);
            memcpy(sn,direntp->d_name,len);

            break;
        }
    }
}


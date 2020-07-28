/*********************************************************************************
 *      Copyright:  (C) 2019 tianjincheng<473892093@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  get_t.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(11/15/2019)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "11/15/2019 10:26:40 PM"
 *                 
 ********************************************************************************/
#include "get_temperature.h"
#include "log.h"

int get_temperature(float *temp)
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
            memset(chip,0,sizeof(chip));
            strncpy(chip,direntp->d_name,sizeof(chip));
            find = 1;
            break;
        }
    }
    if(!find)
    {
        log_error("No such devices.\n");
        goto cleanup;
        return -2;
    }

    closedir(dirp);

    strncat(DIR_PATH,chip,sizeof(DIR_PATH)); 
    strncat(DIR_PATH,"/w1_slave",sizeof(DIR_PATH));

    if((fd = open(DIR_PATH,O_RDONLY))<0)
    {
        log_error("Open %s failure:%s.\n",DIR_PATH,strerror(errno));
        goto cleanup;
        return -3;
    }
    memset(buf,0,sizeof(buf));
    lseek(fd,0,SEEK_SET);
    if((rv = read(fd,buf,sizeof(buf)))<0)
    {
        log_error("Read failure: %s\n",strerror(errno));
        goto cleanup;
        return -4;
    }
    ptr = strstr(buf,"t=")+2;
    if(!ptr)
    {
        log_error("Gte value failure.\n");
        return -5;
    }

    *temp = atof(ptr)/1000;

    return 0;
cleanup:
    close(fd);
}


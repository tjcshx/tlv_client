/********************************************************************************
 *      Copyright:  (C) 2020 chenxiaohong<www.530990157@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  gettemp.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(07/05/20)
 *         Author:  chenxiaohong <www.530990157@qq.com>
 *      ChangeLog:  1, Release initial version on "07/05/20 17:40:50"
 *                 
 ********************************************************************************/

#ifndef  _GETTEMP_H_
#define  _GETTEMP_H_
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#define BUF_SIZE      1024
#define CHIP_SIZE     100

#define DIR_PATH_SIZE 100

extern int get_temperature(float* temp);

#endif   /* ----- #ifndef _GETTEMP_H_  ----- */

/********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  tlv_unpack.h
 *    Description:  This head file is for tlv_unpack head file.
 *
 *        Version:  1.0.0(05/29/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "05/29/2020 10:26:51 PM"
 *                 
 ********************************************************************************/

#ifndef  _TLV_UNPACK_H_
#define  _TLV_UNPACK_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ACK                1
#pragma pack (1)
typedef struct tlv_unpack_s 
{
    unsigned char  HEAD;
    unsigned char  TAG;
    unsigned char  len;
    unsigned char  data;
} tlv_unpack_t;  
/*  
 *  功能描述：用于tlv的数据解包
 *  参数：  
 **/
extern int tlv_unpack (char *buf, int len);

#endif   /* ----- #ifndef _TLV_UNPACK_H_  ----- */

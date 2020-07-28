/*********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  tlv_pack.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/20/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "04/20/2020 03:28:26 PM"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include "tlv_pack.h"
#include "crc-itu-t.h"
#include "log.h"


int packtlv_sn  (char *buf, int size, char *sn)
{
    if (NULL == buf || size < 0 || NULL == sn)
    {                                                     
        log_error("Error input arguemwnts\n");
        return -1;                                         
    }    

    int        ofset = 0;    //定义buf的偏移量 
    int        p_len = 0;    //pack的length 
    int        p_load= 0;    //需要写入buf的数据长度 
    uint16_t   crc16 = 0;
    buf[ofset] = PACK_HEADER;
    ofset += 1;
    buf[ofset] = TAG_SN;
    ofset += 1;
    if (strlen(sn) <= size -  TLV_FIXED_SIZE) //防止写入buf数据溢出
    {
        p_load = strlen(sn);
    }
    else 
        p_load = size -  TLV_FIXED_SIZE;

    p_len =p_load + TLV_FIXED_SIZE;
    buf[ofset] = p_len;
    ofset += 1;
    memcpy(&buf[ofset], &sn, p_load);
    ofset += p_load;  
    crc16 = crc_itu_t(MAGIC_CRC, buf, ofset);
    ushort_to_bytes(&buf[ofset], crc16);
    ofset +=2;
    return ofset;

}

int packtlv_temp(char *buf, int size, uint16_t temp)
{ 
    if (NULL == buf || size < TLV_FIXED_SIZE + sizeof(uint16_t))
    {                                                     
        log_error("Error input arguemwnts\n");
        return -1;                                         
    }    
    int     ofset = 0;  /* 定义buf的偏移量 */
    int     p_len = 0;  /* pack的length */
    int     p_load= 0;  /* 需要写入buf的数据长度 */
    int     integer = 0;
    uint16_t   crc16 = 0;
    int     decimal  = 0;

    buf[ofset] = PACK_HEADER;
    ofset += 1;
    buf[ofset] = TAG_TEMP;
    ofset += 1;
    if (sizeof(uint16_t) <= size -  TLV_FIXED_SIZE) /* 防止写入buf数据溢出 */
    {
        p_load = sizeof(uint16_t);
    }
    else 
        p_load = size -  TLV_FIXED_SIZE;

    p_len = p_load + TLV_FIXED_SIZE;
    buf[ofset] = p_len;
    ofset += 1;
    integer = (temp)/100;
    decimal = (temp)%100; 
    buf[ofset] = integer;
    ofset+=1;
    buf[ofset] = decimal;
    ofset += 1;
    crc16 = crc_itu_t(MAGIC_CRC, buf, ofset);

    ushort_to_bytes(&buf[ofset], crc16);
    ofset +=2;
    return ofset;

}

int packtlv_time(char *buf, int size, struct tm *tm)
{
    if (NULL == buf || NULL == tm || size < TLV_FIXED_SIZE +6)
    {                                                     
        log_error("Error input arguemwnts\n");
        return -1;                                         
    }    
    int     ofset = 0;  /* 定义buf的偏移量 */
    int     p_len = 0;  /* pack的length */
    int     p_load= 0;  /* 需要写入buf的数据长度 */
    uint16_t   crc16 = 0;

    buf[ofset] = PACK_HEADER;
    ofset += 1;
    buf[ofset] = TAG_TIME;
    ofset += 1;
    p_len = TLV_FIXED_SIZE +6;
    buf[ofset] = p_len;
    ofset += 1;

    buf[ofset++] = tm->tm_year -100;
    buf[ofset++] = tm->tm_mon  + 1;
    buf[ofset++] = tm->tm_mday;
    buf[ofset++] = tm->tm_hour;
    buf[ofset++] = tm->tm_min;
    buf[ofset++] = tm->tm_sec;

    crc16 = crc_itu_t(MAGIC_CRC, buf, ofset);

    ushort_to_bytes(&buf[ofset], crc16);
    ofset +=2;
    return ofset;

}


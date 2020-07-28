/*********************************************************************************
 * 
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  tlv_unpack.c
 *    Description:  This file is Description tlv_unpack simple.
 *                 
 *        Version:  1.0.0(05/29/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "05/29/2020 10:33:17 PM"
 *                 
 ********************************************************************************/
#include "tlv_unpack.h"
#include "tlv_pack.h"
#include "crc-itu-t.h"
#include "log.h"

int tlv_unpack (char *buf, int len)
{
    if (NULL == buf || len < TLV_MIN_SIZE)
    {
        log_error("Error input arguements\n");

        return -1;
    }

    char           crc_buf[2] = {0};
    unsigned short crc16 = 0;
    int            ofset = len;  
    int            i = 0;

    if (buf[i] == PACK_HEADER)
    {
        log_info("Find head.\n");
    }

    if (ofset < TLV_MIN_SIZE)
    {
        log_error("Error tlv len\n");
        return -1;
    }

    tlv_unpack_t *tlv_unpack = (tlv_unpack_t *)&buf[i];

    if ( PACK_HEADER == tlv_unpack ->HEAD)
    {
        log_info("Find head.\n"); 
    }
    else
    {
        log_error("Not find head.\n");
        return -1;
    }

    if (TAG_ACK == tlv_unpack->TAG)
    {
        log_info("Find TAG_ACK.\n");
    }
    else 
    {
        log_error("Not find tag.");
        return -1;
    }

    if (tlv_unpack-> len < TLV_MIN_SIZE)
    {
        log_error("Length error.\n");
        return -1;
    }

    crc16 = crc_itu_t(MAGIC_CRC, buf, tlv_unpack->len - 2);
    if (crc16 != bytes_to_ushort((unsigned char *)&buf[tlv_unpack->len - 2], 2))
    {
        log_error("tlv packet crc checkout error.\n");
    }

    if(ACK == tlv_unpack->data)
    {
        return 0;
    }
    else 
    {
        log_error("Not find ack.\n");
        return -1;
    }

} 



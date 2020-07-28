/********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  tlv_pack.h
 *    Description:  This head file tlv packet head file.
 *
 *        Version:  1.0.0(06/05/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "06/05/2020 10:26:44 AM"
 *                 
 ********************************************************************************/

#ifndef  _TLV_PACK_H_
#define  _TLV_PACK_H_

#include <stdint.h>
#include <time.h>

/*  TLV Packet format: 
 *  
 *  +------------+---------+------------+-----------+-----------+
 *  | Header(1B) | Tag(1B) | Length(1B) | Value(1B) | CRC16(2B) |
 *  +------------+---------+------------+-----------+-----------+
 *  tlv 头部 **/

#define PACK_HEADER        0xFD 
#define TLV_FIXED_SIZE     5
#define TLV_MIN_SIZE       (TLV_FIXED_SIZE+1)

/*  tlv的各个数据部分的TAG */
enum
{
    TAG_ACK=1,
    TAG_SN,
    TAG_TEMP,
    TAG_TIME,
};

#define TLV_BUFSIZE        256

#define TLV_FLAG_TX_NEED        1 /*  This TLV message need to be sent by socket    */
#define TLV_FLAG_TX_OK          0
#define TLV_FLAG_DB        1<<1 /*  This TLV message need to be saved in database */
typedef struct tlv_buf_s 
{
    char                   buf[TLV_BUFSIZE];
    int                    len;  /*  data length */
    int                    size; /*  buffer size */
    unsigned char          db_flag;
    unsigned char          sample_flag;


} tlv_buf_t;  

int packtlv_ack (char *buf, int size, int ack);

/*    Packet TLV message for SN */
int packtlv_sn  (char *buf, int size, char *sn);

/*    Packet TLV message for temperature */
int packtlv_temp(char *buf, int size, uint16_t temp);

/*    Packet TLV message for date time */
int packtlv_time(char *buf, int size, struct tm *tm);

/*    Packet 3 TLV message together: with SN, temperature, date&time */
extern int packtlv_msg(tlv_buf_t *tlv, char *sn, uint16_t temp, struct tm *tm);

/*    print buf data in hex and string mode  */
void dump_buf(char *data, int len);

#endif   /*  ----- #ifndef _TLV_PACK_H_  ----- */



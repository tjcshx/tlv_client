/********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  init_db.h
 *    Description:  This head file init database head file.
 *
 *        Version:  1.0.0(06/05/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "06/05/2020 10:16:57 AM"
 *                 
 ********************************************************************************/
#ifndef __INIT_DB_H__
#define __INIT_DB_H__
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "tlv_pack.h"
#define  DB_PATHNAME  "/tmp/temp.db"
#define  BUFSIZE      256
typedef struct st_db_data
{
    char                   buf[TLV_BUFSIZE];
    int                    len;  /*   data length */
    int                    size; /*   buffer size */ 
    unsigned short         db_first_id;
}db_data_t;

enum 
{
    DISSEAVE_DB,
    SEAVED_DB

};

/*   创建数据库 */
extern sqlite3 * sqlite_create();
/*     从数据库中读取数据  */
extern int  read_db_data(sqlite3* db,db_data_t * data);

/*    将数据写入数据库 */
extern int   write_into_db(sqlite3 *db, tlv_buf_t *tlv);

/*    从数据库中删除已经发送成功的数据 */
extern int   rm_tlvda_db(sqlite3 *db, int db_id);

#endif


/*********************************************************************************
 *      Copyright:  (C) 2020 shx
 *                  All rights reserved.
 *
 *       Filename:  init_db.c
 *    Description:  This file intit database simple.
 *                 
 *        Version:  1.0.0(06/05/2020)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "06/05/2020 10:14:12 AM"
 *                 
 ********************************************************************************/

#include "init_db.h"
#include "log.h"
static inline int callback(void* para, int f_num, char ** f_value,char** f_name);

sqlite3 * sqlite_create()
{
    sqlite3*  db = NULL;
    const char*       sql_create = "create table if not exists temp(id integer PRIMARY KEY autoincrement,tlv_data blob)";

    if (access(DB_PATHNAME, F_OK) != 0)  //判断数据库文件是否存在
    {
        if (sqlite3_open(DB_PATHNAME, &db) != SQLITE_OK)
        {
            log_error("Create open db file error:%s.\n", sqlite3_errmsg(db));

            return ;
        }
        else 
        {
            log_info("Open sqlite success.\n");
        } 
        if (sqlite3_exec(db, sql_create, NULL, NULL, NULL)!= SQLITE_OK)
        {
            log_error("create db error:%s.\n",  sqlite3_errmsg(db));
            unlink(DB_PATHNAME);

            return ;
        }

        return db;
    }
    else
    {
        if (sqlite3_open(DB_PATHNAME, &db) != SQLITE_OK) 
        {

            log_error("open db error:&s.\n",sqlite3_errmsg(db));

            return ;
        }

        return db;
    }
}

int  write_into_db(sqlite3 *db, tlv_buf_t *tlv)
{
    if (NULL == tlv || NULL == db)
    {
        log_error("Error input arguemwnts.\n");
        return -1;
    }

    char             sql_insert[64] = {0};
    sqlite3_stmt     *stmt = NULL;

    memset(sql_insert, 0, 64);
    snprintf(sql_insert, 64, "insert into temp values(null,?);");

    sqlite3_prepare(db, sql_insert, strlen(sql_insert),&stmt,0);
    {
        sqlite3_bind_blob(stmt, 1, &tlv->buf, tlv->len,NULL);
        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);

    return 0;
}

int read_db_data(sqlite3* db,db_data_t* s_data)
{
    if (NULL == s_data || NULL == db)
    {                                                     
        log_error("Error input arguemwnts.\n");
        return -1;                                         
    }    

    char sql_select[64]      = "select tlv_data from temp limit 1";
    char sql_id[64]          = "select * from temp limit 1";
    unsigned short first_id  = -1;
    sqlite3_stmt      *pstmt = 0;

    if (sqlite3_exec(db, sql_select, callback, s_data, NULL)!= SQLITE_OK)
    {
        log_error("read db error: %s.\n", sqlite3_errmsg(db));
        return -1;
    }

    if( (sqlite3_prepare(db, sql_id, strlen(sql_id), &pstmt, NULL)) != SQLITE_OK )
    {
        printf(sqlite3_errmsg(db), NULL, __FILE__, __LINE__, __TIME__);
        return -1;
    }

    sqlite3_step(pstmt);

    /*   读取整形数据 第一个数据的ID */
    first_id = sqlite3_column_int(pstmt,0);
    sqlite3_finalize(pstmt);

    return 0;
}

/*  回调函数 */
int callback(void* para, int f_num, char ** f_value,char** f_name)
{
    db_data_t * db_da = (db_data_t* )para;
    memset(db_da->buf, 0, BUFSIZE);
    memcpy(&db_da->buf, *f_value, BUFSIZE);

    return 0;
}

/*   从数据库中删除已经发送成功的数据 */
int rm_tlvda_db(sqlite3 *db, int db_id)
{
    if (db_id < 0 || NULL == db)
    {                                                     
        log_error("Error input arguemwnts");
        return -1;                                         
    }    

    char sql[64] = {0};
    snprintf(sql,64,"delete from temp where id = %d",db_id);
    if (sqlite3_exec(db, sql,NULL, NULL, NULL)!= SQLITE_OK)
    {
        log_error("rm data from db error:%s", sqlite3_errmsg(db));
    }

    return 0;
}


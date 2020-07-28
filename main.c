/********************************************************************************
 *      Copyright:  (C) 2019 tianjincheng<473892093@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  socket_client.c
 *    Description:  This file main function.
 *                 
 *        Version:  1.0.0(11/26/2019)
 *         Author:  tianjincheng <473892093@qq.com>
 *      ChangeLog:  1, Release initial version on "11/26/2019 07:03:35 PM"
 *                 
 ********************************************************************************/

#include "main.h"

static int   g_stop = 0;

static inline void  print_usage(char *paogname)
{
    printf("%s usage : \n", paogname);
    printf("-i(--ipaddr): sepcify server IP address or sepcify hostname.\n");
    printf("-p(--port): specify sever port.\n");
    printf("-d(--damemon): programe runing ing bg.\n");
    printf("-h(--help): print this help information.\n");

    return ;
}

int main (int argc, char **argv)
{
    int                      max_send_num = 6; /* 最大读取并发送的数据库数据的数目 */
    int                      rv = -1;
    char                     *servip = NULL;
    int                      port    = 0;
    char                     buf[BUF_SIZE];
    int                      ch;
    float                    temp = 0;
    char                     sn[SN_SIZE] = {0};
    time_t                   cur_time;
    time_t                   last_time;
    sqlite3*                 db;
    uint16_t                 u16_temp = 0;
    tlv_buf_t                tlv;
    time_t                   tmp;  
    struct tm *              s_ptime;
    int                      i = 0;
    char                     time_buf[32] = {0};
    db_data_t                st_db_data;
    socket_t                 st_socket;


    struct option        opts[] = 
    {
        {"ipaddr/hostname", required_argument, NULL, 'i'},
        {"port", required_argument, NULL, 'p'},            
        {"help", no_argument, NULL, 'h'},          
        {NULL, 0, NULL, 0}
    };

    while((ch=getopt_long(argc, argv, "i:p:h", opts, NULL)) != -1 )    
    {
        switch(ch)
        {
            case 'i':   
                servip=optarg;
                break;
            case 'p':
                port=atoi(optarg);
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
        }           
    }

    if( !servip || !port )        
    {
        print_usage(argv[0]);
        return 0;        
    }

    /* 初始化日志系统 */
    log_open();

    /* 安装信号 */
    install_signal();

    /* 初始化数据库 */
    db = sqlite_create();

    /* 初始化socket  */
    if ( (rv= socket_init(&st_socket, servip, port)) < 0)
    {
        log_error("socket init error:%s\n", strerror(errno));
    }

    /* 记录初始时间 */
    last_time = 0;

    while(!g_stop)
    {
        /* 获取当前时间 */
        cur_time = time(&cur_time);

        /*  控制采样时间为3s */
        if (cur_time - last_time >3)
        {
            log_info("Start to simple tempterature now...\n");
            if (get_temperature(&temp))
            {
                log_error("get temp error:%s\n", strerror(errno));
            }

            if(get_sn(sn, SN_SIZE))
            {
                log_error("Get sn error.\n");
            }

            time(&tmp); 
            s_ptime = localtime(&tmp);
            u16_temp = temp *100;

            if (!(rv = packtlv_msg(&tlv, sn, u16_temp,s_ptime)))  /* tlv数据打包 */
            {
                tlv.db_flag  = SEAVED_DB;
                tlv.sample_flag = SEND_SAMPLE;
                st_db_data.len = tlv.len;
            }

            last_time = cur_time;

            if (st_socket.conn_status == DISCONNECTED)
            {
                if ( (rv = socket_connect(&st_socket)) < 0)
                {
                    log_error("connect error:%s\n", strerror(errno));

                    if(tlv.db_flag)
                    {
                        if (0==(write_into_db(db, &tlv)))
                        {
                            tlv.db_flag = DISSEAVE_DB;
                            log_info("seave simple data into db ok.\n");
                        }

                    }

                    socket_close(&st_socket);
                    continue;
                }

                st_socket.conn_status = CONNECTED;
            }


            if(CONNECTED == st_socket.conn_status) /* 网络连接成功 */
            {
                if (SEND_SAMPLE == tlv.sample_flag)
                {
                    rv = socket_send_tlv_data(&st_socket, &tlv,SEND_MAX_TIMES);
                    if(rv < 0)
                    {
                        log_error("socket send packet data error.\n");
                        if (0 == write_into_db(db, &tlv) && tlv.db_flag)
                        {
                            log_info("Send simle data error, and will seave into db.\n");
                            tlv.db_flag = DISSEAVE_DB;
                        }

                        continue;
                        st_socket.conn_status = DISCONNECTED;
                    }
                    
                    tlv.sample_flag = DISSEND_SAMPLE;
                }


                if(((0 == read_db_data(db,&st_db_data)) >0) && CONNECTED == st_socket.conn_status)  /*从数据库中读取数据并发送*/
                {

                    for (i = 1;i<max_send_num; ++i )
                    {
                        if ( (0 == socket_send_tlv_data(&st_socket,&st_db_data, SEND_MAX_TIMES)))
                        {
                            rm_tlvda_db(db, st_db_data.db_first_id);
                        }
                        else 
                        {
                            break;
                        }

                    }
                }
            }
        }

        sleep(1);

    } 

    log_info("The progream get SIGINT or SIGTERM signal and the paogram will exit now...\n");
    sqlite3_close(db);
    socket_close(&st_socket);
    log_close();
    return 0;
}

/* 安装信号 */
void  install_signal()  
{
    struct sigaction act, oldact; 
    act.sa_handler = sig_handler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT|SIGTERM);
    act.sa_flags = 0; 
    log_info("The programe statr running,and will print mssage into[tlv_message.log].\n");
    sigaction(SIGINT, &act, 0);
    sigaction(SIGTERM, &act, 0);
}
void sig_handler(int signum)      
{                                  
    if (signum == SIGINT || signum == SIGTERM)          
    {
        g_stop = 1;                
    }
}                                  

int socket_send_tlv_data(void *st_data, void * send_data, int error_send_times)
{
    socket_t * st_sock_da = (socket_t *) st_data;
    int i = 0;
    int rv;

    if (st_sock_da->conn_status == DISCONNECTED)
    {
        return -1;
    }

    for (i = 0; i< error_send_times; ++i)
    {
        if ( (rv = socket_send(st_sock_da,send_data)) < 0)
        {
            log_error("Send tlv data error: %s.\n", strerror(errno));
            return -1;
        }

        if(rv > 0)
        {
            log_info("send data ok, wait for ack...\n");
        }

        rv = sock_rcve(st_sock_da);
        if ( rv < 0)
        {
            log_error("Wait ACK error.\n");
            continue;
        }

        return 0;

    }
}

/* 将数据以TLV的形式打包 */
int packtlv_msg(tlv_buf_t *tlv, char *sn, uint16_t temp, struct  tm *tm)
{
    int               rv = 0;
    if( !tlv )
    {
        return -1;
    }

    memset(tlv->buf, 0, sizeof(tlv->buf));   /*   将buf置空 */
    tlv->size = sizeof(tlv->buf);            /*   设置buf的初始size*/
    tlv->len = 0;                            /*   设置buf的初始数据长度 */

    if( sn )
    {
        rv = packtlv_sn(&tlv->buf[tlv->len], tlv->size, sn);
        if( rv > 0 )
        {
            tlv->len += rv;
            tlv->size -= rv;

        }

    }

    if( temp )
    {
        rv = packtlv_temp(&tlv->buf[tlv->len], tlv->size, temp);
        if( rv > 0 )
        {
            tlv->len += rv;
            tlv->size -= rv;
        }
    }
    if( tm )
    {
        rv = packtlv_time(&tlv->buf[tlv->len], tlv->size, tm);
        if( rv > 0 )
        {
            tlv->len += rv;
            tlv->size -= rv;
        }

    }
    return 0;
}

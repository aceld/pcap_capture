#include "dpi.h"
#include "dpi_list.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>




//定义全局的链接协议链接集合
//static struct dpi_connection g_connections[PROTOCOL_TCP_MAX];
static dpi_list *g_connections[PROTOCOL_TCP_MAX];


/* -------------------------------------------*/
/**
 * @brief  初始化链接链表
 */
/* -------------------------------------------*/
void init_connection_list() {
    int i;

    for (i = 0; i < PROTOCOL_TCP_MAX; i ++) {
        g_connections[i] = dpi_list_create();
    }
}


/* -------------------------------------------*/
/**
 * @brief  销毁链接链表
 */
/* -------------------------------------------*/
void destory_connection_list() {
    int i;

    for (i = 0; i < PROTOCOL_TCP_MAX; i ++) {
        dpi_list_destory(g_connections[i]);
    }
}




//对已经探测到的协议报文，保存IP+port 的链接信息
int add_connection(struct dpi_result *res, struct dpi_pkt *pkt, dpi_tcp_protocol protocol) 
{
    int ret = 0;

    dpi_connection *con = malloc(sizeof(dpi_connection));
    if (con == NULL) {
        return -1;
    }
    memset(con, 0, sizeof(dpi_connection));


    /* 保存源ip+port的链接信息 */
    con->ipv4.src_ip = pkt->ip_packet->saddr;
    con->ipv4.dst_ip = pkt->ip_packet->daddr;


    //保存端口信息
    if (pkt->tcp_packet != NULL) {
        /* TCP */
        con->ipv4.src_port = pkt->tcp_packet->source;
        con->ipv4.dst_port = pkt->tcp_packet->dest;

    }

    //将链接添加对应协议的链表中
    dpi_list_append(g_connections[protocol], con);

    return ret;
}

/*
   ip + port 的比较，比较保存的当前的报文的ip_pair是否相同

    return:
        0: 相同
        1: 不同
 */
int cmp_connection(struct dpi_result *res, struct dpi_pkt *pkt, dpi_tcp_protocol protocol)
{
    int ret = 1;
    
    uint32_t sip;
    uint32_t dip;
    uint16_t sport;
    uint16_t dport;

    dpi_list_node *node = g_connections[protocol]->head.next;

    for (; node != &g_connections[protocol]->head; node = node->next) {
        dpi_connection *con = node->data;

        sip = con->ipv4.src_ip;
        dip = con->ipv4.dst_ip;
        sport = con->ipv4.src_port;
        dport = con->ipv4.dst_port;
         

        /*
            计算机A <-----> 计算机B 
            源A == 源A 表示 A---->B的包
            //源A == 目的A, 表示 B---->A的包
         */

        if (sip == pkt->ip_packet->saddr) {
            /* A ---> B */
            if (dip == pkt->ip_packet->daddr) {
                if (pkt->tcp_packet != NULL) {
                    // tcp协议
                    if ((sport == pkt->tcp_packet->source) && (dport == pkt->tcp_packet->dest)) {
                        ret = 0;
                        goto END;
                    }
                } 
            }
        }
#if 1
        else if (sip == pkt->ip_packet->daddr) {
            /* B ---> A 的包 */
            if (dip == pkt->ip_packet->saddr) {
                if (pkt->tcp_packet != NULL) {
                    /* tcp 协议 */
                    if ((sport == pkt->tcp_packet->dest) && (dport == pkt->tcp_packet->source)) {
                        ret = 0;
                        goto END;
                    }
                }
            }
        } else {
            /* nothing to do */
        }
#endif
    }


END:
    return ret;
}


void show_connections()
{
    int i; 
    for (i = 0; i < PROTOCOL_TCP_MAX; i++) {
        printf("========= TCP Connection =========\n");
        printf("tcp protocol : %s\n", protocol_tcp_string[i]);

        dpi_list_node *node = g_connections[i]->head.next;
        while( node != &g_connections[i]->head) {
            dpi_connection *con = node->data;
            struct in_addr in;
            in.s_addr = con->ipv4.src_ip;
            printf("src:%s:%d\t", inet_ntoa(in), ntohs(con->ipv4.src_port));
            in.s_addr = con->ipv4.dst_ip;
            printf("dst:%s:%d\n", inet_ntoa(in), ntohs(con->ipv4.dst_port));
            node = node->next;
        }
    }
}

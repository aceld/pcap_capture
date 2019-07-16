#pragma once

#include <pcap/pcap.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include "utils.h"



//tcp应用层协议索引号
typedef enum _dpi_tcp_protocol
{
    SSH = 0,
    HTTP,
    PROTOCOL_TCP_MAX
}dpi_tcp_protocol;


#define PROTOC_TCP_STRING   \
        "SSH",          \
        "HTTP"

extern char *protocol_tcp_string[PROTOCOL_TCP_MAX];

//udp应用层协议索引号
typedef enum _dpi_udp_protocol
{
    TFTP = 0,
    NTP,
    PROTOCOL_UDP_MAX
}dpi_udp_protocol;


#define PROTOC_UDP_STRING   \
        "TFTP",             \
        "NTP"

extern char *protocol_udp_string[PROTOCOL_UDP_MAX];


//句柄定义
typedef struct dpi_result 
{
    void *pcap_handle;
    
    unsigned int ether_count; //以太坊报文数量
    
    unsigned int ip_count;    //ip报文数量

    unsigned int tcp_count;   //tcp  报文数量
    unsigned int udp_count;   //udp  报文数量

    unsigned int tcp_protocol_count[PROTOCOL_TCP_MAX];
    unsigned int udp_protocol_count[PROTOCOL_UDP_MAX];

}dpi_result;


/*
   一个报文的解析信息
 */
typedef struct dpi_pkt 
{
    uint32_t ether_len; //以太网报文长度
    struct ether_header *ether_packet; //以太坊报文地址
    uint32_t ip_len; //IP报文长度
    struct iphdr *ip_packet; //IP报文地址

    uint32_t tcp_len; //tcp报文长度
    struct tcphdr *tcp_packet; //tcp报文地址

    uint32_t udp_len; //udp报文长度
    struct udphdr *udp_packet; //udp报文地址

    uint8_t *payload;       //报文装载的数据区域
    uint32_t payload_len;   //报文装载的数据区域长度

}dpi_pkt;


/*  保存已探测的写 ip+port对 链接信息 */
struct ipv4_port_pair {
    uint32_t src_ip;
    uint16_t src_port;
    uint32_t dst_ip;
    uint16_t dst_port;
};


//dpi的链接信息定义
typedef struct dpi_connection {
    struct ipv4_port_pair ipv4;

    /* 根据需要添加 ...如果是ipv6 */
    
}dpi_connection;


//对已经探测到的协议报文，保存IP+port 的链接信息
int add_connection(struct dpi_result *res, struct dpi_pkt *pkt, dpi_tcp_protocol protocol);

/*
   ip + port 的比较，比较保存的当前的报文的ip_pair是否相同

    return:
        0: 相同
        1: 不同
 */
int cmp_connection(struct dpi_result *res, struct dpi_pkt *pkt, dpi_tcp_protocol protocol);



void init_connection_list();
void destory_connection_list();
void show_connections();


//1 初始化
//pcapfile: 要处理的pcap文件
//返回值: 设计一个句柄，这个句柄包含了结果
//成功返回非空的指针，失败，返回NULL
dpi_result *dpi_init(const char *pcapfile);

//2 业务处理
//启动执行报文的函数
void dpi_loop(dpi_result *res);

//3 资源释放
void dpi_destory(dpi_result *res);


//处理以太网报文
void dpi_pcap_callback(u_char *user, const struct pcap_pkthdr *h, const u_char *data);

//处理ip报文
void dpi_pkt_ip(dpi_result *res, dpi_pkt *pkt);

//处理tcp报文
void dpi_pkt_tcp(dpi_result *res, dpi_pkt *pkt);

//处理udp报文
void dpi_pkt_udp(dpi_result *res, dpi_pkt *pkt);

//处理icmp报文
//void dpi_pkt_icmp(dpi_result *res, dpi_pkt *pkt);

//处理ssh报文
int dpi_pkt_ssh(dpi_result *res, dpi_pkt *pkt);

//处理tftp报文
int dpi_pkt_tftp(dpi_result *res, dpi_pkt *pkt);

//处理ntp报文
int dpi_pkt_ntp(dpi_result *res, dpi_pkt *pkt);

//处理http报文
int dpi_pkt_http(dpi_result *res, dpi_pkt *pkt);



/* 处理tcp应用层报文的解析函数表 */
//定义一个函数指针，专门用来识别协议报文
typedef int (*dpi_protocol_analyze_func_t)(dpi_result *res, dpi_pkt *pkt);


//tcp应用层报文解析函数表
dpi_protocol_analyze_func_t dpi_tcp_analyze_funcs[PROTOCOL_TCP_MAX];


//udp应用层报文解析函数表
dpi_protocol_analyze_func_t dpi_udp_analyze_funcs[PROTOCOL_UDP_MAX];


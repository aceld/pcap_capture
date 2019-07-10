#pragma once

#include <pcap/pcap.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>


//句柄定义
typedef struct dpi_result 
{
    void *pcap_handle;
    
    unsigned int ether_count; //以太坊报文数量
    
    unsigned int ip_count;    //ip报文数量

    unsigned int tcp_count;   //tcp  报文数量
    unsigned int udp_count;   //udp  报文数量

    unsigned int ssh_count;   //ssh报文数量

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
    union {
        struct {
            uint32_t tcp_len; //tcp报文长度
            char *tcp_packet; //tcp报文地址
        };
        struct {
            uint32_t udp_len; //udp报文长度
            char *udp_packet; //udp报文地址
        };
    };
}dpi_pkt;



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

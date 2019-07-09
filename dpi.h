#pragma once


//句柄定义
typedef struct dpi_result 
{
    void *pcap_handle;
    unsigned int ether_count; //以太坊报文数量
    unsigned int ip_count;    //ip报文数量
    unsigned int tcp_count;   //tcp 报文数量
    unsigned int udp_count;   //udp报文数量
    unsigned int ssh_count;   //ssh报文数量
}dpi_result;



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


void dpi_pcap_callback(u_char *user, const struct pcap_pkthdr *h, const u_char *data);

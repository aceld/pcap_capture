#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "dpi.h"



//1 初始化
//pcapfile: 要处理的pcap文件
//返回值: 设计一个句柄，这个句柄包含了结果
//成功返回非空的指针，失败，返回NULL
dpi_result *dpi_init(const char *pcapfile) 
{
    //1 pcap 打开文件
    char errbuf[PCAP_ERRBUF_SIZE] = {0};//错误信息buffer
    pcap_t *handle = pcap_open_offline(pcapfile, errbuf);
    if (!handle) {
        //出错处理
        DPI_LOG_ERROR("Error in pcap_open_offline :%s\n", errbuf);
        return NULL;
    }

    //创建一个句柄
    dpi_result *res = malloc(sizeof(dpi_result));
    //重置为0
    memset(res, 0, sizeof(*res));
    //将pcap打开文件产生的句柄也存到res句柄中，方便之后调用
    res->pcap_handle = handle;

    return res;
}

//2 业务处理
//启动执行报文的函数
void dpi_loop(dpi_result *res) 
{ 
    //业务处理
    //每次pcap遍历一个报文都会调用我们的回调函数dpi_pcap_callback
    pcap_loop((pcap_t*)res->pcap_handle, 0, dpi_pcap_callback,(u_char*)res);
}

//3 资源释放
void dpi_destory(dpi_result *res)
{
    if(!res) 
        return;

    //释放pcap的句柄
    pcap_close(res->pcap_handle);

    //释放自己的句柄
    free(res);
}


//pcap回调函数
void dpi_pcap_callback(u_char *user, const struct pcap_pkthdr *h, const u_char *data)
{
    dpi_result *res = (dpi_result*)user;

    //以太网报文数量自增
    res->ether_count ++;

    //从package header 中输出报文的长度
    /*
       每个packet header中都有两个字段  caplen len
       caplen -> 是抓取到的报文存到pcap文件中的实际大小
       len    -> 是被抓取的报文的理应该大小

       如果抓包的过程中，2k的报文，抓到1k数据后就中断
       caplen = 1k
       len = 2k 
     */
    //printf("caplen:%d , len %d\n", h->caplen, h->len);

    //保护措施
    //如果两个len不相等，说明报文被中弄断，不应该继续做解析
    if (h->caplen != h->len )  {
        return;
    }


    /*
        解析以太帧   
        ...--------+--------+--------+------+-----------+--------+
           前导码  |目的地址| 源地址 | 类型 |   数据    |  CRC   |
        ...--------+--------+--------+------+-----------+--------+
                        6       6       2      46~1500     4
     */
    //创建一个 pkt 结构体, 标识解析信息
    dpi_pkt pkt;
    memset(&pkt, 0, sizeof(pkt));
    pkt.ether_len = h->caplen;//以太网报文长度
    pkt.ether_packet = (struct ether_header *)data; //以太网报文起始地址

    //以太网的报文类型
    //printf("type : %#x\n", ntohs(pkt.ether_packet->ether_type));

    if (pkt.ether_packet->ether_type == htons(0x0800)) {
        //IP报文
        
        //解析ip报文
        pkt.ip_len = pkt.ether_len-sizeof(*pkt.ether_packet);//计算ip报文长度
        pkt.ip_packet = (struct iphdr*)((char*)pkt.ether_packet + sizeof(*pkt.ether_packet));//计算ip把稳的起始位置

        dpi_pkt_ip(res, &pkt);

    } else if (pkt.ether_packet->ether_type == htons(0x0806)) {
        //ARP 报文
    } else if (pkt.ether_packet->ether_type == htons(0x8035)) {
        //RARP 报文
    }

}

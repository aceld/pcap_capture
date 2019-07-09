#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap/pcap.h>
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
        fprintf(stderr, "Error in pcap_open_offline :%s\n", errbuf);
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

}

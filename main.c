#include <stdio.h>
#include <pcap/pcap.h>
#include "dpi.h"


void usage(const char *argv0) 
{
    fprintf(stderr, "usage : %s <pcap_file>\n", argv0);
}


void displayResult(dpi_result *res)
{
    printf("以太坊报文数量: %u\n", res->ether_count);
    printf("IP报文数量: %u\n", res->ip_count);
    printf("TCP报文数量: %u\n", res->tcp_count);
    printf("UDP报文数量: %u\n", res->udp_count);
    printf("SSH报文数量: %u\n", res->ether_count);
}


int main(int argc, char *argv[])
{
    char errbuf[PCAP_ERRBUF_SIZE] = {0}; //错误信息buffer

    //如果main没有参数，就提示
    if (argc != 2) {
        usage(argv[0]);
        return -1;
    }

    //1 初始化
    dpi_result *res = dpi_init(argv[1]);
    if (!res) {
        fprintf(stderr, "Errr in dpi_init\n");
        return -1;
    }

    //2 业务处理
    dpi_loop(res);


    displayResult(res);


    //3 资料释放
    dpi_destory(res);


	return 0;
}



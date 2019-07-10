#include "dpi.h"

//处理ip报文
void dpi_pkt_ip(dpi_result *res, dpi_pkt *pkt)  
{
    //ip报文计数 ++ 
    res->ip_count++;

    //ip 版本号为4

    if (pkt->ip_packet->version != 4) {
        DPI_LOG_ERROR("IP version not eq 4\n");
        return ;
    }

    //ip首部长度
    int ihl = pkt->ip_packet->ihl << 2; //单位是4字节，所以要乘以4 得到长度字节数，<<2 表示乘以4
    //ip报文总长度
    int ip_totlen = ntohs(pkt->ip_packet->tot_len);

    
    //分片只处理第一个分片 frag_off 网络字节序的后13位 == 0
    if ((pkt->ip_packet->frag_off & htons(0x01fff)) != 0) {
        DPI_LOG_ERROR("IP frag off not eq 0\n"); 
        return ;
    }

    //根据ip报文是什么协议来进行分支
    switch(pkt->ip_packet->protocol) {
        case 6:
            //tcp
            //计算tcp报文数据的长度和起始位置
            pkt->tcp_len = ip_totlen - ihl;
            pkt->tcp_packet = (char*)pkt->ip_packet + ihl;

            //处理tcp报文
            dpi_pkt_tcp(res, pkt);

            break;
        case 17:
            //udp
            //计算udp报文数据的长度和起始位置
            pkt->udp_len = ip_totlen - ihl;
            pkt->udp_packet = (char*)pkt->ip_packet + ihl;

            //处理udp报文
            dpi_pkt_udp(res, pkt);

            break;
        case 1:
            //icmp
            break;
        default:
            break;
    }
}

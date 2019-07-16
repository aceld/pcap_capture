#include "dpi.h"

char *protocol_udp_string[PROTOCOL_UDP_MAX] = {
    PROTOC_UDP_STRING
};

//实现 udp应用层报文解析函数表
dpi_protocol_analyze_func_t  dpi_udp_analyze_funcs[PROTOCOL_UDP_MAX] = 
{
    dpi_pkt_tftp,
    dpi_pkt_ntp
};

//处理udp报文
void dpi_pkt_udp(dpi_result *res, dpi_pkt *pkt) 
{
    //udp报文数量增加
    res->udp_count++;

    uint16_t udp_len = ntohs(pkt->udp_packet->len);
    

    //计算应用数据的起始位置及长度
    pkt->payload_len = udp_len - sizeof(struct udphdr);
    pkt->payload = (unsigned char*)pkt->udp_packet + sizeof(struct udphdr);
    

    //将全部的回调函数放在一个全局表中,抽象出来
    int i;
    for (i = 0; i < PROTOCOL_UDP_MAX; i++) {
        if (dpi_udp_analyze_funcs[i](res, pkt)) {
            //匹配了对应的协议成功
            res->udp_protocol_count[i]++;
            break;
        }
    }
}

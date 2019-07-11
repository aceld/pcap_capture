#include "dpi.h"

//实现 tcp应用层报文解析函数表
dpi_protocol_analyze_func_t  dpi_tcp_analyze_funcs[PROTOCOL_TCP_MAX] = 
{
    dpi_pkt_ssh,
    dpi_pkt_tftp
};

//处理tcp报文
void dpi_pkt_tcp(dpi_result *res, dpi_pkt *pkt) 
{
    res->tcp_count++;

    //计算tcp区域的长度
    int tcphl = pkt->tcp_packet->doff * 4;

    //计算数据区域长度
    pkt->payload_len = pkt->tcp_len - tcphl; //数据区域长度=tcp报文长度-tcp头部长度
    pkt->payload = (uint8_t*)pkt->tcp_packet + tcphl;

    /*
        //逻辑判断法
        if (dpi_pkt_ssh(res, pkt)) {
            //处理ssh报文成功
        } else if (dpi_pkt_tftp(res, pkt)) {
            //处理tftp报文成功
        }
     */


    //表驱动法
    //将全部的回调函数放在一个全局表中,抽象出来
    int i;
    for (i = 0; i < PROTOCOL_TCP_MAX; i++) {
        if (dpi_tcp_analyze_funcs[i](res, pkt)) {
            //匹配了对应的协议成功
            res->tcp_protocol_count[i]++;
            break;
        }
    }

}





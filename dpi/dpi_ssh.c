#include "dpi.h"
#include <string.h>

/*
   处理ssh报文

   @return 
      0  处理失败
      1  处理成功
 */
int dpi_pkt_ssh(dpi_result *res, dpi_pkt *pkt)
{
    //如果报文前面的4个字节是 "SSH-" 那么就是ssh报文

    //保护措施，数据区域长度>4字节
    if (pkt->payload_len <= 4) {
        DPI_LOG_ERROR("ssh payload_len <= 4");
        return 0;
    }


    //识别ssh报文
    if (memcmp("SSH-", pkt->payload, 4) == 0) {
        return 1;
    }


    return 0;
}

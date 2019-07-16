#include "dpi.h"

/* 

       0                   1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |LI |   Status  |      Type     |           Precision           |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                       Estimated Error                         |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                     Estimated Drift Rate                      |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                  Reference Clock Identifier                   |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                                                               |
       |                 Reference Timestamp (64 bits)                 |
       |                                                               |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                                                               |
       |                 Originate Timestamp (64 bits)                 |
       |                                                               |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                                                               |
       |                  Receive Timestamp (64 bits)                  |
       |                                                               |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                                                               |
       |                  Transmit Timestamp (64 bits)                 |
       |                                                               |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 */

typedef struct _dpi_ntp_header
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint16_t mode:3;
    uint16_t vn:3;
    uint16_t li:2;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint16_t li:2;
    uint16_t vn:3;
    uint16_t mode:3;
#endif
    uint8_t  stratum;
    uint8_t  poll;
    uint8_t  precision;
}dpi_ntp_header;

int dpi_pkt_ntp(dpi_result *res, dpi_pkt *pkt)
{
    //条件1: 报文长度 48字节 或者有校验和 60字节
    if (pkt->payload_len != 48 && pkt->payload_len != 60) {
        return 0;
    }

    //条件2: vn:4
    dpi_ntp_header *ntp = (dpi_ntp_header*)pkt->payload;

    if (ntp->vn != 4) {
        return 0;
    }

    //条件3: Stratum: 0-16
    if (ntp->stratum > 16) {
        return 0;
    }

    //没有更多条件 如果以上都符合，认为是ntp报文

    return 1;
}

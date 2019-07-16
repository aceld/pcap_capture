#include "dpi.h"
#include <string.h>

#define DPI_TFTP_NETASCII "netascii"
#define DPI_TFTP_OCTET  "octet"
#define DPI_TFTP_MAIL "mail"

typedef struct _dpi_tftp_header
{
    uint16_t opcode;
}dpi_tftp_header;

typedef struct _dpi_tftp_error_header
{
    uint16_t opcode;
    uint16_t errorcode;
}dpi_tftp_error_header;



/*
   处理TFTP报文

   @return 
      0  处理失败
      1  处理成功
 */
int dpi_pkt_tftp(dpi_result *res, dpi_pkt *pkt)
{

    dpi_tftp_header *tftp = (dpi_tftp_header*)pkt->payload;


    /*
       WRQ|RRQ报文

       2 bytes    string   1 byte string 1 byte
       ------------------------------------------------
       | Opcode | Filename | 0 | Mode | 0 |
       ------------------------------------------------

        Mode: "netascii"，"octet", "mail"
        opcode:1 | 2

     */
    if (pkt->payload_len >= 9) {
        //整个报文至少9个字节
        //opcode : 1 | 2
        if (tftp->opcode == htons(1) || tftp->opcode == htons(2)) {
            //最后几个字节肯定是 "netascii"，"octet", "mail" 中间都夹着一个\0

            if (pkt->payload_len >= strlen(DPI_TFTP_NETASCII) + 4) {
                //4 表示 opcode:2  \0:1 \0:1 
                char *begin = (char *)pkt->payload + pkt->payload_len - 1 - strlen(DPI_TFTP_NETASCII);
                if (memcmp(begin, DPI_TFTP_NETASCII, strlen(DPI_TFTP_NETASCII)) == 0) {
                    //判断成功
                    return 1;
                }
            }


            if (pkt->payload_len >= strlen(DPI_TFTP_OCTET) + 4) {
                //4 表示 opcode:2  \0:1 \0:1 
                char *begin = (char *)pkt->payload + pkt->payload_len - 1 - strlen(DPI_TFTP_OCTET);
                if (memcmp(begin, DPI_TFTP_OCTET, strlen(DPI_TFTP_OCTET)) == 0) {
                    //判断成功
                    return 1;
                }
            }


            if (pkt->payload_len >= strlen(DPI_TFTP_MAIL) + 4) {
                //4 表示 opcode:2  \0:1 \0:1 
                char *begin = (char *)pkt->payload + pkt->payload_len - 1 - strlen(DPI_TFTP_MAIL);
                if (memcmp(begin, DPI_TFTP_MAIL, strlen(DPI_TFTP_MAIL)) == 0) {
                    //判断成功
                    return 1;
                }
            }
        }

    }

    
    /*
        DATA报文

       2 bytes   2 bytes   n bytes
       ----------------------------------
       | Opcode | Block # | Data |
       ----------------------------------

        opcode : 3
        len : 4 - 516字节
     */
    if (pkt->payload_len >= 4 && pkt->payload_len <= 516 && tftp->opcode == htons(3)) {
        return 1;
    }


    /*
       ACK 报文

       2 bytes 2 bytes
       ---------------------
       | Opcode | Block # |
       ---------------------
     
        opcode : 4
        len : 4
     */
    if (pkt->payload_len == 4 && tftp->opcode == htons(4)) {
        return 1;
    }

    /*
       ERROR 报文

        2 bytes       2 bytes string 1 byte
        -----------------------------------------
        | Opcode | ErrorCode | ErrMsg | 0 |
        -----------------------------------------

        opcode : 5
        errorcode : 0-7
        len : >=5 字节
     */
    dpi_tftp_error_header *tftp_error = (dpi_tftp_error_header*)tftp;

    if (pkt->payload_len >= 5 && tftp_error->opcode == htons(5)) {
        if (tftp_error->errorcode >= htons(0) && tftp_error->errorcode <= htons(7)) {
            //最后一个字节是\0
            if (*(pkt->payload + pkt->payload_len-1) == '\0') {
                return 1;
            }
        }
    }


    return 0;
}

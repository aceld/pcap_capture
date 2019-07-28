#include "dpi.h"
#include <string.h>


#define HTTP_VERSION_1_1 "HTTP/1.1"
#define HTTP_VERSION_1_0 "HTTP/1.0"
#define CRLF "\r\n"

enum _http_request_method {
    GET = 0,
    HEAD,
    POST,
    OPTIONS,
    PUT,
    DELETE,
    TRACE,
    CONNECT,
    HTTP_METHOD_MAX
}http_request_method;


char *http_request_method_strings[HTTP_METHOD_MAX] = {
    "GET",
    "HEAD",
    "POST",
    "OPTIONS",
    "PUT",
    "DELETE",
    "TRACE",
    "CONNECT"
};

#define HTTP_RESPONSE_STATUS_NUM (7)

char *http_response_status_strings[HTTP_RESPONSE_STATUS_NUM] = {
    "200 OK",
    "400 Bad Request",
    "401 Unauthorized",
    "403 Forbidden",
    "404 Not Found",
    "500 Internal Server Error",
    "503 Server Unavailable"
};



/*
   处理http报文

   @return 
      0  处理失败
      1  处理成功
 */
int dpi_pkt_http(dpi_result *res, dpi_pkt *pkt) 
{
    //HTTP报文请求头
    //GET url HTTP/1.1\r\n
    if (pkt->payload_len < (strlen(http_request_method_strings[OPTIONS])+strlen(HTTP_VERSION_1_0) + strlen(" ") + strlen(CRLF))) {
        //请求头太小
        return 0;
    }

    
    /*
       POST /index.html HTTP/1.1\r\n
       HOST: www.baidu.com\r\n
       User-Agent: Mozilla/5.0(Windows NT 6.1;rv:15.0) Firefox/15.0\r\n
       Username=admin&password=admin
     */
    // 匹配请求方法
#if 1
    int i;
    for (i = 0; i < HTTP_METHOD_MAX; i++) {
        if (memcmp(http_request_method_strings[i], pkt->payload, strlen(http_request_method_strings[i])) == 0) {
            //匹配到请求方法
            //找到版本号的首地址进行匹配
            char* p;
            for(p = pkt->payload+(strlen(http_request_method_strings[i])+1); *p != ' '; p++);
            p++;
            if ( (memcmp(p, HTTP_VERSION_1_0, strlen(HTTP_VERSION_1_0)) == 0) ||
                 (memcmp(p, HTTP_VERSION_1_1, strlen(HTTP_VERSION_1_1)) == 0) ) {
                //找到版本号
                add_connection(res, pkt, HTTP);
                return 1;
            }
        }
    }
#endif

    //匹配应答方法
    /*
       HTTP/1.1 200 OK
       Content-Encoding: gzip
       Content-Type: text/html;charset=utf-8

       <!DOCTYPE html>
       <html lang="en">
       <head>
       <meta charset="UTF-8" />
       <title>Document</title>
       </head>
       <body>
       <p>this is http response</p>
       </body>
       </html>
     */
#if 1
    //匹配到版本号
    if ( (memcmp(HTTP_VERSION_1_1, pkt->payload, strlen(HTTP_VERSION_1_1)) == 0) ||
         (memcmp(HTTP_VERSION_1_0, pkt->payload, strlen(HTTP_VERSION_1_0)) == 0)) {

        //匹配应答码 及 状态描述
        char* p = pkt->payload+(strlen(HTTP_VERSION_1_0)+1); 

        int i;
        for (i = 0; i < HTTP_RESPONSE_STATUS_NUM; i ++) {
            if (memcmp(http_response_status_strings[i], p, strlen(http_response_status_strings[i])) == 0) {
                add_connection(res, pkt, HTTP);
                return 1;
            }
        }
    }
#endif

#if 1
    //从链接集合中匹配
    if (cmp_connection(res, pkt, HTTP) == 0) {
        return 1;
    }
#endif


    return 0;
}

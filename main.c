#include <stdio.h>
#include "dpi.h"



void usage(const char *argv0) 
{
    DPI_LOG_INFO("usage : %s <pcap_file>\n", argv0);
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
#if 0
    dpi_result *res = dpi_init_dev(argc, argv);
#endif
#if 1
    dpi_result *res = dpi_init(argv[1]);
    if (!res) {
        DPI_LOG_ERROR("Errr in dpi_init\n");
        return -1;
    }
#endif

    //2 业务处理
    dpi_loop(res);


    displayResult(res);


    //3 资料释放
    dpi_destory(res);


	return 0;
}



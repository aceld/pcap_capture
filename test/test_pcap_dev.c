#include "pcap.h"   
#include "stdlib.h"
#include <time.h>
#include <arpa/inet.h>
#define SNAP_LEN 65536

//prototype of the packet handler 
void dispatcher_handler(u_char *temp1,
        const struct pcap_pkthdr *header, const u_char *pkt_data);

int main(int argc, char **argv)
{
    char *dev = NULL;            /* capture device name */
    char errbuf[PCAP_ERRBUF_SIZE];        /* error buffer */
    pcap_t *handle;                /* packet capture handle */
    pcap_if_t *alldev, *p;
    char filter_exp[] = "tcp";        /* filter expression [3] */
    struct bpf_program fp;            /* compiled filter program (expression) */
    bpf_u_int32 mask;            /* subnet mask */
    bpf_u_int32 net;            /* ip */
    int num_packets = 10000;            /* number of packets to capture */   
    /* check for capture device name on command-line */
    if (argc == 2) {
        dev = argv[1];
    }
    else if (argc > 2) {
        fprintf(stderr, "error: unrecognized command-line options\n\n");
        exit(EXIT_FAILURE);
    }
    else {
        /* find a capture device if not specified on command-line */ 
        int i=0,num;  
        if(pcap_findalldevs(&alldev,errbuf)==-1)  
        {  
            printf("find all devices is error\n");  
            return 0;  
        }  
        for(p=alldev;p;p=p->next)  
        {  
            printf("%d:%s\n",++i,p->name);  
            if(p->description)  
            {  
                printf("%s\n",p->description);  
            }  
        }  
        printf("please input which interface you want to use\n");  
        scanf("%d",&num);  
        if(num<1||num>i)  
        {  
            printf("interface is unavillible\n");  
            return 0;  
        }  
        for(p=alldev,i=1;i<=num;p=p->next,i++)  
            dev=p->name;  
        if (dev == NULL) {
            fprintf(stderr, "Couldn't find default device: %s\n",
                    errbuf);
            exit(EXIT_FAILURE);
        }
    }
    /* print capture info */
    printf("Device: %s\n", dev);
    printf("Number of packets: %d\n", num_packets);
    printf("Filter expression: %s\n", filter_exp);
    /* open capture device */
    handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        exit(EXIT_FAILURE);
    }
    /* make sure we're capturing on an Ethernet device [2] */
    if (pcap_datalink(handle) != DLT_EN10MB) {
        fprintf(stderr, "%s is not an Ethernet\n", dev);
        exit(EXIT_FAILURE);
    }
    /* compile the filter expression */
    if (pcap_compile(handle, &fp, filter_exp, 0, 24) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n",
                filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }
    /* apply the compiled filter */
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n",
                filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }
    /* now we can set our callback function */
    pcap_loop(handle, num_packets, dispatcher_handler, NULL);
    /* cleanup */
    pcap_freecode(&fp);
    pcap_close(handle);
    printf("\nCapture complete.\n");
    return 0;
}

void dispatcher_handler(u_char *temp1,
        const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    printf("I get one packet!\n");
}

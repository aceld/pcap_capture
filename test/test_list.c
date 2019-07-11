#include <stdio.h>
#include <stdlib.h>
#include "dpi_list.h"

int main(int argc, char *argv[])
{
    //1 创建链表
    dpi_list *list = dpi_list_create();
    if (!list) {
        fprintf(stderr, "Error in dpi_list_crete\n");
        return -1;
    }

    //2 添加数据
    int *num10 = (int*)malloc(sizeof(int));
    int *num20 = (int*)malloc(sizeof(int));
    int *num30 = (int*)malloc(sizeof(int));
    int *num40 = (int*)malloc(sizeof(int));

    *num10 = 10;
    *num20 = 20;
    *num30 = 30;
    *num40 = 40;

    dpi_list_append(list, num10);
    dpi_list_append(list, num20);
    dpi_list_append(list, num30);
    dpi_list_append(list, num40);

    //遍历整个链表
    dpi_list_node *begin = list->head.next;
    while(begin != &list->head) {
        int *p = begin->data;
        printf("%d\n", *p);

        //遍历下一个
        begin = begin->next;

    }

    //3 释放链表
    dpi_list_destory(list);

	return 0;
}

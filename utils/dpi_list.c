#include "dpi_list.h"
#include <stdlib.h>
#include <string.h>



//创建一个链表
dpi_list *dpi_list_create()
{
    dpi_list *list = malloc(sizeof(dpi_list))  ;
    if (list != NULL) {
        memset(list, 0, sizeof(*list));

        //head 哨兵节点都指向自己
        list->head.next = &list->head;
        list->head.prev = &list->head;
    }

    return list;
}

//将数据追加到链表中
int dpi_list_append(dpi_list *list, void *data)
{
    //当前新增加的节点
    dpi_list_node *node = malloc(sizeof(dpi_list_node));

    if (!node) {
        return -1;
    }

    //链表长度++
    list->size++;
    node->data = data;

    //链表最后一个节点
    dpi_list_node *last_node = list->head.prev;

    //链表的最后一个节点的next指向当前新增的节点
    last_node->next = node;

    //当前新增的节点的prev也要指向链表的最后一个节点
    node->prev = last_node; 

    //哨兵节点的prev要执行当前新增的节点(因为当前节点是最后一个节点)
    list->head.prev = node;

    //新增的节点的netx要指向head 哨兵节点
    node->next = &list->head;

    return 0;
}

//释放链表
void dpi_list_destory(dpi_list *list)
{
    //遍历整个链表
    dpi_list_node *begin = list->head.next;

    while(begin != &list->head) {
        //释放每个节点的数据区域
        if (begin->data != NULL) {
           free(begin->data);
        }

        dpi_list_node *tmp = begin;
        begin = begin->next;

        //释放每个节点的内存
        free(tmp);
    }

    //释放链表的结构体
    free(list);
}


#pragma once
#include <stdint.h>

//节点的定义
typedef struct _dpi_list_node 
{
    void *data; 
    struct _dpi_list_node *prev;
    struct _dpi_list_node *next;
}dpi_list_node;

//链表的定义:
typedef struct _dpi_list
{
    uint32_t size;
    dpi_list_node head; //哨兵节点，链表的起始位置，也是头节点
}dpi_list;

//创建一个链表
dpi_list *dpi_list_create();  

//将数据追加到链表中
int dpi_list_append(dpi_list *list, void *data);

//释放链表
void dpi_list_destory(dpi_list *list);

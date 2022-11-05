/*
 * @Description:
 * @version: Failure
 * @Author: Yihui
 * @Date: 2019-09-25 16:02:00
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-02 21:24:53
 */
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;
//定义一个含有头结点的单链表
typedef struct LNode
{
    struct LNode *next;
    int data;
} LNode;
//尾插法建立链表
void createList(LNode *&A, int n)
{
    LNode *s, *p;
    p = A;
    p->next = NULL;
    int data;
    for (int i = 0; i < n; i++)
    {
        s = new LNode;
        scanf("%d", &data);
        s->data = data;
        p->next = s; // 你会发现这里根本没有s->next的事,这和头插大不一样
        p = p->next;
    }
    p->next = NULL;
}
//头插法建立链表
void createList_head(LNode *&A)
{
    LNode *p = A;
    p->next = NULL;
    int data;
    cin >> data;
    LNode *s;
    while (data != -1)
    {
        s = new LNode;
        s->data = data;
        s->next = p->next;
        p->next = s;
        cin >> data;
    }
}
//合并非递减链表,尾插法
void mergeList(LNode *A, LNode *B, LNode *&C)
{
    LNode *r = A->next;
    LNode *s = B->next;
    // LNode *p=C;
    // p=A;
    // p->next =NULL;当C把A拿过来的时候,C作为结点存在,因而被注释掉的这几句不等于下面几句,事实上只能改变p;也正是因为C自身也要改变,所以C使用引用型
    LNode *p;
    C = A;
    C->next = NULL;
    p = C;
    free(B);
    while (r != NULL && s != NULL)
    {
        if (r->data <= s->data)
        {
            p->next = r;
            r = r->next;
            p = p->next;
        }
        else
        {
            p->next = s;
            s = s->next;
            p = p->next;
        }
        //作为链表,连接的时候是可以当成一粒粒珠子的
    }
    p->next = NULL;
    if (r != NULL)
        p->next = r;
    if (s != NULL)
        p->next = s;
}
// 找到A,B链表中相同的元素并删除A中相同的元素,以助实现最后合并结果的不重复
void findandDelete(LNode *A, LNode *B)
{
    LNode *p = A->next, *q = B->next;
    LNode *pre = A;
    LNode *r;
    while (p != NULL && q != NULL)
    {
        if (p->data < q->data)
        {
            pre = p;
            p = p->next;
        }
        else if (p->data > q->data)
        {
            q = q->next;
        }
        else
        {
            pre->next = p->next;
            r = p;
            p = p->next;
            free(r);
        }
    }
}
//主函数调用
int main()
{
    LNode *A = new LNode;
    LNode *B = new LNode;
    printf("Enter the length of lists:");
    int n1, n2;
    scanf("%d %d", &n1, &n2);
    createList(A, n1);
    createList(B, n2);
    findandDelete(A, B);
    LNode *C;
    mergeList(A, B, C);
    LNode *p = C->next;
    while (p != NULL)
    {
        // printf("hhhh");
        printf("%-3d", p->data);
        p = p->next;
    }
    return 0;
}
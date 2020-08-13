/*
 * @Descripttion: 
 * @version: 
 * @Author: Yihui
 * @Date: 2019-09-24 08:03:25
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-04 11:17:29
 */
#include<stdio.h>
#include<cstring>
#include<stdlib.h>
#define maxsize 100
//定义顺序表
typedef struct {
    int data[maxsize];
    int length;
}Sqlist;
//查找要插入的位置
int findElem(Sqlist L, int x){
    int i;
    for(i=0;i<L.length;i++){
        if(x<L.data[i]){
            return i;
        }
    }
    return i;
}
//顺序表的递增插入
void insertElem(Sqlist &L, int x){//因为顺序表L本身要发生改变,所以用引用型
    int p,i;
    p=findElem(L,x);
    for(i=L.length;i>p;i--){
        L.data[i+1]=L.data[i];
    }
    L.data[p]=x;
    ++(L.length);//不要忘了表长加一
}
//删除顺序表中元素并保留此元素值
int deleteElem(Sqlist &L,int p, int &e){//传入引用型e来保留元素值
    int i;
    if(p<0||p>L.length-1||L.length==maxsize){//当表长达到最大允许值时也不行
        return 0;
    }
    e=L.data[p];
    for(i=p;i<L.length;i++){
        L.data[i]=L.data[i+1];//没有真正的删除操作,后一个覆盖前一个就可
    }
    L.length--;
    return 1;
}
//定义含有头结点的单链表
typedef struct LNode{
    int data;
    struct LNode *next;
} LNode;
//尾插法建立链表C
void list_tailInsert(LNode *&C, int a[],int n){
   LNode *s,*r;
   int i;
   C = (LNode *)malloc(sizeof(LNode));
   C->next=NULL;
   r=C;//为什么一定要要引入第三方呢?
   for(i=0;i<n;i++){
       s=(LNode *)malloc(sizeof(LNode));
       s->data=a[i];
       r->next=s;
       r=r->next;
   }
   r->next=NULL;
}
void createlistR(LNode *&C, int a[],int n){
   LNode *s,*r;
   int i;
   C = (LNode *)malloc(sizeof(LNode));
   C->next=NULL;
   for(i=0;i<n;i++){
       s=(LNode *)malloc(sizeof(LNode));
       s->data=a[i];
       C->next=s;
       C=C->next;
   }
   C->next=NULL;
}//大概是因为C作为指针的时候即使本身能移动,也会与作为结点名的时候的C混淆吧
//头插法建立链表C,其实就最后两步不一样
void list_headInsert(LNode *&C,int a[],int n){
    LNode *s;
    int i;
    C=(LNode*)malloc(sizeof(LNode));
    C->next=NULL;
    for(i=0;i<n;i++){
        s=(LNode*) malloc(sizeof(LNode));
        s->data=a[i];
        s->next=C->next;
        C->next=s;
    }
}
//顺序栈定义
typedef struct
{
    int data[maxsize];
    int top;//栈顶指针,注意是整型变量
}SqStack;
//链栈结点定义
typedef struct LNode{
    int data;
    struct LNode *next;
}LNode;
//顺序队列定义
typedef struct{
    int data[maxsize];
    int front;//队首指针
    int rear;//队尾指针
}SqQueue;
//队结点类型定义
typedef struct QNode{
    int data;
    struct QNode *next;
}QNode;
//链队类型定义
typedef struct{
    QNode *front;
    QNode *rear;
}LiQueue; 
//串的定长顺序存储
typedef struct{
    char str[maxsize+1];//多出一个'\0'作为了结束标志
    int length;
}Str;
//串的变长分配存储
typedef struct{
    char *ch;//指向动态分配存储区首地址的字符指针
    int length;
}Str;  
//串长函数
int strlen(char *s);
//串复制
char *strcpy(char *s1,char *s2);
//串拼接
char *strcat(char *s1,char *s2);
//串比较
int strcmp(char *s1,char *s2);
//串定位函数
char *strchr(char *s,char c);
//串右定位函数/倒着寻找定位
char *srrrchr(char *s,char c);
                                                                                                                                                                                                                                                                                                                                            
/*
 * @Descripttion: 
 * @version: 
 * @Author: Yihui
 * @Date: 2019-08-10 20:37:51
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-04 19:56:53
 */
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<stdlib.h>
//基本思路：先将一行字符串压栈，入队列。弹出栈与出队列进行比较。全相同为回文，否则不是。
#define maxSize 100
#define ERROR 0
#define Ok 1
//顺序栈定义
typedef struct{
    char data[maxSize];//存放回文序列
    int top;
}SqStack;
//顺序队列定义
typedef struct{
    char data[maxSize];
    int front,rear;
}SqQueue;
//初始化栈
void iniStack(SqStack &st){
    st.top=-1;
}
//压栈
int push(SqStack &st,char x){
    if(st.top==maxSize-1)   return ERROR;
    st.data[++st.top]=x;
    return Ok;
}
//出栈
int pop(SqStack &st,char &x){
    if(st.top==-1)  return ERROR;
    x=st.data[st.top--];
    return x;    
}
//初始化队列
void iniQueue(SqQueue &qu){
    qu.front=qu.rear=0;//队首与队尾指针重合并且指向0
}
//入队
int enQueue(SqQueue &qu,char x){
    if((qu.rear+1)%maxSize==qu.front)   return ERROR;
    qu.rear=(qu.rear+1)%maxSize;
    qu.data[qu.rear]=x;
    return Ok;
}
//出队
int exQueue(SqQueue &qu, char &x){
    if(qu.rear==qu.front)   return ERROR;
    qu.front=(qu.front+1)%maxSize;
    x=qu.data[qu.front];
    return x;
}
//判断是否是回文
int main(){
    // SqStack *st=new SqStack;
    // SqQueue *qu=new SqQueue;
    SqStack st; iniStack(st);//不是结点指针,直接是
    SqQueue qu; iniQueue(qu);
    printf("Please enter the palindrome:\n");
    char ch;scanf("%c",&ch);
    while(ch!='\n'){
        push(st,ch);
        enQueue(qu,ch);
        scanf("%c",&ch);
    }
    //分别出栈判断是否相同
    while(st.top!=-1 && qu.front!=qu.rear){
        if(pop(st,ch)!=exQueue(qu,ch)){
            break;
        }
    }
    if(st.top==-1 && qu.front==qu.rear){
        printf("It is a palindrome.\n");
    }else{
        printf("It is not a palindrome.\n");
    }
    return 0;
}

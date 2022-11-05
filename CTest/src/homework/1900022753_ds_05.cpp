/*
 * @Descripttion: 
 * @version: 
 * @Author: Yihui
 * @Date: 2019-10-03 15:24:56
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-03 16:50:11
 */
//由题意,递增链表不为空,不放心还是考虑一下
#include<cstring>
#include<cstdio>
#include<cstdlib>
typedef struct LNode{
    int data;
    struct LNode *next;
}LNode;
void createList(LNode *&A,int n){
    LNode *s;
    A->next=NULL;
    LNode *p=A;
    int data;
    for(int i=0;i<n;i++){
        s= new LNode;
        scanf("%d",&data);
        s->data=data;
        p->next=s;
        p=p->next;
    }
    p->next=NULL;
}
void findandDelete(LNode *A,int i,int k){
    LNode *pre=A,*p=A->next,*r;
    while(p!=NULL){
        if(p->data>i && p->data <k){
            pre->next=p->next;
            r=p;
            p=p->next;
            free(r);
        }else{
            pre=p;
            p=p->next;
        }
    }
}
int main(){
    LNode *A =new LNode;int n;
    printf("Enter the length of list and initialize the list:");scanf("%d",&n);createList(A,n);
    printf("Please enter the min(i) and max(k) separately:");
    int i,k;scanf("%d %d",&i,&k);
    LNode *p=A->next;
    if(p==NULL){
        printf("链表为空\n");
    }else{
        findandDelete(A,i,k);
        p=A->next;
        while(p!=NULL){
            printf("%d ",p->data);
            p=p->next;
        }
    }

    return 0;
}
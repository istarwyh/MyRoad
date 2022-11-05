/*
 * @Descripttion: 
 * @version: 
 * @Author: Yihui
 * @Date: 2019-10-03 09:37:40
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-03 10:59:41
 */

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
LNode *maxnode(LNode *L){
    LNode *p=L->next,*maxp=p;
    while(p!=NULL){
        if(p->data>maxp->data){
            maxp=p;
        }
        p=p->next;
    }
    return maxp;
}

int main(){
    LNode *A = new LNode;
    int n;printf("Enter the length of list:");scanf("%d",&n);
    createList(A,n);
    LNode *p=A->next;
    if(p==NULL){
        printf("链表为空,没有最大结点\n");
    }else{
        LNode *maxp=maxnode(A); 
        printf("最大的结点值为: %d",maxp->data);
    }
  
    return 0;
}
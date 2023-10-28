/*
 * @Descripttion: 
 * @version: 
 * @Author: Yihui
 * @Date: 2019-10-02 21:32:49
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-03 09:33:52
 */
#include<cstring>
#include<cstdio>
#include<cstdlib>
typedef struct  LNode{
    struct LNode *next;
    int data;
}LNode;
void createList(LNode *&A,int n){
    LNode *s,*p;
    A = new LNode;
    A->next=NULL;
    p=A;
    int data;
    for(int i=0;i<n;i++){
        s=new LNode;
        scanf("%d",&data);
        s->data=data;
        p->next=s;
        p=p->next;
    } 
    p->next=NULL;
}
void mergeListHead(LNode *A,LNode*B,LNode *&C){
    LNode *r=A->next;
    LNode *s=B->next;
    C=A;C->next=NULL;
    LNode *p=C;
    while(r!=NULL && s!=NULL){
        if(r->data<=s->data){
            p=r;r=r->next;
            p->next=C->next;
            C->next=p;
        }else{
            p=s;s=s->next;
            p->next=C->next;
            C->next=p;
        }
    }
    while(r!=NULL){
        p=r;r=r->next;
        p->next=C->next;
        C->next=p;
    }
    while(s!=NULL){
        p=s;s=s->next;
        p->next=C->next;
        C->next=p;
    }
}
void findandDelete(LNode *A,LNode *B){
    LNode *p=A->next;
    LNode *q=B->next;
    LNode *pre=p;
    LNode *r;
    while(p!=NULL && q!=NULL){
        if(p->data<q->data){
            pre=p;
            p=p->next;
        }else if(p->data>q->data){
            q=q->next;
        }else{
            pre->next=p->next;
            r=p;
            p=p->next;
            free(r);
        }
    }
}
int main(){
    LNode *A= new LNode;
    LNode *B=new LNode;
    printf("Enter the length of lists:");
    int n1,n2;
    scanf("%d %d",&n1,&n2);
    createList(A,n1);createList(B,n2);
    findandDelete(A,B);
    LNode *C;mergeListHead(A,B,C);
    LNode *p=C->next;
    while(p!=NULL){
        printf("%-3d",p->data);
        p=p->next;
    }
    return 0;
}
/*
 * @Descripttion: 
 * @version: 
 * @Author: Yihui
 * @Date: 2019-10-03 11:02:47
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-03 15:21:13
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
void reverseList(LNode *&A){
    LNode *pre =NULL,*p=A->next,*nextp=p->next;
    while(p!=NULL){//ѭ�������ܹؼ�!�������۰�
        p->next=pre; //һ�߸ı����ӷ���ָ�����
        //һ�߲��������ƶ�
        pre=p;
        p=nextp;//���һ��p�ֿ���nextp���°�����ΪNULL�����Խ���ѭ��
        if(nextp)   nextp=nextp->next;
    }
    A->next=pre;
}
int main(){
    LNode *A =new LNode;int n;scanf("%d",&n);createList(A,n);
    LNode *p=A->next;
    if(p==NULL){
        printf("����Ϊ��,����ת\n");
   }else{
        reverseList(A);
        p=A->next;
        while(p!=NULL){
            printf("%d ",p->data);
            p=p->next;
        }
   }
    return 0;
}
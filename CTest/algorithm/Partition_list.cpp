#include  <stdio.h>
typedef int T;
struct LNode{
    T data;
    LNode* next;
};
LNode* newNode(){
    LNode*p = new LNode();
    p->next = 0;
    return p;
}
LNode* push_back(LNode* head,T d){
    LNode* p = head;
    while(p->next){
        p = p->next;
    }
    p->next = newNode();if(! p->next)   return 0;
    p->next->data = d;
    return p->next;
}
void print(LNode* head){
    LNode* p = head->next;
    while(p){
        printf("%d ",p->data);
        p=p->next;
    }
    printf("\n");
}
LNode* partition_list(LNode* head,T x){
    LNode*p = head->next;
    head->next = 0;
    LNode* q1 = head;
    LNode* h2 = new LNode();
    h2->next =0;
    LNode* q2 = h2;

    while(p){
        LNode* q = p->next;
        if(p->data < x){
            q1->next = p; p ->next =0;q1 =p;
        }else{
            q2->next = p;p->next = 0;q2 =p;
        }
        p= q; 
    }
    q1->next = h2->next;
    delete h2;
    return head;
}
//新颖的输入方法,不用从终端一次次读入了,自己赋给数据
void test_partition(){
    LNode* head = newNode();
    push_back(head,1);
    push_back(head,2);
    push_back(head,3);
    push_back(head,2);
    push_back(head,5);
    push_back(head,2);
    print(head);
    head = partition_list(head,3);
    print(head);
}
int main(){
    test_partition();
    return 0;
}
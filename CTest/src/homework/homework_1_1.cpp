/*
 * @Descripttion: 
 * @version: 
 * @Author: Yihui
 * @Date: 2019-09-23 17:23:29
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-02 18:13:02
 */
#include<stdio.h>
#include<cstring>
#include<stdlib.h>
#include<cstdio>
int n1, n2, data;
const int maxn = 1000000009;
bool visited[maxn] = {false};

 typedef struct Lnode{
    int data;
    struct  Lnode* next;//LNode结构体指针型的指针next,存在指针域中
}Lnode;
void createList(Lnode* &L){
    Lnode *p = L;
    for(int i = 0; i < n1; i++){
        Lnode *s = new Lnode;
        scanf("%d", &data);
        if(visited[data]) continue;
        visited[data] = true;
        s->data = data;
        s->next = NULL;
        p->next = s;
        p = p->next;
    }
}

Lnode* mergesort(Lnode* &A, Lnode* &B){
    Lnode *C = A;
    Lnode *p = A->next, *q = B->next, *r = C;
    C->next = NULL;
    while(p != NULL && q != NULL){
        if(p->data < q->data){
            r->next = p;
            p = p->next;
            r = r->next; 
        }
        else{
            r->next = q;
            q = q->next;
            r = r->next;
        }
    }
    while(p != NULL){
        r->next = p;
        p = p->next;
        r = r->next;
    }
    while(q != NULL){
        r->next = q;
        q = q->next;
        r = r->next;
    }
    r->next = NULL;
    return C;
}


int main(){
    scanf("%d %d", &n1, &n2);
    Lnode *A = new Lnode;
    Lnode *B = new Lnode;
    createList(A);
    createList(B);
    Lnode *C = mergesort(A, B);
    Lnode *p = C->next;
    while(p != NULL){
        printf("%d ", p->data);
        p = p->next;
    }
    return 0;
}

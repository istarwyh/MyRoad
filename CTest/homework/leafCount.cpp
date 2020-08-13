/*
 * @Descripttion: 
 * @version: 1.0
 * @Author: Yihui
 * @Date: 2019-10-20 16:04:12
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-20 19:25:35
 */
#include<cstdio>
#include<iostream>
using namespace std;
typedef int ELEMTYPE;
//Ϊ�˷����ö��������ʾ��,��Ƴ�һ�����������������ָ����������
typedef struct Btnode{
    ELEMTYPE data;
    struct Btnode *lchild;
    struct Btnode *rchild;
}BiTreeNode;
//ǰ�򴴽�������

//ע�ⴴ����������Ϊ��,���Ľ���������涨���,Ϊʲô��?�����Ϊ�˵ݹ����Ҫ
//���ҷ��ص���BiTreeNode*����,��Ϊ�ݹ��Ҫ��������������Һ���,�����ں������ڸı��˾Ϳ���

BiTreeNode *createBiTree(){
    BiTreeNode *T;
    ELEMTYPE data;
    scanf("%d",&data);
    if(data==-1){
        T = NULL;
    }else{
        T = new BiTreeNode;
        T->data = data; 
        cout<< "������"<<data<<"������: ";
        T->lchild=createBiTree();
        cout<< "������"<<data<<"���ҽ��: ";
        T->rchild=createBiTree();
    }
    return T;
}


//�ݹ齨��������,�������ʹ����󷵻ص������,����һ�����������������������������Ľ���

//�������,��Ϊ0��ΪҶ�ӽ��
void leafCount(BiTreeNode *T,int &count) {
    //�����ǿ��������,T!=NULL
    if(T!=NULL){
        if(T->lchild==NULL && T->rchild==NULL){
            count++;
        }else{
            //���������Һ��Ӷ������ڵ�ʱ��,��ΪNULL��ʱ��,�����Լ������µݹ��������Һ�����!
            leafCount(T->lchild,count);
            leafCount(T->rchild,count);
        }
    }
    
}
int main(){
    cout<< "�����������ֵ:  ";
    BiTreeNode *root = createBiTree();
    int count = 0;
    leafCount(root,count);
    printf("Ҷ�ӽ����Ϊ:%d\n",count);
    return 0;
}
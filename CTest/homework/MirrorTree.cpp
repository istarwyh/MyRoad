//���һ������Ӱ���㷨,��һ�ö�������ÿ�����������ӽ�㽻��λ��
#include <cstdio>
#include <iostream>
using namespace std;
typedef char ELEMENT;
typedef struct BiTNode{
    ELEMENT data;
    struct BiTNode *rchild,*lchild;
}BiTree;
//ǰ���������
BiTree *CreateBiTree(){
    ELEMENT data;
    cin>>data;
    BiTree *root;
    if(root == NULL){
        cout<<"�ڴ����ʧ��";
        return NULL;
    }
    if(data=='@'){
        return NULL;
    }else{
        root = new BiTree;
        root->data = data;
        cout<<"������"<<root->data<<"������: ";
        root->lchild=CreateBiTree();
        cout<<"������"<<root->data<<"���Һ���: ";
        root->rchild=CreateBiTree();
    }
    return root;
}
void preOrderTranverse(BiTree *root){
    if(root==NULL) return;//���Ƿ���,û�з���ֵ!!�ݹ���ֹ����
    printf("%c",root->data);
    preOrderTranverse(root->lchild);
    preOrderTranverse(root->rchild);
}
void inOrderTranverse(BiTree *root){
    if(root==NULL) return;
    inOrderTranverse(root->lchild);
    printf("%c",root->data);
    inOrderTranverse(root->rchild);
}
BiTree *reverseTree(BiTree *root){
    if(root==NULL) return NULL;
    root->lchild = reverseTree(root->lchild);
    root->rchild = reverseTree(root->rchild);
    BiTree *temptree = root->lchild;
    root->lchild = root->rchild;
    root->rchild = temptree;
    return root;
}
int main(){
    cout<< "����������: ";
    BiTree *root;
    root=CreateBiTree();
    preOrderTranverse(root);
    cout<<"\n";
    inOrderTranverse(root);
    cout<<"\n";
    reverseTree(root);
    preOrderTranverse(root);
    cout<<"\n";
    inOrderTranverse(root);
    return 0;
}


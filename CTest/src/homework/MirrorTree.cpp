//设计一个镜面影射算法,将一棵二叉树的每个结点的左右子结点交换位置
#include <cstdio>
#include <iostream>
using namespace std;
typedef char ELEMENT;
typedef struct BiTNode{
    ELEMENT data;
    struct BiTNode *rchild,*lchild;
}BiTree;
//前序遍历建树
BiTree *CreateBiTree(){
    ELEMENT data;
    cin>>data;
    BiTree *root;
    if(root == NULL){
        cout<<"内存分配失败";
        return NULL;
    }
    if(data=='@'){
        return NULL;
    }else{
        root = new BiTree;
        root->data = data;
        cout<<"请输入"<<root->data<<"的左孩子: ";
        root->lchild=CreateBiTree();
        cout<<"请输入"<<root->data<<"的右孩子: ";
        root->rchild=CreateBiTree();
    }
    return root;
}
void preOrderTranverse(BiTree *root){
    if(root==NULL) return;//就是返回,没有返回值!!递归终止条件
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
    cout<< "请输入根结点: ";
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


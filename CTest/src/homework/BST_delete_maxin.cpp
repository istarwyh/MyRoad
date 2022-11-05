#include<cstdio>
#include<iostream>
using namespace std;
typedef int ELEMENT;
typedef struct BiTNodes{
    ELEMENT key;//这里将data换成key,代表关键字
    struct BiTNodes *rchild;
    struct BiTNodes *lchild;
}*BiTree,BiTNode;
void preOrder(BiTree root){
    if(root==NULL) return;
    cout<<root->key<<" ";
    preOrder(root->lchild);
    preOrder(root->rchild);
}
int BSTinsert(BiTree &root, ELEMENT key){
    if(root==NULL){
        root= new BiTNode;
        root->lchild=root->rchild=NULL;
        root->key=key;
        return 1;
    }else{
        if(key==root->key)
            return 0;//如果输入的有相同值则丢弃了
        else if(key<root->key)
            return BSTinsert(root->lchild,key);
            //正是因为有这种指向递归的传递,所以才建立了指针的联系
        else
            return BSTinsert(root->rchild,key);
    }
}
void createBSTree(BiTree &root,ELEMENT n){//n为结点个数
    root=NULL;//将树清空
    ELEMENT key;
    for(int i=0;i<n;++i){
        cin >>key;
        BSTinsert(root,key);
    }

}
//找出值最大的结点
BiTree findMax(BiTree root){
    while(root->rchild)
        root = root->rchild;
    return root;
}
//找出值最小的结点
BiTree findMin(BiTree root){
    while(root->lchild)
        root = root->lchild;
    return root;
}
void deleteNode(BiTree &root,ELEMENT x){
    if(root==NULL) return;
    if(root->key==x){
        if(root->lchild==NULL && root->rchild==NULL)
            root=NULL;
        else if(root->lchild!=NULL){
            BiTree pre = findMax(root->lchild);//找root的前驱,左子树最大的也就是必然比root大一个的那一个值
            root->key=pre->key;
            //用前驱覆盖root
            deleteNode(root->lchild,pre->key);
            //在左子树中删除结点pre
        }else{
            BiTree post =findMin(root->rchild);
            //找root的后继,右子树最小的也必然就是比root小一个的那一个值
            root->key=post->key;
            //用后继覆盖root
            deleteNode(root->rchild,post->key);
            //在右子树中删除结点post
        }
    }else if(root->key>x){
        deleteNode(root->lchild,x);
        //大于x说明x应当在root的左子树
    }else deleteNode(root->rchild,x);
}
int main(){
    BiTree root;
    int n;
    cout<<"请输入结点的个数:";
    cin >> n;
    cout<<"请输入关键字:";
    createBSTree(root,n);
    // preOrder(root);
    cout<<"请输入要删除的结点值:";
    ELEMENT target;
    cin>>target;
    deleteNode(root,target);
    preOrder(root);
    return 0;
}

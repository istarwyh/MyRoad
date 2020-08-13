//从二叉搜索树中删除一个关键码:
//输入n为树结点数目,然后前序建树,建树后递归删除指定值为x的结点,然后再输出该树的前序,显示结点是否真的已被删除
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
            return 0;
        else if(key<root->key)
            return BSTinsert(root->lchild,key);
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
//必须先查找到那个结点,然后才可能删除
BiTree BSTSearch(BiTree root,ELEMENT key){
    if(root==NULL) return NULL;
    else{
        if(root->key==key) return root;//返回关键字所在的指针
        else if(key<root->key) 
            return BSTSearch(root->lchild,key);//如果小于则到左结点找<--BST定义
        else    return BSTSearch(root->rchild,key);
    }
}
//当p结点既有左子树,也有右子树的时候,将其转化为叶子结点或只有左子树,或只有右子树的情况,从而使用后者的解决办法
//先沿着p的左子树根结点的右指针一直往右走,直到来到其右子树的最右边的一个结点r,然后将p中的关键字用r中的关键字代替(最右边的是第二大的,正好删掉p后,它就是第一大的!),这样便转化了情况,如果是叶子结点,直接删除;如果只有一个子树,将r删除之后,将r的子树直接连接在原来r与其双亲结点f相连的指针上即可
int deleteNode(BiTree &root){
    if(root->lchild==NULL){//左子树空的话则只需重接它的右子树
        BiTree q = root;
        root=root->rchild;
        free(q);
    }else if(root->rchild==NULL){//只需重接它的左子树
        BiTree q = root;
        root=root->lchild;
        free(q);
    }else{//左右子树均不为空
        BiTree q = root,s=root->lchild;//转左
        while(s->rchild!=NULL){//然后向右到尽头
            q=s;s=s->rchild;
        }
        root->key = s->key;//s指向被删结点的"后继"
        if(q!=root) q->rchild=s->lchild;//重接q的右子树
        else q->lchild = s->lchild;//重接q的左子树
        delete s;
        
    }
    return 1;
}
int main(){
    BiTree root;
    int n;
    cout<<"请输入结点的个数:";
    cin >> n;
    cout<<"请输入关键字:";
    createBSTree(root,n);
    cout<<"请输入要删除的结点值:";
    int target;
    cin>>target;
    BiTree p;
    p = BSTSearch(root,target);
    deleteNode(p);
    preOrder(root);
    return 0;
}

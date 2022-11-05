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
//为了方便用二叉链表表示树,设计出一个含有数据域和两个指针域的树结点
typedef struct Btnode{
    ELEMTYPE data;
    struct Btnode *lchild;
    struct Btnode *rchild;
}BiTreeNode;
//前序创建二叉树

//注意创立的树参数为空,树的结点是在里面定义的,为什么呢?大概是为了递归的需要
//并且返回的是BiTreeNode*类型,因为递归得要这个参数给左孩子右孩子,不是在函数体内改变了就可以

BiTreeNode *createBiTree(){
    BiTreeNode *T;
    ELEMTYPE data;
    scanf("%d",&data);
    if(data==-1){
        T = NULL;
    }else{
        T = new BiTreeNode;
        T->data = data; 
        cout<< "请输入"<<data<<"的左结点: ";
        T->lchild=createBiTree();
        cout<< "请输入"<<data<<"的右结点: ";
        T->rchild=createBiTree();
    }
    return T;
}


//递归建立二叉树,输入必须使得最后返回到根结点,即第一个函数体结束这样才能有这个函数的结束

//遍历结点,度为0的为叶子结点
void leafCount(BiTreeNode *T,int &count) {
    //考虑是空树的情况,T!=NULL
    if(T!=NULL){
        if(T->lchild==NULL && T->rchild==NULL){
            count++;
        }else{
            //当左孩子与右孩子都不存在的时候,即为NULL的时候,不可以继续往下递归找左孩子右孩子了!
            leafCount(T->lchild,count);
            leafCount(T->rchild,count);
        }
    }
    
}
int main(){
    cout<< "请输入根结点的值:  ";
    BiTreeNode *root = createBiTree();
    int count = 0;
    leafCount(root,count);
    printf("叶子结点数为:%d\n",count);
    return 0;
}
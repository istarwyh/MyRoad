#include  <cstdio>
#include <iostream>
#include <vector>
using namespace std;
typedef struct TreeNode{
    char data;
    int childNo,siblingNo;
    TreeNode *lchild;
    TreeNode *rsibling;
}*Tree,TreeNode;
void preOrder(Tree root);
void inOrder(Tree root);
void CreateTree(int root,vector<Tree> tree);

int main() {
    int n;
    cout << "请输入树的结点数目:"<<endl;
    cin>>n;
    int root;
    cout << "根结点的编号:"<<endl;
    cin>> root;
    vector<Tree> tree;
    cout << "请输入每一个结点的字符以及它的左孩子右兄弟的编号"<<endl;
    for(int i=0;i<n;i++){
        Tree temp = new TreeNode;
        cin>>temp->data>> temp->childNo >> temp->siblingNo;
        temp->lchild = temp->rsibling = NULL;
        tree.push_back(temp);
    }
    CreateTree(root,tree);
    cout << "前序遍历:"<<endl;
    preOrder(tree[root]);
    cout <<"\n";
    cout << "中序遍历:"<<endl;
    inOrder(tree[root]);
    return 0 ;
}
void preOrder(Tree root){
    if(!root) return;
    cout <<root->data<<" ";
    preOrder(root->lchild);
    preOrder(root->rsibling);
}
void inOrder(Tree root){
    if(!root) return;
    inOrder(root->lchild);
    cout <<root->data<<" ";
    inOrder(root->rsibling);
}
void CreateTree(int root,vector<Tree> tree){
    if(tree[root]->childNo != -1){
        tree[root]->lchild = tree[tree[root]->childNo];
        CreateTree(tree[root]->childNo,tree);
    }
    if(tree[root]->siblingNo != -1){
        tree[root]->rsibling = tree[tree[root]->siblingNo];
        CreateTree(tree[root]->siblingNo,tree);
    }
}
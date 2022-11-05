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
    cout << "���������Ľ����Ŀ:"<<endl;
    cin>>n;
    int root;
    cout << "�����ı��:"<<endl;
    cin>> root;
    vector<Tree> tree;
    cout << "������ÿһ�������ַ��Լ������������ֵܵı��"<<endl;
    for(int i=0;i<n;i++){
        Tree temp = new TreeNode;
        cin>>temp->data>> temp->childNo >> temp->siblingNo;
        temp->lchild = temp->rsibling = NULL;
        tree.push_back(temp);
    }
    CreateTree(root,tree);
    cout << "ǰ�����:"<<endl;
    preOrder(tree[root]);
    cout <<"\n";
    cout << "�������:"<<endl;
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
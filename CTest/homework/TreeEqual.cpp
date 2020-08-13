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

void CreateTree(int root,vector<Tree> tree);
bool preCompare(Tree root1,Tree root2);
void preOrder(Tree T);

int main(){
   
    vector<Tree> tree1;
    vector<Tree> tree2;
    int n1,n2;
    cout << "�������һ������ڶ������Ľ����Ŀ:"<<endl;
    scanf("%d %d",&n1,&n2);//��һ������ڶ������Ľ����Ŀ

    int root1,root2;
    cout << "�������һ������ڶ������ĸ������:"<<endl;
    scanf("%d %d",&root1,&root2);//�����һ������ڶ������ĸ��ڵ�

    // �����һ����ÿһ�е�data,���ӱ�������ֵܱ��
    cout << "�������һ����ÿһ�е�data,���ӱ�������ֵܱ��:"<<endl;
    getchar();// ������һ�����з���Ҫ����
    for(int i=0;i<n1;i++){
        Tree temp = new TreeNode;
        scanf("%c %d %d",&temp->data,&temp->childNo,&temp->siblingNo);
        getchar();
        temp->lchild = temp->rsibling = NULL;
        tree1.push_back(temp);
    }
    cout << "������ڶ�����ÿһ�е�data,���ӱ�������ֵܱ��:"<<endl;
    // ��һ�����м��Ѿ�������ִ�е�getchar()������
    // ����ڶ�����ÿһ�е�data,���ӱ�������ֵܱ��
    for(int i=0;i<n2;i++){
        Tree temp = new TreeNode;
        scanf("%c %d %d",&temp->data,&temp->childNo,&temp->siblingNo);
        getchar();
        temp->lchild = temp->rsibling = NULL;
        tree2.push_back(temp);
        // cout<<"<"<<temp->data<<"> "<<endl;
    }

    if(n1 != n2){
        cout << "������ͬ\n"<<endl;
        return 0;
    }
    // cout << "<"<< root1<<">"<<endl;
    
    CreateTree(root1,tree1);
    CreateTree(root2,tree2);
    preOrder(tree1[root1]);
    preOrder(tree2[root2]);

    if(preCompare(tree1[root1],tree2[root2])) cout << "������ͬ\n"<<endl;
    else cout << "������ͬ\n";

    return 0;
}
//������ʱ���ܽ�����?���뵽ʲôʱ���������ֹ����?
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
bool preCompare(Tree root1,Tree root2){
    if(root1 !=NULL && root2 !=NULL ){
        if(root1->data != root2->data)  return false;
        return preCompare(root1->lchild,root2->lchild) && preCompare(root1->rsibling,root2->rsibling);
    }else if(( root1 == NULL && root2 != NULL ) || (root2 == NULL && root1 != NULL)) return false;
    return true;
}
void preOrder(Tree T){
    if(!T) return;
    printf("%c\n", T->data);
    preOrder(T->lchild);
    preOrder(T->rsibling);
}
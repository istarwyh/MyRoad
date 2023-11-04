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
    cout << "请输入第一棵树与第二棵树的结点数目:"<<endl;
    // scanf多用于格式化输入的时候
    cin>>n1>>n2;//第一棵树与第二棵树的结点数目
    if(n1 != n2){
        cout << "两树不同\n"<<endl;
        return 0;
    }
    
    int root1,root2;
    cout << "请输入第一棵树与第二棵树的根结点编号:"<<endl;
    cin>>root1>>root2;//输入第一棵树与第二棵树的根节点
    
    // 输入第一棵树每一行的data,左孩子编号与右兄弟编号
    cout << "请输入第一棵树每一行的data,左孩子编号与右兄弟编号:"<<endl;
    getchar();
    for(int i=0;i<n1;i++){
        Tree temp = new TreeNode;
        cin>>temp->data>>temp->childNo>>temp->siblingNo;
        temp->lchild = temp->rsibling = NULL;
        tree1.push_back(temp);
        
    }
    cout << "请输入第二棵树每一行的data,左孩子编号与右兄弟编号:"<<endl;
    // 输入第二棵树每一行的data,左孩子编号与右兄弟编号
    for(int i=0;i<n2;i++){
        Tree temp = new TreeNode;
        cin>>temp->data>>temp->childNo>>temp->siblingNo;
        temp->lchild = temp->rsibling = NULL;
        tree2.push_back(temp);
        // cout<<"<"<<tem->data<<"> "<<endl;
    }
    
    CreateTree(root1,tree1);
    CreateTree(root2,tree2);
    // preOrder(tree1[root1]);
    // preOrder(tree2[root2]);

    if(preCompare(tree1[root1],tree2[root2])) cout << "两树相同\n"<<endl;
    else cout << "两树不同\n";

    return 0;
}
//建立的时候能结束吗?输入到什么时候这个是终止的呢?
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
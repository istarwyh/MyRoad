#include  <cstdio>
#include <iostream>
#include <vector>
#include<queue>
#include<algorithm>
using namespace std;
#define maxSize 100000

struct TreeNode{
    char data;
    vector<int> childNo;
}Tree[maxSize];

void CreateTree(int n);
int levelOrder(int root);
int main(){
    int n;
    cout << "请输入树的结点数目:"<<endl;
    cin >> n;
    if(n==0){
        cout<<"结点数目: "<<n;
        return 0;
    } 
    int root;
    cout << "根的编号:"<<endl;
    cin >> root;
    CreateTree(n);
    cout<<"结点数目: "<<levelOrder(root);
    return 0 ;
}
void CreateTree(int n){
    cout << "请输入树每一个结点的data,孩子的数目与孩子的编号(但是必须输够n行):"<<endl;
    for(int i = 0; i < n;i++){
        int childNum,childNo;
        cin>>Tree[i].data>>childNum;
        while(childNum--){
            cin>>childNo;
            Tree[i].childNo.push_back(childNo);
        }
    }
}
int levelOrder(int root){
    queue<int> q;
    q.push(root);
    int count = 0;
    while(!q.empty()){
        int front = q.front();
        int childNum = Tree[q.front()].childNo.size();
        q.pop();
        int childNo=0;
        count++;//结点计数
        while(childNum--){
            q.push(Tree[front].childNo[childNo++]);
        }
    }
    return count;
}

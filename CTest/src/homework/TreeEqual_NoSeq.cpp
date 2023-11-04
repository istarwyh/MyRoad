#include  <cstdio>
#include <iostream>
#include <vector>
#include<queue>
#include<algorithm>
using namespace std;
#define maxSize 100000

// 这里不可以用typedef
//树的子结点表示法
struct TreeNode{
    char data;
    vector<int> childNo;
}Tree1[maxSize],Tree2[maxSize];

bool cmp1(int a,int b);
bool cmp2(int a,int b);
bool levelOrder(int root1,int root2);

int main (){
    int n1,n2;
    cout << "请输入第一棵树与第二棵树的结点数目:"<<endl;
    cin >>n1>>n2;
    if(n1 != n2){
        cout<<"两树不同\n";
        return 0;
    }

    int root1,root2;
    cout << "请输入第一棵树与第二棵树的根结点编号:"<<endl;
    cin>>root1>>root2;//输入第一棵树与第二棵树的根节点

    cout << "请输入第一棵树每一个结点的data,孩子的数目与孩子的编号(但是必须输够n行):"<<endl;
    for(int i=0;i<n1;i++){
        int childNum,childNo;
        cin >>Tree1[i].data>>childNum;
        while(childNum--){
            cin>>childNo;
            Tree1[i].childNo.push_back(childNo);
        }
    }  
    cout << "请输入第二棵树每一个结点的data,孩子的数目与孩子的编号(但是必须输够n行):"<<endl;
    for(int i=0;i<n2;i++){
        int childNum,childNo;
        cin>>Tree2[i].data>>childNum;
        while(childNum--){
            cin>>childNo;
            Tree2[i].childNo.push_back(childNo);
        }
    }
    if(levelOrder(root1,root2)) 
        cout<<"两树相同\n";
    else
        cout<<"两树不同\n";
    return 0;
}
bool cmp1(int a,int b){
    return Tree1[a].data > Tree1[b].data;
}
bool cmp2(int a,int b){
    return Tree2[a].data > Tree2[b].data;
}
bool levelOrder(int root1,int root2){
    queue<int> q1,q2;
    q1.push(root1);
    q2.push(root2);
    while(!q1.empty() && !q2.empty()){
        int front1 =q1.front();
        int front2 =q2.front();
        q1.pop();
        q2.pop();
        if(Tree1[front1].childNo.size() != Tree2[front2].childNo.size()) return false;

        // 对结点data按Unicode码大小排序,消除孩子顺序的影响
        sort(Tree1[front1].childNo.begin(), Tree1[front1].childNo.end(),cmp1);
        sort(Tree2[front2].childNo.begin(), Tree2[front2].childNo.end(),cmp2);
        // 判断孩子是否相同
        for(int i = 0; i <Tree1[front1].childNo.size();i++){
            if(Tree1[Tree1[front1].childNo[i]].data != Tree2[Tree2[front2].childNo[i]].data)
            return false;
        }
    }
    return true;
}
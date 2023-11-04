#include  <cstdio>
#include <iostream>
#include <vector>
#include<queue>
#include<algorithm>
using namespace std;
#define maxSize 100000

// ���ﲻ������typedef
//�����ӽ���ʾ��
struct TreeNode{
    char data;
    vector<int> childNo;
}Tree1[maxSize],Tree2[maxSize];

bool cmp1(int a,int b);
bool cmp2(int a,int b);
bool levelOrder(int root1,int root2);

int main (){
    int n1,n2;
    cout << "�������һ������ڶ������Ľ����Ŀ:"<<endl;
    cin >>n1>>n2;
    if(n1 != n2){
        cout<<"������ͬ\n";
        return 0;
    }

    int root1,root2;
    cout << "�������һ������ڶ������ĸ������:"<<endl;
    cin>>root1>>root2;//�����һ������ڶ������ĸ��ڵ�

    cout << "�������һ����ÿһ������data,���ӵ���Ŀ�뺢�ӵı��(���Ǳ����乻n��):"<<endl;
    for(int i=0;i<n1;i++){
        int childNum,childNo;
        cin >>Tree1[i].data>>childNum;
        while(childNum--){
            cin>>childNo;
            Tree1[i].childNo.push_back(childNo);
        }
    }  
    cout << "������ڶ�����ÿһ������data,���ӵ���Ŀ�뺢�ӵı��(���Ǳ����乻n��):"<<endl;
    for(int i=0;i<n2;i++){
        int childNum,childNo;
        cin>>Tree2[i].data>>childNum;
        while(childNum--){
            cin>>childNo;
            Tree2[i].childNo.push_back(childNo);
        }
    }
    if(levelOrder(root1,root2)) 
        cout<<"������ͬ\n";
    else
        cout<<"������ͬ\n";
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

        // �Խ��data��Unicode���С����,��������˳���Ӱ��
        sort(Tree1[front1].childNo.begin(), Tree1[front1].childNo.end(),cmp1);
        sort(Tree2[front2].childNo.begin(), Tree2[front2].childNo.end(),cmp2);
        // �жϺ����Ƿ���ͬ
        for(int i = 0; i <Tree1[front1].childNo.size();i++){
            if(Tree1[Tree1[front1].childNo[i]].data != Tree2[Tree2[front2].childNo[i]].data)
            return false;
        }
    }
    return true;
}
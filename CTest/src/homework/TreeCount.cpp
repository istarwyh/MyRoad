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
    cout << "���������Ľ����Ŀ:"<<endl;
    cin >> n;
    if(n==0){
        cout<<"�����Ŀ: "<<n;
        return 0;
    } 
    int root;
    cout << "���ı��:"<<endl;
    cin >> root;
    CreateTree(n);
    cout<<"�����Ŀ: "<<levelOrder(root);
    return 0 ;
}
void CreateTree(int n){
    cout << "��������ÿһ������data,���ӵ���Ŀ�뺢�ӵı��(���Ǳ����乻n��):"<<endl;
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
        count++;//������
        while(childNum--){
            q.push(Tree[front].childNo[childNo++]);
        }
    }
    return count;
}

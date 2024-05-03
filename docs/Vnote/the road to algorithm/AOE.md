## 1. AOE
### 1.1. 基本思路
1. 使用邻接表建图
2. 对图进行拓扑排序,根据此序列计算出顶点即事件的最早发生时间与最迟发生时间
    - 一个事件的最早发生时间为**指向**它的边的权值(即活动持续时间)加上发出这个活动的事件的最早发生时间,考虑最不利的,取**最大值**
    - 一个事件的最迟发生时间为它**发出**的边所指向的事件的最迟发生时间减去指向它的边的权值,考虑最周到的,取**最小值**
3. 对图进行逆拓扑排序,根据此序列计算出边即活动的最早发生时间与最迟发生时间
    - 一个活动的最早发生时间发出它的事件的最早发生时间
    - 一个活动的最迟发生时间它之后的事件的最迟发生事件减去它自己的持续时间(即权值)
4. 最早发生时间与最迟发生时间相等的活动即为关键活动,由关键活动组成的路径即为关键路径
### 1.2. 说明
本人折腾许久,依然未能写完全AOE的算法.在网上学习许久,昏昏沉沉,仍不得法,也不懂自己的为什么错了.先交了这个作业.谢谢助教!
### 1.3. Code
https://blog.csdn.net/hackerain/article/details/6054188
```c++
#include<cstdio>
#include<iostream>
#include<vector>
using namespace std;
const int maxSize = 5;
typedef struct ArcNode{
    int adjvex;// 与当前顶点相邻接顶点的序号
    int weight;
    struct ArcNode *nextarc;// 指向下一条边的指针
}ArcNode;

typedef struct{
    char data;
    int indegree;// 用来统计当前结点的入度
    ArcNode *firstarc;// 指向第一条边
}VNode;
typedef struct{
    VNode adjlist[maxSize];// 结构体数组类型,调用其内部成员使用."
    int n,e;
}AGraph;

bool topoSort(AGraph &G){
    int i,j,n=0;// i在每一处作了每一处不同的角色
    int stack[maxSize],top=-1;
    ArcNode *p;
    for(i=0;i<G.n;i++)
        G.adjlist[i].indegree = 0;
    for(i=0;i<G.n;i++){
        p = G.adjlist[i].firstarc;
        while(p){
            G.adjlist[p->adjvex].indegree++;
            p=p->nextarc;
        }
        if(G.adjlist[i].indegree == 0 )
            stack[++top] = i;
    }
    while(top!=-1){
        i = stack[top--];
        ++n;
        cout<<i<<" ";// 出栈打印

        p=G.adjlist[i].firstarc;// p变成了表头顶点,开始往后遍历
        while(p != NULL){
            j = p->adjvex;
            --(G.adjlist[j].indegree);// adjlist 中的每一个都是一个表头顶点,也就是图中的结点
            if(G.adjlist[j].indegree == 0)
                stack[++top] = j;
            p = p->nextarc;
        }
    }

    return n==G.n ? true : false;
}
void createG(AGraph &G,int n,int e){
    G.n = n;G.e = e;
    cout<<"请输入各个结点值:"<<endl;
    for(int i=0;i<n;i++){
        cin>>G.adjlist[i].data;
        G.adjlist[i].firstarc=NULL;
    }
    for(int i=0;i<e;i++){
        ArcNode* p = new ArcNode;
        cout<<"请输入边的始点:"<<endl;
        cin>>p->adjvex;
        cout<<"请输入结点/边的权值(活动时间):"<<endl;
        cin>>p->weight;
        // 采用头插法建立邻接表,因为它是个入度表
        p->nextarc = G.adjlist[i].firstarc;// 这个时候它自己就是头结点...所以和之前写的链表那个不一样
        G.adjlist[i].firstarc = p;
    }

}
int main(){
    int n,e;
    // freopen("in.txt","r",stdin);
    // freopen("out.txt","w",stdout);
    cout<<"请输入顶点数和边数:"<<endl;
    cin>>n>>e;
    AGraph G;
    createG(G,n,e);
    
   
    topoSort(G);
    // fclose(stdin);
    // fclose(stdout);
    return 0;
}
```
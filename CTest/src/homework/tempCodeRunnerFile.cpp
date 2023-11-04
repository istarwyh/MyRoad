#include <iostream>
using namespace std;
#define MAX 10000000
#define MAX_VERTEX_NUM 20
int ve[MAX_VERTEX_NUM];
/*˳��ջ�Ķ���*/
#define Stack_Size 100
typedef struct sqStack
{
       int *elem;
       int top;
       int stackSize;//ջ���鳤��
}sqStack;
 
 
/*˳��ջ�ĳ�ʼ��*/
void initStack_Sq(sqStack &S)
{	
       S.elem=new int[Stack_Size];
       S.top=-1;
       S.stackSize=Stack_Size;
}
/*��ջ*/
void push(sqStack &S,int x)
{
       if(S.top==Stack_Size-1)
              cout<<"Stack Overflow!";
       S.elem[++S.top]=x;
}
 
/*��ջ*/
int pop(sqStack &S)
{
       int x;
       if(S.top==-1)
              cout<<"Stack Empty!";
       x=S.elem[S.top--];
       return x;
}
typedef struct EdgeNode
{//�߱���Ķ���
	int adjvex;//����ڽӵ��ڶ�����е�λ��
	struct EdgeNode * nextedge;//ָ����һ���߱���
	int weight;
}EdgeNode;
typedef struct VexNode
{//�������Ķ���
	char vex;//��Ŷ�����Ϣ
	EdgeNode * firstedge;//ָ���һ���߱���
	int indegree;
}VexNode;
typedef struct
{//�����Ķ���	
	VexNode vexs[MAX_VERTEX_NUM];
	int vexnum,edgenum;
}LGraph;
/*��������ͼ���ڽӱ�*/
void CreateDG_AL(LGraph &G,int n,int e)
{
	int i,j,k,w;
	G.vexnum=n;
	G.edgenum=e;
    cout<<"������������ֵ:"<<endl;
	for(i=0;i<n;i++)
	{
		cin>>G.vexs[i].vex;
		G.vexs[i].firstedge=NULL;//��ʼ��Ϊ��
	}
	for(k=0;k<e;k++)
	{
		EdgeNode *p;
        cout<<"������ߵ�ʼ��(����):"<<endl;
		cin>>i;
        cout<<"������ߵ��յ�:"<<endl;
		cin>>j;
        cout<<"��������/�ߵ�Ȩֵ(�ʱ��):"<<endl;
		cin>>w;
		p=new EdgeNode;
		p->adjvex=j;
		p->weight=w;
		p->nextedge=G.vexs[i].firstedge;
		G.vexs[i].firstedge=p;//����ͷ�巨
	}
}
//����������������¼������緢��ʱ�估����������
void TopoSort(LGraph &G,sqStack &T)
{
	sqStack S;
	initStack_Sq(S);
	EdgeNode *p;
	
	int count=0;
	int i;
	for(i=0;i<G.vexnum;i++)
		G.vexs[i].indegree=0;//��ʼ��Ϊ0
	for(i=0;i<G.vexnum;i++)
	{//���������������
		p=G.vexs[i].firstedge;
		while(p)
		{
			G.vexs[p->adjvex].indegree++;
			p=p->nextedge;
		}
	}
	for(i=0;i<G.vexnum;i++)
		if(G.vexs[i].indegree==0)
			push(S,i);//����Ϊ0�Ķ�����ջ,�����ջ�������Ϊ0�Ķ����������е�λ��
	for(i=0;i<G.vexnum;i++)
		ve[i]=0;//��ʼ�������¼������緢��ʱ��Ϊ0
	while(S.top!=-1)
	{
		i=pop(S);
		cout<<G.vexs[i].vex<<" ";//��ջ����Ԫ�س�ջ��������������Ϊ0�Ķ������
		push(T,i);//Ϊ������������е������У���Ԫ�����ν�ջ�͵õ���������
		count++;//��������1
		p=G.vexs[i].firstedge;//��pָ�����Ϊ0�Ķ���ĵ�һ���߱���
		while(p)
		{
			int k;
			int dut;
			dut=p->weight;
			k=p->adjvex;
			G.vexs[k].indegree--;//�����Ϊ0�Ķ�����ڽӵ����ȼ�1
			if(G.vexs[k].indegree==0)
				push(S,k);//�ȼ�1��Ķ�����������Ϊ0��������ջ
			if(ve[i]+dut>ve[k])
				ve[k]=ve[i]+dut;//����whileѭ�����������¼��������ڽӵ�����緢��ʱ���������
								//���Ҿ�������whileѭ�������ϵظ���Ϊ�ϴ��ve[k]ֵ
			p=p->nextedge;
		}
	}
	cout<<endl;
	if(count<G.vexnum)
		cout<<"Network G has citcuits!"<<endl;
}
//��ؼ�·���͹ؼ��
void CriticalPath(LGraph &G)
{
	int i,j,k,dut;
	int ee,el;
	int vl[MAX_VERTEX_NUM];
	EdgeNode *p;
	sqStack T;
	initStack_Sq(T);
	TopoSort(G,T);
	for(i=0;i<G.vexnum;i++)
		vl[i]=ve[G.vexnum-1];//��ʼ�������¼�����ٷ���ʱ��Ϊ�������緢��ʱ��
							//��Ϊ����ٷ���ʱ���Ǵӻ����Դ�㿪ʼ�����
	while(T.top!=-1)
	{//����whileѭ��������ջ˳�����ÿ���������ٷ���ʱ��
		for(j=pop(T),p=G.vexs[j].firstedge; p ;p=p->nextedge)
		{//����Ӧ��ע��forѭ���Ļ��ƣ�ÿһ��ѭ����Ҫ�ж�һ��������������һ��
			k=p->adjvex;
			dut=p->weight;
			if(vl[k]-dut<vl[j])
				vl[j]=vl[k]-dut;//����ջT���¼���˳�򣬽��ö����¼�����ٷ���ʱ�侭��forѭ���������
								//ע�⣺����forѭ���������һ���������ٷ���ʱ��							
		}
	}
	for(i=0;i<G.vexnum;i++)
	{//���α���ÿһ���
		for(p=G.vexs[i].firstedge;p;p=p->nextedge)
		{
			k=p->adjvex;
			dut=p->weight;
			ee=ve[i];//�������翪ʼʱ��
			el=vl[k]-dut;//������ٿ�ʼʱ��
			if(ee==el)
			{//��������ȣ�˵��������Ϊ�ؼ��
				cout<<"("<<G.vexs[i].vex<<","<<G.vexs[k].vex<<")"<<dut<<" ";
				cout<<"ee="<<ee<<","<<"el="<<el<<endl;
			}
		}
	}
}
int main()
{
	// freopen("in.txt","r",stdin);
    cout<<"�����붥�����ͱ���:"<<endl;
	int n,e;cin>>n>>e;
	LGraph G;
	CreateDG_AL(G,n,e);
	CriticalPath(G);
	return 0;
}
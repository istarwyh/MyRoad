#include<cstdio>
#include<iostream>
using namespace std;
typedef int ELEMENT;
typedef struct BiTNodes{
    ELEMENT key;//���ｫdata����key,����ؼ���
    struct BiTNodes *rchild;
    struct BiTNodes *lchild;
}*BiTree,BiTNode;
void preOrder(BiTree root){
    if(root==NULL) return;
    cout<<root->key<<" ";
    preOrder(root->lchild);
    preOrder(root->rchild);
}
int BSTinsert(BiTree &root, ELEMENT key){
    if(root==NULL){
        root= new BiTNode;
        root->lchild=root->rchild=NULL;
        root->key=key;
        return 1;
    }else{
        if(key==root->key)
            return 0;//������������ֵͬ������
        else if(key<root->key)
            return BSTinsert(root->lchild,key);
            //������Ϊ������ָ��ݹ�Ĵ���,���ԲŽ�����ָ�����ϵ
        else
            return BSTinsert(root->rchild,key);
    }
}
void createBSTree(BiTree &root,ELEMENT n){//nΪ������
    root=NULL;//�������
    ELEMENT key;
    for(int i=0;i<n;++i){
        cin >>key;
        BSTinsert(root,key);
    }

}
//�ҳ�ֵ���Ľ��
BiTree findMax(BiTree root){
    while(root->rchild)
        root = root->rchild;
    return root;
}
//�ҳ�ֵ��С�Ľ��
BiTree findMin(BiTree root){
    while(root->lchild)
        root = root->lchild;
    return root;
}
void deleteNode(BiTree &root,ELEMENT x){
    if(root==NULL) return;
    if(root->key==x){
        if(root->lchild==NULL && root->rchild==NULL)
            root=NULL;
        else if(root->lchild!=NULL){
            BiTree pre = findMax(root->lchild);//��root��ǰ��,����������Ҳ���Ǳ�Ȼ��root��һ������һ��ֵ
            root->key=pre->key;
            //��ǰ������root
            deleteNode(root->lchild,pre->key);
            //����������ɾ�����pre
        }else{
            BiTree post =findMin(root->rchild);
            //��root�ĺ��,��������С��Ҳ��Ȼ���Ǳ�rootСһ������һ��ֵ
            root->key=post->key;
            //�ú�̸���root
            deleteNode(root->rchild,post->key);
            //����������ɾ�����post
        }
    }else if(root->key>x){
        deleteNode(root->lchild,x);
        //����x˵��xӦ����root��������
    }else deleteNode(root->rchild,x);
}
int main(){
    BiTree root;
    int n;
    cout<<"��������ĸ���:";
    cin >> n;
    cout<<"������ؼ���:";
    createBSTree(root,n);
    // preOrder(root);
    cout<<"������Ҫɾ���Ľ��ֵ:";
    ELEMENT target;
    cin>>target;
    deleteNode(root,target);
    preOrder(root);
    return 0;
}

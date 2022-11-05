//�Ӷ�����������ɾ��һ���ؼ���:
//����nΪ�������Ŀ,Ȼ��ǰ����,������ݹ�ɾ��ָ��ֵΪx�Ľ��,Ȼ�������������ǰ��,��ʾ����Ƿ�����ѱ�ɾ��
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
            return 0;
        else if(key<root->key)
            return BSTinsert(root->lchild,key);
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
//�����Ȳ��ҵ��Ǹ����,Ȼ��ſ���ɾ��
BiTree BSTSearch(BiTree root,ELEMENT key){
    if(root==NULL) return NULL;
    else{
        if(root->key==key) return root;//���عؼ������ڵ�ָ��
        else if(key<root->key) 
            return BSTSearch(root->lchild,key);//���С����������<--BST����
        else    return BSTSearch(root->rchild,key);
    }
}
//��p������������,Ҳ����������ʱ��,����ת��ΪҶ�ӽ���ֻ��������,��ֻ�������������,�Ӷ�ʹ�ú��ߵĽ���취
//������p����������������ָ��һֱ������,ֱ�������������������ұߵ�һ�����r,Ȼ��p�еĹؼ�����r�еĹؼ��ִ���(���ұߵ��ǵڶ����,����ɾ��p��,�����ǵ�һ���!),������ת�������,�����Ҷ�ӽ��,ֱ��ɾ��;���ֻ��һ������,��rɾ��֮��,��r������ֱ��������ԭ��r����˫�׽��f������ָ���ϼ���
int deleteNode(BiTree &root){
    if(root->lchild==NULL){//�������յĻ���ֻ���ؽ�����������
        BiTree q = root;
        root=root->rchild;
        free(q);
    }else if(root->rchild==NULL){//ֻ���ؽ�����������
        BiTree q = root;
        root=root->lchild;
        free(q);
    }else{//������������Ϊ��
        BiTree q = root,s=root->lchild;//ת��
        while(s->rchild!=NULL){//Ȼ�����ҵ���ͷ
            q=s;s=s->rchild;
        }
        root->key = s->key;//sָ��ɾ����"���"
        if(q!=root) q->rchild=s->lchild;//�ؽ�q��������
        else q->lchild = s->lchild;//�ؽ�q��������
        delete s;
        
    }
    return 1;
}
int main(){
    BiTree root;
    int n;
    cout<<"��������ĸ���:";
    cin >> n;
    cout<<"������ؼ���:";
    createBSTree(root,n);
    cout<<"������Ҫɾ���Ľ��ֵ:";
    int target;
    cin>>target;
    BiTree p;
    p = BSTSearch(root,target);
    deleteNode(p);
    preOrder(root);
    return 0;
}

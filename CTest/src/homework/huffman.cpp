#include<cstdio>
#include<cstdlib>
#include<iostream>
using namespace std;
// �������������Ĭ������ȫ����������С����ʵ��,������data[]�洢!!!
typedef struct TreeNode{
    char ch[2];//��Ϊÿ��Ҷ�ӽ���Ȩֵ����һ��,����Ҫ��ʵ��Ҫ������ַ���������
    int weight; //Ȩֵ
    struct TreeNode* left; 
    struct TreeNode* right;
}HuffmanNode,*HuffmanTree;

#define MinData -1 //���Ŷ�Ԫ�صľ���ֵ���ı�

typedef struct HeapStruct{
    int size;//�ѵĵ�ǰԪ�صĸ��� 
    int capacity;//�ѵ��������,�����������ָʾ����Ĵ�С
    HuffmanTree *data; //�洢��Ԫ�ص����� ���±�1��ʼ
    //���data��Ȼ��ָ�� �ṹ��ָ�����͵� ָ��.
    // ��ΪҪ���������洢��ȫ������,���Զѽṹ����һ��������һ�������ָ��,��data
}MinHeapNode,*MinHeap;

HuffmanTree NewHuffmanNode();
MinHeap CreateMinHeap(int MaxSize);
bool Insert(MinHeap H,HuffmanTree item);
HuffmanTree DeleteMin(MinHeap H);
MinHeap BuildMinHeap(MinHeap H);
HuffmanTree Huffman(MinHeap H);
void preOrder(HuffmanTree BST);
void inOrder(HuffmanTree BST);
void HuffmanCode(HuffmanTree BST,int depth,int *cnt);

int main(){
    int i,N;
    MinHeap h; 
    HuffmanTree T,BT = NULL;

    printf("������Ҷ�ӽ��ĸ������ַ�������:\n");
    scanf("%d",&N);
    //Ҷ�ӽ��Ҳ��Ҫ������ַ�
   
    h = CreateMinHeap(2*N); //������С��
    //N��Ҷ�ӽ��������γɵĹ������������2N-1�������

    for(i=1;i<=N;i++){
        T = NewHuffmanNode();
        h->data[++(h->size)] = T;//!!!
    }
   
    printf("������������%d��Ҷ�ӽ����Դ�����ַ�:\n",N);
    getchar();//��������Ļ��з�,��Ϊscanf()�Ǵ���������ȡԪ��-->�ƺ�û��?
    // ���������c1,c2,c3,c4,c5,c6...��ÿ��ch����������string��,��������Ҫһ�������������װ��Щstring,Ȼ����һ��������ch,����������?-->����Ҫ,����scanf����
    // //����С����Ԫ�ص�ch������ֵ
    for(i=1;i<=h->size;i++){//size�ǵ�ǰ��Ԫ�ظ���,Ҳ����N!
        scanf("%s",h->data[i]->ch);
        // h->data[i]->ch = string[i-1];
        //TODO Ϊɶ��i-1?��Ϊ����Ǵ�1��ʼ��ʽ�洢ֵ��
    } 

    printf("������%d��Ҷ�ӽ���Ӧ��Ȩֵ:\n",N);
    //��С��Ԫ�ظ�ֵ
    for(i=1; i<=N; i++){//�����֮ȫ���ǿ�,i��1��ʼ
        scanf("%d",&(h->data[i]->weight));
    }
  
    BT = Huffman(h);//�����������
    printf("�����������������Ȩֵ:\n");
    preOrder(BT);   printf("\n");
    printf("��������˹���������Ȩֵ:\n"); 
	inOrder(BT);    printf("\n");
    
    int cnt=0;
    HuffmanCode(BT,0,&cnt);//cnt���ݽ�ȥ�Լ���ַ���ܵݹ����ʱ����������
    printf("�ܱ�����=��(·������*����Ƶ��(��ӦȨֵ)):%d\n",cnt); 
    return 0;
}

//�������������㷨
HuffmanTree Huffman(MinHeap H){
    //����H->size��Ȩֵ�Ѿ�����H->data[]->weight��
    int i,num;
    HuffmanTree T;

    BuildMinHeap(H);//��H->data[]��Ȩֵ����Ϊ��С��
    //�˴����뽫H->size��ֵ����num,��Ϊ������DeleteMin()��Insert()������ı�H->size��ֵ,һ������,һ������
    num = H->size;//��ʱ,H->size����N,��Ϊ֮ǰ������H�ľ���N��HuffmanTree���
    // cout<<"<"<<H->size<<">";

    for (i = 1; i < num;i++){//��H->size-1�κϲ�!!!!
        T = NewHuffmanNode();//����һ���µĸ����
        T->left = DeleteMin(H);//����С����ȡ��Ȩֵ��С��Ԫ����Ϊ�µ����ӽ��
        // ע��DeleteMin()����ֵ����Ҳ����HuffmanTree����!!
        T ->right = DeleteMin(H);//����С����ȡ��Ȩֵ��С��Ԫ����Ϊ�µ����ӽ��
        //��������֮�����ܱ�������,֮�����ܵݹ����������Ϊ���������䰡!
        T ->weight = T ->left->weight + T ->right->weight;//������Ȩֵ
        Insert(H,T);//����T���뵽��С��
    }
    T = DeleteMin(H);// ԭ��N�����,������N-1��������,����N-1�κϲ�ȡ��2(N-1)�����,���ʣ��1�����ɵ�HuffmanTree���Ǹ����
    // cout<<"<"<<H->size<<">";
    return T;
}
//�������
void preOrder(HuffmanTree BST){
    if(BST == NULL) return;
    cout<< BST->weight<<" ";
    preOrder(BST->left);
    preOrder(BST->right);
}
//�������
void inOrder(HuffmanTree BST){
    if(BST == NULL) return;
    inOrder(BST->left);
    cout<< BST->weight<<" ";
    inOrder(BST->right);
}
//�����µĸ����
HuffmanTree NewHuffmanNode(){
    HuffmanTree BST = new HuffmanNode;//����HuffmanNode���ô��ƺ�������
    BST->weight = 0;
    BST->left = BST->right = NULL;
    return BST;
}
//��������ΪMaxSize����С��,ʹ����������Ľṹ����(��Ԫ�س�ʼ��),Ȼ��������������,�ٿ����������Ե�������������С��
MinHeap CreateMinHeap(int MaxSize){
    MinHeap H = new MinHeapNode;
    H -> data = (HuffmanTree *)malloc((MaxSize+1) * sizeof(HuffmanTree));
    //MaxSize +1,��Ϊ�ѵ���ʽԪ�صĴ���Ǵ��±�Ϊ1��Ԫ�ش�ŵ�
    H->size = 0;
    H ->capacity =MaxSize;
    HuffmanTree T = NewHuffmanNode();

    T->weight = MinData;
    //����"�ڱ�"ΪС�ڶ������п���Ԫ�ص�ֵ,�����Ժ�������
    H->data[0] = T;//û���õ����׵�ַԪ���������ڱ�
    return H;
}   
//���ǽ���˶�������յ��жϷ���?
bool isFull(MinHeap H){
    return (H->size == H->capacity);
}
bool isEmpty(MinHeap H){
    return (H->size == 0);
}

//�����㷨--�����������뵽 ���丸��㵽����������������
bool Insert(MinHeap H,HuffmanTree item){
    //���ṹ��ָ��Ԫ��item���뵽��С��H��,������H->data[0]�ѱ�����Ϊ�ڱ�
    int i;
    if(isFull(H)){
        cout<<"��С������\n";
        return false;
    }
    i = ++H->size; //����Ѳ���,�Ͱ�Ԫ�طŵ��������һ��Ԫ�ص�λ��,Ҳ��size+1��λ��,�ʶ�++
    //��С�����ڱ�����С��Ȩֵ(�������︳��-1)��λ��0��ַ��λ��,ʹ�ú����븸���Ƚϲ���Խ���ڱ��Ľ�,Ҳ���ǲ���Խ���ѵĽ�;���ڱ�,����������о�����i>1,��ֹ����ȨֵС������Ԫ��,��ʹ��0�������Խ��Ķ��������Ƶ����<--�����ڱ���ʵ������һ���ж�,�������Ч��
    //i�ǵ�ǰλ��,i/2�����Ǹ�����λ��,��������λ�ñȽϵ���(�����������Ҫ����Ľ��Ȩֵ),������С�ѵ�������
    while(H->data[i/2]->weight > item->weight){
        H->data[i] = H->data[i/2];//�ò�������С������ֵ�Ĵ�������
        i/=2;//���ϵض�λ�������
    }
    H->data[i] = item;//��item����
    return true;
}
//����С��H��ȡ��ȨֵΪ��С��Ԫ��,��ɾ��һ�����
HuffmanTree DeleteMin(MinHeap H){
    int parent,child;
    HuffmanTree MinItem,temp = NULL;
    if(isEmpty(H)){
        cout<<"��С��Ϊ��\n";
        return NULL;
    }
    MinItem = H->data[1];//Ȩֵ��С,Ҳ����Ҫ����ȡ��,��ɾ����Ԫ��,�ȱ��������return����
    //����С���е����һ��Ԫ�شӸ���㿪ʼ���Ϲ����²���
    //TODO ������ʲô��˼?-->Ϊ���ҳ���С��Ԫ��
    temp = H->data[H->size--];//��С�������һ��Ԫ��,�ȼ�������һ��Ԫ���������ŵ��滻ɾ�������������Сֵ���,������ͨ��ѭ�����ҷ����������ֵ�ĺ���λ��,���Ѻ���λ�õ�Ԫ�ؽ���������������λ��-->��������ѭ������λ��
    //--H->size������?������,��Ϊ����Ҫ�����һ����㸳��temp,��Ҫ��ԭ���ı�ʶ������һ
    for (parent =1;parent*2 <= H->size;parent=child){//parent=1���ŵ�������ĵ�һλ,��ʽԪ�صĴ洢�Ǵ��±�Ϊ1��ʼ��;child�������H->size,child�������һ��Ԫ��֮����
        child = parent*2;//Ĭ�Ͻ�child���������ӵĵط�,�������Ӵ�(��������Ȼ��Ƚ�)
        if((child != H->size) && (H->data[child]->weight > H->data[child+1]->weight))//child�������H->size,�����Ӿ���ָ�����һ��Ԫ��,��ȻҲ��û�б�Ҫ����Ƚ���.��仰Ҳ����Ϊ�����Ч��
            child++;//������������Ļ�,childָ���Һ���,��childָ�����Һ����н�С��
        if(temp->weight < H->data[child]->weight)   break;  // �������λ�õ�temp��ֵС�����Һ����н�С��ֵ,����Ƚϵĳɹ����Ҳ���ֻ��ִ��һ��,���ҵ�parent�ĺ��ʵ�λ��
        else 
            H->data[parent] = H->data[child];// ���Һ����н�С���Ǹ���temp�������ڵ�λ��---parent���бȽ�,�����Ȼ����parent,˵��parentӦ���µ�child��λ��,ͬʱchild�ϵ�parent��λ��
        // �Ƚϵ�ʱ��Ƚ�Ȩֵ,������ʱ�����haffuman
    }

    H->data[parent] = temp;//temp��ŵ��˴�

    return MinItem;//ȡ��Ȩֵ��С�Ľ��
}
//����H->data[]�е�Ԫ��,ʹ������ѵ�������
MinHeap BuildMinHeap(MinHeap H){
    //����������е�H->size��Ԫ���Ѿ�����H->data[]��
    int i,parent,child;
    HuffmanTree temp;
    for(i=H->size/2;i>0;i--){//�����һ������㿪ʼ,һֱ����ֱ�������
        temp = H->data[i];
        for(parent =i;parent*2 <= H->size;parent=child){
            // ���¹���
            child = parent*2;
            if((child != H->size) && (H->data[child]->weight > H->data[child+1]->weight))//���Һ���,��������Ȩֵ�����Һ���
                child++;//child����ֵ�ƶ�-->childָ�����Һ�����Сֵ
            if(temp->weight < H->data[child]->weight )  break;
            else H->data[parent] = H->data[child];
        }//�����ڲ�for()ѭ������H->data[i]Ϊ���������ĵ���
        H ->data[parent] = temp;//temp(ԭH->data[i])��ŵ��˴�
    }
    return H;
}

//һ������·������,Ȼ������Ҷ�ӽ��Ͱ�֮ǰ���ݴ���붼�����,��һ�����¿�ʼ
void HuffmanCode(HuffmanTree BT,int depth,int *p_cnt){
    // depth ΪĿǰ���뵽�������������(���)
    static int code[10];
    if(BT!=NULL){
        if((BT->left == NULL) && (BT->right == NULL)){
            cout<<"�ַ�"<<BT->ch<<"��ӦȨֵΪ"<<BT->weight<<"�Ĺ���������Ϊ:";
            int i;
            //depth��·������,Ϊ�߶�-1,�������,��ԭ���Ǹ�����Ѱ��Ҷ�ӽ��ĵݹ�ĵ���Ȼ��+1��!
            for(i=0;i<depth;i++){
                cout<<code[i];
            }
            cout<<"\n";
            *p_cnt =(*p_cnt) + (BT->weight) * depth;
            // cout<< "<"<<*p_cnt<<"> ";
        }else{
            code[depth] = 0;  //���������������Ϊ0 
            HuffmanCode(BT->left,depth+1,p_cnt);
            // cout<< "<"<<*p_cnt<<"> ";
            code[depth] = 1;  //���������������Ϊ1 
            HuffmanCode(BT->right,depth+1,p_cnt);
        }
    }
}

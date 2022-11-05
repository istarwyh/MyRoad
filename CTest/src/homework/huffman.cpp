#include<cstdio>
#include<cstdlib>
#include<iostream>
using namespace std;
// 在这里哈夫曼树默认用完全二叉树的最小堆来实现,用数组data[]存储!!!
typedef struct TreeNode{
    char ch[2];//因为每个叶子结点的权值可能一样,所以要用实际要编码的字符进行区分
    int weight; //权值
    struct TreeNode* left; 
    struct TreeNode* right;
}HuffmanNode,*HuffmanTree;

#define MinData -1 //随着堆元素的具体值而改变

typedef struct HeapStruct{
    int size;//堆的当前元素的个数 
    int capacity;//堆的最大容量,这个分量用来指示数组的大小
    HuffmanTree *data; //存储堆元素的数组 从下标1开始
    //这个data居然是指向 结构体指针类型的 指针.
    // 因为要用数组来存储完全二叉树,所以堆结构体中一定包含有一个数组的指针,即data
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

    printf("请输入叶子结点的个数即字符的种数:\n");
    scanf("%d",&N);
    //叶子结点也即要编码的字符
   
    h = CreateMinHeap(2*N); //创建最小堆
    //N个叶子结点的最终形成的哈夫曼树最多有2N-1个树结点

    for(i=1;i<=N;i++){
        T = NewHuffmanNode();
        h->data[++(h->size)] = T;//!!!
    }
   
    printf("请连续输入这%d个叶子结点各自代表的字符:\n",N);
    getchar();//吸收上面的换行符,因为scanf()是从输入流中取元素-->似乎没用?
    // 如果比如是c1,c2,c3,c4,c5,c6...那每个ch分量都得是string啊,这样就需要一个更大的容器封装这些string,然后再一个个赋给ch,可以做到吗?-->不必要,如下scanf可以
    // //给最小堆中元素的ch分量赋值
    for(i=1;i<=h->size;i++){//size是当前堆元素个数,也就是N!
        scanf("%s",h->data[i]->ch);
        // h->data[i]->ch = string[i-1];
        //TODO 为啥是i-1?因为结点是从1开始正式存储值的
    } 

    printf("请输入%d个叶子结点对应的权值:\n",N);
    //最小堆元素赋值
    for(i=1; i<=N; i++){//不理解之全都是坑,i从1开始
        scanf("%d",&(h->data[i]->weight));
    }
  
    BT = Huffman(h);//构造哈夫曼树
    printf("先序遍历哈夫曼树的权值:\n");
    preOrder(BT);   printf("\n");
    printf("中序遍历此哈夫曼树的权值:\n"); 
	inOrder(BT);    printf("\n");
    
    int cnt=0;
    HuffmanCode(BT,0,&cnt);//cnt传递进去自己地址才能递归调用时自身不被重置
    printf("总编码数=Σ(路径长度*出现频率(对应权值)):%d\n",cnt); 
    return 0;
}

//哈夫曼树构造算法
HuffmanTree Huffman(MinHeap H){
    //假设H->size个权值已经存在H->data[]->weight里
    int i,num;
    HuffmanTree T;

    BuildMinHeap(H);//将H->data[]按权值调整为最小堆
    //此处必须将H->size的值交给num,因为后面做DeleteMin()和Insert()函数会改变H->size的值,一个减少,一个增加
    num = H->size;//此时,H->size还是N,因为之前拷贝进H的就是N个HuffmanTree结点
    // cout<<"<"<<H->size<<">";

    for (i = 1; i < num;i++){//做H->size-1次合并!!!!
        T = NewHuffmanNode();//建立一个新的根结点
        T->left = DeleteMin(H);//从最小堆中取出权值最小的元素作为新的左子结点
        // 注意DeleteMin()返回值本身也就是HuffmanTree类型!!
        T ->right = DeleteMin(H);//从最小堆中取出权值最小的元素作为新的右子结点
        //哈夫曼树之所以能被称作树,之所以能递归遍历正是因为上面这两句啊!
        T ->weight = T ->left->weight + T ->right->weight;//计算新权值
        Insert(H,T);//将新T插入到最小堆
    }
    T = DeleteMin(H);// 原本N个结点,新生成N-1个结点插入,做了N-1次合并取出2(N-1)个结点,最后剩下1个生成的HuffmanTree就是根结点
    // cout<<"<"<<H->size<<">";
    return T;
}
//先序遍历
void preOrder(HuffmanTree BST){
    if(BST == NULL) return;
    cout<< BST->weight<<" ";
    preOrder(BST->left);
    preOrder(BST->right);
}
//中序遍历
void inOrder(HuffmanTree BST){
    if(BST == NULL) return;
    inOrder(BST->left);
    cout<< BST->weight<<" ";
    inOrder(BST->right);
}
//建立新的根结点
HuffmanTree NewHuffmanNode(){
    HuffmanTree BST = new HuffmanNode;//定义HuffmanNode的用处似乎就在这
    BST->weight = 0;
    BST->left = BST->right = NULL;
    return BST;
}
//创建容量为MaxSize的最小堆,使其满足基本的结构特性(各元素初始化),然后往里面填数据,再考虑其有序性调整成真正的最小堆
MinHeap CreateMinHeap(int MaxSize){
    MinHeap H = new MinHeapNode;
    H -> data = (HuffmanTree *)malloc((MaxSize+1) * sizeof(HuffmanTree));
    //MaxSize +1,因为堆的正式元素的存放是从下标为1的元素存放的
    H->size = 0;
    H ->capacity =MaxSize;
    HuffmanTree T = NewHuffmanNode();

    T->weight = MinData;
    //定义"哨兵"为小于堆中所有可能元素的值,便于以后更快操作
    H->data[0] = T;//没有用到的首地址元素留着做哨兵
    return H;
}   
//这是借鉴了队列满与空的判断方法?
bool isFull(MinHeap H){
    return (H->size == H->capacity);
}
bool isEmpty(MinHeap H){
    return (H->size == 0);
}

//插入算法--将新增结点插入到 从其父结点到根结点的有序序列中
bool Insert(MinHeap H,HuffmanTree item){
    //将结构体指针元素item插入到最小堆H中,在这里H->data[0]已被定义为哨兵
    int i;
    if(isFull(H)){
        cout<<"最小堆已满\n";
        return false;
    }
    i = ++H->size; //如果堆不满,就把元素放到堆中最后一个元素的位置,也即size+1的位置,故而++
    //最小堆中哨兵是最小的权值(所以这里赋了-1)且位于0地址的位置,使得孩子与父结点比较不会越过哨兵的界,也就是不会越过堆的界;无哨兵,则必须增加判决条件i>1,防止插入权值小于所有元素,而使得0结点乃至越界的都会往下移的情况<--增加哨兵其实是少了一次判断,提高运行效率
    //i是当前位置,i/2正好是父结点的位置,把这两个位置比较调换(如果父结点大于要插入的结点权值),保持最小堆的有序性
    while(H->data[i/2]->weight > item->weight){
        H->data[i] = H->data[i/2];//让不满足结点小于下面值的大结点下来
        i/=2;//不断地定位到父结点
    }
    H->data[i] = item;//将item插入
    return true;
}
//从最小堆H中取出权值为最小的元素,并删除一个结点
HuffmanTree DeleteMin(MinHeap H){
    int parent,child;
    HuffmanTree MinItem,temp = NULL;
    if(isEmpty(H)){
        cout<<"最小堆为空\n";
        return NULL;
    }
    MinItem = H->data[1];//权值最小,也就是要优先取出,即删除的元素,先保存着最后return返回
    //用最小堆中的最后一个元素从根结点开始向上过滤下层结点
    //TODO 过滤是什么意思?-->为了找出最小的元素
    temp = H->data[H->size--];//最小堆中最后一个元素,先假设把最后一个元素拿上来放到替换删掉的最上面的最小值结点,接下来通过循环来找放上来的这个值的合适位置,并把合适位置的元素交换到最上面的这个位置-->所以下面循环来找位置
    //--H->size可以吗?不可以,因为不仅要把最后一个结点赋给temp,还要把原本的标识往回退一
    for (parent =1;parent*2 <= H->size;parent=child){//parent=1即放到最上面的第一位,正式元素的存储是从下标为1开始的;child如果大于H->size,child就在最后一个元素之外了
        child = parent*2;//默认将child放在其左孩子的地方,假设左孩子大(接着下面然后比较)
        if((child != H->size) && (H->data[child]->weight > H->data[child+1]->weight))//child如果等于H->size,即左孩子就是指向最后一个元素,自然也就没有必要后面比较了.这句话也就是为了提高效率
            child++;//上面条件满足的话,child指向右孩子,即child指向左右孩子中较小者
        if(temp->weight < H->data[child]->weight)   break;  // 如果上面位置的temp的值小于左右孩子中较小的值,这个比较的成功情况也最多只会执行一次,已找到parent的合适的位置
        else 
            H->data[parent] = H->data[child];// 左右孩子中较小的那个和temp假设所在的位置---parent进行比较,如果依然大于parent,说明parent应该下到child的位置,同时child上到parent的位置
        // 比较的时候比较权值,调换的时候调换haffuman
    }

    H->data[parent] = temp;//temp存放到此处

    return MinItem;//取出权值最小的结点
}
//调整H->data[]中的元素,使其满足堆的有序性
MinHeap BuildMinHeap(MinHeap H){
    //这里假设所有的H->size个元素已经存在H->data[]中
    int i,parent,child;
    HuffmanTree temp;
    for(i=H->size/2;i>0;i--){//从最后一个父结点开始,一直调整直到根结点
        temp = H->data[i];
        for(parent =i;parent*2 <= H->size;parent=child){
            // 向下过滤
            child = parent*2;
            if((child != H->size) && (H->data[child]->weight > H->data[child+1]->weight))//有右孩子,并且左孩子权值大于右孩子
                child++;//child的数值移动-->child指向左右孩子最小值
            if(temp->weight < H->data[child]->weight )  break;
            else H->data[parent] = H->data[child];
        }//结束内部for()循环对以H->data[i]为根的子树的调整
        H ->data[parent] = temp;//temp(原H->data[i])存放到此处
    }
    return H;
}

//一遍沿着路径编码,然后遇到叶子结点就把之前的暂存的码都输出来,下一次重新开始
void HuffmanCode(HuffmanTree BT,int depth,int *p_cnt){
    // depth 为目前编码到哈夫曼树的深度(层次)
    static int code[10];
    if(BT!=NULL){
        if((BT->left == NULL) && (BT->right == NULL)){
            cout<<"字符"<<BT->ch<<"对应权值为"<<BT->weight<<"的哈夫曼编码为:";
            int i;
            //depth是路径长度,为高度-1,等于深度,它原本是跟随着寻找叶子结点的递归的调用然后+1的!
            for(i=0;i<depth;i++){
                cout<<code[i];
            }
            cout<<"\n";
            *p_cnt =(*p_cnt) + (BT->weight) * depth;
            // cout<< "<"<<*p_cnt<<"> ";
        }else{
            code[depth] = 0;  //往左子树方向编码为0 
            HuffmanCode(BT->left,depth+1,p_cnt);
            // cout<< "<"<<*p_cnt<<"> ";
            code[depth] = 1;  //往右子树方向编码为1 
            HuffmanCode(BT->right,depth+1,p_cnt);
        }
    }
}

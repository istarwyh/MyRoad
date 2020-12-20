## 1. HashMap整体认识
HashMap按照组合模式的设计模式进行设计[^组合模式]，顶层接口声明类型，子接口定义功能，抽象类实现部分通用功能，HashMap作为Map子类之一实现自己的方法。接下来实现接口对Map的操作同于Map对于自己子结点的操作。HashMap作为容器，存储叶子结点Node<K,V>.对于叶子结点的内部类又是一个容器，包括了K和V；K和V是泛型，可以由用户自己装填任意类型，最终实现俄罗斯套娃的设计。
HashMap首先要解决的问题是Node的Key应该怎么映射，使得放进去的效率高同时查找的时候效率依然高。首先还是利用取余的分类功能，选取的是hash(key)的二进制尾数，为了保留高位的特征减少碰撞的概率，设计了高位取余算法；其次选取Hash的地址空间和扩容的阈值，初始定为16个桶，并且当达到12个桶的时候，即为了平衡发生碰撞时put元素的平均效率 和 空间很大更少碰撞但是空间紧张 之间的矛盾，默认负载因子为0.75；最后设计Node存在的数据结构形式：采用开地址法，每个Hash桶连接着链表--尾插法，并通过实验选择了当链表长度为8时转化为红黑树，又因为需要防止链表与红黑树在8这个长度处因为增加与删除导致反复转化，引起时间复杂度的震荡，选择红黑树元素存储为6时转化回链表。
而还有一些细节问题需要考虑，能不能提高取余操作本身的效率？因此权衡后强制每次hashmap的大小都是2的合数，这样方便使用位运算完成取余操作。
能不能在resize的时候尽量不要reIndex，因为都再搞一遍真的感觉累。所以找reIndex后的索引与原始Index之间的关系，发现大小都是2的合数时真的可以用更简单的方法完成`(n-1)&hash`操作，总共就两种情况。


[^组合模式]:[组合模式以及HashMap的设计分析（不包括具体实现）](http://www.manongjc.com/detail/17-vpesnqkfshssbzf.html)

## 2. key唯一性的保证--Hash算法
哈希函数是一类数学函数，可以在有限合理的时间内，将任意长度的消息压缩为固定长度的二进制串，其输出值称为哈希值或散列值。以哈希函数为基础构造的哈希算法常用于实现数据完整性和实体认证。
### 2.1. 一次hash之HashMap1.8

```java
    static final int hash(Object key) {
        int h;
        // 首先一个前提,key.hashCode()返回int型的hash值，2……31 * 2的空间太大不能直接用来做索引
        // 所以必须要借用余数或者异或操作来保留hash值特征的同时映射到较小范围做索引

        //取模运算 -> 1.7四次移位和四次异或位运算 ->  1.8一次异或
            // **扰动**一次就够了
        //1.取key得hashcode值
        //2.高位参与运算--h和h右移16位做异或运算
            //事实上是用混合后的低位掺杂高位的部分特征，变相保留高位信息作索引
        // 1000000的二进制: 00000000 00001111 01000010 01000000
        // 右移16位：       00000000 00000000 00000000 00001111
        // 异或后:          00000000 00001111 01000010 01001111

        return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);
    }
```  
### 2.2. 一次hash之多进制转换算法
又称`Rabin-Karp`算法,这是对字符串进行hash返回一个数字的算法。当字符串都为小写字母时,一共26个小写字母,需要取进制为26
### 2.3. 其他Hash算法
1. SHA256是构造区块链所用的主要密码哈希函数。无论是区块的头部信息还是交易数据，都使用这个哈希函数去计算相关数据的哈希值，以保证数据的完整性。同时，在比特币系统中，基于寻找给定前缀的SHA256哈希值，设计了工作量证明的共识机制；SHA256也被用于构造比特币地址，即用来识别不同的用户。[^hash1]

[^hash1]:[ [区块链] 加密算法——Hash算法（进阶）](https://www.cnblogs.com/X-knight/p/9136455.html)

## 3. 并发安全
### 3.1. 平常解决[^CHM]
- `HashTable`是直接在操作方法上加synchronized关键字，锁住整个数组，粒度比较大；
- `Collections.synchronizedMap`是使用Collections集合工具的内部类，通过传入Map封装出一个`SynchronizedMap`对象，内部定义了一个对象锁，方法内通过对象锁实现；
- `ConcurrentHashMap`使用分段锁，降低了锁粒度，让并发度大大提高。

ConcurrentHashMap成员变量使用volatile 修饰，通过内存屏障强制“写提交”保证线程可见性，同时免除了指令重排序，另外使用`CAS`操作和`synchronized`结合实现赋值操作，多线程操作只会锁住当前操作索引的节点。
如下图，线程A锁住A节点所在链表，线程B锁住B节点所在链表，操作互不干涉。
![](https://gitee.com/istarwyh/images/raw/master/1600074100_20200914150224213_24914.png)


[^CHM]: [ConcurrentHashMap的分段锁](https://blog.csdn.net/mrchaochao/article/details/106340059?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-7.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-7.channel_param)

## 4. 有序性
### 4.1. LHM
LinkedHashMap定义Entry结点，除了继承HashMap的Node属性，还有`before`和 `after`结点用于标识前置节点和后置节点，可以实现按**插入的顺序**(`LFU`)或**访问顺序**(`LRU`)排序。
```java
    /**
     * HashMap.Node subclass for normal LinkedHashMap entries.
     */
    static class Entry<K,V> extends HashMap.Node<K,V> {
        Entry<K,V> before, after;
        Entry(int hash, K key, V value, Node<K,V> next) {
            super(hash, key, value, next);
        }
    }
```
以Entry结点为底层维护一个单链表，并有头尾结点：
```java
    /**
     * The head (eldest) of the doubly linked list.
     */
    transient LinkedHashMap.Entry<K,V> head;

    /**
     * The tail (youngest) of the doubly linked list.
     */
    transient LinkedHashMap.Entry<K,V> tail;
```
### 4.2. TreeMap
TreeMap则以Key为基础，按照Key的自然顺序或者Comprator（实现Comparable接口，自己实现比较功能）的顺序进行排序，也通过红黑树来实现。
## 5. 去重
### 5.1. HashSet利用HashMap的<key,>不重复去重
HashSet实现不重复的集合，实际底层就是HashMap放弃了`<,value>`部分的实现来实现的（白白浪费空间?）
```java
    public HashSet() {
        map = new HashMap<>();
    }
```
把要添加进HashSet中的元素当做`key`存入，而value则是一个固定Object常量`PRESENT`:
```java
    public boolean add(E e) {
        return map.put(e, PRESENT)==null;
    }

```
对于每一个要插入的key的重复判断依赖于`putVal()`这个算法中一段:
```java
    // 比较现有p与要插入的key
    // 比较数据与对象的hash值
    // 如果是基本数据类型，直接比较值
    // 如果是对象，多态调用equals()方法比较
if (p.hash == hash &&
        ((k = p.key) == key || (key != null && key.equals(k))))
        //局部工具结点e来保存旧的结点信息
    e = p;
    // 如果已经是红黑树结点，即已经转成了红黑树，调用插入红黑树的方法
        // 传入红黑树结点指针
else if (p instanceof TreeNode)
    e = ((TreeNode<K, V>) p).putTreeVal(this, tab, hash, key, value);
...
```

### 5.2. BitMap利用多次hash去重
#### 5.2.1. BitMap简介
BitMap又称布隆过滤器,它其实是一块分配在内存中由连续二进制位组成的数据结构.核心思想是省去`int`这种`32bit`表示一个数值(可以是数字或hash值)带来的内存资源的吃紧,选择将数值`Key`映射到bitMap中,用1个bit位来标记`Key`对应的状态(如`0`-存在,`1`-不存在),或使用2个及以上bit位(`00`-`01`-`10`-`11`,4种状态或更多)。如下图所示将`4`映射到一个`8bit`大小的bitMap中,因为是从0开始,所以将第`5`位置为`1`表示数字`4`已存在:
![](https://gitee.com/istarwyh/images/raw/master/1603965367_20201028210441481_202.png)

#### 5.2.2. bitMap的功能定位
bitMap可以在海量数据集中过滤外部新的重复数据(只专注唯一性,hash后自然不可逆得到除数字外的原数据),应用场景包括

- 亿级url(爬虫url,邮件链接等)的去重
- 短时并发重复请求的查询或防御
    - 秒杀系统,查看用户是否重复购买
    - 解决如Redis缓存击穿问题:黑客攻击服务器时会构建大量不存在于缓存中的key发起请求,利用短时高并发查询请求造成数据库挂机

此外,bitMap也可以

- **不重复元素**的计数排序(因为如果重复是无法表示在一个bitMap中的)
    如将`{4,3,2,5,7}`映射到`8bit`的bitMap中如下图[^编程珠玑]:
![](https://gitee.com/istarwyh/images/raw/master/1603965369_20201028211721048_27136.png)
则最后将**位**为`1`(表示存在)的数输出,即`{2,3,4,5,7}`,从而达到了排序的目的.

[^编程珠玑]:见<编程珠玑>第一章

#### 5.2.3. bitMap帮助多次hash降低冲突概率

##### 5.2.3.1. 多次hash
我们的数据假设是String类型,并假设String的hashcode可能发生重复的概率都是`p`,那么`n`个hash函数同时对一个字符串hash,由这n个hash值(`h1,h2,h3...hn`)组成的**唯一索引**`<h1,h2,h3...hn>`同样的重复概率就是$p^{n}$.至此理想很丰满,不过又来了问题,难道对于每一个需要多次hash的String都要存储它们的`<h1,h2,...hn>`然后每一次比较每一个hash值?
##### 5.2.3.2. 多次hash后的唯一性存储与查询
所以我们要把hash值用映射到bitMap中的方法**再hash**[^hash的本质]一遍,缩小对hash值的存储空间,同时还可以复用一个bitMap--将多个不同hash函数hash得到的值都放入同一个bitMap中.这样对这个唯一索引的存储和查询所耗资源就会尽可能少.不过注意因为bitMap**位数**有限:

- 如果基于数组表示,length属性是32位的有符号整数,即最大$2^{31}$bit == 2G
- 如果基于String,底层是char[],最大也是2G
- 如果基于链表,理论上倒是JVM堆内存的大小,不过不太可能

而`h1,h2,h3...hn`的数值范围可能很超过最大的**位数数目**,因此再hash在bitMap中的**位**本身也会重复,所以最后唯一索引重复概率达不到$p^{n}$.
如果要尽可能降低重复概率,避免误杀,可以增大`n`或者添加白名单.

[^位数有限]:bitMap

[^hash的本质]:hash除了给数据打上标签,还是为了缩小数据本身的变化范围
## 1. HashMap整体认识
HashMap按照组合模式的设计模式进行设计[^组合模式]，顶层接口声明类型，子接口定义功能，抽象类实现部分通用功能，HashMap作为Map子类之一实现自己的方法。接下来实现接口对Map的操作同于Map对于自己子结点的操作。HashMap作为容器，存储叶子结点Node<K,V>.对于叶子结点的内部类又是一个容器，包括了K和V；K和V是泛型，可以由用户自己装填任意类型，最终实现俄罗斯套娃的设计。
HashMap首先要解决的问题是Node的Key应该怎么映射，使得放进去的效率高同时查找的时候效率依然高。首先还是利用取余的分类功能，选取的是hash(key)的二进制尾数，为了保留高位的特征减少碰撞的概率，设计了高位取余算法；其次选取Hash的地址空间和扩容的阈值，初始定为16个桶，并且当达到12个桶的时候，即为了平衡发生碰撞时put元素的平均效率 和 空间很大更少碰撞但是空间紧张 之间的矛盾，默认负载因子为0.75；最后设计Node存在的数据结构形式：采用开地址法，每个Hash桶连接着链表--尾插法，并通过实验选择了当链表长度为8时转化为红黑树，又因为需要防止链表与红黑树在8这个长度处因为增加与删除导致反复转化，引起时间复杂度的震荡，选择红黑树元素存储为6时转化回链表。
而还有一些细节问题需要考虑，能不能提高取余操作本身的效率？因此权衡后强制每次hashmap的大小都是2的合数，这样方便使用位运算完成取余操作。
能不能在resize的时候尽量不要reIndex，因为都再搞一遍真的感觉累。所以找reIndex后的索引与原始Index之间的关系，发现大小都是2的合数时真的可以用更简单的方法完成`(n-1)&hash`操作，总共就两种情况。


[^组合模式]:[组合模式以及HashMap的设计分析（不包括具体实现）](http://www.manongjc.com/detail/17-vpesnqkfshssbzf.html)

## 2. 并发安全
### 2.1. 平常解决[^CHM]
- HashTable是直接在操作方法上加synchronized关键字，锁住整个数组，粒度比较大；
- Collections.synchronizedMap是使用Collections集合工具的内部类，通过传入Map封装出一个SynchronizedMap对象，内部定义了一个对象锁，方法内通过对象锁实现；
- ConcurrentHashMap使用分段锁，降低了锁粒度，让并发度大大提高。

ConcurrentHashMap成员变量使用volatile 修饰，通过内存屏障强制“写提交”保证线程可见性，同时免除了指令重排序，另外使用`CAS`操作和`synchronized`结合实现赋值操作，多线程操作只会锁住当前操作索引的节点。
如下图，线程A锁住A节点所在链表，线程B锁住B节点所在链表，操作互不干涉。
![](https://gitee.com/istarwyh/images/raw/master/1600074100_20200914150224213_24914.png)


[^CHM]: [ConcurrentHashMap的分段锁](https://blog.csdn.net/mrchaochao/article/details/106340059?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-7.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-7.channel_param)

## 3. 有序性
### 3.1. LHM
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
### 3.2. TreeMap
TreeMap则以Key为基础，按照Key的自然顺序或者Comprator（实现Comparable接口，自己实现比较功能）的顺序进行排序，也通过红黑树来实现。
## 4. 去重
### 4.1. HashSet利用HashMap的<key,>不重复去重
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
### 4.2. BitMap利用多次hash去重
#### 4.2.1. BitMap简介
BitMap又称布隆过滤器,它其实是一块分配在内存中由连续二进制位组成的数据结构.它的功能定位是在海量数据集中过滤外部新的重复数据,应用场景包括

- 亿级url(爬虫url,邮件链接等)的去重
- 短时并发重复请求的查询或防御
    - 秒杀系统,查看用户是否重复购买
    - 解决如Redis缓存击穿问题:黑客攻击服务器时会构建大量不存在于缓存中的key发起请求,利用短时高并发查询请求造成数据库挂机
#### 4.2.2. 多次hash降低冲突概率
我们的数据假设是String类型,并假设String的hashcode可能发生重复的概率都是`p`,那么`n`个hash函数同时对一个字符串hash,由这n个hash值组成的**唯一索引**`<h1,h2,h3...hn>`同样的重复概率就是$p^{n}$.至此理想很丰满,不过又来了问题,难道对于每一个需要多次hash的String都要存储它们的`<h1,h2,...hn>`?
所以我们要把hash值用映射到bitMap中的**再hash**[^hash的本质]一遍,缩小对hash值的存储空间,同时还可以复用一个bitMap--将多个不同hash函数hash得到的值都放入同一个bitMap中.这样对这个唯一索引的存储和查询所耗资源就会尽可能少.不过注意因为bitMap**位数**有限,再hash得到hash值本身也会重复,所以最后唯一索引重复概率达不到$p^{n}$.
如果要尽可能降低重复概率,避免误杀,可以增大`n`或者添加白名单.

[^hash的本质]:hash除了给数据打上标签,还是为了缩小数据本身的变化范围
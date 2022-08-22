### 索引
#### MySQL 索引会常驻内存吗?
会.
#### MySQL索引怎么被加载到内存的?
在Linux中,innodb引擎中为`Innodb_buffer_pool`通过`mmap()`直接向操作系统申请内存,每次申请的大小为`innodb_buffer_pool_chunk_size`,最终会申请`innodb_buffer_pool_size`大小的文件映射段动态内存.以上初始化后,`Innodb_buffer_pool`缓存每次查询加载的目录页和数据页,直到`innodb_buffer_pool_size`大小的物理内存占用.直到在线减少`innodb_buffer_pool_size`或是关闭MySQL才会通过`munmap()`方式释放内存给操作系统.[^mysql_memory]
#### 既然常驻内存,那MySQL索引如何被淘汰?
无外乎还是那几个缓存淘汰算法.`Innodb_buffer_pool`的内存主要是通过 Free List、LRU List、FLU List、Unzip LRU List 等 4 个链表来进行管理分配。[^mysql_memory]

- Free List：缓存空闲页

-  LRU  List：缓存数据页

- FLU  List：缓存所有脏页

- Unzip   LRU List：缓存所有解压页

[^mysql_memory]: [ MySQL 内存管理初探
](https://mp.weixin.qq.com/s/jlFueo-WnR3gILR38uzeIg)

#### 为什么选择B+树?
#### B+树和倒排索引本质区别是什么?
#### B+树的分叉是多少?
B+树的每个结点都是一个`16k`大小的页,目录结点同理,我们也称为目录页.目录页的数据结构也一样,通常包括File Header等占用的空间有1k,剩下的15k用于存索引键和页号.页号是固定的4byte大小,假设
索引键是`Bigint`(8byte),那么索引页中的一条数据也就是一种byte,分叉数 = 15k/12b = 1280.
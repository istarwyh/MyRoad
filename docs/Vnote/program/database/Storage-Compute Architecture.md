[技术揭秘：从双11看实时数仓Hologres高可用设计与实践](https://developer.aliyun.com/article/829794https://developer.aliyun.com/article/829794)
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202308192240287.png)

## Shard Nothing
每个计算节点只能自己挂载存储，一个节点只能处理一个分片的数据，不过节点之间可以通信，也有一个汇总节点。这种和通过中间件利用多个MySQL单机组成集群很像。缺点是节点`Failover`之后就需要数据rebalance或者资源不够时同时扩容计算存储造成浪费。
代表：Oceanbase
不共享存储的好处在于，副本可以采用不同的存储格式，主副本行存支持OLTP，某一备副本用列存支持OLAP。这种方案通过引入列存大幅提升了OLAP的能力，但是主副本与备副本之间会额外引入毫秒级的延迟，适合简单的OLTP的场景加上中等的OLAP的场景。
## Shard Storage
计算节点可以像访问单机数据一样访问共享存储（分布式文件系统）。但是计算节点需要引入共识算法保证数据同步和一致性，继而计算节点扩展有一个上限。
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202309171509717.png)

代表：PolarDB

## Shard Disaggregation
想要综合[[Storage-Compute Architecture#Shard Nothing|Share Nothing]]和[[Storage-Compute Architecture#Shard Storage|Share Storage]]的优点，所以把存储和计算都分离了,计算节点之间也别通信了，就处理自己分片数据。
优点：
1. 一致性问题处理比较简单，计算层只需要保证同一时刻有一个计算节点写入同一分片。
2. 扩展灵活 & 计算节点故障恢复快，没有 shared nothing的问题。
缺点：我猜底层数据的一致性就麻烦了吧
Hologres
### Example
- [[Hologres]]
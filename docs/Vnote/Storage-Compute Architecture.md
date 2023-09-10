[技术揭秘：从双11看实时数仓∂Hologres高可用设计与实践](https://developer.aliyun.com/article/829794https://developer.aliyun.com/article/829794)
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202308192240287.png)

## Shard Nothing
每个计算节点只能自己挂载存储，一个节点只能处理一个分片的数据，不过节点之间可以通信，也有一个汇总节点。这种和通过中间件利用多个MySQL单机组成集群很像。缺点是节点`Failover`之后就需要数据rebalance或者资源不够时同时扩容计算存储造成浪费。

## Shard Storage
计算节点可以像访问单机数据一样访问共享存储。但是计算节点需要引入共识算法保证数据同步和一致性，继而计算节点扩展有一个上限。

## Shard Disaggregation
想要综合shared nothing和shared storage的优点，所以把存储和计算都分离了,计算节点之间也别通信了，就处理自己分片数据。
优点：
1. 一致性问题处理比较简单，计算层只需要保证同一时刻有一个计算节点写入同一分片。
2. 扩展灵活 & 计算节点故障恢复快，没有 shared nothing的问题。
缺点：我猜底层数据的一致性就麻烦了吧

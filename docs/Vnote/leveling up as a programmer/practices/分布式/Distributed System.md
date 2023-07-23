现代系统正变得越来越复杂，从单线程到多线程，从单体到微服务，从单节点到分布式，从本地到云端... … 复杂度使得程序产生预期的结果需要越来越多的必要条件，而每种条件都有其自身的成功概率，即使每种条件的成功概率都很高，根据墨菲定律，或早或晚一定会遇到不可预知的结果。

在数据工程领域，这个问题尤其突出，目前大数据领域几乎都是分布式运行的任务、高可用的消息队列、多备份的存储、主从节点等等。在这样的情况下，如何构建稳定、健壮的任务或服务成为了一个挑战。

## 1. 常见对分布式系统的认知误区

[Fallacies of Distributed Systems](https://architecturenotes.co/fallacies-of-distributed-systems/)
![](vx_images/442172210625012.png =600x)
## 2. 组成

系统可以参考[Spring Cloud Alibaba](https://github.com/alibaba/spring-cloud-alibaba/blob/2021.x/Roadmap.md)
此外还有

- 分布式缓存:Redis
- 分布式追踪: EagleEye
- 搜索引擎: ElasticSearch
- 分布式数据库: TiDB、Mysql的分库分#表
- 分布式计算:Flink流批计算、MaxCompute 离线大批量计算
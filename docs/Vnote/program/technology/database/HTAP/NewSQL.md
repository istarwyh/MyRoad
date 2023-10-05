
NewSQL 是一类关系数据库管理系统，旨在为在线事务处理（OLTP）工作负载提供 NoSQL 系统的可扩展性，同时保持传统数据库系统的 ACID 保证。NewSQL用SQL来代指数据库,然后并不知道给这些新型的数据库起什么名字,就起了个NewSQL这样的名字。

MySQL的没落是必然的,因为数据一直领先于硬件和分库分表这样的应用层工具在增长,以及存储和计算资源的弹性都呼唤着NewSQL.
NewSOL将数据库的计算层和存储层分开,这样就可以通过编排在云中进行横向扩展. 不过没有银弹,[以 TiDB 为例](https://mp.weixin.qq.com/s/qtIo7cAdSJ_9V-RFCJH8PQ)：

>TiDB 的自增 ID 只能保证单个 TiKV 上的自增，并不能保证全局自增。
由于 TiKV 存储是根据 key 的二进制顺序排列的，使用自增 ID 可能会造成热块效应。
TiDB 默认 RC（读已提交）的事务隔离级别，并且不支持 RR（可重复读）隔离级别，虽然提供了基本等价于RR的SI（Snapshot Isolation），但还是存在写偏斜问题
TiDB 的点查（select point）性能比 MySQL 要差不少，在几个亿级别的数据量才能勉强和 MySQL 打平。
因为底层基于 Raft 协议做数据同步，所以 TiDB 数据同步 延迟会比 MySQL 要高。
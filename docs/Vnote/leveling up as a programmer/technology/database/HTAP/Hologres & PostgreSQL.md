What is the difference between PostgreSQL and Hologres in Alibaba Cloud?

之所以这样问，是因为Hologres 兼容大部分PostgreSQL的语法，以及报错堆栈也与之相像。实际上Hologres这个词是holographic和Postgres的组合，Postgres指的是兼容PostgreSQL生态，holographic指的是全部信息，代表官方希望Hologres对数据进行全息的分析(HSAP, )，并且能够兼容PostgreSQL生态[^hologres]。

[^hologres]: https://developer.aliyun.com/article/778789

Hologres 相比PostgreSQL 主要做了以下创新：
- 在tablet层面同时支持了row tablet 和 column tablet,并且是双份而不是raft同步
- 所有查询都在内存中完成,哪怕OOM
- 引入了bitmap、dictionary encoding 提高查询速度

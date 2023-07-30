What is the difference between PostgreSQL and Hologres in Alibaba Cloud?

之所以这样问，是因为Hologres 兼容大部分PostgreSQL的语法，以及报错也与之相像。Hologres主要做了以下创新：
- 在tablet层面同时支持了row tablet 和 column tablet,并且是双份而不是raft同步
- 所有查询都在内存中完成,哪怕OOM
- 引入了bitmap、dictionary encoding 提高查询速度
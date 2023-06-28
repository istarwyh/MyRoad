## 1. NoSQL的诞生:

- 关系数据库难以应付复杂数据,关系模式即表的结构会变得十分复杂
- 大数据可能并不需要ACID,比如转发的微博,可以异步/错误/过时清楚
- 底层是B树/B+树,降低树的高度避免IO,但查找性能值得探讨

## 2. 类比于ACID:

－　CAP定理
－　Base理论，基本可用，最终一致性

## 3. 底层存储
LSM树 --Log Structured  Message,牺牲了读性能,优化了写性能,一个标志就是没有update/delete语句
![](_v_images/20200419110201266_6950.png)
日志式数据:

- 增量式写入
- 线性(返回式)查找

读操作比写操作更容易优化:改进算法, 加过滤器(牺牲准确率)
把随机IO变成顺序IO,把很多东西变成日志式查找.
## 4. InfluxDB简介
| InfluxDB | MySQL | note
| --- | --- | --- |
| Database | Database | curl
| Measurement | Table |
| Filed(key-value) | Column |
| Tag(key-value) | Column |
| Timestamp | 类比ID |第一列永远是Timestamp
| Retention Policy|  |存储策略,比如存多久

Series: 共享同一个retention policy,measurement以及tag set的数据集合,与field无关
Point: 同一个Series中具有相同时间的Field set(相当于一列?)

Filed与Tag的区别:时序数据库每一个measurement都是以时间作为索引,Mysql可能是ID;之所以要建立索引，其实就是为了构建一种数据结构，可以在上面应用一种高效的查询算法，最终提高数据的查询速度。

- Filed必须要有,但没有索引
- Tag可以没有,**但有索引**,
- Filed Key = Filed Set, Tag Key=Tag Set


![](_v_images/20200419112118091_14325.png)
## 5. 命令
SELECT <funtion>(<field_key>) FROM_clause WHERE <time_range> GROUP BY time()
可以用来查询某个时间段范围内time()函数所给出的时间限制内的数据
INSERT <measurement>,<tag_sets><field_sets>,自动打时间戳
持续查询,定期执行查询
CREATE CONTINUOUS QUERY <cq_name> ON <database_name> BEGIN <cq_query> END
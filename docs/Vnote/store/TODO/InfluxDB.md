
## 1. InfluxDB简介
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


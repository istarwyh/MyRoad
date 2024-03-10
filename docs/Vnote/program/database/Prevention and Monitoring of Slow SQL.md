
## 预防慢SQL
### 常见⚠️事项
- 新表上线一定review 有没有加索引
- 因为最左匹配，优先区分度最高的字段在索引最前面
- `not in`、`!=` 、`is not null`等索引无法优化
- `in ('0','1')` => `id_no ='0' or id_no='1'` =>`or策略`,会创建临时表自己建立索引而不会利用即使已经有的索引[^SQL优化]。因此已建立的索引应尽量避免`or策略`，可以分别条件过滤再join

- 索引字段类型和查询类型要匹配，比如id 是varchar,就不要用long 去查。否则会触发隐式类型转换
- 传统行数据库中，where子句中对**列**的任何操作结果都是在SQL运行时**逐列计算**得到的，因此如果where子句包括函数
	- 它将不得不进行表遍历，而无法使用该列上面的索引
	- 对索引字段做函数操作，可能会破坏索引值的有序性，因此优化器放弃走索引[^敖丙调优]
- `group by name`,实际上这既包括了聚合，也默认了排序操作
    - 如果只是对数据做过滤而不需要排序，需要指定不需要排序操作：`group by name order by null`
- 离线拉取数据尽量只选备库
- 单机数据库因为可以通过自增主键强一致保障数据写入的先后性，所以查询时按照索引顺序排列查询更优
### 分页查询
不要用分页处理全量数据！用迭代器模式（如id > xxx 去逐步fetch 处理）
- 子查询法:先取索引,再查询索引范围内的数据
	1. 取索引：`x = (SELECT id FROM table ORDER BY id LIMIT 20000,1)`
	2. 和结果相关的索引起点拿到后回表去取索引范围内的数据：

```sql
SELECT c1,c2,cn... 
FROM table 
WHERE
id >=/in/inner join x
LIMIT 10;

```
当r然两步可以合成一步:
```sql
SELECT c1,c2,cn... 
FROM table A 
JOIN (SELECT PK FROM table WHERE column = ... ORDER BY PK LIMIT 20000,10) B 
ON A.PK = B.PK
```

### 尽量不回表
- 读操作时,在代码里面，可以规范使用`Factory`专门去取id,之后再在`Repository`取由id索引的数据。当然直接写SQL在数据库层面运行是性能最优的，只是维护与扩展性较差。
- 组合条件查询大于5个，考虑换成倒排索引。因为这么多条件很难走到索引覆盖或者走到了也没用。
### 批量插入
通常因为数据库连接和网络传输的开销，批量插入是比单条更快的。不过

1. 批量插入未必总是比单条插入效率高
2. 注意数据的条数，还要注意一条数据的大小
    - 比如一条记录的数据量有1M，10条记录的数据量就10M，这时一次插100条，100M数据
    - 数据库有个参数`max_allowed_packet`，也就是每一个包（sql）命令大小，默认是1M，那么sql的长度大于1M就会报错。
3. 批量插入并不是越快越好, 数据库分读写，有集群，这就意味着，需要同步！！！如果有分库分表分区的情况，如果短时间内插入的数据量太大,读写数据不一致的情况在所难免
## 排查慢SQL
看监控：对[[Common Metrics of MySQL]] 要心理有数

看日志：如果是TDDL，可以在`/home/admin/logs/tddl/tddl-slow.log.*`里面查看

看慢SQL的执行计划:`explain SQL语句`[^explain]

![](https://gitee.com/istarwyh/images/raw/master/vnote/thinking/公开信息/慢sql优化.md/2047303415995.png)

看**物理库**已经建立的索引:`show create table xxx表名_0000`

[^SQL优化]:[如何让你的SQL运行得更快](https://blog.csdn.net/gprime/article/details/1687930)
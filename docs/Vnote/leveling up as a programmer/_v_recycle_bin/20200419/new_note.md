# 1. 时间序列数据库调研之InfluxDB
基于 Go 语言开发，社区非常活跃，项目更新速度很快，日新月异，关注度高。<br />
## 1.1. InfluxDB入门
<br />

<a name="039d392c"></a>
### 1.1.1. 安装部署


```
wget https://dl.influxdata.com/influxdb/releases/influxdb-1.0.0_beta2.x86_64.rpm
sudo yum localinstall influxdb-1.0.0_beta2.x86_64.rpm
```

<br />配置文件路径为 `/etc/influxdb/influxdb.conf`，修改后启动服务<br />

```
sudo service influxdb start
```


<a name="b4d3c72e"></a>
### 1.1.2. 特点


- 可以设置metric的保存时间。
- 支持通过条件过滤以及正则表达式删除数据。
- 支持类似 sql 的语法。
- 可以设置数据在集群中的副本数。
- 支持定期采样数据，写入另外的measurement，方便分粒度存储数据。



<a name="b59c9e0f"></a>
### 1.1.3. 概念


<a name="1f4190bc"></a>
#### 1.1.3.1. 数据格式 Line Protocol


```bash
measurement[,tag_key1=tag_value1...] field_key=field_value[,field_key2=field_value2] [timestamp]

cpu_load,host_id=1 value=0.1 1434055562000000000
```

<br />相比于 JSON 格式，无需序列化，更加高效。<br />
#### 1.1.3.2. 基本描述
| InfluxDB | MySQL | note
| --- | --- | --- |
| Database | Database | 例如 cpu_load
| Measurement | Table |
| Filed(key-value) | Column |
| Tag(key-value) | Column |
| Timestamp | 类比ID |第一列永远是Timestamp

时序数据库每一个measurement都是以`时间`作为索引,Mysql一般是`ID`.

- field-key, field-value: 通常用来存储数据，类似 `opentsdb `中的 value=0.6，但是支持各种类型，数据存储时**不会进行索引**.每条数据必须拥有一个 field-key，如果使用 field-key 进行过滤，需要遍历一遍所有数据。
- tags-key, tags-value: 和 field-key 类似，但是**会进行索引**，方便查询时用于过滤条件。
- Point: 同一个Series中具有相同时间的Field set(相当于一列?)
- Series: 共享同一个retention policy,measurement以及tag set的数据集合,与field无关
    - 假设 cpu_load 有两个 tags，host_id 和 name，host_id 的数量为 100，name 的数量为 200，则 series 基数为 100 * 200 = 20000。

![](_v_images/20200419160417583_17967.png)

<a name="1209e34a"></a>
#### 1.1.3.3. 数据存储

- measurements, tag keys, field keys，tag values 全局存一份。
- field values 和 timestamps 每条数据存一份。

**存储底层**
LSM树 --Log Structured  Message,牺牲了读性能,优化了写性能,把随机IO变成顺序IO.一个标志就是有SELECT/INSERT语句,没有update/delete语句
![](_v_images/20200419155450801_31256.png)
主要是针对日志式数据:

- 增量式写入
- 线性(返回式)查找

<a name="2c4a6578"></a>
#### 1.1.3.4. Retention Policy

<br />保留策略包括设置数据保存的时间以及在集群中的副本个数。<br />
<br />默认的 RP 为 **default**，保存时间不限制，副本个数为 1，默认 RP 是可以修改的，并且我们可以创建新的 RP。<br />

<a name="8afeaf0e"></a>
#### 1.1.3.5. Continuous Query

<br />CQ 是预先配置好的一些查询命令，**SELECT** 语句必须包含 **GROUP BY time()**，influxdb 会定期自动执行这些命令并将查询结果写入指定的另外的 measurement 中。<br />
<br />利用这个特性并结合 RP 我们可以方便地保存不同粒度的数据，根据数据粒度的不同设置不同的保存时间，这样不仅节约了存储空间，而且加速了时间间隔较长的数据查询效率，避免查询时再进行聚合计算。<br />
代码示例:
```sql
CREATE CONTINUOUS QUERY <cq_name> ON <database_name> BEGIN <cq_query> END
```
<a name="Shard"></a>
#### 1.1.3.6. Shard

<br />Shard 这个概念并不对普通用户开放，实际上是 InfluxDB 将连续一段时间内的数据作为一个 shard 存储，根据数据保存策略来决定，通常是保存1天或者7天的数据。例如如果保存策略 RP 是无限制的话，shard 将会保存7天的数据。这样方便之后的删除操作，直接关闭下层对应的一个数据库即可。<br />

<a name="3f113298"></a>
#### 1.1.3.7. 存储引擎


1. LevelDB
2. BoltDB
3. TSM Tree

<a name="API"></a>
### 1.1.4. API

<br />通过 HTTP 访问 influxdb。<br />
<br />语法上是一种类似于 SQL 的命令，官方称为 InfluxQL。<br />

<a name="157d5306"></a>
#### 1.1.4.1. 创建数据库


```bash
curl -POST http://localhost:8086/query --data-urlencode "q=CREATE DATABASE mydb"
```


<a name="131d2cc4"></a>
#### 1.1.4.2. 插入数据
`INSERT <measurement>,<tag_sets><field_sets>`自动打时间戳

```bash
curl -i -XPOST 'http://localhost:8086/write?db=mydb' --data-binary 'cpu_load_short,host=server01,region=us-west value=0.64 1434055562000000000'
```

<br />cpu_load_short 是 measurement，host 和 region 是 tags-key，value 是 field-key。<br />
<br />多条数据时，用换行区分每条数据<br />

```bash
curl -i -XPOST 'http://localhost:8086/write?db=mydb' --data-binary 'cpu_load_short,host=server02 value=0.67
cpu_load_short,host=server02,region=us-west value=0.55 1422568543702900257
cpu_load_short,direction=in,host=server01,region=us-west value=2.0 1422568543702900257'
```

<a name="41b02feb"></a>
#### 1.1.4.3. 读取数据
限制内的数据

```bash
curl -GET 'http://localhost:8086/query' --data-urlencode "db=mydb" --data-urlencode "epoch=s" --data-urlencode "q=SELECT value FROM cpu_load_short WHERE region='us-west'"
```

<br />同时查询多条数据时，以分号分隔<br />

```bash
curl -G 'http://localhost:8086/query' --data-urlencode "db=mydb" --data-urlencode "epoch=s" --data-urlencode "q=SELECT value FROM cpu_load_short WHERE region='us-west';SELECT count(value) FROM cpu_load_short WHERE region='us-west'"
```

<br />这里 `--data-urlencode "epoch=s"` 会使返回的时间戳为 unix 时间戳格式。<br />

<a name="Qyv7V"></a>
- 可以用来查询某个时间段范围内time()函数所给出的时间
`SELECT <funtion>(<field_key>) FROM_clause WHERE <time_range> GROUP BY time()`
## 1.2. 导入netman/Omni数据
1. 首先需要在Omni的数据中添加属性名以及每分钟的时间戳，构成一个csv文件，然后利用这个[工具](https://github.com/fabio-miranda/csv-to-influxdb)把数据导入到influxdb中
2. 整理Omni数据的脚本如下：

```python
import os

import datetime
in_date = '2016-08-31 00:00:00'
dt = datetime.datetime.strptime(in_date, "%Y-%m-%d %H:%M:%S")

def get_time_str(minute):
    return (dt + datetime.timedelta(minutes=minute)).strftime('%Y-%m-%d %H:%M:%S')

f = open("machine-1-1.txt")
lines = f.readlines()

newlines = []

m = 0

newlines.append("timestamp,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23,p24,p25,p26,p27,p28,p29,p30,p31,p32,p33,p34,p35,p36,p37,p38\n")

for line in lines[]:
    if len(line)<=1:
        break
    newlines.append(get_time_str(m)+','+line)
    m+=1

f.close()

f = open("new_data.csv",'x')

f.writelines(newlines)

f.close()
```

3. 执行导入工具
```bash
python csv-to-influxdb.py --dbname netmantest --input new_data.csv \
--fieldcolumns p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23,p24,p25,p26,p27,p28,p29,p30,p31,p32,p33,p34,p35,p36,p37,p38
```

4. 注意导入之前需要提前创建netmantest数据库，导入完成后即可通过相应http接口得到数据<br />

### 1.2.1. 下一步改进
将不同的machine id加入作为记录的tag

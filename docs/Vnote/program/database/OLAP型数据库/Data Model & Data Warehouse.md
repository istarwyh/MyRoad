
## 什么数据模型流行？
[[Data Model Confusion]]
## 数据仓库

MPP数据库（Massively Parallel Processing Database），即为大规模并行处理分布式数据库。它使用一组服务器来处理大量数据的并行分区和计算。MPP DB为大规模数据提供更高的速度、更好的可伸缩性和更好的容错性，是数据仓库的实现基础。
## 实时数仓
- 轻松存储过亿条数据的数据库
- 支持批量和流式计算的计算框架
### 1. 数据层
- ODS: Operational Data Store
- DWD: Data Warehouse Detail
- DWS: Data Warehouse Summary
- ADS: Application Data Service

OLAP注重查询,但是依赖于OLTP产生数据;OLTP注重事务吞吐量.
批处理改为流式处理后,处理实时数据时,通常是用离线数仓merge实时链路数据,比如用Flink导入binlog或kafka的数据作为增量,与Hive等离线数据平台计算后的批量离线数据进行merge从而得到大数据的实时结果.
### 2. Flink计算框架
[Flink 介绍](https://mp.weixin.qq.com/s/3rQURkPEUjdzc8mxVorXOA)
### 案例
[美团点评数仓](https://mp.weixin.qq.com/s/JT6BZzsAM8D8p9F99VmeFw)
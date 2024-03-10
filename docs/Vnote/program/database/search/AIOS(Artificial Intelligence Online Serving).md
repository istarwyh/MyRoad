## 综述
在阿里内部，Tisplus是一个搜索Paas产品，引擎内核是HA3,索引内核是Indexlib,索引构建依赖BuildService系统，数据处理依赖SARO平台。而这一张套被统称为[AIOS](https://developer.aliyun.com/article/650130)（Artificial Intelligence Online Serving）,智能引擎系统。

AIOS 分为数据源聚合（dump)、全量/增量/实时索引构建及在线服务等部分，以Tisplus为入口经由Bahamut(Maat进行工作流调度) -> Flink -> HDFS/Swift -> BuildService -> HA3 -> Vipserver 等组件对用户提供高可用、高性能的搜索服务。[^aiosinali]

[^AIOSINALI]: [10年+，阿里沉淀出怎样的搜索引擎?]( https://developer.aliyun.com/article/719158)

## AIOS 包含与涉及的组件极简介绍Î
[[AIOS Simple Process.canvas|AIOS Simple Process]]
- SARO (Search, Advertisement and Recommendation Offline):引入MaxCompute、TDDL等数据源来产出HA3全量Dump和增量Swift数据
	- Airflow:SARO上配置定时任务的调度平台
- Build Service:从HDFS和Swift中拉取HA3格式数据构建索引，详见 [[Build Service Process ]]
	- Processor:对原始文档的文本处理，包括业务逻辑对字段内容的改写与分词
	- Builder:将原始文档转化，产出索引格式的数据
	- Merger:将Builder产出的索引数据作合并,产出为最终被Searcher加载的索引文件
- Indexlib:Build Service实际构建索引依赖的服务
- Swift:承接HA3 全量和实时doc的消息中间件
- HA3:后端开发者能感知到的搜索引擎产品
	- Qrs:接受用户query的中间层，负责查询优化并分发到实际执行该query的searcher,召回结果后合并、过滤返回给用户。
	- searcher:每一份的partition接受query并负责对索引进行查询、统计、过滤、排序之后再将结果返回给协调节点(Qrs)
		- partition:列，通常会将索引文件分开到几台机器存储 (比如100G一份），机器的台数就是partition,类似于[[Horizontal Scaling|水平分表]]
- summary:允许通过`doc_id`找到需要返回的字段，包括做TopK这种统计以及聚合等服务。summary使用的索引可以很多样，视业务需要配置summary。
- Hippo:HA3和Build Service实际部署的机器所属的管控平台
- Tisplus: HA3集群托管的管控平台，可以通过它找到日志和监控


## 流程追踪
一个完整的全量+增量过程会产生一个`generation_id`,该generation 会经历process full -> builder full -> merger full -> process inc -> builder inc -> merger inc 的过程，其中处于inc过程时，builder inc 和  merger inc 会交替出现。通过`generation_id` 可以在监控系统中追踪这整个过程。
## 1. 成熟的开源产品[^成熟]
Metis 是智能运维领域的**首个**开源产品，旨在通过算法从海量运维数据中学习摸索规则，逐步降低对人指定规则的依赖，减少人为失误。目前，Metis在运维**质量、效率、成本**三个方面逐步构建出**成熟的**智能化运维场景.
## 2. 应用场景多,潜力巨大[^织云]
- **1. 业务高质量保障**
利用机器学习技术，进行异常检测、故障定位、瓶颈分析等，可在无人工干预下，智能地保障业务高质量运行。场景有:
    - 时间序列异常检测
    - DLP 生死指标监控
    - 多维下钻
    - 关联分析
    - ROOT 根源分析
- **2. 运维效率提升**
利用自然语言处理、机器学习技术，深挖智能问答、智能变更、智能决策，显著地提升运维人员的效率。场景有:
    - 智能客服机器人
    - 舆情监控
    - 智能负载均衡
    - 数据库调参
    - 极限调度等场景
- **3. 成本优化管理**
利用大数据智能分析技术，进行资源（设备、带宽、存储）管理，可迅速分析资源使用的明细，并通过横向大数据对比挖掘可优化点。
- 此外还有**智能检测**、**通用模型**和**规则学习**,潜力巨大
## 3. 数据库层
metis没有DB层,只提供了/app/dao的数据库实例层.**存储层主要依赖于django的orm模型**,将数据抽象为对象存储,从外面调数据.如netman里的时间序列的数据传进metis.
我们数据持久化的工作相当于做一个缓冲层(集成进一个时间序列数据库)来对数据存储并处理,在此基础上看了下api的接口,metis的api不多,接口请求参数或传入参数是python字典或结构体这样.
或者另外的手段就是将netman的算法放到学件提供的算法模块中,将netman的时间序列数据直接传进metis.
## 4. OmniAnomaly数据集
它通过建模随机变量的时间相关性对多元时间序列进行异常检测.
OmniAnomaly是一个随机的递归神经网络模型，它将门控递归单元（GRU）和变分自动编码器（VAE）粘合在一起，其核心思想是学习**多元**时间序列的**正态分布**，并利用重构概率进行异常判断。
数据集包括三个部分:
- NASA的土壤水气主动无源卫星SMAP,以及火星科学实验室的漫游者号
- 互联网公司的五周长数据集SMD(实际有的)
SMD来自20台机器,也就有28个子集,维度为38,总异常率4.16%,相比NASA的数据集低了超过60%.SMD分为50%的训练集和50%的测试集,
![](https://gitee.com/istarwyh/images/raw/master/1586063189_20200331103323270_23224.png)
测试集中提供测试标签标志异常:单/多时间序列异常检测,
提供维度解释标签标志不同维度对异常判断的贡献:多维数据的根因定位

一组指标作为一个向量,重构概率越高越好.
### 4.1. **So:**
算法如何调用?接口在哪?
各个维度是什么?能对接吗?
- Metis
    - 当前检测时间窗口选取为3小时，每分钟1个数据点，即窗口值为180
    - 2、率值检测：适用于正态分布类型数据的检测，使用无监督算法进行检测，如成功率等生死指标数据的检测----源数据
    - 1、量值检测：适用于大多数KPI指标数据的检测，使用无监督o和有监督联合检测，会加载检测模型----tset_labels
## 5. main()
1. 数据准备
调用`utils`中的get_data()
- 传入数据集,最大训练大小,最大测试大小,是否打印日志,是否做预处理,训练集与测试集的起始位置
- 返回`(train_data, None), (test_data, test_label)`
2. 调用构造函数` model = OmniAnomaly(config=config, name="model")`     
    - 传入the model under `variable_scope` named 'model'
    - 返回一个model
3. 构造训练者
```python
   trainer = Trainer(model=model,
                          model_vs=model_vs,
                          max_epoch=config.max_epoch,
                          batch_size=config.batch_size,
                          valid_batch_size=config.test_batch_size,
                          initial_lr=config.initial_lr,
                          lr_anneal_epochs=config.lr_anneal_epoch_freq,
                          lr_anneal_factor=config.lr_anneal_factor,
                          grad_clip_norm=config.gradient_clip_norm,
                          valid_step_freq=config.valid_step_freq)
```
4. 构造预测者
```python
predictor = Predictor(model, batch_size=config.batch_size, n_z=config.test_n_z,
                              last_point_only=True)
```
5. 存储来自`save_dir`的变量
6. 训练模型
```py
     train_start = time.time()
                best_valid_metrics = trainer.fit(x_train)
                train_time = (time.time() - train_start) / config.max_epoch
                best_valid_metrics.update({
                    'train_time': train_time
                })
```
7. 得到来自POT算法的训练集结果
8. 解析`argumens`
9. 处理得到的结果对象并预览

## 6. spot.py

- SPOT类:在单变量数据集上运行SPOT算法(upper bounds)
    1. 定义SPOT对象
    2. 给SPOT对象提供数据包括添加数据`def add(self, data):`
    3. 初始化
    4. def _rootsFinder(fun, jac, bounds, npoints, method):`Find possible roots of a scalar function`  
  
    5. def _log_likelihood(Y, gamma, sigma):`Compute the log-likelihood for the Generalized Pareto Distribution (μ=0)`
        - 寻找所有可能的根
    6. def _grimshaw(self, epsilon=1e-8, n_points=10):` Compute the GPD parameters estimation with the Grimshaw's trick`
    7. def _quantile(self, gamma, sigma):` Compute the quantile at level 1-q`
    8. 根据运行的结果画图
- biSPOT类:在单变量数据集上运行biSPOT算法(upper and lower bounds)
- dSPOT类:在单变量数据集上运行DSPOT算法(upper bounds)
- bidSPOT类:在单变量数据集运行DSPOT算法(upper and lower bounds)

**学习思路**
![微信图片_20200512095727](https://gitee.com/istarwyh/images/raw/master/1589852213_20200512100014351_27868.jpg =600x)
## 7. spot.py

- SPOT类:在单变量数据集上运行SPOT算法(upper bounds)
    1. 定义SPOT对象
    2. 给SPOT对象提供数据包括添加数据`def add(self, data):`
    3. 初始化
    4. def _rootsFinder(fun, jac, bounds, npoints, method):`Find possible roots of a scalar function`  
  
    5. def _log_likelihood(Y, gamma, sigma):`Compute the log-likelihood for the Generalized Pareto Distribution (μ=0)`
        - 寻找所有可能的根
    6. def _grimshaw(self, epsilon=1e-8, n_points=10):` Compute the GPD parameters estimation with the Grimshaw's trick`
    7. def _quantile(self, gamma, sigma):` Compute the quantile at level 1-q`
    8. 根据运行的结果画图
- biSPOT类:在单变量数据集上运行biSPOT算法(upper and lower bounds)
- dSPOT类:在单变量数据集上运行DSPOT算法(upper bounds)
- bidSPOT类:在单变量数据集运行DSPOT算法(upper and lower bounds)

主要是功能二次封装(如apeend())寻根-拟合树(日志相似性)-剪枝(GPD参数)-计算得到结果-绘图
有个问题就是对于Omni所使用算法的学习的确有些力不从心,于是关注Metis的一些问题.
## 8. Metis某些问题

于是找Metis自己训练模型与数据的接口,不过完整的接口文档暂时没有找到.
1. metis以过去一周/过去一天/过去180min这样的形式去判断是否为异常点有理论支撑么？
进行时间序列异常检测是需要使用历史数据的。一般情况下可以选择历史14天的数据，或者历史一周的数据。或者选择其中的一个时间序列切片，正如这个项目所示。
关于有监督方法或者特征工程，
- 开源工具：[tsfresh](https://tsfresh.readthedocs.io/en/latest/)，tsfresh is a python package. It automatically calculates a large number of time series characteristics, the so called features. Further the package contains methods to evaluate the explaining power and importance of such characteristics for regression or classification tasks.

- 论文：清华大学裴丹教授在2015年发表的 Opprentice 系统
- blog：
[时间序列简介（一）](https://zhuanlan.zhihu.com/p/32584136)
[如何理解时间序列？— 从 Riemann 积分和 Lebesgue 积分谈起](https://zhuanlan.zhihu.com/p/34407471)
[时间序列的自回归模型—从线性代数的角度来看](https://zhuanlan.zhihu.com/p/35093835)
做时间序列的特征与做推荐系统的特征有类似之处，更多的是结合自身的业务场景，针对自身的时间序列走势和趋势，甚至对正负样本的理解，在通用特征的基础上，做一些贴近业务的特征。在数据选择方面，可以根据自身的需求选择最合适的数据。

2. 单节点的Metis异常检测学件能支持多少条时序曲线的并发？
## 9. 解决思路
**(Kafka)->InfluxDB->Metis**
多指标的数据应该都可以使用Omni算法,单指标的数据也可以,只是基于Meits自己的规约不好改造,接口文档也缺失,所以转向对集成数据库的研究与应用比较靠谱.
## 10. 转向InfluxDB
1. 自己暂停了Omni算法的研究:多指标;缺少文档;较为陌生.开会后转向看InfluxDB,kafka暂时没接触
2. 尝试跑了一下InfluxDB,在之前基础上完善了一下文档:https://www.yuque.com/uuavi8/mc67cr/utkeib
3. [官方说明](https://github.com/Tencent/Metis/issues/2):内部用的moniter时序存储，支持200w以上指标。monitor存储方案本质上是分布式的kv存储，开源的方案有hbase，influxdb等。在内部使用自研的存储方案基于多阶hash表实现，并做了冷热数据分离。2天内的热数据存储在内存，2天前的冷数据转存到文件。应用于智能检测时为提升检测性能，每天定时将历史数据从冷数据存储批量拉入执行检测的机器的内存中，实时的时序数据每分钟从分布式内存存储中拉取，从而实现基于内存数据的高并发异常检测。
4. Metis使用 `sklearn + tsfresh `从中选了一些比较简单的特征
    - [tsfresh](https://tsfresh.readthedocs.io/en/latest/):一个时间序列特征提取的库
    - 有一些是 Opprentice 里使用了一些拟合特征
    - 有一些是结合自身业务所写的同环比类特征，其函数是 time_series_periodic_features
    - 有监督学习模型使用的是开源的 GBDT 和 XGBoost;量值检测：是无监督(从未知认识,聚类) + 有监督(给定输入输出,分类)；率值检测：是无监督
   在有监督算法前加上统计判别算法和无监督算法进行数据的过滤，只有当第一层输出异常的时候，才会给有监督算法进行判断

## 11. detect_server
刘雪瑶是Features
王嘉程和张锡新:
detect的上层调用模块-service
算法流程:
6个方法,生成一个训练模型,是否能用XGBoost,多线程调用一下是否能用这个方法,有一个训练集的模型,量值/率值用来筛选
在多指标基础上划出来一个单指标集.
## 12. Kafka与Meits冷热数据
Kafka在设计之初作为日志流平台和运营消息管道平台,作为消息中间件它可实现消息顺序和海量堆积能力.其典型的使用场景如下:

- 数据管道和系统解耦。
- 异步处理和事件驱动。
- 流量削峰。
- 事务消息和分布式事务的最终一致性。
### 12.1. 组件概念
- `Broker`: 缓存代理,Kafka集群中一台或多台服务器
- `Topic`:区分Kafka的消息类型,将不同的消息写到不同的`Kafka Topic`中
- `Partition`:Topic物理上的分组,一个Topic可以分为多个`Partition`,每个Partition都是一个有序的队列.Partition中的每条消息都会被分配一个有序的id(offset)
- `Producer`:消息生产者
- `Consumer`:消息消费者
- `Consumer Group`: 每一组消费者都有相同的`Group id`,这样可以保证多个消费者*不重复消费*同一条消息
### 12.2. 顺序读写
Kafka数据的写入和读取是顺序的。根据局部性原理，在实际测试中，磁盘顺序写入和随机写入的性能比相差最大可达6000倍[^冷热数据分离方案].Kafka之所以能做到如此,主要是
1. 把磁盘中的数据缓存到内存中，把对磁盘的访问变为对内存的访问,同时这种`Page Cache`中的数据会按照一定的策略更新到磁盘。
2. 将数据**直接**从磁盘文件复制到网卡设备中，而不需要经由应用程序之手。从而大大减少内核和用户模式之间的上下文切换,这种零拷贝技术依赖于底层的 sendfile() 方法实现。
### 12.3. NIO网络模型
Kafka顺序读写磁盘,使用**NIO网络模型**支持数千个客户端同时读/写.传统的 BIO 通过`socket.read()`读取服务端的数据，如果`TCP RecvBuffer`里没有数据，函数会一直阻塞，直到收到数据。当有很多客户端访问服务端，但客户端未发送数据，服务端线程处于线程阻塞，等待客户端响应,在大量并发的情况下，可能会导致服务端崩溃。所以诞生了NIO网络模型:
![watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MzAzNDk1,size_16,color_FFFFFF,t_70](https://gitee.com/istarwyh/images/raw/master/1589852215_20200512100414255_30982.png)>selector：负责管理与客户端建立的多个连接，负责监听注册到上面的一些事件，如有新连接接入、当前连接上有可读消息或可写消息。一旦事件被其监听到，就会调用对应的事件处理器来完成对事件的响应


- 非阻塞式I/O模型。（服务器端提供一个单线程的Selector来统一管理客户端socket接入的所有连接，并负责每个连接所关心的事件）
- 弹性伸缩能力强。（服务器端不再是通过多个线程来处理请求，而是通过一个线程来处理所有请求，对应关系变为 N:1）
- 单线程节省资源。（线程频繁创建、销毁，线程之间的上下文切换等带来的资源消耗得要缓解）
### 12.4. kafka之于Metis
[官方诚言](https://github.com/Tencent/Metis/issues/25):
>算法的角度来讲，最理想的是 14 天的全部数据，但是在百万时间序列的量级上，就要考虑窗口的长度了。

所以事实上`Metis`考虑到线上的计算压力,也不得不对数据做了较多规约.目前实现分布式的Metis似乎还没苗头.为了实现高并发异常检测，Metis在内部使用*自研的存储方案*,基于内存将实时的时序数据每分钟从分布式内存存储中拉取,又因为内存有限,从而以两天为界,做了内存热数据和磁盘冷数据划分,并定时将历史数据从冷数据存储批量拉入执行检测的机器的内存中.Metis内部的做法其实与Kafka不谋而合.Kakfa据消费的实时性将消息消费者行为划分两类[^冷热数据分离方案]：

- 实时消费者：对数据实时性要求较高，需要采用实时消费消息的方式。在实时消费的场景下，Kafka会利用系统的`page cache`缓存，生产消息到`broker`，然后直接从内存转发给实时消费者，磁盘压力为零。
- 离线消费者：又名定时周期性消费者，消费的消息通常是数分钟前或是数小时前的消息。而这类消息通常存储在磁盘中，消费时会触发磁盘的IO操作。

这实际上对应的就是热读与冷读流量.所以**InfluxDB->(Kafka)->Metis**可行.
对于传统的电商业务,实际运行中发现流量峰值时Kafka集群IO压力会很大，扩容也只能暂时解决问题。理论上集群的读压力不应该这么大，因为大部分的读压力应该命中`Page Cache`，不应该再从磁盘里面读取。但因为客户存在多个业务消费同一份消息的业务场景,冷读与热读并发消费同一个集群,就会使得落盘数据和实时数据会频繁的换入换出内存,增加实时业务的响应时延;同时磁盘IO的高触发也会使得集群不稳定.为了缓解传统业务集群的IO压力占比最大的磁盘读压力,还需要做冷热数据连接方案,即加入`Connector`集群.其将离线业务消息按照主题维度从实时集群同步到离线集群中，和实时消费者保持一致,在流量高峰时也保证冷读尽可能与热读分离,冷读主要依靠冷读集群完成.
![](https://user-gold-cdn.xitu.io/2020/2/28/1708989b27c68b19?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
不过作为时序数据库与Metis之间的中间件,Metis的业务暂时未涉及到冷热数据并发处理情况,只是分为分钟持续与每天定时拉取数据,目前并不需要`Connector`集群.
## 13. 如何整合InfluxDB 和 Kafka

### 13.1. 业界的实践
这篇[How companies integrate Kafka with InfluxDB to create tolerant, scalable, fast and simplified data streams](https://www.influxdata.com/blog/influxdb-and-kafka-how-companies-are-integrating-the-two/)提到`Hulu` 和`Wayfair`团队的实践.
`Hulu`声称它们可以做到每秒导入100万条指标数据,[^100]并且通过全新的重定向技术保证可以处理任何有问题的InfluxDB集群.[Hulu官方](https://medium.com/hulu-tech-blog/how-hulu-uses-influxdb-and-kafka-to-scale-to-over-1-million-metrics-a-second-1721476aaff5)给了一个架构图:
![](https://gitee.com/istarwyh/images/raw/master/1589123280_20200510214807021_27695.png)
而`Wayfair`在2017年经过波士顿,西雅图和爱尔兰跨球集群之间海量时间序列数据连接的复杂探索:
![](https://gitee.com/istarwyh/images/raw/master/1589123281_20200510215806324_29544.png)

最终选择了在`InfluxDB`和`Kafka`基础上重构以Kafka为中心的管道来集成来自三个不同数据中心（包括每个安全区域和非安全区域）的指标数据:
![](https://gitee.com/istarwyh/images/raw/master/1589123282_20200510220649624_21135.png)
团队提到:
>InfluxData’s Telegraf service made it relatively easy to configure a multi-layered pipeline by which applications could send data to Telegraf and allow Telegraf to pipe it into Kafka for later consumption. In practice we are using Telegraf twice: One layer to receive raw InfluxDB line protocol from applications via UDP and forward it on to Kafka, and an additional layer to consume metrics from the Kafka buffer and write to InfluxDB.

于是发现`InfluxData`团队已经开发了`Telegraf`服务来帮助在Kafka和InfluxDB之间导出和导入数据.
### 13.2. Telegraf
事实上`Telegraf`能连接各种数据源比如 `MongoDB`, `MySQL`, `Redis`,还有包括`Kafka`这样的消息队列然后进行收集和再次发送数据.[官方](https://www.influxdata.com/time-series-platform/telegraf/)称`Telegraf`可以扮演`Agent`和`Collector`和`Ingest Pipeline`来减少对于大量数据来源情况下写操作的请求数量,而其本身是 `plugin-driven`驱动的服务器.
![](https://gitee.com/istarwyh/images/raw/master/1589123283_20200510221541767_27223.png)
### 13.3. InfluxDB Cloud 2.0


2019年5月7日–InfluxDat发布了`InfluxDB Cloud 2.0`.InfluxDB Cloud 2.0集成了Kafka[^InfluxDB2.0],实现高效稳健容忍度高.当客户将一批指标点写入Kafka层时，数据将分配给拥有分区的Kafka生产者，在此处被短暂保存，而不是立即写入Kafka分区。 Kafka生产者保留数据，直到生产者有足够大的批量来写。 Kafka生产者可以保留不断增长的批次，同时将其他批次的数据写入Kafka分区，从而使存储引擎可以有效地处理大量数据，而不必处理来自单个客户端的少量数据。同样，当客户端写入WAL[^WAL]时，不会阻塞它们在存储层中。事实上,预先写入日志或WAL是几乎每个性能数据库（包括时间序列数据库）的通用做法,而kafka作为WAL工具维护了数据库系统中的写持久性和原子性,也保证了高效.
![](https://gitee.com/istarwyh/images/raw/master/1589123283_20200510224536355_2105.png)
### 13.4. Confluent Platform
The [InfluxDB Connector](https://docs.confluent.io/current/connect/kafka-connect-influxdb/) is used to copy data between Kafka and InfluxDB Server.

- The InfluxDB source connector is used to export data from InfluxDB Server to Kafka topics.
- The InfluxDB sink connector is used to import data from Kafka topics to InfluxDB Server.

但是好像是个收费产品,具体封装已经做的挺好了,不过还需要`Confluent Hub Client`的支持,直接
```
confluent-hub install confluentinc/kafka-connect-influxdb:latest
```
就可以,也可以直接下载[安装包](https://www.confluent.io/hub/confluentinc/kafka-connect-influxdb.)
## 14. Kafka与Flink
### 14.1. Kafka对于InfluxDB的工程缺点
对于目前的异常检测而言,重要的不是在各个数据源集群之间获得数据,最后导入InfluxDB,更重要的是一开始提到的导入异常数据的实时性,即通过对热数据的实时判断实现系统异常的快速报警.于是之前有考虑过**InfluxDB->(Kafka)->Metis**,Kafka的实时/离线消费者的区别与冷读/热读的思想也较为契合.
但是受张之逸的指导,这样工程上实现技术难度较大,因为没有办法用InfluxDB的类sql语法了.而对**监控数据流的特征提取**计算,可以用`flink`来搞.
### 14.2. 反思
总思路:**(Kafka)->flink->InfluxDB->Metis**
## 15. Flink的概念
### 15.1. [Definition](https://www.bilibili.com/video/BV1nb411v7JP?t=2143)
Apache Flink is a **framework** and **distributed** processing engine for **stateful** computations over **unbounded** and **bounded data streams**.
![](https://gitee.com/istarwyh/images/raw/master/1589852215_20200519085043623_22399.png)
#### 15.1.1. Words

- `framwork`:统一的流和批处理框架
- `distributed`:分布式处理引擎
- `data streams`:基本数据模型是数据流(如事件`Event`的序列)
- `stateful`:设计中贯穿的观点:**批是流的特例**,通过**有状态的计算**(`incremental processing`),数据的处理和之前处理过的数据或者事件将会有关联。比如，在做聚合操作的时候，一个批次的数据聚合的结果依赖于之前处理过的批次。类似于循环神经网络模型`LSTM`中忘记门/记忆门/输出门的设计
- `unbounded`:,数据流只有起点而没有确定的结束点,但每一条数据都可以出发计算逻辑[^ApacheFlink]
- `bounded`: `time-window`内到达的所有数据做一批处

![](https://gitee.com/istarwyh/images/raw/master/1589852217_20200519092334638_4790.png)
 :[^ApacheFlink]《Stream Processing with Apache Flink》

#### 15.1.2. [Time](https://www.bilibili.com/video/BV1G441177wT/?spm_id_from=333.788.videocard.17)
时间标签在Flink中比关系型数据库中的`key`更重要,因为它必然是key.因此对`Flink`中的key做一介绍:

- `Event time`: the time when an event **created**. It is uaually described by a timestamp in the events
- `Ingestion time`: the time when an event **enters** the Flink dataflow at the source operator
- `Processing Time`: the local time at each **operator** that performs a time-based operation

![](https://gitee.com/istarwyh/images/raw/master/1589852218_20200519093608667_403.png)
### 15.2. Flink的生态及API
**生态支持**
![](https://gitee.com/istarwyh/images/raw/master/1589852216_20200519085307894_15750.png)
**API[^API]**
层次化的API在表达能力和易用性方面各有权衡。表达能力由强到弱（易用性由弱到强）依次是：ProcessFunction、DataStream API、SQL/Table API。
![](https://gitee.com/istarwyh/images/raw/master/1589852217_20200519090021201_18752.png)
Flink API提供

- 通用的流操作原语（如窗口划分和异步操作）以及精确控制时间和状态的接口
- 详细、可自由定制的系统及应用指标（`metrics`）集合，用于提前定位和响应问题
- 嵌入式执行模式可将应用自身连同整个Flink系统在单个JVM进程内启动，方便在IDE里运行和调试Flink作业
### 15.3. Flink Vs Spark
双方都想构建包含 AI 的统一大数据计算平台
**Flink之于Spark的优点**

- `Stateful`
- `Multi-level API`
- `unbounded data stream`

**Flink的未来发力点**

- 支持对机器学习和深度学习的集成
- 提高易用性
- 继续完善生态(如支持更多语言),提高社区知名度和活跃度

当5G解决网络传输这个瓶颈之时,IOT和实时计算将会迎来爆发时发展，届时Flink在流式计算中的天然优势也将在线上学习等领域大放异彩.
## 16. 回归可行的方案
查鑫池:准备用 fastjson转换一下格式 ,然后用influxdb 的Java客户端转到库
张之逸:在Omni的数据中添加属性名以及每分钟的时间戳，构成一个csv文件，然后利用这个[工具](https://github.com/fabio-miranda/csv-to-influxdb)把数据导入到influxdb中
再把metis里数据读取的接口都改成从influxDB里读,最后对metis的=已有的算法打个补丁,最终也算是有成果了。
### 16.1. 最后落地的计划
1. OmniAnomaly提供数据集,38维
2. csv->本地->k-v(`timestamp`)->influxDB
3. 做成Spring Boot项目,启动服务监听指定端口
4. InfluxDB->Metis
5. 前端放入docker容器,上述服务放入docker容器,InfluxDB放入docker容器

负责将csv2influxDB做成一个API或者服务.
## 17. 数组/CSV转入InfluxDB
### 17.1. 使用云端InfluxDB
![](https://gitee.com/istarwyh/images/raw/master/1591715147_20200608102558826_12383.png)
[Get Started](https://v2.docs.influxdata.com/v2.0/get-started/#start-with-influxdb-cloud-2-0)

- Token: `export INFLUX_TOKEN=ZAiCu6Os2vOak5q9Ky3bcPdHWPGZHOqcgvBI52YhatD3FOjyxS05wAaDx8QPj_145Y6wnX-olAjsvPTFRO-9Qw==`
- Organization:`sspku`;ID:`87e2941a63ad495d`
- Bucket:`Metis`;ID:`05d50679c9497000`
- Start Telegraf: `telegraf --config https://us-central1-1.gcp.cloud2.influxdata.com/api/v2/telegrafs/05d0726a72a9f000`

![](https://gitee.com/istarwyh/images/raw/master/1591715152_20200608210059539_11592.png)
### 17.2. [Write Data to InfluxDB](https://v2.docs.influxdata.com/v2.0/write-data/)
1. Do one of the following:
- **InfluxDB 2.0 OSS:** In your terminal, run influxd and then in your browser, go to the location where you’re hosting the UI (by default, `localhost:9999`).
- **InfluxDB 2.0 Cloud:** In your browser, go to `https://cloud2.influxdata.com/`.
2. In the navigation menu on the left, select **Data (Load Data) **> **Buckets**.
3. Under the bucket you want to write data to, click **$+$Add Data**.
4. Select from the following options:
    - Configure Telegraf Agent
    - Line Protocol
    - Client Library

Telegraf Agent支持从不同的`buckets`导入到InfluxDB,不过本地没有在运行的集群/容器或者数据库,主要介绍`Line Protocol`和`Client Library`.
#### 17.2.1. LineProtocol
**[数据格式](https://v2.docs.influxdata.com/v2.0/reference/syntax/line-protocol)**
```
measurementName,tagKey=tagValue fieldKey="fieldValue" 1465839830100400200
--------------- --------------- --------------------- -------------------
       |               |                  |                    |
  Measurement       Tag set           Field set            Timestamp
  
mem,host=host1 used_percent=23.43234543 1556892576842902000
cpu,host=host1 usage_user=3.8234,usage_system=4.23874 1556892726597397000
mem,host=host1 used_percent=21.83599203 1556892777007291000
```
每一行代表一个数据点,每个数据点需要指明表名(`measurement`)和key-value对(`field set`)

1. Select **Upload File** or **Enter Manually**.
    - **Upload File**: Select the time precision of your data. Drag and drop the line protocol file into the UI or click to select the file from your file manager.
    ![](https://gitee.com/istarwyh/images/raw/master/1591715148_20200608201459304_13387.png)
    - **Enter Manually**: Select the time precision of your data. Manually enter line protocol.
2. Click **Write Data**. A message indicates whether data is successfully written to InfluxDB.
To add more data or correct line protocol, click **Previous**.
3. Click **Finish**.
![](https://gitee.com/istarwyh/images/raw/master/1591715150_20200608205907396_14806.png)
#### 17.2.2. Client Library
支持主流语言客户端:
![](https://gitee.com/istarwyh/images/raw/master/1591715149_20200608201650559_20447.png)
[Java客户端](https://github.com/influxdata/influxdb-client-java):

##### 17.2.2.1. Maven引入依赖
```xml
<dependency>
  <groupId>com.influxdb</groupId>
  <artifactId>influxdb-client-java</artifactId>
  <version>1.8.0</version>
</dependency>
```
##### 17.2.2.2. 初始化客户端:

```java
package example;

import java.time.Instant;
import java.util.List;

import com.influxdb.annotations.Column;
import com.influxdb.annotations.Measurement;
import com.influxdb.client.InfluxDBClient;
import com.influxdb.client.InfluxDBClientFactory;
import com.influxdb.client.WriteApi;
import com.influxdb.client.domain.WritePrecision;
import com.influxdb.client.write.Point;
import com.influxdb.query.FluxTable;

public class InfluxDB2Example {
  public static void main(final String[] args) {
  
    // You can generate a Token from the "Tokens Tab" in the UI
    String token = "token";
    String bucket = "bucketID";
    String org = "87e2941a63ad495d";

    InfluxDBClient client = InfluxDBClientFactory.create("https://us-central1-1.gcp.cloud2.influxdata.com", token.toCharArray());
  }
}
```
##### 17.2.2.3. 写数据
主要有三种方式:

1. 使用`Line Protocol` 
```java
String data = "mem,host=host1 used_percent=23.43234543";
try (WriteApi writeApi = client.getWriteApi()) {
  writeApi.writeRecord(bucket, org, WritePrecision.NS, data);
}
```
2. 自己构造`Data Point`

```java
Point point = Point
  .measurement("mem")
  .addTag("host", "host1")
  .addField("used_percent", 23.43234543)
  .time(Instant.now(), WritePrecision.NS);

try (WriteApi writeApi = client.getWriteApi()) {
  writeApi.writePoint(bucket, org, point);
}
```
3. 使用注解与IndluxDB约定好指定格式的Java对象

```java
Mem mem = new Mem();
mem.host = "host1";
mem.used_percent = 23.43234543;
mem.time = Instant.now();

try (WriteApi writeApi = client.getWriteApi()) {
  writeApi.writeMeasurement(bucket, org, WritePrecision.NS, mem);
}
------
@Measurement(name = "mem")
public static class Mem {
  @Column(tag = true)
  String host;
  @Column
  Double used_percent;
  @Column(timestamp = true)
  Instant time;
}
```
## 18. References

[^成熟]:[腾讯三大运维开源项目齐聚“OSCAR开源先锋日”](https://segmentfault.com/a/1190000016749190)
[^织云]:[织云 Metis：看腾讯怎么做智能运维](https://zhuanlan.zhihu.com/p/36333033)
[^冷热数据分离方案]:[蘑菇街Kafka](https://juejin.im/post/5e1bad1ce51d453cee48c976)
[^100]:[scale to over 1M metrics per second](https://medium.com/hulu-tech-blog/how-hulu-uses-influxdb-and-kafka-to-scale-to-over-1-million-metrics-a-second-1721476aaff5)
[^InfluxDB2.0]: [InfluxDB2.0](https://www.influxdata.com/blog/influxdb-and-kafka-how-influxdata-uses-kafka-in-production/)
[^WAL]:Write-Ahead-Log,对数据库执行的操作的日志（仅作为附录文件）
[^API]: [Flink的特点和优点](https://www.cnblogs.com/zgq25302111/p/12258371.html)
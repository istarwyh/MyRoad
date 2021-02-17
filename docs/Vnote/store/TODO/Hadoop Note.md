## 1. 概述
分布式/关系式
- 宽带是什么?带宽?
宽带指运营商给用户接入的使其能上网的服务,有10M宽带,100M宽带等类型.以这两种类型为例,它们分别指的是此宽带预计提供给用户的最高下载速度是10Mb/s也写作10Mbps),100Mb/s.
带宽则就是指刚刚的10Mbps,100Mbps,指的是线路的信息传输速率.
带宽在网络术语中也指线路传输的频段宽度.
By the way,流量是指对外发送数据与接受数据包的大小总和.
- 视频流是什么?
基于视频流传输协议RTP/HTTP等的视频传输技术

大机和高端机都是不能随便重启的.然后我们使用软件使得普通PC的重启在处理中被当作一种常态.
信息时代,一个文件几十上百G,集群处理
64M一块,冗余存储,假设崩溃是常态.
Google公布了三驾马车---因为它已经有了新的三驾马车.尽管这个是20前的技术.**各自解决了什么问题?**
- 硬件成本
- 可靠性
- 并行计算
认真读这三篇文章.

抓取网页,存取网页,提供搜索引擎的数据
- 搭建大型数据仓库,实现PB级的数据存储/处理/分析/统计

hadoop大数据平台是事实上的标准.hadoop在硬盘中操作,延迟高,计算慢是一个弱点.

### 1.1. HDFS
**HDFS的设计初衷**:
- 一次写入,多次读取
- 不支持文件并发写入
- 不支持文件修改

HDFS的使用范围
- 存储并管理**PB**级数据
- 处理非结构化数据(网页时非结构化的);结构化数据每一行时必有每一列.
- 注重吞吐,对延时不敏感
(21号ftp一般连接,20号端口传输数据,20号就是注重的吞吐量)
- 一次写入,多次读取的模式

Namenode,管理结点,放的是数据的数据--元数据--Metadata.Metadata由文件扫描系统得到.

**在大数据中故障是常态,5%**
一个硬盘坏了就很快换,一个网卡坏了也很快换.但是如果硬盘挂了,数据取不出来就完了,所以备份三份数据,但是这三份各自存储在很多个硬盘上,Metadata存储各数据的位置.可以从不同的地方取备份数据最后恢复换掉的硬盘.之所以不把三份都放在一个datanode结点上,降低三份都挂掉的概率.那么Namenode挂了元数据也可能丢失,怎么防止?有策略依然有备份,尽管时间差依然会造成数据丢失.
此外,实际环境中,Namenode中依然是一个集群,不过相对datanode存在的设备性能较高.

一写写三份,全部写完了再close

### 1.2. Mapreduce
上千cpu并行处理
来源于函数式编程语言,Map+Reduce两部分
Map: 任务拆解
Reduce: 任务汇总分析.为了提高速度,并不是结果直接写入硬盘,而是先在缓存中进行缓存和排序.
![](https://images2015.cnblogs.com/blog/923309/201604/923309-20160406215236172-340369929.jpg)

**以wordcount为例,应该这个例子完全搞明白,或者遇到问题先想着搞明白啊!**
[TextInputFormat](https://www.cnblogs.com/pengyingzhi/p/5361008.html)是Hadoop默认的输入方法，在TextInputFormat中，每个文件（或其一部分）都会单独作为Map的输入，之后，每一行数据都会产生一个<key,value>形式：其中key值是每个数据的记录在数据分片中的字节偏移量，而value值是每行的内容。所以，图5中画红圈的两个数据应该是有误的（在上面只是为了方便表示），正确的值应该是第二行第一个字符的偏移量才对。
[WordCount](https://www.cnblogs.com/huxinga/p/6939896.html)
[wordcount补充](https://blog.csdn.net/xiaoxiaolove_i/article/details/98737608?depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1&utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1)
[数据清洗](https://blog.csdn.net/xiaoxiaolove_i/article/details/98737608?depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1&utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1)
https://www.cnblogs.com/edisonchou/p/4458219.html

[!!!](https://www.cnblogs.com/zjfstudio/p/3887551.html)
[!!!](https://www.cnblogs.com/xia520pi/archive/2012/05/16/2504205.html#_label4)


![映射-洗牌-重组](_v_images/20200227084436177_31314.png)
### 1.3. Ques
1. Job running in uber mode : false;但是可以正常运行
[What is the purpose of “uber mode” in hadoop?](https://stackoverflow.com/questions/30284237/what-is-the-purpose-of-uber-mode-in-hadoop)
>If you have a small dataset or you want to run MapReduce on small amount of data, Uber configuration will help you out, by reducing additional time that MapReduce normally spends in mapper and reducers phase

2. 为什么hadoop要对基本数据类型封装一遍,如`int`->`IntWritable`,`String`->`Text`?能互转吗?
在hadoop中能运行的格式,估计得看源码如何封装.
关于互转,一边,以传统数据格式作参数`new`可;另一边,对于`IntWritable`提供了`get()`方法转换为`int`,`Text`提供了`toString()`转换为`String`,且相比于原生`String`,也提供了`set()`可改变自身.
3. 怎么样就生成了`<key,List>`?
split->map->(combine->partition)->reduce->result
所谓reduce生成list,其实也是并行的许多次reduce,逐个累积生成list的.

## 2. 相关的
CAP
zookeeper能保持强一致性，但是又主节点，master一旦发生故障之后会重新选举出新的主节点，保持一致性但是选举时间较长。Eureka 保持的是最终一致性，只要还有节点存在，那么就可以快速的响应。弱一致性可能会对项目造成一定的影响，比如所见的信息不是最新信息。

作者：悬壶醉世
链接：https://www.zhihu.com/question/289129028/answer/1065600854
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
## 3. 问题与解决方案
>1. ubuntu重启不断闪屏

据说是gdm启动过程与什么冲突了,然后不断崩溃又重启造成了这个效果.重启两次之后偶尔会出现这个bug.
>2. 不知怎么查看配置第二个网卡

原来VMware默认只有一个网卡,可以手动加载[双网卡](https://blog.csdn.net/weixin_43291687/article/details/82863419?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)
>3. 电脑低电量关机,重启后两个Linux都打不开

- 如果提示无法锁定文件,可以[删掉所有`.lck`文件](https://www.jianshu.com/p/be7e523678ba).因为那时突然系统崩溃了，虚拟机来不急把已经锁定的文件解锁.
- 或重来一遍

>4. ssh启动失败:`The authenticity of host 'localhost (127.0.0.1)' can't be established.localhost: Host key verification failed.`

ubuntu默认禁止root账户的ssh连接,可以选择[不让ssh检查了](https://blog.csdn.net/stpeace/article/details/79426471)[并且进一步放松限制](https://www.cnblogs.com/hmy-blog/p/6500909.html).

>5. 启动hadoop时输入密码总是提示`Permission denied`

据说最新的ssh免密登录已经不支持id-dca,改用id-rca;并且通过scp公钥到其他ip机`/.ssh/`目录下,可以实现[一次密码,全面放行](https://blog.csdn.net/situliang/article/details/72904449?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task).


>6. 机器卡死

之后`datanode`节点调到了1.3G.

>7. 上传文件时提示`no datanode`,当时jps显示有同时启动Hadoop没有任何错误

我这个机器的解决方案:注释掉master的`interfaces`的前两行即可.
*简记过程:*
造成这种问题可以有很多原因,比如[namenode格式化失败](https://www.cnblogs.com/zwgblog/p/6064022.html),比如[namenode与datanode参数不匹配](https://blog.csdn.net/qq_43193797/article/details/85674720),比如防火墙原因.排查后发现`datanode`根本就没有生成`VERSION`文件,那些专注于解决datanodeSHUTDOWN的方法只会让debug越走越远.
排除了路径错误,文件权限,`hdfs-site.xml`或`core-site.xml`各种问题,在[这个帖子](https://blog.csdn.net/fireblue1990/article/details/51096350?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)受到了启发:
- **发现问题最好的方法就是查看日志，根据日志提示找问题** grep error:xxx xxx.log

data1的日志中记载它反复尝试连接`master:9000`失败,以及发生了`java.lang.IOException`,此前尽管已经反复检查过`interfaces`中的配置,没有使用`telnet检查端口连接`,最后原因应该是其前两行干扰了主机ip与主机名的映射,把它们注释掉问题终于解决了.

>8. 关机之后，重启的时候黑屏，无鼠标的解决方案：

使用管理员权限打开powershell 输入：`netsh winsock -reset`

即可解决问题

>9. hadoop3.X之后端口有一些变更，导致打开50070端口无响应

应该改为9870端口。附[参考教程](https://blog.csdn.net/sinat_35045195/article/details/88774512)

>10. master崩溃，拷贝slave虚拟机，修改配置使其成为master节点，开始运行hadoop后没有datanode?

这是因为GROUP ID不同的原因，通过将name 文件夹下的version文件中的GROUPID拷贝到DATA文件夹下的version文件中，重启后可以解决问题。
>11. 上一问题的情况下，新的master节点无法ssh登陆到master自身。生成新的ssh秘钥后，拷贝到authorized-keys后报错

这是因为之前slave节点中已经存放了一个master的秘钥，拷贝新的秘钥后会导致重复，这时，按照指引删除掉旧的秘钥（oldkey）,重新连接即可，注意要把authorized_keys同步到slave上以更新

>12. 下载速度过慢

更换镜像是个好方法，但是安装时是无法选择镜像的，这时我们可以连接上北京大学VPN..速度会有显著的提高。


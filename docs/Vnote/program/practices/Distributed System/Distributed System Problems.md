现代系统正变得越来越复杂，从单线程到多线程，从单体到微服务，从单节点到分布式，从本地到云端...… 复杂度使得程序产生预期的结果需要越来越多的必要条件，而每种条件都有其自身的成功概率，即使每种条件的成功概率都很高，根据墨菲定律，或早或晚一定会遇到不可预知的结果。比如大数据处理，基本都是分布式运行的任务、高可用的消息队列、多备份的存储、主从节点等，这样大数据处理就是比较容易出错的领域。
所以我们更要认识到我们容易犯的[认知误区](https://architecturenotes.co/fallacies-of-distributed-systems/)：
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202309171622093.png)


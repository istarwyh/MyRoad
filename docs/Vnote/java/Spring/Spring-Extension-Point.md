[Spring的Bean生命周期和扩展点](https://mp.weixin.qq.com/s/sK1qbsOj0v0UrAyLy6-zMw) ：

![]([[]]https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202312032330027.png)

必须要注意的是 `@PostConstruct` 只能保证自己的bean被装载，但并不能保证所有被需要的bean被装载，所以在方法内加载 或 获取bean 会有问题，比如在方法内 使用 ApplicationContext#getBeansWithAnnotaion 获取到的Bean 可能就是没有加载完全的Bean。这种情况下需要使用 `ApplicationListener`。[ApplicationListener](https://mp.weixin.qq.com/s/GbY_pwql9zzxPBPVgq-Yrw) 是用来监听容器中发生的事件，只要事件发生，就通过循环获取到所有的ApplicationListener 判断其是否需要回调，完成事件驱动模型的开发。

https://mp.weixin.qq.com/s/UNB4Nty-GuXqa448RdtfgQ


https://mp.weixin.qq.com/s/ACSjWjrdgeKI8aMzZPDN8w

Spring阅读
github.com/xuchengsheng/spring-reading



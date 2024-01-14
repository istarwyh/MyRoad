---
reference_link: https://read.engineerscodex.com/p/how-instagram-scaled-to-14-million
author: 天舟
---
从 2010 年 10 月到 2011 年 12 月，Instagram 在短短一年多的时间里，用户数量从 0 增长到 1400 万。他们仅用 3 名工程师就做到了这一点。

他们之所以能做到这一点，是因为遵循了 3 个关键原则并拥有可靠的技术栈。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5Sg8eQMaE1Jebvf2DIiaoC8CR2ZQ8Nvw2yJMEU1pftTyLWd1WDhic511uvx8epcA6ayMd6CVicXWDWmxw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## **Instagram 的指导原则**

- 一切从简。
    
- 不要重复发明轮子。
    
- 尽可能使用经过验证的可靠技术。
    

---

早期 Instagram 的基础设施运行在 AWS 上，使用 EC2 和 Ubuntu Linux。为了方便起见，让我们通过用户会话（session) 的生命周期来描述整个技术栈。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5Sg8eQMaE1Jebvf2DIiaoC8CR2R3y6zrzX4z4Znk5FA2VTnmKqHW6o3doOTOSJOVdZfoDH6S20anhGA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## **前端**

> 会话：用户打开 Instagram 应用程序。

Instagram 最初是在 2010 年作为 iOS 应用程序推出的。Instagram 是使用 Objective-C 以及对应的 UIKit 框架来写的

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5SjFW9wq5K6PibsYOF7gNichRiaqkIg1GSawXehbU8U2WZZRTQECyEvaXw4ian8RHtfvQllcZcEAAibiassA/640?wx_fmt=png&from=appmsg&wxfrom=5&wx_lazy=1&wx_co=1)

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5Sg8eQMaE1Jebvf2DIiaoC8CRLfRZPEsR396Zr4xkgicjanHlaA4Y6fWRbicTtMtVJibpDqavfBKP4wVPQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## **负载均衡**

> 会话：打开应用后，抓取主 feed 照片的请求会被发送到后台，在那里会碰到 Instagram 的负载均衡器。

Instagram 使用 AWS 的 Elastic Load Balancer (ELB)。他们有 3 个 NGINX 实例，这些实例会根据其是否健康进行交换。

每个请求在路由到实际应用服务器之前都会首先到达 ELB。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5SjFW9wq5K6PibsYOF7gNichRiacaCTdpQqvNgGBopibvO7X1CibGDbGAXUT7CQCOmmpT9cdhOgiabGhXrHA/640?wx_fmt=png&from=appmsg&wxfrom=5&wx_lazy=1&wx_co=1)

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5Sg8eQMaE1Jebvf2DIiaoC8CRVBSl7xGUgicoq9LVAplRwM4ymeibk8lIyFSbPdqk3n7wicIOH3SLyFI7g/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## **后端**

> 会话：ELB 将请求发送到应用服务器，应用服务器拥有正确处理请求的逻辑。

Instagram 的应用服务器使用了 Django，它是用 Python 编写的，Gunicorn 是他们的 WSGI 服务器。

作为复习，WSGI（Web Server Gateway Interface，网络服务器网关接口）将请求从网络服务器转发到网络应用程序。

Instagram 使用 Fabric 同时在多个实例上并行运行命令。这样就能在几秒钟内部署代码。

这些实例运行在超过 25 台 AWS CPU High-CPU Extra-Large 的机器上。由于服务器本身是无状态的，因此当他们需要处理更多请求时，可以添加更多机器。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5SjFW9wq5K6PibsYOF7gNichRiadsUBNUYupmS5YicTDyd6CFeOAwHG0TFTvlHia1iak0Qwm9ym2NUAgjgmg/640?wx_fmt=png&from=appmsg&wxfrom=5&wx_lazy=1&wx_co=1)

### **通用数据存储**

> 会话：应用服务器收到请求，这些请求需要主 feed 上数据。

为此，它大致需要：

1. 最新的相关照片 ID
    
2. 与这些照片 ID 匹配的实际照片
    
3. 这些照片的用户数据
    

### **数据库：Postgres**

> 会话：应用服务器从 Postgres 中获取最新的相关照片 ID。

应用服务器将从 PostgreSQL 中提取数据，该数据库存储了 Instagram 的大部分数据，如用户和照片元数据。

PostgreSQL 和 Django 之间的连接使用 Pgbouncer 连接池。

由于 Instagram 接收的数据量很大（每秒超过 25 张照片和 90 个赞），**他们对数据进行了分片**。他们使用代码将几千个「逻辑」分片映射到几个物理分片上。

Instagram 面临并解决的一个有趣挑战是**生成可按时间排序的 ID**。他们生成的可按时间排序的 ID 是这样的：

- 41 位比特表示以毫秒为单位的时间（提供 41 年的 ID 和自定义纪元）
    
- 13 位比特表示逻辑分片 ID
    
- 10 位比特表示自动递增序列，模数为 1024。这意味着我们可以为每个分区每毫秒生成 1024 个 ID
    

得益于 Postgres 中可按时间排序的 ID，应用服务器成功接收到了最新的相关照片 ID。

### **照片存储：S3 和 Cloudfront**

> 会话：然后，应用服务器通过快速 CDN 链接获取与这些照片 ID 匹配的实际照片，以便快速加载给用户。

AWS S3 中存储了数 TB 的照片。这些照片通过 AWS CloudFront 快速提供给用户。

### **缓存：Redis 和 Memcached**

> 会话：为了从 Postgres 中获取用户数据，应用服务器（Django）使用 Redis 将照片 ID 与用户 ID 进行匹配。

Instagram 使用 Redis 存储了约 3 亿张照片与创建这些照片的用户 ID 的映射，以便在为主 feed、活动 feed 等获取照片时知道要查询哪个分片。所有 Redis 都存储在内存中，以减少延迟，并在多台计算机上进行分片。

**通过一些巧妙的哈希算法，Instagram 能够在不到 5 GB 的空间内存储 3 亿个键值的映射。**

为了知道要查询哪个 Postgres 分区，需要将照片 ID 与用户 ID 进行键值映射。

> 会话：由于使用了 Memcached 进行高效缓存，从 Postgres 获取用户数据的速度非常快。

对于一般缓存，Instagram 使用的是 Memcached。当时他们有 6 个 Memcached 实例。将 Memcached 架设在 Django 上相对简单。

有趣的事实是：2 年后的 2013 年，Facebook 发布了一篇具有里程碑意义的论文，介绍了他们如何扩展 Memcached 以帮助他们处理每秒数十亿次的请求。

> 会话：用户现在能看到首页 feed 了，上面有他所关注的人的最新图片。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5SjFW9wq5K6PibsYOF7gNichRiagupOogwMhz52H1cIB9Qia1CxaQe6ZQIjdkhbJgI5bX36A6hHibibPqT2Q/640?wx_fmt=png&from=appmsg&wxfrom=5&wx_lazy=1&wx_co=1)

### **主从设置**

Postgres 和 Redis 都是用主从设置，并使用亚马逊 EBS（弹性块存储）快照功能对系统进行频繁备份。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5Sia2kMQIvpjMz1Mg9NTWQmUt8vaU00SgL5KG3oicftDOfudH8LCtWuGBDL8vGUeBHsmnJubW5UbjNhQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## **推送通知和异步任务**

> 会话：现在，假设用户关闭了应用，但又收到推送通知说朋友发布了一张照片。

该推送通知是使用 pyapns 发送的，同时发送的还有 Instagram 已发送的十亿多条推送通知。Pyapns 是一个开源、通用的苹果推送通知服务（APNS）提供商。

> 会话：用户非常喜欢这张照片！所以他决定在 Twitter 上分享。

在后端，任务被推送到 Gearman，这是一个任务队列，可以将工作分派给更合适的机器。Instagram 有大约 200 个 Python Workers 在使用 Gearman 任务队列。

Gearman 用于执行多个异步任务，例如向用户的所有关注者推送活动（如发布新照片）（称为 fanout）。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5SjFW9wq5K6PibsYOF7gNichRia5qlA5BecWPRx1enib6cNu8h2JlHYpxPoNsewUfvZJIhDFuWibicD0lXSw/640?wx_fmt=png&from=appmsg&wxfrom=5&wx_lazy=1&wx_co=1)

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5Sia2kMQIvpjMz1Mg9NTWQmUtkpWp1DJ4QaRHxNTXpnFicw63icuGMHowTfshpIrYTUpaFKZBjaEyc7VA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## **监控**

> 会话：啊哦！Instagram 挂了，原因是服务器出错，发送了错误的响应。Instagram 的三名工程师立即收到了警报。

Instagram 使用开源 Django 应用程序 **Sentry** 实时监控 Python 错误。

**Munin** 被用来绘制全系统指标图，并对异常情况发出警报。Instagram 有许多自定义 Munin 插件，用于跟踪应用级指标，如每秒发布的照片。

**Pingdom** 用于外部服务监控，**PagerDuty** 用于处理事件和通知。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5Sia2kMQIvpjMz1Mg9NTWQmUt5H8BJzIz4f6sCpQCb2be5Yl4woUQygv4WhoA1lQu0OQ0fyk0Rt4TqQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## **最终的架构概览图**

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/MYlicw4Vl5SjFW9wq5K6PibsYOF7gNichRiauqCEQleJ0o1ia4dMptaP5EAftWd4AgwparJwoDr5HF2UXhjArDr8K4A/640?wx_fmt=png&from=appmsg&wxfrom=5&wx_lazy=1&wx_co=1)

  

--

## **译者后记**

在之后的 2012 年，Facebook 就以 10 亿美金收购了当时只有 13 名员工的 Instagram，折算成 RMB 的话，人均身价超过 5 亿。当时 Facebook 可以说是下了血本，但现在来看，这笔收购是 Facebook 至今为止最成功的一次收购。

还是那句话，我们应该采用无聊的技术去构建创新的产品，而不是倒过来。
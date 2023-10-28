## 1. 上游接口/前端调用
别人调我,需要限流,需要问别人QPS.

### 1.2. 我的系统到底能抗多少QPS呢?
- QPS:Query Per Second,即当前接口每秒被请求的次数

经验值,对于单台机器来说,
4核8G的机器,SpringBoot+Tomcat大约能抗500-1000的QPS;
8核16G的数据库大约1000-5000的QPS.
### 2. 全链路调用TPS
我在全链路里,不能拖后腿,需要知道自己TPS.

- TPS: Transaction Per Second,即每秒钟可以完成的事务数量
假设一个应用的接口响应需要100ms,其实意味着每个tomcat中的线程每秒也只能跑完10次应用内链路.如果该应用开启了200个线程,则TPS = 200 * (1000/100) = 2000.

那全链路跑完一次要多久呢?全链路压测可以告诉我们答案,假设500ms一次吧,那么按照上述假设,全链路TPS就是400,如果想扩大,就需要扩机器了.
### 限流
[5 种限流算法，7 种限流方式，挡住突发流量？](https://www.wdbyte.com/java/rate-limiter.html)
单机推荐GUAVA的`Ratelimiter`,分布式系统推荐`Sentinel`[l^limit].

[l^limit]: https://mp.weixin.qq.com/s/ntgBj8hm4u0udl8qwsukBg
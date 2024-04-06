"Full-Stream" 是ThoughtWork 中国区CTO 徐昊仿照"Full-Stack"全栈造出来的一个词，用于表示业务开发的整个流程，按照徐昊的话来说，是知识工程中知识产生、消费、传递的一整个过程。Full-Stream 在我这里体现如下：
[[full-stream.canvas|full-stream]]
## [[Analyzing]]

## [[Modeling]]

## Task Breaking down

## Testing
[[Unit Level Test Theory、Tool、Discussion]]
[[JUnit5 & Mockito]]

## Coding
- 开发需要的各种小工具，可搭配 raycast 等使用：https://he3app.com/zh/
- 自动从页面跳转代码： https://github.com/zh-lx/code-inspector
## Enhanced By AI
[[Frequently-Used-Prompt]]
[用 LLM 构建企业专属的用户助手](https://mp.weixin.qq.com/s/bpeszhmyMC_aRHt1fb0NLA)
- 本地LLM:https://jan.ai/ 、 ollma
- API: https://api.zetatechs.com/
- AI加持的terminal: warp
### Ask
https://devv.ai
https://www.phind.com/
https://copilot.microsoft.com/
### IDE
#### IDEA
- [[Frequently-IDEA-Shortcut]]
- 自动切换输入法：Smart Input
- 基于JVM做各种事：Arthas
- 本地调用接口
- 本地发送消息
- 本地Debug 远程

### Framework
- Web Extension开发框架: https://github.com/wxt-dev/wxt
- [[Java Framework & Dependency]]

#### 版本管理
git: https://learngitbranching.js.org/
[[Frequently Git Command]]
https://github.com/version-fox/vfox

### DataBase
#### Choose the Suitable Database
- [[How-to-Choose-the-Suitable-Database]]
- https://blog.bytebytego.com/p/understanding-database-types
- Free redis: https://upstash.com/
#### 快速利用数据库
- 面向小白的retool: https://retool.com/
- 面向开发者的airplane：https://www.airplane.dev/
- 可视化创建数据库🀄️数据：https://visualdb.com/
- 数据库数据 mock: Snaplet 
#### 数据库开发工具
- https://webdb.app/page/demo
#### 数据库管理工具
- bytebase:https://www.bytebase.com/zh
- 数据库、服务器二合一：https://github.com/clockworklabs/SpacetimeDB

### i18n
https://github.com/linyuxuanlin/Auto-i18n
https://www.i18next.com/
## Releasing
### DevOps Platform
- jira
- https://coding.net/
### Domain
[how to custom domains](https://www.netlify.com/blog/2020/03/26/how-to-set-up-netlify-dns-custom-domains-cname-and-a-records/)
网站防火墙：https://github.com/chaitin/SafeLine
### Website Registration

- ICP Query Extension

## Operation
### Cloud Platform
- 使用云平台像使用操作系统一样 https://sealos.io/ 以及 laf serverless 平台
- [善用GitHub](https://link.zhihu.com/?target=https%3A//www.bmpi.dev/self/use-github-better/)
- Cloudflare：Serverless最好的平台，能提供如常用的存储R2、KV数据库、CDN、Pages、Worker、MQ、甚至运行AI模型
- Vercel：很适合部署运行Next.js的前端项目，Cloudflare Pages运行Next.js还需要额外的一些配置。
- [http://Fly.io](https://link.zhihu.com/?target=http%3A//Fly.io)：非常好的容器化平台，它能弥补Cloudflare只能运行Serverless的不足，能以Docker的方式运行各种重型API服务。
- AWS/Azure/GCP：作为巨头平台，他们可以补充上面云平台所缺失的东西，比如AWS SES就是一个非常好的发邮件服务。
- [Newrelic](https://link.zhihu.com/?target=https%3A//newrelic.com/)：非常好的监控平台，尤其是它能作为免费的日志搜索平台，我把我产品的[系统日志](https://www.zhihu.com/search?q=%E7%B3%BB%E7%BB%9F%E6%97%A5%E5%BF%97&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra=%7B%22sourceType%22%3A%22answer%22%2C%22sourceId%22%3A3249282055%7D)都上传到这个平台，然后对日志进行监控。一般的日志平台都是ES架构，[价格](https://www.zhihu.com/search?q=%E4%BB%B7%E6%A0%BC&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra=%7B%22sourceType%22%3A%22answer%22%2C%22sourceId%22%3A3249282055%7D)并不便宜，单Newrelic却能免费。
- [Optimizely](https://link.zhihu.com/?target=https%3A//www.optimizely.com/)：一个不错的feature toggle平台，可以很方便的做产品的A/B测试。
### Document
- confluence
- trello
## Practices
[[Practices-for-Processing-l0-Billion-Bill-data]]
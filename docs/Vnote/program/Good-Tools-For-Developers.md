- [[#Theory & Resource|Theory & Resource]]
- [[#Practices|Practices]]
- [[#Story|Story]]
- [[#Start-up|Start-up]]

## Theory & Resource
- [[#Resources Router|Resources Router]]
- [[#Communication & Analysis|Communication & Analysis]]
- [[#Modeling & Design|Modeling & Design]]
- [[#Development|Development]]
- [[#Cloud Platform|Cloud Platform]]
- [[#Testing & Quality Assurance|Testing & Quality Assurance]]
- [[#Deploy|Deploy]]
- [[#Saving Time|Saving Time]]


### Resources Router
#### RPA
rpa: 浏览器行为录制自动化
自动投简历： https://github.com/Frrrrrrrrank/auto_job__find__chatgpt__rpa

#### Raycast / Alfred
Mac Spotlight 加强

### Tools 🔍
https://www.apigpts.dev/tools
https://sinqi.tools/
https://www.91wink.com/index.php/sites/
https://tool.browser.qq.com
### Communication & Analysis
- 先想直接一点的哪怕傻一点的解决方案--KISS--Keep It Simple, Stupid!
- 传统方式：设计师产生想象力，用工具把想象力做出来，再评审修改，然后上线。再由数据反馈让设计师调整不断迭代改进产品，再继续维护，直到有新的需求出来。
- 现在的AI：直接在数据中洞察需求，生成产品，不需要评审也不需要反复修改。这里有两个关键点，一个是对产品的理解算法，一个是设计模型的算法。比如我的产品需要在运营活动中提升销量，对于算法来说，它就需要知道提升销量的“公式”或“模型”有哪些，比如流量漏斗模型，有大流量进来，做好landingpage，再怎么样，再怎么样……有了这样一个个的算法，就能把对产品需求的理解交给设计模型，让设计模型来推荐一个风格。
- [[Information-Handler]]
### DevOps
- jira
- confluence
- trello
- https://coding.net/
### Modeling & Design
- UML
- 架构图绘制：https://www.onemodel.app/
- 绘图时需要的一些配色，可以访问: https://coolors.co/ 或者 http://khroma.co/ 来自动生成，比自己瞎配要美观不少
### Development
- 开发瑞士军刀，可搭配 raycast 等使用：https://he3app.com/zh/
- 自动从页面跳转代码： https://github.com/zh-lx/code-inspector
#### Terminal
##### Warp

#### IDE
##### IDEA
- [[Frequently-IDEA-Shortcut]]
- 本地RPC:
- 本地发送消息
- 本地Debug 远程
- 自动切换输入法：Smart Input
#### GPT
[[Frequently-Used-Prompt]]
[用 LLM 构建企业专属的用户助手](https://mp.weixin.qq.com/s/bpeszhmyMC_aRHt1fb0NLA)
https://devv.ai
https://www.phind.com/
本地LLM:https://jan.ai/
API: https://api.zetatechs.com/
#### 版本管理工具
git: https://learngitbranching.js.org/
[[Frequently Git Command]]
https://github.com/version-fox/vfox
#### DataBase
##### Choose the Suitable Database
- [[How-to-Choose-the-Suitable-Database]]
- https://blog.bytebytego.com/p/understanding-database-types
- Free redis: https://upstash.com/
##### 快速利用数据库
- 面向小白的retool: https://retool.com/
- 面向开发者的airplane：https://www.airplane.dev/
- 可视化创建数据库🀄️数据：https://visualdb.com/
- 数据库数据 mock: Snaplet 
##### 数据库开发工具
- https://webdb.app/page/demo
##### 数据库管理工具
- bytebase:https://www.bytebase.com/zh
- 数据库、服务器二合一：https://github.com/clockworklabs/SpacetimeDB
#### Framework
- Web Extension开发框架: https://github.com/wxt-dev/wxt
- [[Java-Project-Jar]]
#### 网站设计工具
- 设计师： https://webdesigner.tools/
- https://js.design/ai-upcoming 描述生成原型
- AI 界面转代码: https://kombai.com/
- ai 生成网页： https://v0.dev/
- 简笔画转AI图片：https://clipdrop.co/stable-doodle
- wix:https://zh.wix.com/
- strikingly:https://cn.strikingly.com/
### Cloud Platform
- 使用云平台像使用操作系统一样 https://sealos.io/ 以及 laf serverless 平台
- [善用GitHub](https://link.zhihu.com/?target=https%3A//www.bmpi.dev/self/use-github-better/)
- Cloudflare：Serverless最好的平台，能提供如常用的存储R2、KV数据库、CDN、Pages、Worker、MQ、甚至运行AI模型
- Vercel：很适合部署运行Next.js的前端项目，Cloudflare Pages运行Next.js还需要额外的一些配置。
- [http://Fly.io](https://link.zhihu.com/?target=http%3A//Fly.io)：非常好的容器化平台，它能弥补Cloudflare只能运行Serverless的不足，能以Docker的方式运行各种重型API服务。
- AWS/Azure/GCP：作为巨头平台，他们可以补充上面云平台所缺失的东西，比如AWS SES就是一个非常好的发邮件服务。
- [Newrelic](https://link.zhihu.com/?target=https%3A//newrelic.com/)：非常好的监控平台，尤其是它能作为免费的日志搜索平台，我把我产品的[系统日志](https://www.zhihu.com/search?q=%E7%B3%BB%E7%BB%9F%E6%97%A5%E5%BF%97&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra=%7B%22sourceType%22%3A%22answer%22%2C%22sourceId%22%3A3249282055%7D)都上传到这个平台，然后对日志进行监控。一般的日志平台都是ES架构，[价格](https://www.zhihu.com/search?q=%E4%BB%B7%E6%A0%BC&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra=%7B%22sourceType%22%3A%22answer%22%2C%22sourceId%22%3A3249282055%7D)并不便宜，单Newrelic却能免费。
- [Optimizely](https://link.zhihu.com/?target=https%3A//www.optimizely.com/)：一个不错的feature toggle平台，可以很方便的做产品的A/B测试。
### Testing & Quality Assurance

### Deploy
#### Domain
https://www.netlify.com/blog/2020/03/26/how-to-set-up-netlify-dns-custom-domains-cname-and-a-records/
网站防火墙：https://github.com/chaitin/SafeLine
#### Website Registration

- ICP Query Extension
#### i18n
https://github.com/linyuxuanlin/Auto-i18n
https://www.i18next.com/
### Saving Time
#### [跳过Android 开屏](https://github.com/zfdang/Android-Touch-Helper)

## Practices
[[Practices-for-Processing-l0-Billion-Bill-data]]

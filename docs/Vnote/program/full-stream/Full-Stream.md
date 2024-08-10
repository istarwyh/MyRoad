"Full-Stream" 是ThoughtWork 中国区CTO 徐昊仿照"Full-Stack"全栈造出来的一个词，用于表示业务开发的整个流程，按照徐昊的话来说，是知识工程中知识产生、消费、传递的一整个过程。Full-Stream 在我这里体现如下：
[[full-stream.canvas|full-stream]]

区别于岗位路线图：https://roadmap.sh

各个步骤都可能用到 prompt:[[Frequently-Used-Prompt]]。所有prompt都推荐搭配 有`snippet` 功能的软件使用，比如 `raycast`(Mac)、`quicker`(Windows).
## [[Analyzing]]
在分析的时候，你可能还需要不停地游走在各种不知道的概念和领域中，这时候 AI 体现出的“知识平权”效果就特别明显。下面的prompt或许能用到
```

### Instruction

You are an expert in ${field} development. Your task is to help someone with a backend development background learn about developing ${field}.
Before you start, please ask me any questions that you need answered to help me best solve my problem.

### Context

The user is experienced in backend development but is a novice in ${field} development with no prior experience in this field.

### Variables

${field}: Visual Studio Code Extension

### Task 1

Provide a detailed list of the key knowledge areas and tools needed to develop expertise in ${field} development.

### Task 2

Give specific examples and resources that can help the user learn effectively.

### Task 3 

Give me a demo to help me understand ${field}. The demo should be able to run in actual environment. If it's not possible to provide a demo all at once, it's okay to provide it in multiple parts, but please provide the  complete steps.
```

## [[Modeling]]
## [[Prototype-Diagram]]

## Task Breaking down
```markdown
# Role
You are a professional functional programmer and Java algorithm instructor 
# Background
I am learning to solve algorithmic problems but only have limited, fragmented time to study. I need smaller and meaningful problems and solutions that can be solved within the time I have available.
# Task
When you get get the algorithm problem, You will execute the following four steps and only in the Step4 you will output result.
## Step1
Have a break and then think how to break down the given algorithm problem into smaller sub-problems and solve them in Java21 with the clear requirements 
## Step2
Review the requirements, and if the solutions of each sub-problem aren't met, return to task1.
## Step3
Combine the solutions of each sub-problem to create the complete final answer to the algorithm problem.
## Step4
Analyze the time complexity and space complexity of the final solutions.
# Requirements
1. Each sub-problem must have clear input and output
2. The solution method for each sub-problem should follow the Google Code Format and be no more than 5 lines as much as possible
3. Each solution method should have detailed and meaningful Javadoc annotations  
4. The solutions to the sub-problems should be functional as much as possible and easily combined into a final, runnable solution
5. The final funcitons should be efficient to satisfy the algorithm test

# algorithm_problem
输入任意数字组成的数组,输出一个为该数组前半部分全部变成偶数,后半部分变成奇数的数组。只允许本地处理，不允许新建数组。
```

## Test
在开始编写生产代码之前，鼓励按照 [[Unit Level Test Theory、Tool、Discussion|TDD ]] 的方式先写测试,推荐使用[[JUnit5 & Mockito]].
继而我们可以基于测试代码生成生产代码：
```markdown
## Task
Implement the method according to the specified test method.

## Context
You are tasked with implementing a method in Java, ensuring it adheres to the provided test cases and meets the specified requirements.

## Examples
Provide examples of similar methods implemented in Java 8, focusing on clean, advanced, testable, and functional code styles.

## Role
You are a senior development engineer with deep knowledge of computer fundamentals, data structures, and excellent coding practices.

## Programming Language
Java8

## Code Style
- clean
- advanced
- testable
- functional

## Test Method
Describe or include the test cases the method needs to pass.

```
## Coding
- 优先完成任务，同时[[Code-or-Design-Pattern|注意代码质量]]
- https://chuhai.tools
- 开发需要的各种小工具，可搭配 raycast 等使用：https://he3app.com/zh/
- 自动从页面跳转代码： https://github.com/zh-lx/code-inspector

[用 LLM 构建企业专属的用户助手](https://mp.weixin.qq.com/s/bpeszhmyMC_aRHt1fb0NLA)
- 本地LLM:https://jan.ai/ 、 ollma
- API:
	- https://api.zetatechs.com/
	- https://www.closeai-asia.com/
- AI加持的terminal: warp
- RPA：LaVague
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
- 基于数据库操作表格：https://github.com/teableio/teable
- 可视化创建数据库🀄️数据：https://visualdb.com/
- 数据库数据 mock: Snaplet 
- LLM 的知识库问答系统：https://github.com/1Panel-dev/MaxKB
- 数据库 DDL 设计工具：DrawDB
#### 数据库开发工具
- https://webdb.app/page/demo
- 
#### 数据库管理工具
- bytebase:https://www.bytebase.com/zh
- 数据库、服务器二合一：https://github.com/clockworklabs/SpacetimeDB

### i18n
https://github.com/linyuxuanlin/Auto-i18n
https://www.i18next.com/

## Test & Review
对于遗留代码，或者事先不确定需求的代码，依然需要在已有代码基础上再补测试。
### Test
```markdown
  
# task  
Have a break, and then output full tests for given INPUT CODE. NOT STEPS ONLY TESTS.  
# Test Code Requirements  
  - Coverage goal: 100% branch as much as possible  
  - Code style  
    - Use class variables instead of local variables as much as possible  
    - use static imports  
  - Test types  
    - ParametrizedTest as much as possbile
    - DynamicTest when necessary for more complex scenarios
  - Avoid unnecessary annotations  
  - Naming convention: lowercase_underscore   
  - Mock requirements  
    - if face with void methods, skip tests with `doNothing()` of Mockito  
    - avoid to use `any()` directly instead of `anyString()` 、 `anyLong()` .etc  
# Dependencies  
  - Base dependencies: Java8、Junit5、Mockito  
  - Additional dependencies maybe you will use:   
    - Lombok  
    - EasyRandom   
    - Guava  
    - SpringTest  
    - Other common test dependencies  
# INPUT CODE  

# OUTPUT TEST
```

### Review
```markdown
- task: review code and modify it  
- role:你是一位优秀的软件工程师，擅长对代码进行重构.重构的目的是在不改变代码外部行为的前提下，通过优化代码结构以改善其结构、可读性、可维护性和性能等方面。  
总体而言，代码重构可以通过以下几个步骤进行:  
  1. 理解代码: 首先要深入理解要重构的代码，包括其功能逻辑和结构等方面的特点。  
  2. 设计重构计划: 根据代码的特点和需求，制定具体的重构计划。可以根据以下列举的重构方式和技术，选择适合的重构方法。  
  3. 提取函数(Extract Function): 将一段代码提取为一个独立的函数，以提高代码的可读性和可维护性。  
  4. 内联函数(Inline Function): 将某个函数调用的地方替换为函数本体，以减少不必要的函数调用开销。  
  5. 封装字段(Encapsulate Field): 将类中的字段封装起来，通过提供访问器函数来访问和修改字段的值，以提高类的封装性和灵活性。  
  6. 重命名(Rename): 通过修改标识符的名称来使代码更易于理解和维护。  
  7. 拆分临时变量(Split Temporary Variable): 将一个临时变量拆分为多个，以减少代码的复杂度和提高可读性。  
  8. 移除重复代码(Remove Duplicate Code): 通过抽象和封装来消除重复的代码，以减少代码量和提高代码的可维护性。  
  9. 引入解释性变量(Introduce Explaining Variable): 将复杂的表达式或计算过程提取为一个变量，以增加代码的可读性和可维护性。  
  10. 替换算法(Replace Algorithm): 通过使用更高效或更简洁的算法来替换现有的算法，以提高代码的性能。  
  11. 简化条件表达式(Simplify Conditional Expressions): 简化复杂的条件表达式，以提高代码的可读性和可维护性。  
  12. 简化函数调用(Simplify Function Calls): 简化函数调用的方式，以减少不必要的参数和提高代码的可读性。  
  13. 搬移函数(Move Function): 将函数从一个类或模块中移动到另一个类或模块中，以减少代码的耦合性和提高代码的可维护性。  
  14. 搬移字段(Move Field): 将字段从一个类中移动到另一个类中，以减少代码的耦合性和提高代码的可维护性。  
  15. 提炼类(Extract Class): 将一个类中的一部分代码提取为一个新的类，以提高代码的模块化和可维护性。  
  16. 提炼接口(Extract Interface): 将一个类的公共接口提取为一个独立的接口，以增加代码的灵活性和可扩展性。  
  17. 以委托取代继承(Replace Inheritance with Delegation): 使用委托方式替代继承关系，以减少代码的耦合性和提高代码的可维护性。  
  18. 抽象超类(Abstract Superclass): 将多个相关的子类中的共同部分抽象为一个超类，以减少代码的重复和提高代码的可维护性。  
  
- code_style:  
  - clean  
  - advanced  
  - testable  
  - functional  
- code:  
  
  
```

## Releasing
- goreleaser：快速、优雅地发布 Go 应用。
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
### Promotion&SEO
[[Search Engine Optimization]]
ReplyGuy: 帮你推广产品的AI回复工具
- 你的关键词、截图、描述、评分、评论、下载量、应用可靠性等都相辅相成，所以构建一个惊人的解决方案并清晰、吸引地展示它几乎是至关重要的基础。
## Practices
[[Practices-for-Processing-l0-Billion-Bill-data]]

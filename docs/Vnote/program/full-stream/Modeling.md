
建模之前首先要明确什么是业务系统，什么是领域系统。
支撑业务运营，利用领域系统赚钱 / 省钱的系统，叫做业务系统。而与运营无关的问题域就是领域，处理领域的系统是领域系统。以支付宝来说，支付是其业务系统，合同是支付协议；对于一般公司来说，支付是领域系统，是一个购买的能力，不涉及合同。
对于业务部门来说，我们建模的目的，就是寻找到恰当的业务模型，更好地支撑业务系统的构建与演进。而区分业务和领域的必要性在于业务和领域的建模方法并不相同，各自建模的同时也需要相互隔离。
这里我们先谈业务建模。
## 方法论
### 四色建模法
[四色建模法](https://www.infoq.cn/article/xh-four-color-modeling)
1. 首先以满足管理和运营的需要为前提，寻找需要追溯的事件。
2. 根据这些需要追溯，寻找足迹以及相应的<span style="color: pink;">时标性对象（moment-interval）</span>
3. 寻找时标对象周围的<span style="color: green;">人、地点、物（party/place/thing）</span>
4. 从中抽象<span style="color: yellow;">角色（Role）</span>
5. 把一些信息用<span style="color: lightblue;">描述（description）</span>对象补足
可以得到如这样的模型图：
![](https://static001.infoq.cn/resource/image/23/91/234d9ff4f3d24620d5459a0aa1f91191.jpg)

### 履约建模法

## 工具

- plantUML
- mermaid
- excalidraw
- 架构图绘制：https://www.onemodel.app/
- [[Code-Oriented-Draw]]
## Prompt模版
```
# 角色
你是一个经验丰富的电商领域Java开发工程师，精通设计模式和《Clean Code》
# 背景
你正在给一个计费系统设计添加计费比例的模型，并且已经设计好了mermaid 类图。你是在一个成熟的应用中开发，很多类库已经安装好了，比如lombok ，不需要操心依赖问题。
# 任务
现在你需要产出对应的Java代码。
# 要求
1. 类代码上标示作者是 xiaohui
2. 代码需要有全面的Javadoc注释，给字段加注释也用Javadoc,不要有inline注释，不要有同行的注释
# 设计好的mermaid类图

```
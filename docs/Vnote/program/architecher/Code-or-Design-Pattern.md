## 什么叫代码质量差？
有一类代码有着这样的症状：**难维护、易出错、开发效率低**，并且他们往往有这样的特征：
- 名字名不副实
- 超大类
- 超大方法
- 重复代码
- 扩展性差: 新增需求代码多处修改
......可谓是“好的代码总是千篇一律，不好的代码各有各的不好”。
总结起来，影响代码质量差的原因有几类：
- 编程范式问题：过于习惯过程式编程（俗称面条型代码）、没有合理运用设计模式、滥用全局变量等。
	- 重复处理流程的工作是否可以抽象成模版类
	- 新增加的能力是否可以自动注册去使用
	- 异步处理是否可以用是事件模式去处理
	- 串行处理是否需要用并发提高性能
	- ......
- 可读性问题：
	- 命名：变量、函数、类的命名没有反映其真实意图,导致代码难以理解。好的命名应该做到见名知意。
	- 代码结构：初看工程代码，模块划分、类划分、方法划分就好比书的目录一样可以反映内容的质量。
	- 代码的整洁度、注释的质量和数量、代码的复杂度等。如果代码难以阅读，那么它就难以维护和扩展。
- 扩展性问题: 当需求变更或新增时，应该尽可能地减少对现有各处代码的关注和修改。要修改尽量修改一个地方。如果代码的扩展性差，那么每次需求变更都可能带来大量的修改，这不仅会消耗更多的开发时间，也会增加出错的风险

## 为什么会代码质量差？
一般我们遇到问题更容易责怪于外部。
### 外部原因
- 项目排期紧张，只能当时怎么快怎么来
- 日常杂活太多，没有空闲时间思考重构
- 紧急问题修复，临时方案大干快上

但是内部原因往往更为本质。
### 内部原因
- 自我要求不高，仅仅满足于完成当前的需求，给后面不停埋坑，也不追求先进生产力
- 无外部反馈，团队没有代码review 氛围，代码上线质量也没有监控
## 如何提升代码质量
1. 对于面向对象的编程范式，首先考虑[[Modeling|如何转化需求到模型]]
2. 考虑后面提到的原则、思想和设计模式
3. TDD 编程或者对已有代码做到一定测试覆盖
4. 打磨重构自己的代码
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202405290851575.png)
### 通用原则
#### Brief
人类的阅读和识别能力都是有限的，根据实践我倡议每个方法、函数都应该简短，原则上不应该超过 7 行。
#### SPOT(Single Point of Truth)/DRY(Don't Repeat yourself)
尽量不要有重复的代码。有时候一味追求不重复会让代码难懂，而保证自己写的代码在需要的时候只需要改动一处，是更为实际的原则。
#### KISS (Keep It Simple, Stupid)
把简单留给别人；如果有复杂的东西，把复杂留给自己。尽可能保证代码和产品易上手，不易出错，把用户当什么都不知道的傻瓜。
### 设计原则
一般我们说设计原则，都会谈到“SOLID“：

1. **S - 单一职责原则（Single Responsibility Principle, SRP）**：一个类应该只有一个引起它变化的原因。简单来说，一个类应该只负责一项职责。

2. **O - 开放封闭原则（Open Closed Principle, OCP）**：软件实体（类、模块、函数等等）应该可以扩展，但是不可修改。也就是说，对于扩展是开放的，对于修改是封闭的。

3. **L - 里氏替换原则（Liskov Substitution Principle, LSP）**：子类型必须能够替换掉它们的父类型。也就是说，在软件中，如果使用某个基类的地方都可以使用其子类，那么子类可以替换父类。

4. **I - 接口隔离原则（Interface Segregation Principle, ISP）**：A client should not to be forced to implement an interface that it doesn't use。也就是说，一个类对另一个类的依赖应该建立在最小的接口上，这鼓励我们在**内部实现中**将大的、复杂的接口拆分为更小、更具体的接口。

5. **D - 依赖倒置原则（Dependency Inversion Principle, DIP）**：高层模块不应该依赖低层模块，两者都应该依赖其抽象。也就是说，要针对接口编程，不要对实现编程。

对其中两个扩展解释下：
#### SRP(Single Responsibility Program)

```javascript
const transform1 = (str) => {
    if(typeof str === "string"){
        return `${str.toUpperCase()}!`
    }
    return "Not a string";
}

const transform2 = (str) => {
    if(typeof str === "string"){
        return `${str.toLowerCase()}!`
    }
    return "Not a string";
}

```
有时候你觉得你的程序没有办法拆分成单一职责的,那有可能是语法工具还不够多.

有时候你觉得业务复杂，那是因为有太多业务逻辑耦合在了一起。我们需要不停地把业务逻辑分开，把公共逻辑抽象出来并合并。


```javascript
const { flow } = require("lodash/fp");
const toUpper = (str) => str.toUpperCase();
const toLower = (str) => str.toLowerCase();
const exclaim = (str) => `${str}!`;
const isString = (str) => (typeof str === "string" ? str : "Not a string");


const transform1 = flow(isString, toUpper, exclaim);
const transform2 = flow(isString, toLower, exclaim);


transform1("hello world"); // "HELLO WORLD !"
transform2("HELLO WORLD"); // "hello world !"
```
对于面向对象的设计来说，单一职责往往意味着一个对象承担一个职责。

#### LSP（Liskov Substitution Principle）
里氏替换原则强调的不仅仅是子类不能更改父类的核心，以便维护系统的健壮性；更是强调实现类不应该实现不适合的接口，比如自行车不应该实现汽车实现的“驾驶”接口。
#### DIP(Dependency Inversive Principle)
DIP 强调模块之间不应该直接依赖，而应该通过抽象产生依赖。所谓抽象，一般就是接口。这样就可以做到高层模块不因底层模块而变化，从而降低系统的耦合度，使得使用这个原则的模块可以**自闭环**。


### 思想
#### AOP
面向切面封装识别功能的收益，埋点、报表、同比环比等等。
面向切面做缓存

### 设计模式
**设计模式的本质是找到变化，封装变化。** 这个总结提到了使用设计模式的几个核心的问题：
- 什么在变化？
- 如何封装变化？
#### 什么在变化？
- 对象的属性
- 对象的行为？
- 对象的数量？
- 对象的展示？
- 对象的关系：链表、树、图状？
因为解决的对象变化问题不同，设计模式才会分为创建型、结构型和行为型。
#### 如何封装变化？
封装可以视为任何形式的对复杂度的隐藏，比如对数据、方法和类型变化的封装。
#### 具体的封装手段
配置项、接口、类、注解、插件等等。
下面再介绍一些具体的设计模式（套路）。

#### Visitor
visitor模式相当于将原本需要在多态子类中完成的方法放到统一的Visitor类中
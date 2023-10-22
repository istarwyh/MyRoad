由配置VS Code引发的思考:产品设计,LSP,DAP,VSCRD

>对于C/C++的配置, [一位](https://blog.csdn.net/bat67/article/details/76095813) 全面一些,[一位](https://www.cnblogs.com/TAMING/p/8560253.html)排版好一些.对于Java[这位](https://www.cnblogs.com/zhaoshizi/p/9524421.html)不错。
>因为认识到具体的配置细节会随着版本迭代,环境不同等原因而改变,自己在这里试着介绍与解释**一些不那么会变的东西**.

## 1. VS Code的来源与被选择
### 1.1. 为什么尝试VS Code
个人观点：

1. 统一一个开发环境支持所有语言开发
2. 细节的活暴露给开发者
3. 背靠微软和开源社区，已成为FaceBook指定开发工具
4. 足够轻巧，美观，生态丰富
5. 支持Git的可视化操作
6. 远程开发环境·`VS Code Remote Development`支持一键开发环境配置[^VSCRD]

### 1.2. 官方介绍
>Visual Studio Code is a lightweight but powerful **source code editor** which runs on your desktop and is available for Windows, macOS and Linux. It comes with built-in support for *JavaScript*, *TypeScript* and *Node.js* and has a rich ecosystem of extensions for other languages (such as *C++, C#, Java, Python, PHP, Go*) and runtimes (such as *.NET and Unity*).[^VSCode官方文档]

[^VSCode官方文档]: [VSCode官方文档](https://code.visualstudio.com/docs)

VS Code是一个跨平台的轻量级开源代码编辑器,类似于notepad++。它原生支持`JS`,`TS`和`Node.js`，而通过插件可以为其他几乎所有语言，如`C/C++、Java、Python`以及运行时`.NET`和` Unity`提供
开发环境.

### 1.3. VS Code的定位
>在Java的开发环境配置中,设计是一种取舍，idea 提供的无微不至的保姆一般的图形界面，终将会显得友好但啰嗦，会有那么一天，你成熟了，长大了，就嫌她啰里啰嗦了。<sup>[墨城之左](https://blog.csdn.net/antony1776/article/details/802983260)</sup>

#### 1.3.1. 产品定位
为了实现`1.1.1`中的优点，VS Code必须在臃肿与弱小之间中找到平衡，而其最终的定位是：**编辑器+代码理解+调试**。而代码理解和调试，除交互部分外的部分也都由第三方插件来实现，即需要自己配置编译器与调试器才能使其像一个`IDE`(Intergreted Development Environment).配置的中间协调者则是两大协议——`Language Server Protocol`(`LSP`)和`Debug Adapter Protocol`(`DAP`)。
这也赋予了VS Code具有高度的定制性,采用类似于Linux的中**一切都是文件**的设计理念,通过改变某些配置文件(如`.json`[^json])的参数可以即时改变VS Code的配置。
[^json]: `json`是一种对人类阅读友好的字符串,因而也是一种通用数据结构格式，同时它不仅支持被`JavaScript`转为对象,也被其他语言支持.

#### 1.3.2. 插件的双刃剑
若插件运行在主进程中，那么任何插件性能不佳或者不稳定都会导致IDE慢与不稳定,Eclipse深受其害;IDEA则独掌大局,自己集成插件的功能,于是逐渐取代了Eclipse.Eclipse求生之时,其团队成员主持开发了VS Code。
VS Code基于`Electron`[^Electron]的`node.js`环境开发。这一次团队没有踏进同一条河流，插件们被关在`Extension Host`进程（也即后文的`VS Code Server`）里，而UI则在`主进程`里。
所有用户的操作都被转化为各种请求发送给插件，插件只需响应这些请求即可.这迫使插件只专注于**业务逻辑**[^概述],也降低了用户在交互层面上对插件的学习成本。

[^Electron]:可以将 electron 看作是一个由 JavaScript 控制的一个小型的 Chrome 内核浏览器[^循环]。
![](https://user-gold-cdn.xitu.io/2020/5/16/1721bd2e77deba84?w=720&h=405&f=png&s=82928)

[^循环]:[electron开发入门（一）Hello world程序](https://www.jianshu.com/p/2d7bc19cae64)

## 2. 代码理解
### 2.1. 引入代码理解中间人LSP
代码理解包括悬浮提示，定义跳转，自动诊断，代码补全等功能。看似要对每个功能逐个划分模块实现，但是**深入一点**从实现原理上剖析，一般基本流程则是：**代码分析-调用功能API-实现功能**。
于是VS Code推出`Language Server Protocol`(LSP)，基于这个中间人[^中间人]，统一标准化语言工具（各种语言的`Language Server`）和代码编辑器之间的通信。如此，语言官方提供语言的分析工具（如生成`AST`）再调用VS Code提供的API即可实现代码理解。

LSP是VS Code统一实现这些功能多语言支持的基础。
[^中间人]:可以实现解耦的效果，有句玩笑话是计算机的世界没有加中间件解决不了的问题。比如很多提供`Completion`或`Hover`等功能的插件,往往也需要用到"中间人"对资源模块和代码解析模块进行整合.

![2](https://user-gold-cdn.xitu.io/2020/5/16/1721bd2b20f8f4b5?w=1162&h=538&f=png&s=71365)

### 2.2. LSP简要介绍
LSP足够高效和简单，主要是由于它基于`JSON`

它关心的是用户在编辑代码时最经常处理的物理实体（比如文件、目录）和状态（光标位置）。LSP最重要的概念是**动作**和**位置**，LSP的大部分请求都是在表达”**在指定位置执行规定动作**“。这意味着插件开发者只需要考虑何时触发，何处触发指定操作即可。
通过LSP开发属于自己的插件其实并不难，欢迎参考我们已经上线的一款DevUI组件库的插件[DevUIHelper](https://juejin.im/post/5eae73aee51d454d90750652).
## 3. 编译与调试
### 3.1. 集成的终端与命令行
VS Code集成了终端(`terminal`)[^terminal]与命令行解析器(`shell`[^shell]).当打开终端输入命令,经过`bash`[^bash]进程[^进程]解释调用编译器(如`javac`)等，结果在默认情况下会通过终端接口输出。类似的查询调用指令可以被工具自动化输入，比如`Code Runner` 可以帮开发者自动输入指令在命令行中实现编译和运行，只是某些时候开发者也需要在`c_cpp_properties.json`这样的配置文件中做一些个人的配置。而涉及到工程时，也需要配置`.classpath` `.project`等。

[^terminal]:提供基于字符的命令交互页面,以实现用户对计算机的控制.
[^bash]:Bourne Again shell,即Bourne shell的增强版,是shell的一种.
[^shell]:linux内核的一个外壳.shell接收命令,转化后传给内核,内核执行命令后将结果返回给应用程序.
[^进程]:操作系统分配的内存、CPU时间片等资源的基本单位

### 3.2. 编译的源工具
- 如C/C++中， `mingw`,`mingw64`,`clang`编译器提供的`gcc`和`g++`分别为c和c++编译器,对应调用可编译成可执行文件：
```linux
g++ Draft.cpp -o Draft
.\Draft
```
- 如Java中，Java Development Kit提供`javac`用以编译成字节码，`java`用以运行这个类
```linux
javac -encoding utf-8 Test.java 
java Test
```
### 3.3. 引入调试中间人DAP

为了防止出现用户一个插件一个学习成本的惨剧，VS Code 提供了一套通用的图形界面和交互方式，比如怎么创建断点、如何添加条件断点、如何查看当前调试状态下参数的值等。当用户在界面上完成一系列调试相关的操作时，VS Code 则通过 DAP **唤起调试插件**，由插件完成最终的操作[^debug]。
[^debug]:[VS Code 代码调试器](https://geek-docs.com/VSCode/VSCode-tutorials/VSCode-code-debugger.html)

![debugger-1](https://user-gold-cdn.xitu.io/2020/5/16/1721bd2e66c953c0?w=1164&h=521&f=png&s=76721)
### 3.4. 配置细节
#### 3.4.1. 环境变量

- `JAVA_HOME`:自己创建的路径变量,可使用`%`引用.
- `MAVEN_HOME` (or  M2_HOME):与上同理.
- `PATH`:在Windows和DOS操作系统中的环境变量，当要求系统运行一个程序而没有告诉它程序所在的完整路径时，系统除了在当前目录下面寻找此程序外，还可以的到path中指定的路径去找然后调用原本存在于计算机中的(可执行)文件.
- `CLASSPATH`:自己动手创建的，存储的是` .class `字节码文件的目录,目的在于告诉Java执行环境在哪里找到所要执行的Java程序(`.class`文件).

>一般来说,环境变量是即时生效的,但是我的经验表明重启往往是有作用的.重启都干了什么?
#### 3.4.2. 用户配置
三个级别依次递增

1. 默认/用户配置`preferences`
2. 全局配置
3. 工作区配置

当电脑就一个用户时,只用用户配置即可;对于团队项目,一些规范可以通过项目目录下建一个`.VS Code/setting.json`文件进行配置.

#### 3.4.3. 小白注意事项
>我作为小白，就被这些细节深深困扰过
##### 3.4.3.1. 文件路径
`.json`中的文件路径可以使用`/`,例如：
```linux
"miDebuggerPath":"D:/mingw64/bin/gdb.exe"
```
也可以使用`\\`,例如：
```linux
"miDebuggerPath":"D:\\mingw64\\bin\\gdb.exe"
```
##### 3.4.3.2. Readme is important
Official document is commonly more useful and comprehensiver than a host of corresponding folk versions, but is usually says too many possibly information that tends to be beyond your understanding and what you need. The [guidebook of VS Code](https://code.visualstudio.com/docs/cpp/config-msvc) seems to be more logical and complete than [its Chinese version](https://jeasonstudio.gitbooks.io/VSCode-cn-doc/content/md/Overview.html). If you want to see shortcuts that cannot  be presented in the Chinese version, you can look up the original English version.
## 4. VS Code Remote Development（VSCRD）

VSCRD支持在远程环境（比如虚机、容器）里开一个VS Code工作区，然后用本地的VS Code传输操作请求和响应进行远程开发，并且远程文件系统可以被完整映射到本地[^概述]。
[^概述]:[VS Code线程运行](https://zhuanlan.zhihu.com/p/35303567)

![](https://user-gold-cdn.xitu.io/2020/5/16/1721bd2e6754258d?w=720&h=232&f=png&s=92285)
`Extension Host`也就是`VS Code Server`,它可以在本地跑(与Mysql服务器类似)也可以在远程服务器上跑--本地通过`RPC`调用远程服务即可。
![](https://user-gold-cdn.xitu.io/2020/5/16/1721bd2e7553f4f4?w=678&h=234&f=png&s=73293)
## 5. 实际运行一些问题
```sh
1. warning:implicit declaration of function 'xxx'
```

**Reasons**
> 1. This file like **.h** file includes the declaration for the built-in function 'xxx'. If you don't do that, the compiler thinks you want to define your own function named 'xxx'.
> 2. Did not generate the **.o** object file from the **.c** file where the function is designed.
> 3. The parameter prototype of the function being dispatched does not match the real-parameter argument type.

```sh
2. Classpath is incomplete. Only syntax errors will be reported.
```
**Reasons**
>VS Code-java is unable to compute a proper classpath.<sup>[Classpath is incomplete](https://github.com/redhat-developer/VSCode-java/wiki/%22Classpath-is-incomplete%22-warning)
</sup>

**Solutions**
>You need to open a folder containing a`pom.xml`, `build.gradle` or at least default eclipse setting files, so that a complete classpath and project hierarchy can be set.<sup>[Classpath is incomplete](https://github.com/redhat-developer/VSCode-java/wiki/%22Classpath-is-incomplete%22-warning)
</sup>

个人理解就是这是一个单独文件没有存在工程中，没有`pom.xml`, `build.gradle`或者基本的eclipse配置这些配置文件，所以VS Code告诉文件所有者“我不能解析你的类关系图和项目结构，我只能给你报告语法信息”。

## 6. References

 [^VSCRD]:[Remote Development with VS Code](https://code.visualstudio.com/blogs/2019/05/02/remote-development)

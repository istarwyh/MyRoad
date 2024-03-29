

歌词经理:
>人类的语言学研究中有"语言决定论"这样的说法,在计算机世界中类似.如果我们把计算机语言解决的问题范围认为是计算机语言世界的边界:
>- python的学习者在学习计算机体系结构前难以理解C语言世界的设定
>- 对于C语言的学习者,也难以理解LISP机的运作机理,因为C语言对世界的解释和LISP 完全不同

Altman:
>语言是「压缩信息的好方法」，因此可能演化出智能。

所以如果希望语言决定论失效,就需要让不同语言的使用者去感受相同的世界.而我们学习不同的语言,**理解每一个词的本意**,才有可能靠近真正的世界.

## 1. 不同的语言特征列举
|    语言    |           仓库           |              API               |                语法                |                             语义                              |                            执行                            |                       集成                        |
| ---------- | ----------------------- | ------------------------------ | ---------------------------------- | ------------------------------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------- |
| C/C++      |                         | 头文件                          | 函数可单独存在，可使用函数指针传递函数 |                                                               | 1. 静态编译[^静态编译]                                       |                                                   |
| C++        |                         |                                | OO，函数可单独存在                   |                                                               |                                                            |                                                   |
| Java       | maven;集中式             | Class                          | OO，函数不可单独存在                 | 1. 泛型<br>2.反射<br>3.动态代理->AOP                           | 1. 字节码[^字节码] 2.JIT                                    |                                                   |
| C#         |                         |                                |                                    |                                                               |                                                            |                                                   |
| Python     | pip                     |                                | OO。函数也是对象，可作为参数传递      | 1. 不需要声明类型,Runtimes检查<br>2.运行时调用反射与修改方法或属性 | 1. 动态语言<br>2. 解释型[^解析型]->Python字节码<br>3.GIL单线程 |                                                   |
| JavaScript | npm;集中式               | 第三方                          | Webpack从入口遍历所有依赖文件翻译;Babel打包向下兼容                                   |                                                               | 1. 解释型<br>2.V8->字节码                                   |                                                   |
| TypeScript | deno;分布式从网站直接导入 | [标准库](https://deno.land/std) |                                    |                                                               | AOT                                                        | 内置打包/格式清理/测试/安装/文档生成/linting/脚本编译 |
| Golang     |                         |                                |                                    |                                                               |                                                            |                                                   |
| Rust       |                         |                                |                                    |                                                               | 静态编译                                                    |                                                   |
| Lisp       |                         |                                | 函数式                             |                                                               |                                                            |                                                   |


```flow
st=>start: 源码
e=>end: 解释执行
op1=>operation: Parser
op2=>operation: AST
st->op1->op2
op2->e
```

```flow
st=>start: 源码
e=>end: 在硬件上直接执行(加载到内存,其他同)
op1=>operation: 编译成汇编语言
op2=>operation: 机器语言模块&链接库函数(机器语言模块)
op3=>operation: 机器指令
st->op1->op2->op3
op2->e
```

[^静态编译]: 系统级编程,操作系统/网络/数据库/虚拟机首选
[^字节码]:将编译器与不同的硬件解耦,通过虚拟机/解释器跨平台
```flow
st=>start: 源码
e1=>operation: 在硬件上直接执行
e2=>operation: 在虚拟机上执行
e3=>end: 一次编写,到处运行
op1=>operation: 编译
op2=>operation: 字节码
cond=>condition: 是热点代码吗?
op3=>operation: JIT
op4=>operation: 机器指令
st->op1->op2
op2->cond(no)->e2
op2->cond(yes)->op3->op4
op4->e1
e1->e3
e2->e3
```
[^解析型]:解释型语言遍历AST解释执行
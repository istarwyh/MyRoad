

## 1. 面向对象与数据库的不协调
[分库分表常见玩法及跨库查询/事务等问题](https://www.jianshu.com/p/6f5662908dae)

## 2. Java语言特性
java语法特性差是公认的事实，对比起同样是静态OO语言的C#和kotlin半毛线的优势都没有，除了在KPI按代码行数算的公司里。
简单举几个方面你说说这不是语法特性烂是什么：
1. 变量类型没有自动推导
2. 没有属性而是用getter/setter
3. j8的stream用起来简直想死，看看C#的linq和kotlin的Sequence
4. 没有扩展方法，工具类只能一层套一层调用，有扩展方法的轻松链式风格
5. 异步支持全靠库，async/await？想都别想。


1. j9以后我知道啊，问题是别人10年前就支持了啊，后面补的只能表扬下有进步。

2. 这个不叫java的语法特性，这是预处理增强，理论上所有语言只要加上代码生成的技术都能做到。你说可以用注解，更加证明了java语法烂到一定程度才需要这样的第三方增强插件。

3. 我说的就是j8的stream api非常烂，你看看实现一个group by的要怎么写代码，然后再看看基于C#的linq或者Kotlin的Sequence是怎么写的？（这点是非常大程度取决于Java没有第四点提及的「扩展方法」功能）

4. 你说的filter那是「设计模式」不是「语法特性」，概念不清前请别抬杠。
例如Java能做到空安全的「stringValue.isEmpty()」吗？不行的，只能「StringUtils.isEmpty(stringValue)」。看看C#和Kotlin就能通过扩展方法实现「一路点下去」而不是「一个静态方法包一个静态方法」。

5. 你了解过async/await（ES7, C#）或者suspend function（kotlin）的概念没？同样道理，没了解过请别胡乱举例子。
async/await是「使用同步调用的方法调用异步过程」，是「语法特性」。
线程什么的根本就不在这个点上。
@Async和2的观点是一样的，更加印证了Java语法特性烂的这一回事。


[Spring中的设计模式](https://blog.csdn.net/caoxiaohong1005/article/details/80039656)

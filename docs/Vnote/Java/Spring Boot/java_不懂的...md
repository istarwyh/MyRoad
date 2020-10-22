

## 1. 面向对象与数据库的不协调
[分库分表常见玩法及跨库查询/事务等问题](https://www.jianshu.com/p/6f5662908dae)
#### 1.1.1. 数据库优化
[鏖兵](https://juejin.im/post/5ee193a1e51d45783d0eb8b2)
#### 1.1.2. 使用
1. [js返回json格式](https://www.jianshu.com/p/04127d74d88c)
2. [java返回json格式]
2. [后台返回json数据给前台和前台解析json数据](https://blog.csdn.net/ITBigGod/article/details/83754397)  

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
## 3. 上课挑刺儿
老师写的代码充斥着很多 for 循环，我会改成 stream，forEach 或者 map 等；null 的判断借助 Optional；复杂对象的创建采用 builder 模式；代码规范按照《Effective Java》和《阿里巴巴开发规约》进行改进
## 4. Java中的那些坑
List不能直接一边遍历一边删除？因为for each循环每次都会调用 Iterator，然后修改modCoun，最后造成与expectedModCoun不等报错。
https://zhuanlan.zhihu.com/p/146995089
- 使用Iterator的remove()方法
- 使用for循环正序遍历（i = i-1 ) 
- 使用for循环倒序遍历
- stream+filter
- list.removeIf(s -> s.contains("要删除的"));
## 5. 函数式接口传递代码块
https://zhuanlan.zhihu.com/p/166566005

https://blog.csdn.net/itguangit/article/details/78623964     
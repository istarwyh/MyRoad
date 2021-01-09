## 1. 就是错误之坑
### 1.1. Stack
https://www.cnblogs.com/cosmos-wong/p/11845934.html
基于 Vector 实现的栈 Stack。底层实际上还是数组，所以还是存在需要扩容。Vector 是由数组实现的集合类，它包含了大量集合处理的方法。而 Stack 之所以继承 Vector，是为了复用 Vector 中的方法，来实现进栈（push）、出栈(pop)等操作。这里就是 Stack 设计不好的地方，既然只是为了实现栈，不用链表来单独实现，而是为了复用简单的方法而迫使它继承 Vector，Stack 和 Vector 本来是毫无关系的。这使得 Stack 在基于数组实现上效率受影响，另外因为继承 Vector 类，Stack 可以复用 Vector 大量方法，这使得 Stack 在设计上不严谨。
## 2. 有意为之之坑
## 3. 不好说之坑
### 3.1. List
List不能直接一边遍历一边删除？因为for each循环每次都会调用 Iterator，然后修改modCoun，最后造成与expectedModCoun不等报错。
https://zhuanlan.zhihu.com/p/146995089
- 使用Iterator的remove()方法
- 使用for循环正序遍历（i = i-1 ) 
    - 删除一个元素后,其后续元素的索引都往左挪一位,即`-1`,因此i也需要往[0]处`-1`,指向被删除元素的前一位,保持与list缩小的同步
    - 之后`i++`是正常的迭代过程,`i++`后的指向的是被删除元素的下一个元素
- 使用for循环倒序遍历
    - 倒序遍历时,`i--`的下一位就是被删除元素的下一个元素,因此不需要做多余操作
    - 另一个方面也可以解释为指针`i`指向的元素被删除后,`i`因为来自右端,所以退回也是右端,即被删除元素的前一位,当被删除元素的前一位往左移动后,`i`恰好保持了与list元素移动的同步
- stream+filter
- list.removeIf(s -> s.contains("要删除的"));

https://www.cnblogs.com/maoyali/p/8805975.html
## 4. 与操作系统之坑
### 4.1. 操作系统命令行长度限制
1. 在容器中运行测试类报错：
>Error running 'ServiceStarter': Command line is too long. Shorten command line for ServiceStarter or also for Application default configuration.

2. [解决办法](https://stackoverflow.com/questions/47926382/how-to-configure-shorten-command-line-method-for-whole-project-in-intellij):

```xml
Inside your .idea folder, change workspace.xml file

Add

<property name="dynamic.classpath" value="true" />
to

  <component name="PropertiesComponent">
.
.
.
  </component>
Example

 <component name="PropertiesComponent">
    <property name="project.structure.last.edited" value="Project" />
    <property name="project.structure.proportion" value="0.0" />
    <property name="project.structure.side.proportion" value="0.0" />
    <property name="settings.editor.selected.configurable" value="preferences.pluginManager" />
    <property name="dynamic.classpath" value="true" />
  </component>
If you don't see one, feel free to add it yourself

 <component name="PropertiesComponent">
    <property name="dynamic.classpath" value="true" />
  </component>
```
3. **原理**
该选项控制如何将classpath传递给JVM：通过命令行或通过文件。大多数操作系统都有最大的命令行限制，当它超过时，IDEA将无法运行您的应用程序。 当命令行长于32768个字符时，IDEA建议您切换到动态类路径。长类路径被写入文件，然后由应用程序启动器读取并通过系统类加载器加载。 如果您对实施细节感兴趣，可以查看IDEA社区版的源代码，JdkUtil.java文件，setupJVMCommandLine方法。
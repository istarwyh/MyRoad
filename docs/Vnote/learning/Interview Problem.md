## 务实
1. [帖子1](https://www.nowcoder.com/discuss/364739)
2. 关于私有方法的测试,你有什么看法?平常你是怎么做的?这个没有固定答案,主要看你对单元测试的思考.
3. 某些时候你对代码的优化可能使得你发现了错误或者提高了下一次需求的开发效率.你有过这样的时候吗?可以举个例子吗?(即如何利用了自己学到的知识)
业务方需要查看更多的字段信息,本来以为需要一顿操作,但因为之前定义的方法职责很清晰,所以简单替换了一个方法名就完成了该需求.

4. IPC 和 RPC 的实现方式有哪些，有什么不同，为什么会有这些不同？
5. UDF 在数据库中如何运行机制如何，对其使用方法有什么启示？
## 务虚

### 过去
- 在你共事过的人中,你钦佩谁,为什么?
九霄,因为他技术好,发明了Alimock全链路mock数据的测试框架,更因为他始终对自己的作品保持谦虚,也不用它过分争取奖励.

- 你在哪件事上发挥了超常的主动性?追问:能再给我讲一个吗?
对于业务未来方向的思考,包括和合作上下游沟通请教,从客户那里发现痛点,从日志和监控知道当前系统的不足等.
对许多事都想有提前的规划,比如百年阿里和百年技术中带头开启讨论项目和节目主题.

- 你现在的工作好在哪里?为什么?
大平台本身的资源和基础设施完善,工作中优秀的人,以及工作的内容会接触到各种技术栈.

- 你过去的成就?
独立地提出观点,实现这个观点;进而提出更大的想法,组织更多的资源实现它.

- 你是如何准备这次面试的?
我整理好自己,于是也就有了面对任何面试的准备.那我是怎么整理自己的呢?...

- 你认为当前职位最需要的3个特质是什么?你认为自己哪个最强,哪个最弱?
负责,对自己的代码,负责过的事情负责;好奇,好奇这个需求是怎么来的,合理吗,好奇每一行代码是怎么运行的?规划,明白这个岗位为什么存在,现在的价值在哪,之后要做什么?
我最负责,但最缺乏确定的规划,常常纠结,对价值本身理解不清.

- 你与你的经理意见相左过吗?你是怎么让他相信你是对的? 数据证明

- 分享一下你曾经遇到过的最好的和最差的老板,他们有什么不同?

- 你把工作搞砸过吗,你是如何处理的?多分享几个?
上下游不够了解,因为粗糙的技术方案延期.(职责范围越小,犯的错误也就越小,甚至不会犯错)

- 你的同事对你有什么误解?
我认为,当然只是猜测,他们会觉得我是一个内卷的人,但其实我只是想把份内工作做好,偶尔会在公司自主探究更多.

- 那些对你评价不高的人一般会说些什么?
不够主动,对业务思考不够.


### 未来
- 你想在下一个职位做些什么不同的事情?

- 3年后的你和现在的自己有什么不同?

- **有什么事情你很擅长,但你再也不想做了?**

- 你希望将你以前公司文化的哪一部分带到你的下一个公司?哪些你不希望有? 
对业务价值的思考与鼓励主动立项的文化.不希望在这个过程中忽视团队中的其他人以及每个人的个人成长.

- 你会给其他的面试者什么经验?
### 工作过渡期
>只有在两份工作之间的间隙,人们才能完全独立地做出决定.没有哪个团队成员可以窃取他们的功劳,或者为他们出力.在他们的职业生涯中,这是唯一一次你能深入了解他们想法和动机的机会. -- Alex Austin

- 关于你最近工作的几家公司,带我了解一下:(1)你为什么离开?(2)你为什么选择了下一个?
   重点在于如何**讲一个好故事**
   
- 我们为什么不该聘用你?
如果你们想要的只是一个埋头干活的工人,那就不应该聘用我.

- 关于在这里工作,你希望我告诉你什么?


## 2. 网络
- 三次握手四次挥手
- 输入网址发生了什么
- `http`报错402,502,505什么的
- http/https
## 3. 操作系统
几大组成
堆和栈
pagecache
## 4. Java
- 多线程如何实现
- Jvm内存泄漏

![阿里笔试题](20200711094550079_21093.jpg)
## 5. 数据库
1. 高考考分数据库如何设计 
    - 加的是什么索引 
    - 数据库数据怎么导入

## 6. 框架
遇到哪些问题,怎么优化
[hashMap](https://www.jianshu.com/p/3094437bc819)
[组合模式以及HashMap的设计分析](http://www.manongjc.com/detail/17-vpesnqkfshssbzf.html)
## 7. 算法
- LRU https://blog.csdn.net/Apeopl/article/details/90137398
- 100亿个数据如何排序

案例:
```java
1 下面程序期望通过过设置stop=true，使得线程B停止，但程序未按照期望执行，请说明为什么，如下提供了三个选项，哪些选项（多选）能实现当设置stop=true，线程B停止
public class CPUCacheTest {
  private static boolean stop = false;
  public static void main(String[] args){
    Thread a = new Thread("B"){
      public void run(){
        while (!stop) {
          int a = 1;
        }
        System.out.println("exit");
      }
    };
    a.start();
    pause(100);
    //停止标记，但未能停止线程B
    stop = true;
  }
  public static void pause(int time){
    try {
      TimeUnit.MILLISECONDS.sleep(time);
    }catch(Exception ex){
    }
  }
}
a stop变量使用volatile 关键字
b 在线程B的while语句里，增加pause(1)
c 在线程B的while语句里，增加 System.out.println(stop)
2  下列代码来自俩个日志输出框架Slf4j和common log，前者有占位符，后者字符串拼接，你觉得哪个好，为什么spring源码使用了后者
log.info(" order id {}，user id {}",order.getId(),order.getUserId());
log.info(" order id "+order.getId()+"，user id "+order.getUserId());
3  while (true) 和 for (;;) 哪个运行快,为什么，如何验证？
4 请列出下面代码需要调整的地方
Connection conn = null;
PrepareStatement ps = null;
try {
   conn = .....
   ps = conn.prepareStatement(sql);
   this.setPreparedStatementPara(ps, objs);
   int rs = ps.executeUpdate();
   return rs!=0;
} catch (SQLException e) {
  e.printStackTrace();
} finally {
  try{
    if(ps!=null){
      ps.close()
    }
    if(conn!=null){
      conn.close();
    }
  }catch(Exception ex){
    ex.printStackTrace();
  }
}
```
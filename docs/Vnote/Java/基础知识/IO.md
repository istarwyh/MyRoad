## 1. IO的设计
对称性: 

- InputStream/OutputStream 读取字节流
- 为了避免文本翻译成字符,又提供了Reader/Writer


## 2. 文件IO

- `File`可以封装文件路径,如`java.io.File file = new java.io.File("in.txt");`和`Path.get()`类似
- 然后重定向输入输出流
```java
import java.io.*;
import java.util.*;
public class Test {
    public static void main(String[] args) throws Exception{
        File sourceFile  = new File(args[0]);
        File targetFile = new File(args[1]);
        if (!sourceFile.exists() || !targetFile.exists()) return;

        Scanner in = new Scanner(sourceFile);
        PrintWriter out = new PrintWriter(targetFile);
        while (in.hasNext()) {
            String s = in.nextLine();
            String s2 = s.replaceAll(args[2],args[3]);
            out.println(s2);
        }
        
        in.close();out.close();
    }
}
```
## 3. 网页IO
File封装换成了`URL`封装,以及需要调用对象的`openStream()`方法来打开输出流.文件不需要大概是因为本机访问便利.

```java

import java.io.*;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.*;
public class Test {
    public static void main(String[] args) throws Exception{
        System.out.println("Enter a url:");
        Scanner input = new Scanner(System.in);
        String URLString = input.next();
        try {
            URL url = new URL(URLString);
            Scanner in = new Scanner(url.openStream());
            PrintWriter out = new PrintWriter("out.txt");
            while (in.hasNext()) {
                String line = in.nextLine();
                out.println(line);
            }
            in.close();out.close();
        }
        catch (MalformedURLException ex){
            System.out.println(ex.toString());
        }
        catch (IOException ex){
            System.out.println(ex.toString());
        }
       
        input.close();
    }
}
```
## 4. IO中的阻塞问题

```java
BufferedReader reader = .....
String name = reader.readLine();
```
当一个线程执行这段代码,readLine()方法等待过程中就会被阻塞,实际处理如下:
```java
BufferedReader reader = .....
String name = reader.readLine();
while(数据还没准备好){  
继续等待
}
处理数据
```
对于传统的阻塞问题,没有什么好的解决办法,列举几种:

1.  客户端在`write()`后马上`close()`关闭输出流，服务器不阻塞
    - 缺点:除非协议是单向的
2. 发送方返回数据长度,接收方接收完后退出`read()`
    - 缺点:如果不是一次返回总的数据长度,每次都告知发送长度,开销大也麻烦
3. 用指定大小数组当容器,当数组未读满时,退出`read()`
```java
//如果读的长度小于1024，说明是最后一次读,后面已经没有数据，跳出循环
if (len < 1024) {
	break;
}
```

4. 指定等待时间,超时退出`read()`
5. 商定结束标志,如`\n`或`EOF`等
6. 直接写入结束标志,如`socket.shutdownOutput()`
    - 缺点:后面将无法交互
## 5. NIO(New IO)
但是面对成百上千个文件需要读取,就必须改成非阻塞的,即允许同时处理---通过轮询操作循环检查是否可以关闭输入流.

**文字解释**
具体的实现方式如NIO(`non-blocking IO`),使用一个线程管理高并发的多个socket连接线程.

- Channel: 类似Stream,但是NB.将一个socket视为Channel的一种
- Buffer:Channel读取的数据在Buffer中.而由于Buffer不是流,Channel可以任意位置读
- Selector:和Channel 配合使用,Channel注册自己及自己监听的`XXX事件`在Selector中

**代码解释**
```
每一个Channel注册一个Channel在Selector

while(true){  
    检查Selector有没有事件
    if(事件是Channel可读){  
        从产生事件的Channel中读取数据
    }
    if(事件是Channel可写){  
        从产生事件的Channel中写入数据
    }
    ...
}

```
**图表解释**
![](https://gitee.com/istarwyh/images/raw/master/1589943618_20200520105140401_10630.png)
### 5.1. NIO的易用性
因为JDK 原生API开发NIO需要开发者理解`Selector`、`Channel`和`ByteBuffer`三大组件,编程模式较为复杂;原生实现也不够稳定.所以应运而生`Netty`,将三大组件封装在了内部.
`dubbo`、`spark`、`zookeeper`和`elasticSearch`等框架使用`Netty`作为底层通信IO框架.
## 6. 常用服务器运行软件
对于服务器往往会接触多个线程与对象.
### 6.1. Ngnix
- 静态连接处理
- 维持动态连接,将连接请求分发给Tomcat集群
### 6.2. Tomcat
Web容器(`Jetty`和`Tomcat`)对用户的每个请求都会从线程池中取出一个单独的`servlet`线程去处理请求.同一时刻,可能有多个线程在处理多个请求:

- 执行`servlet`代码
- 访问Database
- 访问微服务
- 本地IO(1~10ms)
- 网络IO(例如http服务,30~100ms)

线程往往只能阻塞.而垃圾服务器,CPU核心一般不超过4核,线程之间还需要常常切换.
### 6.3. Spring框架
1. Spring的单例保证在一个IOC容器中只有一个action单例,从而保证开发者使用的是同一个实例.
2. 对于Dao层的bean对象,Spring对于`ThreadLocal`对象,区别常用的加锁方式,用空间换时间--给每个线程分配独立的变量副本,从而隔离多线程访问对于数据访问的冲突.
3. 不过Spring没有保证所有Bean的线程安全.

实际工作中,直接使用Java IO的API来操作IO并不常见.通常配置文件(`xml`,`properties`等)被框架读取后,框架会把**配置数据**变成**Java对象**方便程序员调用,而不用程序员关注IO细节.
### 6.4. Redis
因为对Redis的请求都是访问内存,不涉及文件/DB/IO等操作,时间很短(`100ns`左右),所以用同一线程就可处理多个请求.同时也避免了线程切换的开销和共享数据的问题.
### 6.5. Node.js
事件驱动编程

- 对于大部分非阻塞的线程,用一个线程处理所有请求,遇到耗时长的IO等操作,留下回调函数,等到I/O操作完成后再去执行回调函数
- 个别会阻塞的线程,例如Linux文件I/O,CPU密集型的任务如加密/压缩等,从线程池拿线程


#### 6.5.1. 代表产品
Electron桌面开发
高并发异步减少线程
#### 6.5.2. 缺点
- 单一主线程不能发挥多核优势
- 对于计算密集型的项目应当考虑其他技术
### 6.6. Node.x->Vert.x

Verticle循环关联事件:
```java
接收事件
    加入事件队列
while(true){
    获取下一个事件
    找到下一个handler
    调用handler
}
```
Handler,以http服务器为例:
```java
import io.vertx.core.AbstractVerticle;
public class Server extends AbstractVerticle{
    public void start(){
        vertx.createHttpServer().requestHandler(req ->{
            res.response()
                .putHeader("content-type","text/plain")
                .end("Hello Word!");
        }).listen(8080);
    }
}
```
这样就摆脱了对Tomcat这些容器的需求,自己就完成了一个HTTP服务器.

但是必须全是异步操作,如同时有服务器运行和数据库查询:
![](https://gitee.com/istarwyh/images/raw/master/1596795258_20200608162028899_15414.png)
数据库的`Verticle`代码:
```java
public class DatabaseVerticle extends AbstractVerticle{
    ......
    dbClient.getConnection(ar -> {
        if (ar.succeeded()) {
            SQLConnection connection = ar.result();
            connection.query("select .. from...", res -> {      
                 if (res.succeeded()) {
                   ......
                 } else {
                   ......
                 }
          });
        } else {
           ......
        }
    });
}
```

同时这里使用了`Event Bus`，让不同的`Verticle`之间通过消息传递.因为不用共享内存,所以不用加锁.
## 7. 其他
### JdbcType类型和Java类型[^Jdbc]
[^Jdbc]:[JdbcType类型和Java类型的对应关系](https://blog.csdn.net/qq_39019865/article/details/80800649)

![](https://gitee.com/istarwyh/images/raw/master/1596795260_20200807181300916_6888.png)
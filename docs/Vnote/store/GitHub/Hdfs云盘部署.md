[toc]

----
## 1. 项目说明[^源]
[^源]: [基于hadoop （hdfs）+ javaweb （ssh）实现的网盘](https://blog.csdn.net/LEoe_/article/details/71194440?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-8)

这是一个基于Hadoop的云盘系统，实现的界面是用javaweb完成的，使用的是`Spring Struts2  hibernate`集合框架，配有sql文件。

其实[同类型](https://shuwoom.com/?p=1353)的项目有很多,有的也写的很详细,只是都太久远了(17年以前...).
## 2. 部署环境

### 2.1. Linux安装mysql
>ubuntu使用`APT`仓库;centos使用`YUM`仓库。

1. 在MySQL官网`ART`仓库中找到下载链接:
 `sudo wget https://dev.mysql.com/get/mysql-apt-config_0.8.15-1_all.deb`
 2. 安装MySQL并配置
 注意结尾为`deb`,为`Debian`系发行版,可使用Debian Package,即`dpkg`来安装这个主面板,`-i`应该是`--install`的短命令版本:
 `sudo dpkg -i mysql-apt-config_0.8.15-1_all.deb`
 - `Enter`之后可以选择**5.7版本**,这里才开始下载服务器端`mysql-server`(默认就是5.7),可以指定版本:`sudo apt-get install mysql==version`

3. 开始更新安装
- `sudo apt-get update`
- `sudo apt-get install mysql-server`
密码:`mysql`
4. 验证安装
- MySQL安装完成后默认是启动的,以下命令分别是查看mysql状态|启动mysql|停止mysql|重启mysql
`sudo service mysql status|start|stop|restart`
- 或`mysql -u root -p` ,然后`show databases`
- 或`sudo netstat -tap | grep mysql`

#### 2.1.1. 导入数据库
1. 建空数据库
`create database netcloud;`
2. 导入数据库
- `use netcloud;`
- `set names utf8;`
- `source /home/netcloud/netcloud.sql;`
### 2.2. Linux安装Tomcat
下载解压与安装mysql类似,然后[^tomcat配置]:
[^tomcat配置]: [tomcat安装](https://blog.csdn.net/weixx3/java/article/details/80808484)
- 赋权限,755--可读可写可使用:`sudo chmod 755 -R apache-tomcat-7.0.104`
- 修改`bin`目录下的启动脚本:` vim startup.sh`,在最底下加入路径设置,路径设置为个人的实际路径

```sh
#set java environment
export JAVA_HOME=/usr/lib/jvm/jdk1.7.0_67
export JRE_HOME=${JAVA_HOME}/jre
export CLASSPATH=.:%{JAVA_HOME}/lib:%{JRE_HOME}/lib
export PATH=${JAVA_HOME}/bin:$PATH

#tomcat
export TOMCAT_HOME=/usr/local/bin/apache-tomcat-8.5.54
```
- 启动: `./startup.sh`,可在`8080`端口验证

**多出来的几步**应该与

- 启动所需的依赖
- 开发者有没有自动化自身环境配置

有关系

**注意**
Eclipse默认对于6/7/8只提供整数版本的`Server Adapter`,所以最好还是下整数版本的tomcat.导入的项目是v7.0的.
### 2.3. Linux安装IDEA
1. 学生账户名`istarwyh`,登陆后可以下载linux版放到虚拟机`share`目录.如果在ubuntu内下载,文件夹在`/home/[用户名]/Downloads`或`/home/[用户名]/下载`
2. 解压文件并进入`bin`目录
- `sudo tar -zxvf 文件名 -C /opt`
- `cd /opt/文件名/bin`
3. 启动安装向导并允许创建启动脚本
- `./idea.sh`
- 勾选`Create Launcher Script`,记下脚本所在目录,如 `/usr/local/bin/idea`

>不是学生版破解[可见](https://blog.csdn.net/qq_40950957/article/details/81386387)
**注意**
如果内存不够,务必牺牲一些idea的功能换取流畅运行:

- 卸载/关闭不需要用的插件
- 根据自己的项目依赖只导入自己相关的`Modules`(`Project Structure`中)
- 修改`bin`中的配置文件中内存堆大小:
    - `idea.exe.vmoptions`
    - `idea64.exe.vmoptions`
- 调低`Inspection`级别--右下角的人头标志
### 2.4. 搭建HDFS存储平台
注意其实只要有`hdfs`就够了
### 2.5. IDEA整合hadoop开发环境
#### 2.5.1. 没有整合之前
Java类被编译成一个或多个字节码`.class`文件，打包到`.jar`文件中，Java虚拟机从相应的`.class`文件和`.jar`文件中获取相应的字节码,故以`WordCount.java`为例的核心运行步骤:

1. 编译:`hadoop com.sun.tools.javac.Main WordCount.java`
2. 打成`wc`jar包:`jar cf wc.jar WordCount*.class`
3. 上传文件:`hadoop fs -copyFromLocal ja.txt /user/用户名/wordcount/input`
4. 运行jar包:`hadoop jar wc.jar WordCount /user/用户名/wordcount/input/ja.txt /user/用户
名/wordcount/output`
5. 查看输出文件`hadoop fs -cat /user/用户名/wordcount/output/part-r-00000|more`
#### 2.5.2. 第一种[^1]
[^1]: [第一种把所有jar包都引入](https://blog.csdn.net/Ding_xiaofei/article/details/80376049)

1. `project structure--modules--dependencies`中引入Jar包;也即Eclipse中的`build path`:
    - ”/usr/local/hadoop/share/hadoop/common”目录下的hadoop-common-2.7.1.jar和haoop-nfs-2.7.1.jar；
    - `usr/local/hadoop/share/hadoop/common/lib`目录下的所有JAR包；
    - `/usr/local/hadoop/share/hadoop/hdfs`目录下的haoop-hdfs-2.7.1.jar和haoop-hdfs-nfs-2.7.1.jar；
    - `/usr/local/hadoop/share/hadoop/hdfs/lib`目录下的所有JAR包。

2. eclipse下`bin`目录，idea下`src`目录放置`core-site.xml`和`hdfs-site.xml`(默认`/hadoop/etc/hadoop`)
看一下`core-site.xml`提供了什么信息:
- core-site.xml:
```xml
<configuration>
    <property>
        <name>fs.default.name</name>
        <value>hdfs://master:9000</value>
    </property>
</configuration>
```

#### 2.5.3. IDEA整合之后

**背景信息**

- NameNode :`IP:192.168.56.100` NetMask: 255.255.255.0
- MasterNode port:9000
- jobtracker port:54311(是`yarn`吗?因为其实没有计算任务,云盘用不到这个)

1. 引入依赖
修改`pom.xml`，添加Hadoop依赖[^远程HDFS文件的操作]


```xml
<dependencies>
    <dependency>
        <groupId>org.apache.hadoop</groupId>
        <artifactId>hadoop-common</artifactId>
        <version>${cdh.hadoop.version}</version>
    </dependency>
    <dependency>
        <groupId>org.apache.hadoop</groupId>
        <artifactId>hadoop-client</artifactId>
        <version>${cdh.hadoop.version}</version>
    </dependency>
</dependencies>

```

2. 打成jar包
通过构建`Artifacts`的方式装载项目资产,maven可主动找到主类.开发者将项目依赖的jar包导入,最后合成一个jar包.以下为具体步骤:
对项目名称右击，选择`Open Module Settings`->`Artifacts`->`Jar`->`include in project build`->`Build Artifacts`

**注意**

如果按传统方式(即`1.4.1`中所述)运行jar包,打`jar`时一定不要选择主类[^IDEA开发hadoop项目配置及打包]

[^IDEA开发hadoop项目配置及打包]:  [IDEA 开发hadoop项目配置及打包](https://blog.csdn.net/a377987399/article/details/80510776?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-8&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-8)

#### 2.5.4. IDEA连接Hadoop
//TODO:问题是真的是连接而不是导入包吗?我也不知道....
#### 2.5.5. 客户端其他用途
HDFS文件的操作用到类有以下3个：

- org.apache.hadoop.conf.Configuration[^远程HDFS文件的操作];
- org.apache.hadoop.fs.FileSystem;
- org.apache.hadoop.fs.Path;

以`Configuration`类为例,客户端可以配置文件的切块大小和存储的副本数量[^hdfs客户端的理解，多种方式操作hdfs]:

- 切块大小的参数:`dfs.blocksize`
- 副本数量的参数:`dfs.replication`

代码示例:

```java
 Configuration conf = new Configuration();
    conf.set("dfs.replication", "2");
    conf.set("dfs.blocksize","64m");
     fs= FileSystem.get(new URI("hdfs://hdp-01:9000"),conf,"root");
```
### 2.6. (window客户端访问HDFS) 
#### 2.6.1. 说明
HDFS的客户端有多种形式：

- 网页形式
- 命令行形式
- APP客户端

客户端在哪里运行没有约束，只要运行客户端的机器能够跟Linux中的HDFS集群**通信**即可[^hdfs客户端的理解，多种方式操作hdfs].在本地windows上开发一个Java客户端,利用HDFS提供的Java API[^hdfs客户端的理解，多种方式操作hdfs]可实现访问HDFS.
[^hdfs客户端的理解，多种方式操作hdfs]: [hdfs客户端的理解，多种方式操作hdfs](https://blog.csdn.net/weixin_38223828/article/details/78265384?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-4&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-4)

但是windows下远程连接linux下的hadoop集群不像在linux下直接配置IDE一样方便,**处处是坑**,并且似乎不是简单的通信,运行MapReduce还需要Windows本地环境        [^windows下eclipse远程连接linux下的hadoop]，
[^windows下eclipse远程连接linux下的hadoop]:[windows下eclipse远程连接linux下的hadoop集群并测试wordcount例子](https://blog.csdn.net/qq_32166627/article/details/53055811?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-22&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-22)

#### 2.6.2. 具体操作
>很可惜,并没有找到具体通信的资料,不过有步骤的参考文章

##### 2.6.2.1. 确保hadoop已正确发布 [^hadoop正确发布]

[^hadoop正确发布]: [hadoop正确发布](https://blog.csdn.net/daxiang12092205/article/details/52717598?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-18&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-18)

>启动`start-dfs.sh`

1. 确定Linux内可以通过IP访问集群,如:
![](https://gitee.com/istarwyh/images/raw/master/1590158515_20200507164752616_6700.png)

2. 确定Windows host主机可以与Linux中的master通信,可在 `C:\Windows\System32\drivers\etc`的`hosts`添加IP与节点名映射[^在Windows下使用浏览器访问HDFS的小问题]:
[^在Windows下使用浏览器访问HDFS的小问题]:[在Windows下使用浏览器访问HDFS的小问题](https://blog.csdn.net/Andrea_null/article/details/83025329)

![](https://gitee.com/istarwyh/images/raw/master/1590158513_20200507164512132_6480.png)
之后访问`master:57900`.**必须注意**,交换机与主机之间的连接有三种模式[^Vmware三种网络模式详解],为了集群之间的统一往往用`Host-only`模式,这时候在开始配置虚拟机网卡时,需要让主机的虚拟网卡通过虚拟交换机与`master`的虚拟网卡连接.

[^Vmware三种网络模式详解]: [Vmware虚拟机三种网络模式详解](https://www.cnblogs.com/Genesis2018/p/8304738.html)

3. 通过master机上命令行，可正常上传、删除文件/文件夹；

4. 检查hadoop集群master/slaver的日志(默认位置`$HADOOP_HOME/logs`)，看是否有报错信息.


#### 2.6.3. 类比
也可以在`Windows`搭建Hadoop开发平台[^Windows系统上搭建Hadoop伪分布式环境].回顾Hadoop的三种运行模式:
[^Windows系统上搭建Hadoop伪分布式环境]: [Windows 7 64位系统上搭建Hadoop伪分布式环境](https://blog.csdn.net/u013159040/article/details/81939662)

- 独立（或本地）模式：无需运行任何守护进程，所有程序都在同一个JVM上执行。在独立模式下测试和调试MapReduce程序很方便，因此**该模式在开发/作业阶段比较适合。**
- **伪分布式(yarn)模式:** Hadoop守护进程运行在本地机器上，模拟一个小规模的集群
- **全分布式(yarn)模式:** Hadoop守护进程运行在一个集群上


## 3. 编译云盘程序
### 3.1. 源代码上传
1. 源代码NetCloud.rar解压缩为一个文件夹NetCloud并上传至master主机`/root`
### 3.2. 配置依赖
>注意Linux的一般目录结构:

```linux
/bin 用来贮存用户命令。/usr/bin 也被用来贮存用户命令。  
   
/sbin 许多系统命令（例如 shutdown）的贮存位置。/usr/sbin 中也包括了许多系统命令。  
   
/root 根用户（超级用户）的主目录。  
   
/mnt 该目录中通常包括系统引导后被挂载的文件系统的挂载点。比如，默认的光盘挂载点是 /mnt/cdrom/。可以为U盘建一个挂载点：mkdir /mnt/usb。  
   
/boot 包括内核和其它系统启动期间使用的文件，最大不超过200M。通常我为 /boot 建一个primary分区是1G。  
   
/lost+found 被 fsck 用来放置零散文件（没有名称的文件）。  
   
/lib 包含许多被 /bin 和 /sbin 中的程序使用的库文件。目录 /usr/lib 中含有更多用于用户程序的库文件。/lib 目录下放置的是 /bin 和 /sbin 目录下程序所需的库文件。简单说，/lib 是内核级的；/usr/lib 是系统级的；/usr/local/lib 是用户级的。仅仅被 /usr 目录下的程序所使用的共享库不必放到 /lib 目录下。只有 /bin 和 /sbin 下的程序所需要的库有必要放到 /lib 目录下。  
   
/dev 贮存设备文件。  
   
/etc 包含许多配置文件和目录。  
   
/var 用于贮存variable（或不断改变的）文件，例如日志文件和打印机假脱机文件，虚拟机镜像文件等。  
   
/usr 包括与系统用户直接有关的文件和目录，例如应用程序及支持它们的库文件。  
   
/proc 一个虚拟的文件系统（不是实际贮存在磁盘上的），它包括被某些程序使用的系统信息。  
   
/initrd 用来在计算机启动时挂载 initrd.img 映像文件的目录以及载入所需设备模块的目录。不要删除 /initrd 目录。如果你删除了该目录后再重新引导 Red Hat Linux 时，你将无法引导你的计算机。  
   
/tmp 用户和程序的临时目录。 /tmp 给予所有系统用户读写权。  
   
/home 用户主目录的默认位置。  
   
/opt 可选文件和程序的贮存目录。该目录主要被第三方开发者用来简易地安装和卸装他们的软件包。
```
#### 3.2.1. JDK
- `java-11-openjdk-amd64`目录:

```
bin  conf  docs  include  jmods  legal  lib  man  release

```

-  `jdk1.7.0_71`目录:

```
bin        jre          src
COPYRIGHT  lib          THIRDPARTYLICENSEREADME-JAVAFX.txt
db         LICENSE      THIRDPARTYLICENSEREADME.txt
debug.log  README.html
include    release

```

##### 3.2.1.1. terminal的颜色
`/usr/lib/jvm/`目录下有不同颜色`jdk`包:
![](https://gitee.com/istarwyh/images/raw/master/1590158516_20200520173459019_15957.png)

- 复制:即复制了一个副本,独立于原文件,类似于深拷贝
- 硬链接: 在复制的基础上还指向同一个内存地址
- 软链接:只是一个引用,类似于浅拷贝或者`=`

<font color="white">白色</font>: 普通文件
<font color= "blue">蓝色</font>: 目录文件/文件夹
<font color= "red">红色</font>：压缩文件  
<font color= "green">绿色</font>：可执行文件  
<font color= "red">红色闪烁</font>：错误的符号链接
<font color= "skyblue">淡蓝色</font>：符号链接
<font color= "yellow">黄色</font>：设备文件
<font color= "purple">紫色</font>：图片文件
<font color= "gray">灰色</font>:其它文件

所以终于发现了`jdk1.7.0_71`颜色的不对劲,应该是因为擅自解压了`src.zip`:

- 正确的`jdk1.7.0_71`目录:
```
bin        lib          src.zip
COPYRIGHT  LICENSE      THIRDPARTYLICENSEREADME-JAVAFX.txt
db         man          THIRDPARTYLICENSEREADME.txt
include    README.html
jre        release
```
而实际上最后使用的是`jdk1.7.0_67`....我真的不知道自己当初怎么看的
##### 3.2.1.2. 切换JDK
本项目使用的是`jdk1.7.0_71`,等价`JDK7` /J2SE7 /JAVA7.

1. 环境变量(`/etc/profile`(所有用户的变量)或`~/.bashrc`(当前用户的变量))如下:

- `export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64`
- `export JAVA_HOME=/usr/lib/jvm/jdk1.7.0_71`

不确定是否需要改动,但是真正起作用的其实是`/usr/bin/`底下的`java`/`javac`这些引用的指向,这个指向可以使用下面的命令改变

2. **必须**参考使用[这篇](https://www.cnblogs.com/a2211009/p/4265225.html)的命令.

- `update-alternatives --install gen link alt pri [--slave sgen slink salt]`
- `update-alternatives --remove name path`
- `update-alternatives –-config java`
>从Sun被收购,Java分为`Oracle JDK`与`OpenJDK`,2020使用较多是`OpenJDK 8`,最新版是`OpenJDK 14`.

3. 对于一般的应用来说,他们还是去找系统的`JAVA_HOME`


#### 3.2.2. Lib下的jar包
- Eclipse jar 包是放在`webinfo --lib`目录下，IDEA没法自动加载，需要手动选择加载[^eclipse转idea]

[^eclipse转idea]: [eclipse转idea](https://blog.csdn.net/ssh159/article/details/71305706?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1)

----
放弃从Eclipse迁移到IDEA,因为

- 动摇: 始终报找不到jdk的错误
- 这个项目有个bug是要在MapReduce里加目录那种...

**直接用Eclipse它不香吗?**

-----
### 3.3. 运行环境说明[^JavaAPI操纵Hadoop]
#### 3.3.1. 安装Eclipse
用`apt-get`安装eclipse和直接下载安装eclipse有什么区别?

- 软件库安装目录一般就是`usr/lib/`
- 自行安装安装版本可以自定义

此外,最起码2020年5月22日从apt库安装的Eclipse是缺少某个启动类而无法直接启动的....

- start eclipse with debug parameter(事先配置好环境变量):
```
/opt/eclipse/eclipse -clean -consolelog -debug
```

**注意**

1. Eclipse放置好之后还需要指定运行的jre,在`eclipse.ini`的顶部插入:

```
-vm
/usr/lib/jvm/jdk1.7.0_67/jre/bin
```
2. `Neon`/`4.5`版本开始的Eclipse只支持jdk1.8以上[^Eclipse-JDK],4.5版本[下载](https://archive.eclipse.org/eclipse/downloads/drops4/R-4.5-201506032000/)
[^Eclipse-JDK]: [Eclipse版本和JDK版本对应关系](https://blog.csdn.net/qq_31772441/article/details/80017933?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.nonecase)

3. 高版本的如`Mars`亲试不支持`hadoop2x-eclipse-plugin-master`的`New hadoop Location`;而低版本的可能不支持你的高版本java语法.最后只能选择**Eclipse Luna**

4. `Eclipse-jee`版本的对于开发者是友好的,**其他版本(如`Eclipse-java`版本)有许多的插件(如tomcat)需要自己配**,如[从官网引入](https://blog.csdn.net/wugangsunny/article/details/25246565?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase),而对于久远的版本,官方可能已经停止了对插件仓库的维护....
#### 3.3.2. Hadoop2x-eclipse插件[^Hadoop2x-eclipse]

[^Hadoop2x-eclipse]:[Hadoop2.9.1 + Eclipse配置（解决New Location无反应）](https://www.cnblogs.com/sissie-coding/p/9449941.html)

使用`ant`对hadoop的jar包进行打包并适应Eclipse加载,所以参数里有hadoop和eclipse的目录.
必须注意对于不同的hadoop版本,` HADDOP_INSTALL_PATH/share/hadoop/common/lib`下的jar包版本都不同,需要一个个调整

- `hadoop2x-eclipse-plugin-master/ivy/library.properties`
- `hadoop2x-eclipse-plugin-master/src/contrib/eclipse-plugin/build.xml`

以使`ant`可以按照`hadoop2x-eclipse-plugin-master/src/contrib/eclipse-plugin`下的`build.xml`进行bulid生成插件的`jar`包.

本次试验遇到了`htrace-core4-4.1.0-incubating.jar `这个只能被`hadoop2x-eclipse-plugin-master`识别为`htrace-core-4.1.0-incubating.jar `的包,多了一个`4`,这需要在`build.xml`中进行修改

最后在`build/contrib/eclipse-plugin`底下生成插件.
- To know the '`Port`' numbers, start Hadoop and open http://localhost:8088/cluster in a browser. `Click Tools --> Configuration` and search for the following properties.[^HDT]

[^HDT]:[Apache Hadoop Development Tools](https://www.srccodes.com/build-install-configure-eclipse-plugin-apache-hadoop/)

**DFS Master**:Address of the Distributed FileSystem Master node (The Name Node).
```xml
<property>
<name>fs.defaultFS</name>
<value>hdfs://master:9000</value>
<final>false</final>
<source>core-site.xml</source>
</property>
```
**Map/Reduce(V2) Master**:Address of the Map/Reduce master node (The Job Tracker).
原本的是
```xml
<property>
    <name>mapreduce.jobtracker.http.address</name>
   <value>0.0.0.0:50030</value>
    <source>mapred-default.xml</source>
</property>
```
本机实际的是:
```xml
<name>mapreduce.jobtracker.address</name>
<value>master:54311</value>
<final>false</final>
<source>mapred-site.xml</source>
</property>
```

**注意**
1. DFS Locations在`project explorer`中而不是`package explorer`中,而且Eclipse对于窗口的隐藏做得"太好",以至于可能需要多点几个按钮
2. 这是一个[好案例](https://blog.csdn.net/QYmufeng/article/details/79580582),但是个人依然可能因为某些原因Eclipse无法连接上Hadoop集群

**注意**

1. 如果是在windows底下编译插件,还需要`hadoop.dll`和`winutils.exe`[^windows_hadoop]
[^windows_hadoop]:[windows下编译hadoop-eclipse-plugin-2.10.0.jar](https://blog.csdn.net/yancie_/article/details/103378311)
2. 如果遇到了`New Hadoop Location`失败,还需要加载`woodstox-core-5.0.3.jar`和`stax2-api-3.1.4.jar`


#### 3.3.3. 导入jdk相关和`servlet`相关的jar
在Eclipse中一个完整的JavaWeb项目，必须有以下类库(jar依赖库)：

- 指定Server Runtime--运行项目的服务器，比如tomcat
    - jsp
    - tomcat-api
    - 必须注意如果项目版本不一致,而旧版IDE又不提供修改,自己找出现错误的路径或类型如`Faceted Project Problem`然后去`org.eclipse.wst.common.project.facet.core`配置文件里改,比如直接删掉[^!]`  <runtime name="apache-tomcat-8.5.54"/>`

[^!]:[Target runtime Apache Tomcat v7.0 is not defined.](https://www.cnblogs.com/zoured/p/5885146.html)

- 指定jdk--负责Java的编译和运行；
- 项目jar包--ib文件夹下的jar包会由`Web App Libraries`统一管理。

在编译之前,目录可能不完全按照上述组成呈现,但是编译之后会像这样:
![](https://gitee.com/istarwyh/images/raw/master/1590158517_20200522224140303_26409.png)

#### 3.3.4. Eclipse导入Web项目的常见问题
Eclipse导入新项目,常见问题就是`XXX cannot be resolved to a type`[^Eclipse如何导入MyEclipse的项目]

[^Eclipse如何导入MyEclipse的项目]:[Eclipse如何导入MyEclipse的项目](https://www.cnblogs.com/Marydon20170307/p/8526863.html)

1. jar包冲突或缺失。查看项目中是否存在“XXX”所在的jar包，如果不存在，则导入对应jar包.导入jar包除了手动添加,可能某个`lib`文件夹下自带了. 
如果存在，查看jar包是否冲突，如果冲突，删除其中一个。如果一切正常，则执行2方法。

2. jdk不存在或不匹配。项目环境要求jdk7，而eclipse中使用的却是jdk6；重新配置eclipse的jdk环境，使之与项目要求环境一致。

3. Eclipse查找项目类型策略所致。如果1和2没问题，但还是报错；这时，需要执行`“Project” | "Clean…"`，完成后，报错消失！

### 3.4. Rebulid~
>为了提高validation的速度,可以通过`Preferences->Validation->Suspend all validators`直接禁止validation[^validation].

[^validation]: [eclipse 的validating长时间卡住不动](https://blog.csdn.net/zf_14159265358979/article/details/104803056?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-2)


- 导出`.war`文件

**注意**

1. 推荐使用`Eclipse-jee`,自带导出`war`功能
### 3.5. 修改配置
#### 3.5.1. 修改数据库和集群IP
##### 3.5.1.1. 修改连接数据库的IP
左侧视图，双击工程名NetCloud，可以打开工程结构，找到Java Resources 下的 src文件夹下的`jdbc.properties`文件，打开，修改你的IP,同时记得添加编码信息（否则，可能会出现注册成功用户名后不能成功登陆的问题）
```preperties
jdbc.url=jdbc\:mysql\://124.124.132.224\:3306/netcloud?characterEncoding=utf-8
```

`jdbc.properties`中的信息将会被`applicationContext.xml`中的`dataSource`Bean 所引用.

**注意**
本机地址一定是`localhost`或`127.0.0.1`,网卡所代表的本机地址可能无法使得数据库被连接上.进而报类似
```linux
The last packet sent successfully to the server was 0 milliseconds ago. The driver has not received any packets from the server.
```
这样的error,意即(mysql)服务器不响应.为了确保服务器可被正常连接并响应,可以用另一台客户机访问它[^外机访问mysql],如
(但是也有mysql[^mysql真的懒]默认是拒绝`127.0.0.1`之外的其他IP的,可以[戳我看](https://blog.csdn.net/txl13109187932/article/details/78846014)怎么远程连接mysql)


[^外机访问mysql]:  [如何在本地远程连接linux虚拟机上面的mysql](https://www.cnblogs.com/smileyes/p/7221365.html?utm_source=itdadao&utm_medium=referral)

[^mysql真的懒]:  [mysql的`my.cnf`默认bind-address = 127.0.0.1](https://www.cnblogs.com/zihanxing/p/7049244.html)



```linux
mysql -h 192.168.192.134 -P3306 -uroot -p
```
如果连不上可能是OS防火墙的缘故.Linux原始的防火墙工具`iptables`,Ubuntu基于`iptables`提供了`ufw`(uncomplicated firewall),其允许客户端访问所有端口的[命令](https://www.cnblogs.com/OnlyDreams/p/7210914.html):
`sudo ufw allow from 192.168.0.1`
其实防火墙默认是关闭的,即一般端口都是打开的.
##### 3.5.1.2. 修改hadoop集群的IP
打开Java Resources 下的 src文件夹下的`applicationcontext.xml`文件,修改你的集群IP,具体位置在：
```xml
	<bean id="dealFile" class="com.nitcloud.netdisk.utils.DealFile">
		<property name="address" value="hdfs://124.124.132.220"/>
		<property name="hdfs" ref="hdfs"/>
	</bean>
```
### 3.6. 在tomcat 7 上运行[^tomcat-war]
[^tomcat-war]:[Linux部署Tomcat并发布web项目](https://blog.csdn.net/qq_35733535/article/details/79358154?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-4)

## 4. 其他
如果的确未知原因连接不上hadoop集群,也可以shell或网页使用[hdfs基本操作](https://www.cnblogs.com/gulang-jx/p/8568439.html)
## 5. References
[^远程HDFS文件的操作]:[远程HDFS文件的操作](https://blog.csdn.net/huangpengbo2596/article/details/52518558)
[^JavaAPI操纵Hadoop]: [JavaAPI操纵Hadoop](https://www.cnblogs.com/zhangyinhua/p/7678704.html#_label0)
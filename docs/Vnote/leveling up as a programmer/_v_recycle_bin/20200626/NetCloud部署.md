## 1. 项目说明[^源]
[^源]: [基于hadoop （hdfs）+ javaweb （ssh）实现的网盘](https://blog.csdn.net/LEoe_/article/details/71194440?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-8)

这是一个基于Hadoop的云盘系统，实现的界面是用javaweb完成的，使用的是`Spring Struts2  hibernate`集合框架，配有sql文件。
![](https://gitee.com/istarwyh/images/raw/master/1591766942_20200610132738443_24609.png)
![](https://gitee.com/istarwyh/images/raw/master/1591766943_20200610132848013_6998.png)
其实[同类型](https://shuwoom.com/?p=1353)的项目有很多,有的也写的很详细,只是都太久远了(17年以前...).
## 2. 部署环境

### 2.1. Linux安装mysql
>ubuntu使用`APT`仓库;centos使用`YUM`仓库。

1. 在MySQL官网`ART`仓库中找到下载链接:
 `sudo wget https://dev.mysql.com/get/mysql-apt-config_0.8.15-1_all.deb`
 2. 安装MySQL并配置
 注意结尾为`deb`,为`Debian`系发行版,可使用Debian Package,即`dpkg`来安装这个主面板,`-i`应该是`--install`的短命令版本:
 `sudo dpkg -i mysql-apt-config_0.8.15-1_all.deb`
 - `Enter`之后可以选择**5.7版本**,这里才开始下载服务器端`mysql-server09`

3. 开始更新安装
- `sudo apt-get update`
- `sudo apt-get install mysql-server`
密码:`mysql`
4. 验证安装
- MySQL安装完成后默认是启动的,以下命令分别是查看mysql状态|启动mysql|停止mysql|重启mysql
`sudo service mysql status|start|stop|restart`
- 或者使用`mysql -u root -p` ,然后`show databases`

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

### 2.3. JDK
`jdk1.7.0_67`
#### 2.3.1. 切换JDK
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
### 2.4. 确保hadoop已正确发布 [^hadoop正确发布]

[^hadoop正确发布]: [hadoop正确发布](https://blog.csdn.net/daxiang12092205/article/details/52717598?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-18&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-18)

1. 确定Linux内可以通过IP访问集群,如:
![](https://gitee.com/istarwyh/images/raw/master/1590158515_20200507164752616_6700.png)

2. 确定Windows host主机可以与Linux中的master通信,可在 `C:\Windows\System32\drivers\etc`的`hosts`添加IP与节点名映射[^在Windows下使用浏览器访问HDFS的小问题]:
[^在Windows下使用浏览器访问HDFS的小问题]:[在Windows下使用浏览器访问HDFS的小问题](https://blog.csdn.net/Andrea_null/article/details/83025329)

![](https://gitee.com/istarwyh/images/raw/master/1590158513_20200507164512132_6480.png)
之后访问`master:57900`.**必须注意**,交换机与主机之间的连接有三种模式[^Vmware三种网络模式详解],为了集群之间的统一往往用`Host-only`模式,这时候在开始配置虚拟机网卡时,需要让主机的虚拟网卡通过虚拟交换机与`master`的虚拟网卡连接.

[^Vmware三种网络模式详解]: [Vmware虚拟机三种网络模式详解](https://www.cnblogs.com/Genesis2018/p/8304738.html)

3. 通过master机上命令行，可正常上传、删除文件/文件夹；

4. 检查hadoop集群master/slaver的日志(默认位置`$HADOOP_HOME/logs`)，看是否有报错信息.

## 3. 编译云盘程序
### 3.1. 源代码上传
1. 源代码NetCloud.rar解压缩为一个文件夹NetCloud并上传至master主机`/root`

### 3.2. 运行环境说明[^JavaAPI操纵Hadoop]
#### 3.2.1. 安装Eclipse
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
#### 3.2.2. Hadoop2x-eclipse插件[^Hadoop2x-eclipse]

[^Hadoop2x-eclipse]:[Hadoop2.9.1 + Eclipse配置（解决New Location无反应）](https://www.cnblogs.com/sissie-coding/p/9449941.html)

使用`ant`对hadoop的jar包进行打包并适应Eclipse加载,所以参数里有hadoop和eclipse的目录.
**必须注意**对于不同的hadoop版本,` HADDOP_INSTALL_PATH/share/hadoop/common/lib`下的jar包版本都不同,需要一个个调整

- `hadoop2x-eclipse-plugin-master/ivy/library.properties`
- `hadoop2x-eclipse-plugin-master/src/contrib/eclipse-plugin/build.xml`

**以使**`ant`可以按照`hadoop2x-eclipse-plugin-master/src/contrib/eclipse-plugin`下的`build.xml`进行bulid生成插件的`jar`包.

对于`Hadoop2.10.0`会遇到`htrace-core4-4.1.0-incubating.jar `这个只能被`hadoop2x-eclipse-plugin-master`识别为`htrace-core-4.1.0-incubating.jar `的包,多了一个`4`,这需要在`build.xml`中进行修改

最后在`build/contrib/eclipse-plugin`底下生成插件.

##### 3.2.2.1. 通信端口号的确定
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

##### 3.2.2.2. **注意**

1. 如果是在windows底下编译插件,还需要`hadoop.dll`和`winutils.exe`[^windows_hadoop]
[^windows_hadoop]:[windows下编译hadoop-eclipse-plugin-2.10.0.jar](https://blog.csdn.net/yancie_/article/details/103378311)
2. 如果遇到了`New Hadoop Location`失败,还需要加载`woodstox-core-5.0.3.jar`和`stax2-api-3.1.4.jar`

**注意**
**如果的确未知原因连接不上hadoop集群,也可以shell或网页使用**[hdfs基本操作](https://www.cnblogs.com/gulang-jx/p/8568439.html)

#### 3.2.3. 导入jdk相关和`servlet`相关的jar
在Eclipse中一个完整的JavaWeb项目，必须有以下类库(jar依赖库)：

- 指定Server Runtime--运行项目的服务器，比如tomcat
    - jsp*.jar
    - tomcat-api*.jar
    - 必须注意如果项目版本不一致,而旧版IDE又不提供修改,自己找出现错误的路径或类型如`Faceted Project Problem`然后去`org.eclipse.wst.common.project.facet.core`配置文件里改,比如直接删掉[^!]`  <runtime name="apache-tomcat-8.5.54"/>`

[^!]:[Target runtime Apache Tomcat v7.0 is not defined.](https://www.cnblogs.com/zoured/p/5885146.html)

- 指定jdk--负责Java的编译和运行；
- 项目jar包--ib文件夹下的jar包会由`Web App Libraries`统一管理。

在编译之前,目录可能不完全按照上述组成呈现,但是编译之后会像这样:
![](https://gitee.com/istarwyh/images/raw/master/1590158517_20200522224140303_26409.png)

#### 3.2.4. Eclipse导入Web项目的常见问题
Eclipse导入新项目,常见问题就是`XXX cannot be resolved to a type`[^Eclipse如何导入MyEclipse的项目]

[^Eclipse如何导入MyEclipse的项目]:[Eclipse如何导入MyEclipse的项目](https://www.cnblogs.com/Marydon20170307/p/8526863.html)

1. jar包冲突或缺失。查看项目中是否存在“XXX”所在的jar包，如果不存在，则导入对应jar包.导入jar包除了手动添加,可能某个`lib`文件夹下自带了. 
如果存在，查看jar包是否冲突，如果冲突，删除其中一个。如果一切正常，则执行2方法。

2. jdk不存在或不匹配。项目环境要求jdk7，而eclipse中使用的却是jdk6；重新配置eclipse的jdk环境，使之与项目要求环境一致。

3. Eclipse查找项目类型策略所致。如果1和2没问题，但还是报错；这时，需要执行`“Project” | "Clean…"`，完成后，报错消失！

### 3.3. Rebulid~
>为了提高validation的速度,可以通过`Preferences->Validation->Suspend all validators`直接禁止validation[^validation].

[^validation]: [eclipse 的validating长时间卡住不动](https://blog.csdn.net/zf_14159265358979/article/details/104803056?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-2)


- 导出`.war`文件

**注意**

1. 推荐使用`Eclipse-jee`,自带导出`war`功能
### 3.4. 修改配置
#### 3.4.1. 修改数据库和集群IP
(1)	修改连接数据库的IP
左侧视图，双击工程名NetCloud，可以打开工程结构，找到Java Resources 下的 src文件夹下的jdboc.properties文件，打开，修改你的IP,同时记得添加编码信息（否则，可能会出现注册成功用户名后不能成功登陆的问题。这是本工程的一个bug.）
```preperties
jdbc.url=jdbc\:mysql\://124.124.132.224\:3306/netcloud?characterEncoding=utf-8
```
(2)	修改hadoop集群的IP
打开Java Resources 下的 src文件夹下的`applicationcontext.xml`文件,修改你的集群IP,具体位置在：
```xml
	<bean id="dealFile" class="com.nitcloud.netdisk.utils.DealFile">
		<property name="address" value="hdfs://124.124.132.220"/>
		<property name="hdfs" ref="hdfs"/>
	</bean>
```
### 3.5. 在tomcat 7 上运行[^tomcat-war]
[^tomcat-war]:[Linux部署Tomcat并发布web项目](https://blog.csdn.net/qq_35733535/article/details/79358154?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-4)

## 4. References
[^远程HDFS文件的操作]:[远程HDFS文件的操作](https://blog.csdn.net/huangpengbo2596/article/details/52518558)
[^JavaAPI操纵Hadoop]: [JavaAPI操纵Hadoop](https://www.cnblogs.com/zhangyinhua/p/7678704.html#_label0)
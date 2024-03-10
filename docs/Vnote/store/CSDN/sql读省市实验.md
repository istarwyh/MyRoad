
## 1. 概念回顾
### 1.1. Hive介绍
Hive 是基于 Hadoop 的一个数据仓库工具，可以将结构化的数据文件映射为一张数据库表，并使用 sql 语句转换为 MapReduce 任务进行运行。其优点是学习成本低，可以通过类 SQL 语句快速实现简单的 MapReduce 统计，不必开发专门的 MapReduce 应用，十分适合数据仓库的统计分析。
### 1.2. Sqoop介绍
Sqoop 即 `SQL to Hadoop` ，是一款方便的在传统型数据库与 Hadoop 之间进行数据迁移的工具，充分利用 MapReduce 并行特点以批处理的方式加快数据传输
### 1.3. Hive Vs SQL
Hive 使用HDFS，关系数据库则是服务器本地的文件系统。因为Hive 则是为海量数据做数据挖掘设计的，其实时性很差;而关系数据库都是为实时查询的业务进行设计的.
## 2. 环境说明
- Ubuntu 18.04
- Hadoop 2.10.0
- MySQL 5.7
- apache-hive-2.3.7
- mysql-connector-java_8.0.20-1ubuntu18.04_all
- sqoop-1.4.7
## 3. 将sql文件导入MySQL

### 3.1. 安装MySQL
1. 在MySQL官网`ART`仓库中找到下载链接:
 `sudo wget https://dev.mysql.com/get/mysql-apt-config_0.8.15-1_all.deb`
 2. 安装MySQL并配置
 注意结尾为`deb`,为`Debian`系发行版,可使用Debian Package,即`dpkg`来安装这个主面板,`-i`应该是`--install`的短命令版本:
 `sudo dpkg -i mysql-apt-config_0.8.15-1_all.deb`
 - `Enter`之后可以选择**5.7版本**,这里才开始下载服务器端`mysql-server`

3. 开始更新安装
- `sudo apt-get update`
- `sudo apt-get install mysql-server`
密码:`mysql`
4. 验证安装
- MySQL安装完成后默认是启动的,以下命令分别是
    - 查看mysql状态|启动mysql|停止mysql|重启mysql
    - `sudo service mysql status|start|stop|restart`
![](https://gitee.com/istarwyh/images/raw/master/1592828102_20200621101828682_32662.png)
- 或者使用`mysql -u root -p` ,然后`show databases`
![](https://gitee.com/istarwyh/images/raw/master/1592828104_20200621102901933_273.png)

### 3.2. 设置Hive用户并创hive数据库
创建hive用户并赋予所有权限:
![](https://gitee.com/istarwyh/images/raw/master/1592828104_20200621104150921_4877.png)
使用 hive 用户登录，创建 hive 数据库：
![](https://gitee.com/istarwyh/images/raw/master/1592828105_20200621104815797_26946.png)  
### 3.3. 导入sql源文件
#### 3.3.1. 将中文名改名
将`中国省市县地区编码.sql`改为`Location.sql`
#### 3.3.2. 加载源文件
![](https://gitee.com/istarwyh/images/raw/master/1592828109_20200621164048047_29285.png)
## 4. MySQL导入Hive
### 4.1. 安装Hive
#### 4.1.1. 检查并配置主机名
在`/etc/hosts`中...
**我个人建议**不要这样做,后续直接用`IP`会省去很多麻烦.
#### 4.1.2. 启动Hadoop
`start-all.sh`
![](https://gitee.com/istarwyh/images/raw/master/1592828105_20200621110559967_22373.png)
#### 4.1.3. 解压并移动Hive安装包

放在`/usr/local`目录下
![](https://gitee.com/istarwyh/images/raw/master/1592828106_20200621110831712_8707.png)
#### 4.1.4. 解压并移动 MySql 驱动包
使用`dpkg -i mysql-connector-java_8.0.20-1ubuntu18.04_all.deb`安装,其位置如下:
![](https://gitee.com/istarwyh/images/raw/master/1592828106_20200621113239474_9417.png)
#### 4.1.5. 配置`/etc/profile`环境变量
在`/etc/profile`中添加
![](https://gitee.com/istarwyh/images/raw/master/1592828106_20200621113558175_19787.png)
使配置文件生效：
```vi
source /etc/profile
echo $PATH
```

验证是否安装成功
![](https://gitee.com/istarwyh/images/raw/master/1592828106_20200621150245861_21742.png)
#### 4.1.6. 配置hive-site.xml
主要是关于和mysql数据库的相关连接信息:
![](https://gitee.com/istarwyh/images/raw/master/1592828107_20200621161027642_6779.png)

**注意**
mysql上只存储hive的元数据，而原始数据是存储在hdfs上的，默认存储路径在hive-site.xml中---`hive.metastore.warehouse.dir`属性，则默认值是`/user/hive/warehouse`
#### 4.1.7. 复制mysql的驱动程序到hive/lib
安装其`deb`包后在`/usr/share/java/`下
#### 4.1.8. 验证hive命令
![](https://gitee.com/istarwyh/images/raw/master/1592828107_20200621153932645_22876.png)
#### 4.1.9. 源数据初始化
`schematool -dbType mysql -initSchema`
![](https://gitee.com/istarwyh/images/raw/master/1592828108_20200621161455460_6348.png)
#### 4.1.10. 验证插入数据
![](https://gitee.com/istarwyh/images/raw/master/1592828108_20200621161954658_20350.png)
最后花了61.767s插入了一条数据.

### 4.2. 安装Sqoop[^安装Sqoop]


必须选择编译包含了对 hadoop 支持库的 sqoop 版本,并把对应的`jar`复制到Hadoop的依赖中.
否则会出现`NoClassDefFoundError: org/apache/avro/LogicalType` 等错误.


### 4.3. 以表province_city_district为例,导入Hive
尽管
`sqoop list-tables --connect jdbc:mysql://127.0.0.1:3306/hive --username hive --password hive`
这类的命令能使用,但是
`sqoop import --connect jdbc:mysql://127.0.0.1:3306/hive?characterEncoding=utf-8 --username hive --password hive --table province_city_district --warehouse-dir /user/hive/`
这类命令总是会无法报错无法连接上`database server`:

```vi
Caused by: java.lang.RuntimeException: java.sql.SQLNonTransientConnectionException: Could not create connection to database server. Attempted reconnect 3 times. Giving up.
```
几乎将能看到的解决方法都排查过了,仍然无解.遂
**重新卸载mysql再试图远程连接安装**
再用:
`root@master:/usr/local/apache-hive-2.3.7-bin/conf# sqoop import --connect jdbc:mysql://192.168.192.134:3306/hive?characterEncoding=utf-8 --username root --password root --table province_city_district
`
![](https://gitee.com/istarwyh/images/raw/master/1592828109_20200622155033645_18892.png)
发现没有reduce,遂加上`-m 1`
如果不是输到hdfs上还需要配置`${SQOOP_HOME}/bin/sqoop`中`HIVE_HOME`的位置,来告诉sqoop怎么找到hive.或者偷懒直接改掉依赖的引用路径[^安装Sqoop]也可以.

最后用
`sqoop import --connect jdbc:mysql://192.168.192.134:3306/hive?characterEncoding=utf-8 --username root --password root --table province_city_district --delete-target-dir -m 1 --hive-import --create-hive-table
`
导入成功:
![](https://gitee.com/istarwyh/images/raw/master/1592828110_20200622170729500_11424.png)
## 5. hiveSQL输出我所在的省市
![](https://gitee.com/istarwyh/images/raw/master/1592828110_20200622174304167_14305.png)

[^安装Sqoop]:[设计者自身设计有问题](https://zhuanlan.zhihu.com/p/33677303)
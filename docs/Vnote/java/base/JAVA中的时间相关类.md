---
reference_link: https://mp.weixin.qq.com/s/xMbGFwMhKMAoBKk4LNL22A
---
## 时间类

| **类名**         | **具体描述**                                                                                             |
| -------------- | ---------------------------------------------------------------------------------------------------- |
| Date           | Date对象算是JAVA中历史比较悠久的用于处理日期、时间相关的类了，但是随着版本的迭代演进，其中的众多方法都已经被弃用，所以Date更多的时候仅被用来做一个数据类型使用，用于记录对应的日期与时间信息 |
| Calender       | 为了弥补Date对象在日期时间处理方法上的一些缺陷，JAVA提供了Calender抽象类来辅助实现Date相关的一些日历日期时间的处理与计算。                              |
| TimeZone       | Timezone类提供了一些有用的方法用于获取时区的相关信息                                                                       |
|                |                                                                                                      |
| LocalDate      | 获取当前的日期信息，仅有简单的日期信息，不包含具体时间、不包含时区信息。                                                                 |
| LocalTime      | 获取当前的时间信息，仅有简单的时间信息，不含具体的日期、时区信息。                                                                    |
| LocalDateTime  | 可以看做是LocalDate和LocalTime的组合体，其同时含有日期信息与时间信息，但是依旧不包含任何时区信息。                                           |
| OffsetDateTime | 在LocalDateTime基础上增加了时区偏移量信息                                                                          |
| ZonedDateTime  | 在OffsetDateTime基础上，增加了时区信息                                                                           |
| ZoneOffset     | 时区偏移量信息， 比如+8:00或者-5:00等                                                                             |
| ZoneId         | 具体的时区信息，比如Asia/Shanghai或者America/Chicago                                                             |
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202404202126748.png)


## 时间间隔

| **类**    | **描述**                     |
| -------- | -------------------------- |
| Duration | 时间间隔，用于秒级的时间间隔计算           |
| Period   | 日期间隔，用于天级别的时间间隔计算，比如年月日维度的 |


![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202404202129505.png)

|          |                            |
| -------- | -------------------------- |
| **类**    | **描述**                     |
| Duration | 时间间隔，用于秒级的时间间隔计算           |
| Period   | 日期间隔，用于天级别的时间间隔计算，比如年月日维度的 |
## 消失的8小时问题
### **日期字符串存入DB后差8小时**

在后端与数据库交互的时候，可能会遇到一个问题，就是往DB中存储了一个时间字段之后，后面再查询的时候，就会发现时间数值差了8个小时，这个需要在DB的连接信息中指定下时区信息：
```sh
spring.datasource.druid.url=jdbc:mysql://127.0.0.1:3306/test?serverTimezone=Asia/Shanghai
```


### **界面时间与后台时间差8小时**

在有一些前后端交互的项目中，可能会遇到一个问题，就是前端选择并保存了一个时间信息，再查询的时候就会发现与设置的时间差了8个小时，这个其实就是后端时区转换设置的问题。比如 jackson框架之于Spring在中国这么配置：
```sh
spring.jackson.time-zone=GMT+8
```


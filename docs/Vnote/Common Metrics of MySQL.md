## 机器指标
- CPU Usage: 30%
- Memory Usage: ？
- Disk Usage: 70%
## 进程指标
- QPS
- TPS
- RT: 10ms
- 活跃连接数：一般10以下，高配数据库20
- DML（select、insert、update、delete）执行次数
- innoDB行锁平均锁定时间
- Buffer Pool总页数、空闲页比例、脏页比例
## 业务指标
- 接口RT
- 读写成功率
	- SQL bug
	- 异常数据触发字段长度不够
	- 唯一性约束
	- DB长期高负载
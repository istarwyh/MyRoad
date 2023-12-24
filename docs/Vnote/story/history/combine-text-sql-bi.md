

轻量的只负责功能，不连接数据库的产品：
https://www.text2sql.ai/

连接数据库的产品：
https://sqlgpt.cn/zh **chat2DB** 

Xander: https://flowus.cn/share/5f2d300f-468c-4ab6-b7cd-f88d1e6500e4
- 连接数据库但是让对方提供接口,port & key

数据库开发人员不是太需要可视化的能力，之前生成表在BI工具中再处理，最终交付给运营。

这里面需要
- 中间表固化结果
	- ODS: Operational Data Store OLTP 
	- DIM: 商家、品、营销活动,指标层？ 
	- DWD: Data Warehouse Detail
	- DWS: Data Warehouse Summary
	- ADS: Application Data Service

- 比较复杂的逻辑需要人为介入 
	- AI 反推研发补全，产生
	- DTS-》 处理
	- 人要承担责任 java -> SQL 客户？
	- 结果的渲染
- Tableau 


[[Classic Process Data Infrastructure.canvas|Classic Process Data Infrastructure]]
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202312241728178.png)

![](https://mmbiz.qpic.cn/mmbiz_png/OmCbZ5JK30FY9vvhcCU8ZMILohtuSulkAO00yxQ43ndQIq5hlUFRPIDLUMG1u7rLzbKicenGPiaqkMU6jyHlYt9Q/640?wx_fmt=png)

https://mp.weixin.qq.com/s/vCRhU_jjh1AM7AYy9Zdr8w
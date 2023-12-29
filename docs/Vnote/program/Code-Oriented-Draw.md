#tool
## Markdown
|        操作         |   Hash   | 有序数组 |  二叉树  |     b树(B-tree,Balanced Tree)     |                 b+树                 |                 b*树                  |
| ------------------ | -------- | ------- | ------- | --------------------------------- | ------------------------------------ | ------------------------------------- |
| Insert             | O(1)     | O(n)    | O(logn) | n叉树，每个节点可以有1200个叉         | n叉树，优化b树存储，提供范围查找          | n叉树，优化b+树页分裂空间利用率低的问题     |
| Remove             |          |         | O(logn) | 树高不超过3                        | **所有Key只在叶子节点上出现一次**        | 非叶子节点的兄弟节点之间也通过指针相连      |
| Update             |          |         |         | 最多只需要访问3次磁盘就可定位数据块    | 所有非叶子节点都是**叶子节点**的索引      | 如果节点上的子节点满了                    |
| Find               | O(1)     | O(logn) | O(logn) | 极少io--根节点总在内存中             | 叶子结点如果是聚簇索引直接定位到记录       | 就将它的子节点挪一部分到旁边没满的兄弟节点上 |
| Iterator(范围查找n) | $\times$ | O(n)    | O(n)?   | **所有数据只在节点上出现一次**        | 叶子结点如果是非聚簇索引存储指向记录的指针 | 避免了重新创建新的节点的过程               |
| Sort/Group by      | $\times$ |         |         | 搜索可以在非叶子节点结束              | 各个叶子节点通过指针跟兄弟节点关联(双指针) |                                       |
| supplement         |          |         |         | 子节点数目超过后再二分，称作**页分裂** |                                      |                                       |

### Extradraw
本身是JSON格式存储，渲染为手绘风格的图画。可以导入Mermaid 格式的Flowchart

### Mermaid
#### 画流程图
```mermaid
graph LR
id1(lineText)--基础校验-->id2[validText]--业务校验-->id7{是否是issue查找}
id7--是-->id4[issue补全]--补全词伙-->id6[词伙补全]
id7--否-->id3{关键词是否已经补全}--是-->id6
id3--否-->id5[关键词补全]--补全关键词-->id7
```


1. 这里的射线是不能转弯的
2. 结构换行必须加subgraph & end,也就是独立出来才行
3. 文本换行得用`<br/>`标签...
4. 文字中不能包含`~`

#### 画思维导图
https://blog.csdn.net/skytruine/article/details/105828748

```mermaid
	graph LR
	Output--> NAT(对应给客户端)
	NAT-->为了向Client屏蔽集群信息,Ngnix修改IP-Packet的起始地

	Output--> DR(Direct-Server模式)
	DR-->处理服务器直接返回给Client
	DR-->Loopback(包括LoadBalance在内全都是同一IP)
    Loopback-->为了ARP可以定位LoadBalance,抑制除LR外的ARP响应
```

```mermaid
graph TD
保险--> A(核心域)
A-->A1(承保上下文)
A-->A2(理赔上下文)
A1-->投保
A1-->核保
A1-->保单
A1-->收费
A1-->再保
A1-->批单
A1-->通知
保险--> B(通用域)
B-->B1(产品上下文)
B-->B2(收付上下文)
B-->B3(渠道上下文)
B-->B4(客户上下文)

保险--> C(支撑域)
C-->C1(单证上下文)
C-->C2(再保)

保险-->D(...)
```
#### 画时序图
https://www.imooc.com/article/292709?block_id=tuijian_wz

```mermaid
sequenceDiagram
participant 信贷
participant 信保
participant 产品配置中心
participant 服务编排
participant 无界山

信贷 ->> +信保 :  @RequestBody uwApplyPolicyReq
信保 ->> 信保 : 校验Base参数
信保 ->> 信保 : 查询数据库初始化InsPolicyDO、InsPolicyLoanDO

信保 ->> + 产品配置中心 : 初始化InsProductResp
产品配置中心 -->> -信保 : 产品配置(产品属性,模板,计费方式等)
信保 ->> 信保 : 更新之后需要用的context
信保 ->> 信保 : 校验Biz(组合)参数
alt 状态为“PRE_SUCCEED”
信保 ->> 信保: 根据uwStatus判断是否幂等
信保 ->> 服务编排 : Map<String, AbstractUwFlow> 通过核保状态路由
服务编排 -->> 信保 : 对应的AbstractUwFlow
信保 ->> 信保 : 多态,AbstractUwFlow执行对应的行为
信保 ->> 信保 : fill入参到uwPolicyContext
信保 ->> + 无界山 : 查询保单状态
无界山 -->> - 信保 : 保单状态

信保 ->> + 无界山 : 告知修改后的保额保费,起止期,账单信息数据
无界山 -->> - 信保 : 修改成功或失败
信保 ->> + 无界山 : 核保
无界山 -->> - 信保 : 核保成功或失败
信保 ->> 信保 : 补充或更新context
信保 ->> +信保 : updatePolicyFromStatus(依据状态做乐观锁),updateByPrimaryKeySelective
信保 -->> -信保: 事务结束
opt 乐观锁结果:首次更新
信保 ->> 服务编排: 执行后续核保流程
end
else 状态为其他终态如“SUCCEED”/“UW_REJECTED”
信保 -->> 信保: log.info(“申请幂等返回”)
end


信保 -->> -信贷 : 返回组装的响应对象
```

## Draw.io/Diagrams.net
GUI画图，xml 格式存储，渲染为drawio风格的图画。支持图形较多。
## CSS3
### 3.1. Papaerclip
<a class="paperclip icon"></a>

<style type="text/css">
.paperclip.icon {
  color: #000;
  position: absolute;
  margin-left: 9px;
  margin-top: 2px;
  width: 6px;
  height: 12px;
  border-radius: 4px 4px 0 0;
  border-left: solid 1px currentColor;
  border-right: solid 1px currentColor;
  border-top: solid 1px currentColor;
  -webkit-transform: rotate(45deg);
          transform: rotate(45deg);
}

.paperclip.icon:before {
  content: '';
  position: absolute;
  top: 11px;
  left: -1px;
  width: 4px;
  height: 6px;
  border-radius: 0 0 3px 3px;
  border-left: solid 1px currentColor;
  border-right: solid 1px currentColor;
  border-bottom: solid 1px currentColor;
}

.paperclip.icon:after {
  content: '';
  position: absolute;
  left: 1px;
  top: 1px;
  width: 2px;
  height: 10px;
  border-radius: 4px 4px 0 0;
  border-left: solid 1px currentColor;
  border-right: solid 1px currentColor;
  border-top: solid 1px currentColor;
}
</style>
### 3.2. Danger Icon
<a class="gg-danger"></a>

<style>
 .gg-danger {
 box-sizing: border-box;
 position: relative;
 display: block;
 transform: scale(var(--ggs,1));
 width: 20px;
 height: 20px;
 border: 2px solid;
 border-radius: 40px
}

.gg-danger::after,
.gg-danger::before {
 content: "";
 display: block;
 box-sizing: border-box;
 position: absolute;
 border-radius: 3px;
 width: 2px;
 background: currentColor;
 left: 7px
}

.gg-danger::after {
 top: 2px;
 height: 8px
}

.gg-danger::before {
 height: 2px;
 bottom: 2px
} 
</style>
## 4. SVG
[手摸手，带你优雅的使用 icon](https://www.jianshu.com/p/f14982b2b737)

<svg t="1613551927172" class="icon" viewBox="0 0 1024 1024" version="1.1" xmlns="http://www.w3.org/2000/svg" p-id="1221" width="20" height="20"><path d="M665.6 374.4c-12.8-57.6-57.6-105.6-118.4-118.4-67.2-16-131.2 9.6-169.6 64-28.8 41.6 0 60.8 16 73.6 6.4 3.2 12.8 9.6 22.4 19.2 12.8 12.8 32 9.6 44.8-3.2 12.8-12.8 9.6-32-3.2-44.8-9.6-6.4-16-12.8-22.4-19.2 22.4-25.6 60.8-38.4 96-28.8 35.2 9.6 60.8 35.2 70.4 70.4 9.6 48-12.8 92.8-54.4 112-41.6 19.2-70.4 57.6-70.4 102.4l0 22.4c0 19.2 12.8 32 32 32 0 0 0 0 0 0 16 0 32-12.8 32-32l0-22.4c0-19.2 12.8-38.4 32-44.8C643.2 528 681.6 451.2 665.6 374.4zM480 729.6c-9.6 9.6-12.8 22.4-12.8 35.2 0 12.8 6.4 25.6 12.8 35.2 9.6 9.6 22.4 12.8 35.2 12.8 12.8 0 25.6-6.4 35.2-12.8 9.6-9.6 12.8-22.4 12.8-35.2 0-12.8-6.4-25.6-12.8-35.2C531.2 710.4 499.2 710.4 480 729.6zM512 67.2c-80 0-160 22.4-227.2 64C246.4 86.4 188.8 64 124.8 64 115.2 64 102.4 70.4 96 80c-6.4 9.6-6.4 22.4 0 32 0 0 32 54.4 41.6 137.6 3.2 16 16 28.8 32 28.8 0 0 3.2 0 3.2 0C192 275.2 204.8 262.4 204.8 243.2 198.4 201.6 188.8 163.2 179.2 137.6 204.8 144 230.4 160 249.6 188.8c0 0 0 0 0 0 0 0 0 0 0 0C252.8 192 252.8 192 252.8 195.2c0 0 3.2 3.2 3.2 3.2 3.2 0 3.2 3.2 6.4 3.2 3.2 0 3.2 3.2 6.4 3.2 3.2 0 3.2 0 6.4 0 3.2 0 3.2 0 6.4 0 3.2 0 3.2 0 6.4-3.2 3.2 0 3.2 0 6.4-3.2 0 0 0 0 0 0 0 0 0 0 0 0 64-44.8 140.8-67.2 217.6-67.2 211.2 0 384 172.8 384 384s-172.8 384-384 384-384-172.8-384-384c0-54.4 9.6-105.6 32-156.8 6.4-16 0-35.2-16-41.6-16-6.4-35.2 0-41.6 16C76.8 390.4 64 451.2 64 515.2c0 246.4 201.6 448 448 448 246.4 0 448-201.6 448-448C960 268.8 758.4 67.2 512 67.2z" p-id="1222" fill="#1296db"></path></svg>
## 5. HTML
### 5.1. 表格
#### 5.1.1. 压缩前
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <meta http-equiv="Content-Style-Type" content="text/css" />
    <meta name="generator" content="Aspose.Words for .NET 17.1.0.0" />
    <title>Bug-1</title>
  </head>
  <style>
    #default {
      background: black;
    }
  </style>
<body>
    <div>
      <table cellspacing="0" cellpadding="0" style="border-collapse: collapse;">
        <tbody>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">BugID</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;B-1</span>
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">Bug</span>
                <span style="font-family:&#39;宋体&#39;">标题</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-top-style: solid;
                border-top-width: 0.75pt;
                border-right-style: solid;
                border-right-width: 0.75pt;
                border-left-style: solid;
                border-left-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;布尔表达式输入框为空时无提示</span>
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">软件版本</span>
              </p>
            </td>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;CellCover1.0</span>
              </p>
            </td>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">测试环境</span>
              </p>
            </td>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;1</span>
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">测试人员</span>
              </p>
            </td>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;王艺辉</span>
              </p>
            </td>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">开发人员</span>
              </p>
            </td>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;</span>
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">创建时间</span>
              </p>
            </td>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;2020年5月31日</span>
              </p>
            </td>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">Bug</span
                ><span style="font-family:&#39;宋体&#39;">状态</span>
              </p>
            </td>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;待解决</span>
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">测试阶段</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-right-style: solid;
                border-right-width: 0.75pt;
                border-left-style: solid;
                border-left-width: 0.75pt;
                border-bottom-style: solid;
                border-bottom-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <input type="button" style="height: 11px; width: 10px;" />
                <span style="font-family:&#39;宋体&#39;">接口测试、</span>
                <input
                  type="button"
                  id="default"
                  style="height: 11px; width: 10px;"
                />
                <span style="font-family:&#39;宋体&#39;">功能测试、</span>
                <input type="button" style="height: 11px; width: 10px;" />
                <span style="font-family:&#39;宋体&#39;">配置测试、</span>
                <input type="button" style="height: 11px; width: 10px;" />
                <span style="font-family:&#39;宋体&#39;">回归测试</span>
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">Bu</span
                ><span style="font-family: Calibri;">g</span
                ><span style="font-family:&#39;宋体&#39;">严重程度</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <input type="button" style="height: 11px; width: 10px;" />
                <span style="font-family:&#39;宋体&#39;">紧急、</span>
                <input type="button" style="height: 11px; width: 10px;" />
                <span style="font-family:&#39;宋体&#39;">严重、</span>
                <input
                  type="button"
                  id="default"
                  style="height: 11px; width: 10px;"
                />
                <span style="font-family:&#39;宋体&#39;">一般、</span>
                <input type="button" style="height: 11px; width: 10px;" />
                <span style="font-family:&#39;宋体&#39;">轻微</span>
              </p>
            </td>
            <p></p>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">问题优先级</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >高</span
                >
                <span style="font-family:&#39;宋体&#39;">、</span>
                <input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >较高</span
                >
                <span style="font-family:&#39;宋体&#39;">、</span>
                <input
                  type="button"
                  id="default"
                  style="height: 11px; width: 10px;"
                /><span style="font-family:&#39;宋体&#39;">一般</span>
                <span style="font-family:&#39;宋体&#39;">、</span>
                <input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >低</span
                >
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">问题来源</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <input
                  type="button"
                  id="default"
                  style="height: 11px; width: 10px;"
                /><span style="font-family:&#39;宋体&#39;">测试</span
                ><span style="font-family:&#39;宋体&#39;">、</span
                ><input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >升级</span
                ><span style="font-family:&#39;宋体&#39;">、</span
                ><input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >其他</span
                >
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">问题类型</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >功能问题</span
                ><span style="font-family:&#39;宋体&#39;">、</span
                ><input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >版本问题</span
                ><span style="font-family:&#39;宋体&#39;">、</span
                ><input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >遗留问题</span
                ><span style="font-family:&#39;宋体&#39;">、</span
                ><input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >新需求</span
                ><span style="font-family:&#39;宋体&#39;">、</span
                ><input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >配置错误</span
                ><span style="font-family:&#39;宋体&#39;">、</span
                ><input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >性能问题</span
                ><span style="font-family:&#39;宋体&#39;">、</span
                ><input type="button" id="default" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >设计问题</span
                ><span style="font-family:&#39;宋体&#39;">、</span
                ><input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >偶发性错误</span
                ><span style="font-family:&#39;宋体&#39;">、</span
                ><input type="button" style="height: 11px; width: 10px;" /><span
                  style="font-family:&#39;宋体&#39;"
                  >其他</span
                >
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">缺陷的触发条件</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;</span>
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">操作步骤（简）</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;1.打开测试用例生成页面<br>2.设置表达式输入框为空<br>3.点击生成按钮</span>
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">实际结果</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;无任何结果</span>
              </p>
            </td>
          </tr>
          <tr>
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">预期结果</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family: Calibri;">&nbsp;弹出"请重新输入表达式不为空"的提示框</span>
              </p>
            </td>
          </tr>
          <tr style="height: 96.1pt;">
            <td
              style="
                width: 92.9pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;">附图</span>
              </p>
            </td>
            <td
              colspan="3"
              style="
                width: 300.3pt;
                border-style: solid;
                border-width: 0.75pt;
                padding-right: 5.03pt;
                padding-left: 5.03pt;
                vertical-align: middle;
              "
            >
              <p
                style="
                  margin-top: 0pt;
                  margin-bottom: 0pt;
                  text-align: center;
                  widows: 0;
                  orphans: 0;
                  font-size: 10.5pt;
                "
              >
                <span style="font-family:&#39;宋体&#39;"
                  >(如果语言不清楚可附图，图过多可附件)</span
                >
              </p>
            </td>
          </tr>
        </tbody>
      </table>
      <p
        style="
          margin-top: 0pt;
          margin-bottom: 0pt;
          text-align: justify;
          widows: 0;
          orphans: 0;
          font-size: 10.5pt;
        "
      >
        <span style="font-family: Calibri;">&nbsp;</span>
      </p>
    </div>
  </body>
  <script>
    document.getElementById("default").style.background = "gray";
  </script>
</html>

#### 5.1.2. 压缩后

[html压缩工具](https://www.haorooms.com/tools/html_minifier/)
<html> <head> <meta http-equiv=Content-Type content="text/html; charset=UTF-8"> <meta http-equiv=Content-Style-Type content="text/css"> <meta name=generator content="Aspose.Words for .NET 17.1.0.0"> <title>Bug-1</title> <style>#default{background:#000}</style><body> <div> <table cellspacing=0 cellpadding=0 style=border-collapse:collapse> <tbody> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>BugID</span> </p> <td colspan=3 style=width:300.3pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;B-1</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>Bug</span> <span style=font-family:&#39;宋体&#39>标题</span> </p> <td colspan=3 style=width:300.3pt;border-top-style:solid;border-top-width:.75pt;border-right-style:solid;border-right-width:.75pt;border-left-style:solid;border-left-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;布尔表达式输入框为空时无提示</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>软件版本</span> </p> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;CellCover1.0</span> </p> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>测试环境</span> </p> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;1</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>测试人员</span> </p> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;王艺辉</span> </p> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>开发人员</span> </p> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>创建时间</span> </p> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;2020年5月31日</span> </p> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>Bug</span><span style=font-family:&#39;宋体&#39>状态</span> </p> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;待解决</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>测试阶段</span> </p> <td colspan=3 style=width:300.3pt;border-right-style:solid;border-right-width:.75pt;border-left-style:solid;border-left-width:.75pt;border-bottom-style:solid;border-bottom-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <input type=button style="height:11px;width:10px"> <span style=font-family:&#39;宋体&#39>接口测试、</span> <input type=button id=default style="height:11px;width:10px"> <span style=font-family:&#39;宋体&#39>功能测试、</span> <input type=button style="height:11px;width:10px"> <span style=font-family:&#39;宋体&#39>配置测试、</span> <input type=button style="height:11px;width:10px"> <span style=font-family:&#39;宋体&#39>回归测试</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>Bu</span><span style=font-family:Calibri>g</span><span style=font-family:&#39;宋体&#39>严重程度</span> </p> <td colspan=3 style=width:300.3pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <input type=button style="height:11px;width:10px"> <span style=font-family:&#39;宋体&#39>紧急、</span> <input type=button style="height:11px;width:10px"> <span style=font-family:&#39;宋体&#39>严重、</span> <input type=button id=default style="height:11px;width:10px"> <span style=font-family:&#39;宋体&#39>一般、</span> <input type=button style="height:11px;width:10px"> <span style=font-family:&#39;宋体&#39>轻微</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>问题优先级</span> </p> <td colspan=3 style=width:300.3pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>高</span> <span style=font-family:&#39;宋体&#39>、</span> <input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>较高</span> <span style=font-family:&#39;宋体&#39>、</span> <input type=button id=default style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>一般</span> <span style=font-family:&#39;宋体&#39>、</span> <input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>低</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>问题来源</span> </p> <td colspan=3 style=width:300.3pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <input type=button id=default style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>测试</span><span style=font-family:&#39;宋体&#39>、</span><input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>升级</span><span style=font-family:&#39;宋体&#39>、</span><input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>其他</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>问题类型</span> </p> <td colspan=3 style=width:300.3pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>功能问题</span><span style=font-family:&#39;宋体&#39>、</span><input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>版本问题</span><span style=font-family:&#39;宋体&#39>、</span><input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>遗留问题</span><span style=font-family:&#39;宋体&#39>、</span><input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>新需求</span><span style=font-family:&#39;宋体&#39>、</span><input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>配置错误</span><span style=font-family:&#39;宋体&#39>、</span><input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>性能问题</span><span style=font-family:&#39;宋体&#39>、</span><input type=button id=default style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>设计问题</span><span style=font-family:&#39;宋体&#39>、</span><input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>偶发性错误</span><span style=font-family:&#39;宋体&#39>、</span><input type=button style="height:11px;width:10px"><span style=font-family:&#39;宋体&#39>其他</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>缺陷的触发条件</span> </p> <td colspan=3 style=width:300.3pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>操作步骤（简）</span> </p> <td colspan=3 style=width:300.3pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;1.打开测试用例生成页面<br>2.设置表达式输入框为空<br>3.点击生成按钮</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>实际结果</span> </p> <td colspan=3 style=width:300.3pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;无任何结果</span> </p> <tr> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>预期结果</span> </p> <td colspan=3 style=width:300.3pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;弹出"请重新输入表达式不为空"的提示框</span> </p> <tr style=height:96.1pt> <td style=width:92.9pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>附图</span> </p> <td colspan=3 style=width:300.3pt;border-style:solid;border-width:.75pt;padding-right:5.03pt;padding-left:5.03pt;vertical-align:middle> <p style=margin-top:0;margin-bottom:0;text-align:center;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:&#39;宋体&#39>(如果语言不清楚可附图，图过多可附件)</span> </p> </table> <p style=margin-top:0;margin-bottom:0;text-align:justify;widows:0;orphans:0;font-size:10.5pt> <span style=font-family:Calibri>&nbsp;</span> </p> </div> <script>document.getElementById("default").style.background="gray";</script>
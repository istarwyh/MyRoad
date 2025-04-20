所谓Deep Research ,针对的是所有搜索资料-分析-得出结论的研究性问题。具体到各行各业的应用场景中，可能就有“这个零食的销售额怎么下降了？”、“这个保险产品的赔付率为什么上升了？”、“我要去潮汕旅游，有什么好吃好玩的推荐”等等问题。这种问题中，核心要解决几个问题：

1. 找什么资料？
2. 什么资料符合标准？
3. 根据当前的资料还能有什么启发？
4. 找到什么时候停止？
5. 用什么方式来解读这些资料？

可以适用以下架构： ![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202503151632803.png)

```plantuml
@startuml

!define MAIN_ORANGE #FF8C00
!define LIGHT_BLUE #87CEEB
!define LIGHT_GREEN #90EE90
!define LIGHT_YELLOW #FFFACD
!define LIGHT_PURPLE #D8BFD8

skinparam defaultFontName Arial
skinparam defaultTextAlignment center
skinparam rectangle {
  BorderColor black
  BackgroundColor LIGHT_YELLOW
  roundCorner 15
}

rectangle "触发源" <<Process>> ##LIGHT_GREEN {
  [用户问题] as UserQ
  [上游调用] as warnQ
  [系统自动触发\nAuto-Approve] as AutoQ
}

rectangle "用户交互" <<Interface>> ##LIGHT_GREEN {
  [问题澄清] as QC
note right of QC : qwen-7b
}

rectangle "决策模块" <<Decision>> ##MAIN_ORANGE {
  [context理解&决策] as MultiTurn
note right of MultiTurn : qwen-72b
}

rectangle "数据分析模块" <<Data>> ##LIGHT_PURPLE {
  [查询专家框架] as ExpertFrame
note left of ExpertFrame : qwen-7b
  [按专家框架 or 发散思考] as DivergentThinking
note right of DivergentThinking : deepseek-r1
  [语义实体值识别] as EntityRecog
note right of EntityRecog : qwen-7b

  rectangle "多方向搜集数据" <<SubSystem>> {
    [关键词召回] as Keywords
note right of Keywords : 正则
    [API调用/SQL] as APICall
note bottom of APICall : qwen-7b
    [数据清洗&总结] as DataAssembly
note right of DataAssembly: qwen-7b
  }

  [宏Prompt] as UserIdentity
  [风险报告生成] as ReportGen
note right of ReportGen: deepseek-r1
}

rectangle "推理模块" <<Process>> ##LIGHT_BLUE {
  [推理回答] as Reasoning
note right of Reasoning: deepseek-r1
}

rectangle "问题推荐模块" <<Process>> ##LIGHT_GREEN {
  [相关问题/动作推荐] as Recommendation
note right of Recommendation : qwen-7b

}

' Connections
UserQ --> MultiTurn
AutoQ --> MultiTurn
warnQ -> MultiTurn
MultiTurn --> QC
QC --> MultiTurn

MultiTurn --> ExpertFrame : 需要数据支撑
MultiTurn --> Reasoning : 不需要数据支撑

ExpertFrame --> EntityRecog : 可能无匹配框架
EntityRecog --> DivergentThinking: 条件明确

EntityRecog --> QC : 条件不明确

DivergentThinking --> Keywords
DivergentThinking --> APICall
Keywords --> DataAssembly
APICall --> DataAssembly

DataAssembly --> ReportGen
UserIdentity --> ReportGen

ReportGen --> Recommendation
Reasoning --> Recommendation

Recommendation --> AutoQ : 推荐问题\n作为新输入

@enduml
```

目前文档搜索类产品的**局限性集中在：**

1.用户提出的问题太复杂，需要深层次分析能力，**在现有文档中没有回答，** 它们应该是人工智能自行生成的全新见解。

2.答案不是单纯的**文字**，而是**图表、表格、图像**，无法直接输出结构化数据。

3.输出结果非常重要，容错率很低，因此过程不能依赖**黑匣子系统。**

**Hebbia**以引用的形式提供答案，并且所有的这些都在一个电子表格的形式中呈现。对于每个文档（行！），你将得到一组问题（列！）的答案，除了总结每个查询外，它还展示了得出结论的来源和各个步骤，并允许用户验证，提供AI处理流程的透明度，建立用户对输出结果的信任和信心。

## 知识图谱增加可解释性

https://neo4j.com/labs/genai-ecosystem/llm-graph-builder/

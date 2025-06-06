## 定义问题

### 业务问题

各行各业都有一些枯燥乏味易出错还不得不做的工作，比如 a16z 举的[律师工作例子](https://a16z.com/announcement/investing-in-hebbia/)：

> 作为一名专业律师，我在双显示器屏幕前度过了数千小时。我的右屏用于“真正”的工作，比如给客户起草法律意见书或诉状，这项工作专业性要求很高。我的左屏用于辅助我的工作，比如翻阅数百页的尽职调查报告，从庞大的案例库中搜寻相关判例，并对成千上万的证据材料进行梳理分析，目的仅在于挖掘出少数几个关键的法律问题和证据线索。这项工作，既不具挑战性也不令人愉快，但确是现阶段必要的。

其实我们保险内部理赔专家也是这样。当保单发生理赔报案，保险公司首先审查和评估检查这个索赔申请，以确定是否符合保险合同的条款和条件，特别是该案件是否存在欺诈风险，最终决定是否进入赔付流程。 ”核赔“是保险领域核心节点，以前这个节点由理赔专家来人肉把关，平均一个案件需要 2 小时来判断，碰到复杂案件可能需要一两天。效率低、消耗专家精力的同时，专家精力之外大量的案件难以处理，存在在大量案件之中的理赔线索也一直沉睡着没有人可以消费。当 AI Agent 被提出来时，我们很自然想到这是一个解放专家的好机会。第一步我们需要利用 AI Agent 来自动化、智能化分析案件、判断风险；第二步我们还可以消费存在在现实大量案件中的理赔知识。

### 传统业务解法

先从第一步说起，我们需要先看看传统的单独的案件分析流程是怎么样的？

1. **收集资料**：获取案件被调查人基本信息以及相关的索赔材料，如医疗报告、事故报告、检方报告。
2. 收集线索：根据获得的基本信息，搜集被调查人的就医情况、LBS 轨迹、财务状况、投保记录等线索信息,可能在某个数据查询过程中触发其他更多线索信息搜集
3. 推理风险：发现线索之间的关联性，推断案件是否存在理赔反欺诈风险。如果有必要需要继续收集被调查人亲友的线索。
4. 出具核赔报告：综合以上信息以及推理的过程，结构化、标准化按要求出具给保司核赔报告

举个案例： 24 年 5 月 赵某在家中一氧化碳中毒死亡，家属发现其还在意外险保障期限内，为其报案索赔。专家接手这个案件后，排查到关于赵某的线索如下：

|  | 资金流水 | 购物记录 | 财务情况 | 就医记录 | 同业投保 |
| --- | --- | --- | --- | --- | --- |
|  | 某男性20 笔送礼记录，半月前停止 | 一个月内购买烤炉+木炭 | 预测月入 1 w,无逾期记录,家庭流动性资产 3w+ | 近期没有 | 意外险连续 4 年续保、未多投，23 年加保至100w |
| 推理点 | 疑似男友，近期关系破裂 | 寒冷天气已过，疑似非正常购买 | 财务状况无明显异常 | 身体无明显异常 | 不是多头投保，但是不久前加保了 |

赵某的死亡表面上是由于一氧化碳中毒引起的意外事件，但是综合起来会发现存在以下疑点：**非正常购买行为**、**关系破裂**、**意外险加保**，所以专家判断这个案件很有可能是被调查人自杀！如果能确证自杀，保险公司就不需要受理案件索赔。但即使证据不足以完全确认被调查人自杀，保司的理赔金额也可以减少。

之于以上的案件分析流程，就有了我们通过 AI Agent 打造案件分析智能体的基础。

### 技术挑战

#### 数据安全

AI 本身是基于公域数据训练的，想要做公司内部的业务必须提供私域数据给 LLM 。但是私域数据参与提问和推理存在不可控安全隐患。

#### LLM 存在幻觉

在自由生成文本时，可能产生与现实不符的“幻觉”内容，影响信息的准确性。比如判断是否投保逆选择，有一个简单的判断依据: 投保日期与出险日期相距的时间远近。假如某个案件的投保日期是 2024 年 4 月 1 日，出险日期是 2024 年4 月 10 日，这是一个很明显的逆选择案例，那么 LLM 可以判断准确吗？不一定的，如果我们不对判断结果进行检查， LLM 是否能判断准确是个概率性问题。

#### LLM推理能力不足

在做意外险反欺诈推理的过程中，什么时候 LLM 能判断出一个原本是自杀风险案件的自杀风险？比如有刚提到的死者死于煤炭一氧化碳中毒，LLM 应该如何找寻线索？实际专业的尽调人员思考后会发掘：

1. 死者的关于煤炭的商品浏览记录、交易记录
2. 死者所处的地域时间当时烧煤取暖必要性
3. 死者生前的情感关系这样一些线索，LLM 通过推理未必能知道要从这些方向去收集线索。

#### 专业知识理解

比如无法理解核赔专业领域“黑话“：

1. **逆选择（Adverse Selection）**：指投保人在选择保险时，比保险公司拥有更多信息，往往更倾向于选择那些对他们自己有利而对保险公司不利的保险。
2. **道德风险（Moral Hazard）**：指保险合同生效后，投保人或被保险人可能因为有了保险保障而采取更高风险的行为。
3. **多头投保**（Multiple Insurance）：指被保险人在不同的保险公司购买了多份同类型的保险，以期在发生保险事故时获得超过实际损失的多重赔偿。
4. **多头借贷**（**Over-borrowing**）：指的是借款人在多个金融机构同时申请贷款。
5. **失信被执行**（person subject to enforcement for breach of trust）：指被法院判决后不履行法律文书确定的义务，被列入失信名单的个人或单位。
6. **除外责任（Exclusion）**：保险合同中明确列出的保险公司不予承担赔偿责任的风险或情况。
7. **风险评估（Risk Assessment）**：核保过程中对被保险人或保险标的潜在风险的评估。

## 技术调研

### Agent Vs RPA

RPA(`Robot Process Automation`) 是对业务流程自动化技术的传统称呼。我们首先面临的一个问题，就是为什么要选择AI Agent 来做案例分析？使用 Agent 来做和 RPA 有什么不一样。在 SOP(Standard Operating Procedure) 中，通过 LLM的结构化输出已经实现工具使用，这时 LLM 充当的是 SOP 中的一个普通的角色，认为整个流程是 RPA 也未尝不可。然而重要的区别在于，这些应用程序利用 LLM 作为NER(Named Entity Recognition)和推理、总结的“工具”，但它们下一步做什么仍然是由代码预先确定的。相比之下，将 LLM 置于应用程序的控制流中并让它动态决定要采取哪些操作、使用哪些工具以及如何解释和响应输入时，这才是真的 Agent 。从目前Agent 的发展我们可以看到Agent 提供：

- 高度的自主性
- 自然语言交互能力。从 传统GUI（图形用户界面）、TUI（触控用户界面）到LUI（自然语言用户界面）的转换。LUI 替换掉结构化输入中间环节，例如：菜单、导航、表单等在LUI下可能就不再必要。只要我们的意图表达清晰，结果就能以更自然的多模态方式呈现。
- 为在复杂多变的业务环境、执行多样化的任务中灵活应对储备能力。

上一代 RPA 公司无法实现真正自动化的承诺，无数做到真正的“数字员工”，因为当时的技术还不够先进。但是 AI Agent 不是硬编码每个确定性步骤，而是根据最终目标（而触发点只是一个相对模糊的指令）在内部决定采用什么工具和什么响应。AI Agent 能够适应各种数据输入，并能够处理业务流程中的变化。正因为这种灵活性，它们将比传统的 RPA 系统更容易实施和维护[^flexible]。 ![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202411171611874.png)

所以 Agent 肯定是 RPA 的下一个阶段。

### Agent 组成

2023 年 OpenAI 研究员 [lilianweng](https://lilianweng.github.io/posts/2023-06-23-agent/) 提出了 Agent 系统的基本组成：

- 规划。Agent不会尝试通过单线程的下一个标记预测序列来解决复杂问题（例如一口气写完一篇文章，从第一个单词开始，直到最后一个单词才停止），而是遵循更像人类的思维过程，将工作分解为更小的子任务和计划，反思进度，并根据需要重新调整。

- 记忆。除了一般知识外，代理还需要（外部）记忆来存储和调用特定领域的知识以及他们正在解决的问题的有界上下文，比如使用 Postgres + Vector或 chroma作向量数据库。

- 工具使用&执行。代理使用工具来执行任务，以增强其解决问题的能力。操作工具箱包括网页浏览、代码解释、身份验证和授权，以及大量的企业内部的 API 。

![](https://lilianweng.github.io/posts/2023-06-23-agent/agent-overview.png)

后来在实践中人们往往还会强调“推理”这个组成:

- 推理。基础模型（如Anthropic 和 GPT4o）将部分世界模型编码到 LLM 的预训练权重中，并利用这些权重来获取一般知识和基本逻辑。这是 Agent 能够对非结构化数据进行推理的基础。

### Agent 框架选型

这里值得单独再写一篇文章，目前看来我们遇到的 Agent 框架包括：

- AgentUniverse:亮点在于有蚂蚁及内部业务方在背后支持，比较完善与落地
- [Autogen](https://github.com/microsoft/autogen)：支持不同领域或工具的专门化代理之间的协作，模拟人类团队合作
- [LangGraph](https://www.langchain.com/langgraph)：很灵活，同时提供了 Human-In-The-Loop
- [Taskweaver](https://github.com/microsoft/taskweaver/)：亮点在于对代码执行的场景做了优化，使得代码执行本身有状态
- [Swarm](https://github.com/openai/swarm)：agent之间互相传递信息与任务简单
- [Magentic-One](https://github.com/microsoft/autogen/tree/main/python/packages/autogen-magentic-one#magentic-one): 亮点在于计划是动态生成的，并且集成了多模态识图、代码生成并执行以及浏览器内容提取等agent

以及业界流行的 4 种基本 Agent 模式： ![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202410140018655.png)

最后我们选择了蚂蚁开源的 Agentuniverse 和 综合了 4 种基本 Agent模式的 PEER （Plan-Execute-Expressing-Review）模式。

不过 Au

### AgentUniverse 原理简介

- load the configuration file
- load user custom key
- init loguru loggers
- init web server
- init all extension module
- init monitor module
- scan and register the components

## 问题拆解

### 技术栈

检测层输入层控制层数据层 记忆层模型层

### 流程图

意图 shot 意图 key 和方法调用一一对应，方法参数由 NER 提取得到

ask -> Facade Agent -> NER agent -> Planner Agent -> planning agent 决定调查方向 -> 查询调查方向对应 Agent 是否已经已经在 executing 数组中,在就执行 -》 expressing agent -》 review agent

planner Agent 是某一个模式的封装

### 解法

#### 工具准备

LLM 自己的知识再丰富，也需要 Tool 来帮他获取现实中的信息。通过将工具描述作为上下文输入，可以让模型自己决策选择哪些工具。为了让 LLM 处理更容易点，工具描述应当简单并且结构化。但是还有另外一个问题，就是参数。LLM 选择完工具调用时必须要提供足够的参数，考虑到我们的场景不会涉及写入数据，只有查询并消费数据，我们将所有必要的中间过程数据都放入 context 中，这样被调用的工具提取自己需要的参数消费即可。值得一提的是，直接在 prompt 中提供工具描述容易受到 LLM 窗口大小的限制，所以微软提出来 [ToolGen](https://arxiv.org/abs/2410.03439),直接将工具知识包括参数直接 fine-tune 到 LLM 内部。这样用户的查询不用再经过单独的工具上下文补充，LLM 原生可以输出工具标识。不过我们的场景中每个 Agent 负责的是一个调查方向，LLM 只用决策哪些调查方向，而不用具体关心要用哪些工具，就不会存在 ToolGen 要解决的问题。

#### Prompt 工程

尽管某些技术特定于某些类型的问题，但其中许多是基于可应用于广泛任务的一般原则[^reliability]。比如

- 给出更清晰的指示
- 将复杂任务分解为更简单的子任务
- 构建指令以使模型专注于任务
- 提示模型在回答前进行解释

#### RAG 召回few-shot

RAG 系统常常无法全面理解用户查询背后的上下文；这是 RAG 最大的缺点之一：

- **与用户意图不一致**：如果系统未能准确理解用户的意图，可能会检索到与用户需求不一致的文档。这可能导致响应不相关或偏离主题。
- RAG 可能检索到与查询不相关的文档，或者当检索的文档未能提供足够的上下文或模型误解信息时，可能导致事实错误或误导性的答案。

为了解决这个问题，微软亚洲研究院（MSRA）提出了一种**RAG任务分类方法**，根据所需的外部数据类型和任务的主要**焦点**，**将用户查询分为四个层次**：

1. 显式事实查询（Level-1）：直接从数据中检索明确的事实。

2. 隐式事实查询（Level-2）：需要推理或整合数据中的隐含信息。

3. 可解释推理查询（Level-3）：需要理解并应用领域特定的理由和逻辑。

4. 隐式推理查询（Level-4）：需要从数据中推断出隐含的推理逻辑。

在实践中，我们可以按需要生成原始查询的多种解释，使系统能够涵盖更广泛的潜在含义和上下文，从而帮助检索到更相关的文档。重新排序过程进一步细化结果，使最终输出即使在查询模糊或多层面时也能与用户意图更加一致，这与人类的查询方式相似。

#### 回测

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202410072328421.png)

- 准确率
- 阳性发现率

## 总结思考

### 智能的实现路径

在我们这个案件分析实现的过程中，或许可以总结一个顺口溜，”先线下再线上、先线上再SOP、先 SOP 再 RPA、先 RPA 再Agent“。拆看来看下便是：

- **线下**：最初的核赔流程是完全依赖人工操作的，所有的审查和评估工作都是由理赔专家手动完成。
- **线上**：在这个阶段，保险公司开始将一些核赔流程数字化，例如使用电子表单和在线系统来收集和存储索赔信息。
- **SOP (Standard Operating Procedure)**：制定标准操作流程，以确保线上核赔操作的高效、一致和可重复性。有了标准化的操作流程后，可以开始考虑自动化。
- **RPA (Robotic Process Automation)**：使用机器人流程自动化技术来执行标准化的操作流程。RPA 可以处理大量重复性、高频率的任务，进一步解放人工，提高效率。虽然 RPA 可以自动化很多重复性任务，但它主要是基于规则和预定义的流程，缺乏智能化分析和决策能力。
- **Agent (AI Agent)**：引入 AI Agent，可以在 RPA 的基础上，增加智能化分析和决策功能。AI Agent 能够处理复杂的核赔任务，如风险评估和欺诈检测，进一步提升核赔流程的智能化水平。

正因为在做案件分析智能体之前，我们已经积累了案例库和大量案件分析需要的原始数据，比如就医数据、LBS 数据、财务状况数据、投保记录数据等，并且形成了一定的 SOP，每个阶段都为下一阶段奠定基础，我们才能顺畅地做 Agent 。

### AI 发展如火如荼

大模型技术的革新，极大地降低了技术门槛，真正实现了智能化技术的普惠与平权，也加速了整个 AI 行业的发展，各种新的开发范式、架构、类库需要参与者保持足够的敏感和开放，才能跟上时代的发展。

### 准确率提升：乘积效应

RAG效果提升是一个非常系统性的工作，要做到比较好的效果，有非常多的智能化和工程策略的事情要做，没有银弹，要抓关键细节一个个去做实做深。从Query理解到知识维护，再到多路召回策略的优化，每一个环节精细打磨，都是实现效果飞跃的关键。当效果达到70%后，则更需保持耐心与毅力，继续深挖细节，以精益求精的态度，逐步突破瓶颈，迈向更高的巅峰。

## 未来展望

### 引入强化学习增强推理过程

语言代理树搜索(LATS)，它将蒙特卡洛树搜索（AlphaGo背后的基于模型的强化学习技术）应用于语言代理。LATS 允许代理探索实现目标函数的多种轨迹，优先考虑高回报路径，纳入反馈并在必要时回溯。在生产力（如办公软件、效能类软件、设计软件等）、搜索和信息（如新闻头条）等领域，大模型的颠覆速度很快。因为大模型的“解题”能力已经很强，最新的GPT-o1 达到了博士水平，在某些细分领域超越了人类专家。大模型在这些领域能够大幅度帮助人类Save Time，提升人类的效率。

### 有向无环图回溯规划

让 LLM 思考时不单单使用 Chain,鼓励使用 DAG。

### 人类干预

Human-In-The-Loop

### LUI + 多模态

[^flexible]: _https://a16z.com/rip-to-rpa-the-rise-of-intelligent-automation/_
[^reliability]: https://cookbook.openai.com/articles/techniques_to_improve_reliability

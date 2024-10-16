## 定义问题
### 业务问题
### 传统业务解法
业务流程图
### 业务挑战
在当前的商业环境中，销运团队作为企业连接市场与客户的桥梁，面临着众多挑战。然而，随着业务规模扩大，一系列痛点逐渐显现，亟待通过产品建设来加以解决。  

- 知识碎片化：业务多元化发展，各类重要产品知识、营销通案、政策、流程文档等关键信息，往往未能有效整合零散地分布在不同的团队或个人手中，进而导致信息获取不便利。
    
- 问题解决慢：在日常工作中，销运团队需要与多个部门或团队进行协作。然而，由于架构和人员的变动，当面临问题时无法快速定位到对接人，加之人工响应的时效性难以保证，这进一步加剧了问题解决效率。
    
- 销运支持资源有限：即便增加人力也无法赶上日益增长的业务需求，且宝贵的经验与专业知识难以得到有效传播与深入交流。
    

针对上述痛点，快手商业化团队急需开发一款集知识管理、团队协作与智能答疑于一体的产品。
### 技术挑战

#### 数据安全
私域数据参与提问和训练存在不可控安全隐患。

#### LLM 存在幻觉
在自由生成文本时，可能产生与现实不符的“幻觉”内容，影响信息的准确性。比如判断是否投保逆选择，有一个简单的判断依据: 投保日期与出险日期相距的时间远近。假如某个案件的投保日期是 2024 年 4 月 1 日，出险日期是 2024 年4 月 10 日，这是一个很明显的逆选择案例，那么 LLM 可以判断准确吗？不一定的，如果我们不对判断结果进行检查， LLM 是否能判断准确是个概率性问题。
#### LLM推理能力不足
在做意外险反欺诈推理的过程中，什么时候 LLM 能判断出一个原本是自杀风险案件的自杀风险？比如有一死者死于煤炭一氧化碳中毒，LLM 应该如何找寻线索？实际专业的尽调人员思考后会发掘：
1. 死者的关于煤炭的商品浏览记录、交易记录
2. 死者所处的地域时间当时烧煤取暖必要性
3. 死者生前的情感关系
这样一些线索，LLM 通过推理未必能知道要从这些方向去收集线索。

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
RPA(`Robot Process Automation`) 是对业务流程自动化技术的一种传统称呼。我们首先面临的一个问题，就是为什么要选择AI Agent 来做案例分析？使用 RPA 来做会有什么不一样。
回顾业务问题的传统人工处理流程我们发现，
1、 从案件信息中提取被保人实体信息
2、根据被保人信息查询就医、活动轨迹、财务、投保行为等数据，可能在某个数据查询过程中触发其他更多线索信息的查询
3、根据被保人信息判断是否要查询投保人和其他相关人员的信息提供线索
4、综合信息推理是否存在理赔反欺诈风险
5、给保司出具理赔报告

通过 LLM的结构化输出可以实现工具使用。然而，重要的区别在于，这些应用程序利用 LLM 作为NER(Named Entity Recognition)和推理、总结的“工具”，但它们下一步做什么仍然是由代码预先确定的。

相比之下，当你将 LLM 置于应用程序的控制流中并让它动态决定要采取哪些操作、使用哪些工具以及如何解释和响应输入时，代理就会出现。

逻辑控制流仍由应用程序预先定义。LLM 动态编写部分或全部自己的控制逻辑。

Agent 提供了：
- 高度的自主性
- 自然语言交互能力。从 传统GUI（图形用户界面）、TUI（触控用户界面）到LUI（自然语言用户界面）的转换。LUI 替换掉结构化输入中间环节，例如：菜单、导航、表单等在LUI下可能就不再必要。只要我们的意图表达清晰，结果就能以更自然的多模态方式呈现。
- 为在复杂多变的业务环境、执行多样化的任务中灵活应对储备能力。

在生产力（如办公软件、效能类软件、设计软件等）、搜索和信息（如新闻头条）等领域，大模型的颠覆速度很快。因为大模型的智力水平已经超越了普通人类，最新的GPT-o1 已经达到了博士水平，在某些细分领域超越了人类专家。大模型在这些领域能够大幅度帮助人类Save Time，提升人类的效率。
所以我们要选择 Agent


有人会畅想 AGI（Artificial General Intelligence,通用人工智能），认为大模型终有一天你给它讲一个需求，它会一步到位，把所有东西都帮你干完了。但如果我们仔细思考，就会发现人类不是这样子的，并不存在一个一超多能、一步到位的智能。智能需要迭代、试错、大规模协作、和使用工具。这正是Agent可以做到的。
### Agent 组成
2023 年 OpenAI 研究员 [lilianweng](https://lilianweng.github.io/posts/2023-06-23-agent/) 提出了 Agent 系统的基本组成：

- 规划。Agent不会尝试通过单线程的下一个标记预测序列来解决复杂问题（例如一口气写完一篇文章，从第一个单词开始，直到最后一个单词才停止），而是遵循更像人类的思维过程，将工作分解为更小的子任务和计划，反思进度，并根据需要重新调整。

- 记忆。除了一般知识外，代理还需要（外部）记忆来存储和调用特定领域的知识以及他们正在解决的问题的有界上下文，比如使用 Postgres + Vector或 chroma作向量数据库。
    
- 工具使用&执行。代理使用工具来执行任务，以增强其解决问题的能力。许多早期的代理平台都提供预定义在代码中的自定义操作工具箱，供代理选择。但许多通用代理工具也开始出现，包括网页浏览、代码解释、身份验证和授权，以及与 CRM 和 ERP 等企业系统的连接器，以在这些系统内执行 UI 操作。

![](https://lilianweng.github.io/posts/2023-06-23-agent/agent-overview.png)

后来在实践中人们往往还会强调“推理”这个组成:
- 推理。在最基本的层面上，Agent必须能够对非结构化数据进行推理。基础模型（如Anthropic * 和 OpenAI）已经非常有效地实现了这一功能，它们将部分世界模型编码到 LLM 的预训练权重中，并利用这些权重来获取一般知识和基本逻辑。

### Agent 框架选型
- AgentUniverse
- LangGraph
- [Autogen](https://github.com/microsoft/autogen)
- [Langraph](https://www.langchain.com/langgraph)
- [Taskweaver](https://github.com/microsoft/taskweaver/)
- Swarm
4 种基本 Agent 模式：
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202410140018655.png)
## 问题拆解


### 技术栈
检测层
输入层
控制层
数据层 记忆层
模型层
### 流程图
意图 shot 
意图 key 和方法调用一一对应，方法参数由 NER 提取得到

### 解法
#### 工具准备
LLM 自己的知识再丰富，也需要 Tool 来帮他获取现实中的信息。通过将工具描述作为上下文输入，可以让模型自己决策选择哪些工具。为了让 LLM 处理更容易点，工具描述应当简单并且结构化。但是还有另外一个问题，就是参数。LLM 选择完工具调用时必须要提供足够的参数，考虑到我们的场景不会涉及写入数据，只有查询并消费数据，我们将所有必要的中间过程数据都放入 context 中，这样被调用的工具提取自己需要的参数消费即可。
值得一提的是，直接在 prompt 中提供工具描述容易受到 LLM 窗口大小的限制，所以微软提出来 [ToolGen](https://arxiv.org/abs/2410.03439),直接将工具知识包括参数直接 fine-tune 到 LLM 内部。这样用户的查询不用再经过单独的工具上下文补充，LLM 原生可以输出工具标识。不过我们的场景中每个 Agent 负责的是一个调查方向，LLM 只用决策哪些调查方向，而不用具体关心要用哪些工具，就不会存在 ToolGen 要解决的问题。
#### Prompt 工程

尽管某些技术特定于某些类型的问题，但其中许多是基于可应用于广泛任务的一般原则[^reliablity]。比如
- 给出更清晰的指示
- 将复杂任务分解为更简单的子任务
- 构建指令以使模型专注于任务
- 提示模型在回答前进行解释

[^reliablity]: https://cookbook.openai.com/articles/techniques_to_improve_reliability
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
### AI 发展如火如荼
大模型技术的革新，极大地降低了技术门槛，真正实现了智能化技术的普惠与平权，也加速了整个 AI 行业的发展，各种新的开发范式、架构、类库需要参与者保持足够的敏感和开放，才能跟上时代的发展。
### 准确率提升：乘积效应
RAG效果提升是一个非常系统性的工作，要做到比较好的效果，有非常多的智能化和工程策略的事情要做，没有银弹，要抓关键细节一个个去做实做深。从Query理解到知识维护，再到多路召回策略的优化，每一个环节精细打磨，都是实现效果飞跃的关键。当效果达到70%后，则更需保持耐心与毅力，继续深挖细节，以精益求精的态度，逐步突破瓶颈，迈向更高的巅峰。

## 未来展望
### 引入强化学习增强推理过程
语言代理树搜索(LATS)，它将蒙特卡洛树搜索（AlphaGo背后的基于模型的强化学习技术）应用于语言代理。

LATS 允许代理探索实现目标函数的多种轨迹，优先考虑高回报路径，纳入反馈并在必要时回溯。

### 有向无环图回溯规划
目前的
### 人类干预

### LUI + 多模态


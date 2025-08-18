## 1. 引言：智能体工作流中的状态挑战

### 1.1 智能体工作流中的状态挑战

传统的LLM应用常常被视为一系列无状态的API调用，每个请求都是独立的，无法记忆先前的交互或复杂的推理过程。这种设计对于简单的问答或内容生成任务尚可，但面对需要多步骤、多轮次、甚至需要工具调用的复杂任务时，其固有的局限性便会显现。例如，一个需要通过网络搜索、代码执行并生成报告的深度研究任务，如果无法在步骤间维持上下文和中间结果，将无法连贯地完成。

LangGraph正是为了解决这一根本挑战而诞生的。它将复杂的智能体应用建模为有状态的图（Stateful Graph），其中每个节点（Node）代表一个计算步骤，而边（Edge）则控制着工作流的流向。这种架构允许工作流在节点间传递和共享一个中央的、不断演进的全局状态。

### 1.2 为什么需要状态持久化：容错、记忆与人机协作

LangGraph的持久化为智能体工作流带来了三大核心能力，这些能力也是LangGraph官方和社区反复强调的核心价值。

1. **记忆（Memory）**：这是最直观的价值。通过将对话历史和其他上下文信息保存在持久化存储中，智能体能够跨多次调用甚至跨会话维持记忆，从而实现自然的、连贯的对话体验 1。这种“记忆”能力是构建多轮聊天机器人或个性化助手的基石。

2. **容错（Fault-Tolerance）**：在复杂的生产环境中，中断和失败是无法避免的。工作流可能会因为外部API超时、网络问题或内部错误而中断。持久化机制允许LangGraph在每个关键步骤后保存状态，从而在发生故障时能够从上一个有效的检查点（checkpoint）恢复执行，而无需从头开始 3。这种能力对于确保长程任务的可靠性和成本效益至关重要。

3. **人机协作（Human-in-the-Loop）**：许多企业级工作流需要人工审批或干预。通过持久化，LangGraph可以在特定节点暂停执行，将当前状态保存起来，等待人工输入或审查。一旦人类提供反馈，工作流便能从中断点无缝恢复。这种能力将智能体从完全自主的“黑盒”转变为可控的、可信赖的协作伙伴 3。

这些能力并非LangGraph的附加功能，而是其`checkpointer`持久化机制的直接结果。通过将图的状态与外部存储绑定，LangGraph将状态管理提升到可信赖、可恢复的水平，为构建真正的生产级智能体应用铺平了道路 3。

## 2. 核心机制：检查点与线程的解耦设计

### 2.1 状态图的共享状态模型（Graph State）

LangGraph的核心理念是其共享状态模型，即`StateGraph`。`State`是一个基于`TypedDict`定义的中央数据结构，充当整个工作流的“单一事实来源”。图中的所有节点和边都可以访问并更新这个共享状态，从而实现信息传递和上下文共享。

与传统的变量覆盖不同，LangGraph采用了一种优雅的更新机制。当一个节点执行完毕并返回一个包含部分更新的字典时，框架会自动将其合并到全局状态中。这得益于`TypedDict`中的`reducer`注解。例如，一个用于保存消息历史的列表可以被注解为`Annotated[list, operator.add]`。这意味着，当一个节点返回一个新的消息列表时，LangGraph不会覆盖原有的列表，而是将新消息追加到现有列表中，确保了历史信息的累积和完整性。

`State` 是 LangGraph 最被低估但至关重要的组件，它在所有节点和边之间共享数据和上下文。这种设计模式使得 LangGraph 不仅仅是存储数据，它实际上是在建模一个智能体的“心智状态”和“思考路径”。与传统 LangChain 的线性链（linear chain）不同，LangGraph 的 `StateGraph` 和 `Super-step` 提供了非线性的“认知过程”。这种架构使得开发者可以检查（`inspect`）或“时间旅行”（`time-travel`）到任意一个检查点 ，这相当于在审视智能体在某个特定时间点的“思维快照”，而不仅仅是数据快照。这种能力对于调试、审计和“`Human-in-the-loop`”（人工干预）至关重要。

### 2.2 检查点（Checkpointer）层：持久化的基石

`Checkpointer`是LangGraph内置的持久化层，负责将图的共享状态保存到外部存储中。它的工作机制紧密围绕着“超步”（super-step）这一核心概念展开。

**超步（Super-step）**：一个超步不是单个节点的执行，而是一个或多个并行执行的节点所构成的一个完整步骤。在每个超步开始时，LangGraph会根据最新的检查点状态决定哪些节点可以并行执行。当所有激活的节点都执行完毕后，框架会收集它们返回的所有状态更新。这些更新随后被合并到中央状态中，并在超步结束时，将这个最新的状态快照保存为一个`checkpoint`。

**状态快照（StateSnapshot）**：一个`checkpoint`本质上是一个`StateSnapshot`对象，它不仅仅是图状态的简单副本，还包含了丰富的元数据，例如：

- `values`：图状态通道的值，即共享状态的实际数据 3。这个值是一个字典，包含了所有需要持久化的数据，例如对话历史（ messages）、工具调用的中间结果（tool_results）以及任何其他自定义状态字段。例如，在一个研究代理中，这可能包括用户初始问题、生成的搜索查询列表、从网络上检索到的内容以及最终的答案。

- `config`：与此快照关联的配置信息，例如线程ID 3。

- `metadata`：描述本次超步的元数据，包括写入了哪些节点、步骤编号等 3。

- `next`：一个元组，包含了下一个要执行的节点名称 3。

- `tasks`: 一个元组，包含待执行任务的信息 。如果某个步骤之前已尝试过，它还会包含错误信息，这对于实现容错和重试机制至关重要。

```mermaid
graph TD
    subgraph "客户端"
        A[用户] --> B((前端应用))
    end
    subgraph "后端服务 (FastAPI)"
        C[API 网关] --> D(LangGraph Core)
        D --> |持久化请求| E(Checkpointer)
    end
    subgraph "基础设施"
        F[RDB]
        G[REDIS]
    end
    subgraph "可观测性"
        H((LangSmith / OpenTelemetry))
    end

    B -->|REST/WebSocket| C
    E -->|读/写状态| F
    D -->|异步任务/流式传输| G
    D -->|发送追踪/指标| H
    E -->|发送追踪/指标| H
    H --> |告警/仪表盘| I[运维团队]

    style B fill:#f9f,stroke:#333,stroke-width:2px
    style C fill:#ccf,stroke:#333,stroke-width:2px
    style D fill:#9cf,stroke:#333,stroke-width:2px
    style E fill:#f9f,stroke:#333,stroke-width:2px
    style F fill:#c9c,stroke:#333,stroke-width:2px
    style G fill:#f9c,stroke:#333,stroke-width:2px
    style H fill:#9c9,stroke:#333,stroke-width:2px
```

这种详细的快照结构使得状态不仅可恢复，还为调试、审计和分析提供了强大的数据支撑。

### 2.3 线程（Threads）：会话与历史管理

为了管理一系列连续的检查点，LangGraph引入了“线程”（Thread）的概念。`Thread`是一个唯一的标识符，通常与用户ID或会话ID相关联，用于组织和存储整个对话或任务的历史状态 3。

每个`thread`都包含一个按时间顺序排列的检查点序列。通过将`thread_id`与一个特定的`checkpoint_id`（检查点的唯一标识符）结合起来，开发者可以实现所谓的“时间旅行”（time travel）能力 3。这意味着，即使工作流已经执行了很长一段时间，开发者也可以通过指定一个历史检查点，让图从该点恢复并重新执行，这对于复杂的调试、错误恢复或探索不同的执行路径至关重要。框架能够识别哪些步骤已经被执行过，并跳过这些步骤的重新计算，从而只执行从指定检查点开始的新步骤，确保了效率 3。

**在一个包含多个节点和复杂逻辑（如循环、分支和多代理协作）的 LangGraph 图中，持久化发生在每个“超步”结束时 。这意味着，当一个或多个节点完成执行并更新图状态后，检查点机制会捕获这个新的状态快照并将其写入数据库 。这为调试提供了极大的便利，因为开发者可以“回溯”到任何一个中间状态快照，查看特定节点的输出 。此外，LangGraph 的“回放”（replay）功能允许工作进程从指定的checkpoint_id 开始恢复执行，跳过那些已经完成的步骤，从而实现精确的任务续航 3。**

## 3. 数据流与生产级架构图剖析

### 3.1 数据流图：超步执行与检查点生成

以下是LangGraph在一次超步执行中，数据流经图和持久化层的详细步骤：

1. **调用与加载**：用户通过`invoke`或`stream`调用触发一次图执行，并提供一个`thread_id`。LangGraph框架根据此`thread_id`从`checkpointer`中检索最新的`checkpoint`，加载图的最新状态。

2. **并行执行**：图引擎识别出当前状态下所有待执行的节点，并开始并行执行它们。

3. **局部更新**：每个节点读取当前共享状态，执行其逻辑（如调用LLM、工具或函数），然后返回一个只包含其所需更新的局部字典（Partial Update）。

4. **状态合并**：在所有并行节点执行完毕后，LangGraph收集所有局部更新，并根据`reducer`规则将它们合并到全局状态中。

5. **原子性写入**：新的全局状态作为一个完整的`checkpoint`，通过`checkpointer`的原子写入操作（例如`put`或`put_writes`）被保存到持久化后端。

6. **结果返回/流式传输**：如果执行已达终点，返回最终结果。如果工作流尚未结束，LangGraph会根据新的状态决定下一个超步，并可以立即将本次超步的中间结果（例如`tool_calls`或`intermediate thoughts`）流式传输给前端 9。

```mermaid
graph TD
    subgraph "LangGraph Core"
        A[用户输入] --> C(Graph State)
        C --> D{节点 1}
        D --> E{节点 2}
        E --> F{节点 N}
        D -->|局部更新| C
        E -->|局部更新| C
        F -->|局部更新| C
    end

    C <--> |读/写| B[Checkpointer]
    B <--> |持久化| G[数据库]
```

值得注意的是，LangGraph的流式传输（`stream()`和`astream()`）功能看似与“持久化”这种听起来缓慢的操作相悖，但实际上，持久化正是实现健壮实时体验的底层保障。传统的长程任务`invoke`模式必须等待整个图执行完毕，用户体验较差 12。然而，通过在每次超步后立即保存状态，并利用Redis等消息代理将中间结果推送给前端，LangGraph实现了“实时+持久”的混合模式。这种设计确保了即使在网络中断或前端故障的情况下，工作流也能从上一个保存的超步状态恢复，而不会丢失进度，从而极大地提升了用户体验和应用的可靠性。

### 3.2 生产级部署架构图：解耦LangGraph与基础设施

为了在生产环境中可靠地运行LangGraph应用，一个典型的部署架构会解耦核心逻辑与基础设施服务。以下是该架构中各组件的角色：

- **前端（Frontend）**：例如基于React的应用，通过WebSocket或Server-Sent Events（SSE）与后端API通信，以实现实时、双向的流式交互 13。

- **API服务（Backend API Service）**：通常使用FastAPI或LangGraph Platform Server构建，作为所有外部请求的入口。它承载着核心的LangGraph逻辑，处理用户请求，并协调与持久化层和任务队列的交互 14。

- **持久化数据库（Persistence Database）**：**PostgreSQL**通常是首选。其关系型特性和对ACID事务的强大支持，使其成为存储需要强一致性保障的持久化状态的理想选择。PostgreSQL存储着所有`assistants`、`threads`、`runs`以及长期记忆，确保了数据的高可用性和完整性 14。

- **任务队列/消息代理（Task Queue / Message Broker）**：**Redis**在此架构中扮演着关键的辅助角色。其低延迟和发布-订阅（pub-sub）模式使其非常适合处理两类任务：一是实时流式传输，当LangGraph生成中间事件时，Redis可以立即将这些事件推送到等待的前端客户端；二是后台长程任务，它能够将耗时的工作流作为后台任务启动，并使用`polling`或`webhooks`进行状态监控 14。

```mermaid
sequenceDiagram
    participant U as 用户
    participant F as 前端
    participant B as 后端服务 (FastAPI)
    participant L as LangGraph Core
    participant P as Checkpointer
    participant D as PostgreSQL
    participant R as Redis

    U->>F: 发送请求 (e.g., "帮我研究量子计算")
    activate F
    F->>B: POST /stream (带 thread_id)
    activate B
    B->>L: graph.astream(input, config)
    activate L
    L->>P: get(config)
    activate P
    P->>D: 检索最新检查点
    activate D
    D-->>P: 返回状态
    deactivate D
    P-->>L: 状态快照
    deactivate P

    loop 超步执行 (Super-step)
        L->>L: 执行超步 (节点计算)
        L->>P: put(new_state)
        activate P
        P->>D: 写入新检查点
        activate D
        D-->>P: 确认
        deactivate D
        P-->>L: 确认
        deactivate P
        L->>B: 返回流式块 (chunk)
        B->>F: 发送流式数据 (SSE/WebSocket)
        F->>U: 实时显示响应
    end

    L->>B: 结束流式传输
    deactivate L
    B-->>F: 结束
    deactivate B
    F-->>U: 最终响应
    deactivate F
```

这种PostgreSQL（持久性）+Redis（实时/异步）的**双数据库架构**是一种经过实践验证的生产级最佳实践。它将LangGraph的`durability`（耐用性）与`scalability`（可扩展性）完美结合，使得应用既能保证状态的完整性，又能处理高并发的实时请求和耗时的后台任务。

## 4. 持久化后端技术选型与考量

### 4.1 本地与实验环境：`InMemorySaver`与`SQLiteSaver`

- **`InMemorySaver`**：这是最简单的`checkpointer`实现，所有状态都保存在内存中，无法持久化。它适用于快速原型开发和概念验证，一旦程序关闭，状态便会丢失 5。

- **`SQLiteSaver`**：作为一个文件级的轻量级数据库，`SQLiteSaver`非常易于部署，无需独立的服务器进程。它非常适合本地工作流、演示和小型个人项目。其局限性在于不适合高并发和多进程环境 3。

### 4.2 生产环境：关系型数据库（`PostgresSaver`）与非关系型数据库

- **`PostgresSaver`**：LangGraph官方推荐的生产级`checkpointer`实现。它能够处理高并发场景，支持事务，并具有强大的数据完整性保障。在实际部署中，需要额外配置`autocommit=True`和`row_factory=dict_row`以确保正确操作。

- **其他数据库**：LangGraph提供了开放的`BaseCheckpointSaver`接口，允许社区和企业根据自身需求开发自定义的持久化后端。例如，社区已经实现了基于Couchbase的`checkpointer`，这展示了框架的灵活性和可扩展性，使其能够集成到各种复杂的数据生态系统中 6。

### 4.3 异步任务队列：`Redis`在流式传输和后台任务中的作用

长程任务，例如需要数小时才能完成的复杂研究或报告生成，不适合传统的HTTP请求-响应模式，因为长时间的连接会因超时而中断。在这种情况下，异步任务队列至关重要。

`Redis`作为消息代理，可以启动后台`background job`来处理这些长程任务。API服务可以立即返回一个任务ID，然后客户端可以使用`polling endpoints`或`webhooks`来监控任务状态，从而实现优雅的异步处理。这种设计模式使得LangGraph能够有效地处理需要长时间运行的复杂任务，例如定时执行的`cron`任务。

### LangGraph 持久化后端技术选型对比

| 后端名称 | 部署环境 | 安装复杂度 | 适用场景 | 核心优势 | 主要局限性 |
| --- | --- | --- | --- | --- | --- |
| `InMemorySaver` | 内存 | 极低（内置） | 原型验证、概念展示 | 快速，无依赖 | 不持久化，仅供演示 |
| `SQLiteSaver` | 本地文件 | 低 | 本地开发、个人项目 | 易于部署，无需服务器 | 不适合高并发，无法跨进程共享 |
| `PostgresSaver` | 生产服务器 | 中等 | 生产环境、企业级应用 | 高并发、强一致性、事务支持 | 需要独立部署和管理数据库 |
| `Redis` | 生产服务器 | 中等 | 生产环境、高并发实时任务 | 极低延迟，支持`pub-sub`和任务队列 | 不适合作为主持久化层，需要配合其他数据库 |
| `Couchbase` | 生产服务器 | 中等 | 企业级特定需求 | 可扩展性强，适应特定数据生态系统 | 需要额外的自定义集成 |

\*\*

以 PostgresSaver 的同步和异步实现为例，其配置和使用方式如下 31：

```Python
# PostgreSQL 检查点同步示例
from langgraph.checkpoint.postgres import PostgresSaver
DB_URI = "postgresql://postgres:postgres@localhost:5442/postgres?sslmode=disable"
with PostgresSaver.from_conn_string(DB_URI) as checkpointer:
    # 首次使用时需调用 setup() 来创建必要的表
    # checkpointer.setup()
    graph = builder.compile(checkpointer=checkpointer)
    config = {"configurable": {"thread_id": "1"}}
    # 调用并自动保存检查点
    graph.invoke({"messages": [{"role": "user", "content": "hi! I'm bob"}]}, config)**
```

## 5. 真实项目案例深度分析

### 5.1 案例一：字节跳动 DeerFlow — 多智能体协作与人机回路

`DeerFlow`是字节跳动开源的一个基于LangGraph的深度研究框架，其核心是一个由多个智能体（如`Planner`、`Researcher`、`Coder`、`Reporter`）组成的协作系统 19。该项目对状态持久化的需求是其架构的内在驱动力。一个深度研究任务可能需要跨越数小时，甚至在不同时间点进行。

- **多智能体状态共享**：在`DeerFlow`中，`Planner`智能体创建的研究计划、`Researcher`智能体获取的网页内容和`Coder`智能体执行的代码结果，都必须通过一个共享的全局状态来传递，以供后续的`Reporter`智能体使用。LangGraph的共享状态模型正是实现这种复杂信息流的核心机制。

- **人机协作**：`DeerFlow`支持`Human-in-the-loop`模式，允许用户对`Planner`生成的初始研究计划进行自然语言修改 24。要实现此功能，LangGraph必须在生成计划后暂停执行，并保存当前状态，直到接收到用户的反馈。这正是

  `checkpointer`机制的关键应用，它确保了工作流的可控性，使其能够优雅地等待外部事件。

`DeerFlow`的成功正是建立在LangGraph强大的状态管理和持久化能力之上，它将一个耗时、多步骤的复杂任务拆解成多个可控、可恢复的智能体协作环节，展示了LangGraph在复杂多智能体系统中的巨大价值。

### 5.2 案例二：Google Gemini 全栈快速入门 — 生产级架构实践

Google的`Gemini Fullstack LangGraph Quickstart`项目是一个展示如何将LangGraph应用于生产环境的经典蓝图 14。该项目集成了一个React前端、一个基于FastAPI的LangGraph后端，并使用PostgreSQL和Redis作为持久化层，完整地呈现了第3节中描绘的生产级双数据库架构。

- **PostgreSQL的角色**：在此架构中，PostgreSQL是主要的数据持久化层。它存储着所有核心实体，包括`assistants`、`threads`、`runs`和`long-term memory`。更重要的是，它以“精确一次”（exactly once）的语义来管理后台任务队列的状态，确保在长程任务执行过程中的数据完整性和可靠性 14。

- **Redis的角色**：Redis则被专门用作`pub-sub`（发布-订阅）消息代理，以实现**实时流式传输**。当LangGraph智能体执行搜索或反思等中间步骤时，它会通过Redis将这些中间状态变化（例如“正在搜索...”或“反思中...”）实时推送给前端。这种设计将后台计算逻辑与前端实时体验解耦，使得用户能够看到智能体的“思考”过程，显著改善了用户体验 14。

该项目通过一个可一键部署的`docker-compose`配置，为开发者提供了清晰的生产级部署范例 14。它证明了LangGraph不仅是一个概念框架，更是一个能够与FastAPI、PostgreSQL、Redis等经典后端基础设施无缝集成的、可扩展、可恢复的完整应用。

### 真实项目持久化方案对比

| 项目名称 | 核心应用场景 | LangGraph作用 | 持久化后端 | 流式传输机制 | 人机协作支持 |
| --- | --- | --- | --- | --- | --- |
| **字节跳动DeerFlow** | 深度研究自动化 | 协调多智能体，管理复杂工作流 | 未明确，但需持久化 | 实时中间结果流式传输 | 是（支持自然语言修改计划） |
| **Google Gemini Quickstart** | 增强型研究会话AI | 构建动态研究智能体 | PostgreSQL（状态）+ Redis（流式） | `astream()` + Redis `pub-sub` | 是（支持人机回路） |

## 6. 结论与未来展望

### 6.1 总结：LangGraph状态持久化机制的核心优势

通过对LangGraph核心机制和生产级案例的深入分析，可以得出以下结论：

1. **可控性与可靠性**：LangGraph的`checkpointer`与`thread`机制为智能体工作流提供了前所未有的精细控制。它使得工作流能够从任何一个超步恢复，从而实现了强大的容错能力，尤其是在处理长程任务时。

2. **内存与上下文**：内置的持久化层解决了智能体“记忆”的关键挑战，使得复杂的多轮对话和跨会话交互成为可能。这对于构建个性化、情境感知的智能系统至关重要。

3. **生态与集成**：LangGraph的开放设计使其能够无缝集成多种持久化后端，从本地的SQLite到生产级的PostgreSQL和Redis。此外，它与FastAPI等现代Web框架以及LangSmith等可观测性工具的紧密集成，使其成为构建可部署、可扩展、可维护的企业级智能体应用的理想选择 25。

LangGraph的持久化机制不仅仅是一项技术实现，它更是一种根本性的**认知架构**转变。它将智能体从传统的“一次性”API调用转变为一个可记忆、可协作、可信赖的智能系统，为AI应用从演示阶段迈向生产环境提供了坚实的基础。

### 6.2 展望：长期运行任务与可观测性的重要性

随着智能体能力的不断增强，其应用场景将从实时的聊天交互扩展到需要数小时甚至数天才能完成的**长期运行任务**。这些任务通常在后台执行，例如自动生成市场分析报告、进行学术文献综述或监控金融数据流。LangGraph的持久化和后台任务调度能力（例如通过`cron`）为此类应用的实现提供了优雅的解决方案。

然而，在生产环境中，状态持久化所带来的巨大历史记录也对**可观测性**提出了更高要求。这些保存的检查点和线程历史是调试和评估智能体行为的宝贵数据源。因此，将LangGraph与LangSmith或Langfuse等专业可观测性平台相结合变得尤为重要。通过追踪每一次调用、分析中间步骤的延迟、成本和质量，开发者能够持续优化智能体行为，确保其在复杂、动态的生产环境中稳定可靠 25。

\*\*

#### Works cited

1. LangGraph memory - Overview, accessed on August 17, 2025, [https://langchain-ai.github.io/langgraph/concepts/memory/](https://langchain-ai.github.io/langgraph/concepts/memory/)

2. Understanding Short-Term Memory in LangGraph: A Hands-On Guide | by Sajith K - Medium, accessed on August 17, 2025, [https://medium.com/@sajith_k/understanding-short-term-memory-in-langgraph-a-hands-on-guide-5536f39d0cb3](https://medium.com/@sajith_k/understanding-short-term-memory-in-langgraph-a-hands-on-guide-5536f39d0cb3)

3. LangGraph persistence - GitHub Pages, accessed on August 17, 2025, [https://langchain-ai.github.io/langgraph/concepts/persistence/](https://langchain-ai.github.io/langgraph/concepts/persistence/)

4. Orchestrating Long-Running Processes Using LangGraph Agents - Auxiliobits, accessed on August 17, 2025, [https://www.auxiliobits.com/blog/orchestrating-long-running-processes-using-langgraph-agents/](https://www.auxiliobits.com/blog/orchestrating-long-running-processes-using-langgraph-agents/)

5. langgraph-checkpoint - PyPI, accessed on August 17, 2025, [https://pypi.org/project/langgraph-checkpoint/](https://pypi.org/project/langgraph-checkpoint/)

6. Tutorial - Persist LangGraph State with Couchbase Checkpointer, accessed on August 17, 2025, [https://developer.couchbase.com/tutorial-langgraph-persistence-checkpoint/](https://developer.couchbase.com/tutorial-langgraph-persistence-checkpoint/)

7. LangGraph Platform - Docs by LangChain, accessed on August 17, 2025, [https://docs.langchain.com/docs](https://docs.langchain.com/docs)

8. Persistence, accessed on August 17, 2025, [https://langchain-ai.github.io/langgraphjs/concepts/persistence/](https://langchain-ai.github.io/langgraphjs/concepts/persistence/)

9. Streaming - ️ LangChain, accessed on August 17, 2025, [https://python.langchain.com/docs/concepts/streaming/](https://python.langchain.com/docs/concepts/streaming/)

10. What is AG-UI? Protocol Stack Addressing Critical Gaps in AI System Design | Mindflow Blog, accessed on August 17, 2025, [https://mindflow.io/blog/what-is-ag-ui](https://mindflow.io/blog/what-is-ag-ui)

11. AG-UI (Agent-User Interaction Protocol): An Open, Lightweight, Event-based Protocol that Standardizes How AI Agents Connect to Front-End Applications - MarkTechPost, accessed on August 17, 2025, [https://www.marktechpost.com/2025/05/12/ag-ui-agent-user-interaction-protocol-an-open-lightweight-event-based-protocol-that-standardizes-how-ai-agents-connect-to-front-end-applications/](https://www.marktechpost.com/2025/05/12/ag-ui-agent-user-interaction-protocol-an-open-lightweight-event-based-protocol-that-standardizes-how-ai-agents-connect-to-front-end-applications/)

12. Streaming LLM Responses with FastAPI - YouTube, accessed on August 17, 2025, [https://www.youtube.com/watch?v=xTTtqwGWemw](https://www.youtube.com/watch?v=xTTtqwGWemw)

13. Deploying LangGraph with FastAPI: A Step-by-Step Tutorial | by Sajith K - Medium, accessed on August 17, 2025, [https://medium.com/@sajith_k/deploying-langgraph-with-fastapi-a-step-by-step-tutorial-b5b7cdc91385](https://medium.com/@sajith_k/deploying-langgraph-with-fastapi-a-step-by-step-tutorial-b5b7cdc91385)

14. google-gemini/gemini-fullstack-langgraph-quickstart: Get ... - GitHub, accessed on August 17, 2025, [https://github.com/google-gemini/gemini-fullstack-langgraph-quickstart](https://github.com/google-gemini/gemini-fullstack-langgraph-quickstart)

15. LangGraph Server - Docs by LangChain, accessed on August 17, 2025, [https://docs.langchain.com/langgraph-platform/langgraph-server](https://docs.langchain.com/langgraph-platform/langgraph-server)

16. Unlocking the Power of Research-Augmented Conversational AI: A Deep Dive into the Gemini Fullstack LangGraph Quickstart - Onegen, accessed on August 17, 2025, [https://onegen.ai/project/unlocking-the-power-of-research-augmented-conversational-ai-a-deep-dive-into-the-gemini-fullstack-langgraph-quickstart/](https://onegen.ai/project/unlocking-the-power-of-research-augmented-conversational-ai-a-deep-dive-into-the-gemini-fullstack-langgraph-quickstart/)

17. Add memory - GitHub Pages, accessed on August 17, 2025, [https://langchain-ai.github.io/langgraph/how-tos/memory/add-memory/](https://langchain-ai.github.io/langgraph/how-tos/memory/add-memory/)

18. External Persistent Memory For Agents: Building Robust Applications with LangGraph | by Prince Krampah | Medium, accessed on August 17, 2025, [https://medium.com/@princekrampah/external-persistent-memory-for-agents-building-robust-applications-with-langgraph-8415b170beef](https://medium.com/@princekrampah/external-persistent-memory-for-agents-building-robust-applications-with-langgraph-8415b170beef)

19. ByteDance DeerFlow: Multi AI Agent framework for Deep Research | by Mehul Gupta | Data Science in Your Pocket | Medium, accessed on August 17, 2025, [https://medium.com/data-science-in-your-pocket/bytedance-deerflow-multi-ai-agent-framework-for-deep-research-acfbc4d90fbd](https://medium.com/data-science-in-your-pocket/bytedance-deerflow-multi-ai-agent-framework-for-deep-research-acfbc4d90fbd)

20. Streamlining Deep Research with DeerFlow | by Michela Tjan Sakti Effendie - Medium, accessed on August 17, 2025, [https://medium.com/operations-research-bit/streamlining-deep-research-with-deerflow-62e1da7530da](https://medium.com/operations-research-bit/streamlining-deep-research-with-deerflow-62e1da7530da)

21. DeerFlow, accessed on August 17, 2025, [https://deerflow.tech/](https://deerflow.tech/)

22. langchain-ai/langgraphjs-gen-ui-examples: A collection of ... - GitHub, accessed on August 17, 2025, [https://github.com/langchain-ai/langgraphjs-gen-ui-examples](https://github.com/langchain-ai/langgraphjs-gen-ui-examples)

23. DeerFlow: Your New Secret Weapon for Deep Research - DEV Community, accessed on August 17, 2025, [https://dev.to/githubopensource/deerflow-your-new-secret-weapon-for-deep-research-5d5c](https://dev.to/githubopensource/deerflow-your-new-secret-weapon-for-deep-research-5d5c)

24. bytedance/deer-flow: DeerFlow is a community-driven Deep Research framework, combining language models with tools like web search, crawling, and Python execution, while contributing back to the open-source community. - GitHub, accessed on August 17, 2025, [https://github.com/bytedance/deer-flow](https://github.com/bytedance/deer-flow)

25. Building a Multi-Agent AI Platform with LangGraph and LangSmith - Medium, accessed on August 17, 2025, [https://medium.com/@cthecm/building-a-multi-agent-ai-platform-with-langgraph-and-langsmith-6d3e03c14b11](https://medium.com/@cthecm/building-a-multi-agent-ai-platform-with-langgraph-and-langsmith-6d3e03c14b11)

26. LangSmith - LangChain, accessed on August 17, 2025, [https://www.langchain.com/langsmith](https://www.langchain.com/langsmith)

27. wassim249/fastapi-langgraph-agent-production-ready ... - GitHub, accessed on August 17, 2025, [https://github.com/wassim249/fastapi-langgraph-agent-production-ready-template](https://github.com/wassim249/fastapi-langgraph-agent-production-ready-template)

28. Observability Quick Start - ️🛠️ LangSmith - LangChain, accessed on August 17, 2025, [https://docs.smith.langchain.com/observability](https://docs.smith.langchain.com/observability)

29. Example - Trace and Evaluate LangGraph Agents - Langfuse, accessed on August 17, 2025, [https://langfuse.com/guides/cookbook/example_langgraph_agents](https://langfuse.com/guides/cookbook/example_langgraph_agents)

30. Trace with LangGraph (Python and JS/TS) | 🦜️🛠️ LangSmith, accessed on August 17, 2025, [https://docs.smith.langchain.com/observability/how_to_guides/trace_with_langgraph](https://docs.smith.langchain.com/observability/how_to_guides/trace_with_langgraph)\*\*
31. langgraph-checkpoint-postgres - PyPI, accessed on August 17, 2025, [https://pypi.org/project/langgraph-checkpoint-postgres/](https://pypi.org/project/langgraph-checkpoint-postgres/)

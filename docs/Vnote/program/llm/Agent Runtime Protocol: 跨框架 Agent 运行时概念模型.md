## 0. 为什么需要这份文档

LangGraph 的 `Checkpoint`、OpenAI 的 `Thread`、AutoGen 的 `ChatHistory`——这三个名字指向同一个底层问题：**如何持久化 Agent 的执行状态**。但如果你只学了 LangGraph，你会认为"Checkpoint"就是这个问题本身，而不是这个问题的一种解法。

这就是框架特定知识的脆弱性。框架会更迭，但它们解决的问题不会消失：

```
框架特定知识                         框架无关概念
┌──────────────────┐               ┌──────────────────┐
│ LangGraph        │               │                  │
│ · Checkpoint     │──┐            │  State Snapshot   │
│ · SuperStep      │  │  提取       │  + Persistence    │
│ · Channel        │  ├──────────► │  + Versioning     │
├──────────────────┤  │            │  + Recovery       │
│ OpenAI           │  │            │                  │
│ · Thread         │──┤            └──────────────────┘
│ · Run            │  │
├──────────────────┤  │            框架会死，
│ AutoGen          │  │            对问题的理解不会。
│ · Conversation   │──┘
└──────────────────┘
```

本文的目标是：从 5 个主流 Agent 框架中，提取 **8 个通用维度**，定义框架无关的概念体系。每个维度先给出通用定义，再用各实现作为证据，最后判断哪些设计决策是持久的、哪些是流行的。

**这不是一份规范**，而是一个个人认知框架——帮助我在框架迭代时快速定位"新框架解决了哪个老问题"，而不是每次都从零开始学。

---

## 1. Agent Runtime 概念模型总览

### 1.1 什么是 Agent Runtime

Agent Runtime 是 Agent 的执行环境，负责：接收输入 → 调用 LLM → 执行工具 → 管理状态 → 产出结果。

不同框架对 Runtime 的定义边界不同：LangGraph 包含了从编排到持久化的完整栈；MCP 只管工具调用协议；OpenAI Assistants 把整个 Runtime 藏在服务端。但它们都必须回答同一组问题。

### 1.2 八个通用维度

```
┌─────────────────────────────────────────────────────────────────┐
│                    Agent Runtime 概念模型                        │
│                                                                 │
│  ┌──────────────┐  ┌───────────────┐  ┌──────────────────────┐  │
│  │ 1. 执行模型   │  │ 2. 状态管理    │  │ 3. 工具协议          │  │
│  │  Execution   │  │  State        │  │  Tool Protocol       │  │
│  │  Model       │  │  Management   │  │  (可独立分层)         │  │
│  └──────┬───────┘  └──────┬────────┘  └──────────┬───────────┘  │
│         │                 │                      │              │
│  ┌──────▼───────┐  ┌──────▼────────┐  ┌──────────▼───────────┐  │
│  │ 4. 流式输出   │  │ 5. 中断与恢复  │  │ 6. 错误恢复          │  │
│  │  Streaming   │  │  Interrupt &  │  │  Error Recovery      │  │
│  │              │  │  Resume       │  │                      │  │
│  └──────┬───────┘  └──────┬────────┘  └──────────┬───────────┘  │
│         │                 │                      │              │
│  ┌──────▼─────────────────▼──────────────────────▼───────────┐  │
│  │ 7. 多 Agent 协作                                           │  │
│  │  Multi-Agent Coordination                                 │  │
│  └──────────────────────────┬────────────────────────────────┘  │
│                             │                                   │
│  ┌──────────────────────────▼────────────────────────────────┐  │
│  │ 8. 可观测性 (横切关注点)                                     │  │
│  │  Observability                                            │  │
│  └───────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
```

**分层关系**：

- **第一层**（1-3）：基础能力——执行、状态、工具是构成 Agent Runtime 的三根支柱
- **第二层**（4-6）：运行时特性——流式输出、中断恢复、错误处理决定了 Runtime 的生产可用性
- **第三层**（7）：协作能力——多 Agent 是在单 Agent Runtime 之上的扩展
- **横切层**（8）：可观测性贯穿所有层

**关键分层洞察**：工具协议（维度 3）可以完全独立于执行模型（维度 1）。MCP 证明了这一点——它定义了工具的发现和调用协议，但不关心是谁在编排 Agent。这是整个 Agent 生态中最重要的架构分离。

### 1.3 框架覆盖

| 框架           | 全称                                        | 核心定位                  | 版本基准   |
| -------------- | ------------------------------------------- | ------------------------- | ---------- |
| **LangGraph**  | LangGraph + LangGraph Platform              | 图执行引擎 + Agent Server | 0.3.x      |
| **OpenAI**     | Assistants API + Responses API + Agents SDK | 托管式 Agent Runtime      | 2025.04    |
| **AutoGen**    | AutoGen 0.4（Core + AgentChat）             | 多 Agent 对话框架         | 0.4.x      |
| **Claude SDK** | Claude Agent SDK（Anthropic）               | 代码执行 Agent            | 0.1.x      |
| **MCP**        | Model Context Protocol                      | 工具互操作协议            | 2025-03-26 |

> 注意：OpenAI 有三个不同的 Agent 表面（Assistants API、Responses API、Agents SDK），它们的设计哲学差异很大，在后续对比中会分别标注。

---

## 2. 执行模型 (Execution Model)

### 2.1 通用概念

执行模型定义了 Agent 计算如何被编排：什么是执行的基本单元、单元之间如何调度、控制流由谁决定。

**子概念**：

- **执行单元 (Execution Unit)**：一次不可分割的计算步骤——一个 LLM 调用、一次工具执行、一个决策节点
- **调度模型 (Scheduling)**：执行单元的排列方式——顺序、并行、条件分支
- **控制流 (Control Flow)**：谁决定下一步做什么——显式的图边、LLM 的推理、代码逻辑

### 2.2 四种执行范式

当前 Agent 框架的执行模型可以归纳为四种范式：

```
         显式程度高                        显式程度低
         (开发者定义控制流)                 (LLM/框架决定控制流)
         ◄──────────────────────────────────────────────►

图 (Graph)           代码 (Code)          对话 (Conversation)      托管 (Managed)
LangGraph            Agents SDK           AutoGen                  Assistants API
                     Claude SDK

开发者画图定义         开发者写代码循环        多 Agent 对话决定         平台黑盒执行
节点+边               if/else/while         谁说话=谁执行            你只看结果
```

### 2.3 跨框架映射

| 概念 | LangGraph | OpenAI Assistants | Agents SDK | AutoGen | Claude SDK |
| --- | --- | --- | --- | --- | --- |
| **执行单元** | Node（函数/Runnable） | Run Step | Agent turn | Agent message handler | Agent turn |
| **调度模型** | Graph（DAG + 循环） | 服务端托管循环 | Python 控制流 | 对话协议（轮转/选择） | 代码驱动循环 |
| **控制流** | 条件边 / `Command` | 服务端决定（不透明） | Handoff / 代码分支 | Selector / RoundRobin | 工具结果驱动 LLM |
| **并行执行** | `Send` API（fan-out/fan-in） | 不支持 | 不支持 | GroupChat 内并行 | 不支持 |
| **执行容器** | Thread + Run | Thread + Run | Runner 上下文 | Runtime + Team | Session |

### 2.4 设计决策分析

**Graph vs Code vs Conversation vs Managed**——每种选择决定了什么容易、什么困难：

| 范式             | 容易做到                     | 困难做到                   | 典型场景                     |
| ---------------- | ---------------------------- | -------------------------- | ---------------------------- |
| **Graph**        | 分支、并行、可视化、断点调试 | 简单的线性对话（过度建模） | 复杂工作流、审批流、研报生成 |
| **Code**         | 灵活、学习曲线低、调试直观   | 持久化、断线恢复、可视化   | 简单 Agent、脚本任务         |
| **Conversation** | 多 Agent 自然协作            | 单 Agent 精确控制、调试    | 团队模拟、头脑风暴           |
| **Managed**      | 零运维、开箱即用             | 自定义执行逻辑、成本控制   | 快速原型、客服 Bot           |

### 2.5 关键洞察

执行模型是**最不可能收敛**的维度。不同场景天然适合不同范式——复杂工作流需要 Graph，简单任务用 Code 就够，多角色协作适合 Conversation。

趋势是**混合**：LangGraph 0.3 新增了 `@entrypoint` / `@task` 函数式 API，本质上是在 Graph 框架里引入 Code 范式；OpenAI Agents SDK 用 Handoff 在 Code 范式里模拟了 Conversation。

**作为开发者应该关注的**：不要把执行模型和其他维度绑定。状态管理、工具调用、流式输出应该独立于执行模型——这样切换执行范式时，其他能力可以复用。

---

## 3. 状态管理 (State Management)

### 3.1 通用概念

状态管理定义了 Agent 执行过程中的可变数据如何表示、持久化、版本化和恢复。

**子概念**：

- **状态表示 (State Schema)**：数据的形状——类型化的结构（TypedDict）、消息列表、JSON blob
- **状态持久化 (Persistence)**：数据存到哪——内存、数据库、服务端托管
- **状态版本化 (Versioning)**：能否查看/回滚历史——快照链、消息追加、无版本
- **状态作用域 (Scope)**：数据对谁可见——全局、Agent 级、Channel 级
- **增量更新 (Update Mechanism)**：如何修改状态——Reducer 函数、直接覆盖、追加消息

### 3.2 持久化光谱

各框架在状态持久化上的立场差异巨大，形成了一个光谱：

```
无持久化                                                            完全持久化
◄──────────────────────────────────────────────────────────────────────►

Claude SDK      Agents SDK       AutoGen          LangGraph          Assistants API
                                 (Memory)         (Checkpoint)       (Server-managed)
│                │                │                │                  │
你管理全部        你管理全部        框架管历史        框架管状态快照       平台管一切
(进程死=丢失)     (进程死=丢失)     (手动save/load)   (自动per-step)      (不透明)
```

### 3.3 跨框架映射

| 概念 | LangGraph | OpenAI Assistants | Agents SDK | AutoGen | Claude SDK |
| --- | --- | --- | --- | --- | --- |
| **状态表示** | `TypedDict` + Channel 级 Reducer | Thread（消息列表 + 元数据） | `RunContext`（Python 对象） | `ChatCompletionContext` + 共享状态 | 对话历史（隐式） |
| **持久化** | Checkpointer（PG/Redis/SQLite） | 服务端托管（不透明） | 无（手动 `save_state`/`load_state`） | 无内置 | 无内置 |
| **版本控制** | Checkpoint 链（`parent_id`，类 Git） | Thread 消息历史（追加制） | 无 | 无 | 无 |
| **状态作用域** | Channel 级（每个字段独立 Reducer） | Thread 级 | Agent 级 | Agent/Team 级 | Session 级 |
| **增量更新** | `Annotated[list, add_messages]` 等 Reducer | 追加消息 | 直接修改 | 追加消息 | 直接修改 |

### 3.4 设计决策分析

**LangGraph 的 Checkpoint 模型**是目前最完整的状态管理方案：

- 每个节点执行后自动快照（不需要手动调用 save）
- 快照通过 `parent_id` 形成链式结构，支持时间旅行
- Content-addressed blob 存储，大状态只存一次
- 通过 `graph.update_state()` 支持手动修正状态后重新执行

代价是：学习曲线陡峭，Reducer 函数的语义需要理解，Checkpoint 存储占空间。

**OpenAI Assistants 的 Thread 模型**走了另一个极端：

- 你不需要（也无法）管理状态——服务端全包
- 状态只能通过追加消息来修改，不能直接改内部状态
- 没有回滚——你只能创建新 Thread

代价是：零控制权。调试困难，无法做"如果当时走了另一条路"的分析。

**AutoGen / Claude SDK / Agents SDK** 基本没有内置持久化，把这个问题留给开发者。对于短生命周期的 Agent 这没问题，但一旦需要跨请求保持状态（如人机协作工作流），就必须自己搭建。

### 3.5 关键洞察

**状态持久化是区分"玩具"和"生产"的分水岭**。没有持久化的 Agent：

- 无法在进程崩溃后恢复
- 无法支持真正的 Human-in-the-Loop（用户可能几小时后才回复）
- 无法调试"为什么 Agent 走了这条路"
- 无法做 A/B 测试（从同一个状态分叉执行不同策略）

行业趋势：无持久化的框架正在补课（Agents SDK 加了 Tracing，AutoGen 加了 Memory），有持久化的框架正在降低门槛（LangGraph 的函数式 API 隐藏了 Checkpoint 复杂度）。

**收敛方向**：某种形式的"自动快照 + 可回溯"会成为标配，但具体的存储后端和快照粒度会因场景而异。

---

## 4. 工具协议 (Tool Protocol)

### 4.1 通用概念

工具协议定义了 Agent 如何发现、调用和处理外部能力。

**子概念**：

- **工具定义 (Tool Definition)**：描述工具的名称、参数、返回值——通常用 JSON Schema
- **工具调用 (Tool Invocation)**：调用的请求/响应格式和传输方式
- **工具结果 (Tool Result)**：返回给 Agent 的数据格式
- **工具发现 (Tool Discovery)**：Agent 如何知道有哪些工具可用
- **错误处理 (Error Handling)**：工具调用失败时的行为

### 4.2 跨框架映射

| 概念 | LangGraph | OpenAI | AutoGen | Claude SDK | MCP |
| --- | --- | --- | --- | --- | --- |
| **定义格式** | `@tool` + JSON Schema | Function Calling JSON Schema | `FunctionTool` + JSON Schema | `Tool`（JSON Schema） | JSON Schema + MCP 规范 |
| **调用约定** | `ToolNode` 自动执行 | `requires_action` → 客户端执行 | Agent 内部直接调用 | Agent 内部直接调用 | JSON-RPC 2.0 |
| **结果格式** | `ToolMessage` | Function output（字符串） | `FunctionExecutionResult` | `ToolResult` | JSON-RPC Response |
| **发现机制** | 构建时 `bind_tools()` | 创建 Assistant/Response 时指定 | 创建 Agent 时注册 | 创建时 `allowed_tools` | `tools/list` 动态发现 |
| **错误处理** | 可配置：`handle_tool_errors=True` | 错误作为 output 返回 LLM | 异常转为错误消息 | 错误在结果中 | `isError: true` 在结果中 |

### 4.3 MCP：工具协议独立分层的证明

MCP 的核心贡献不是技术上的——JSON-RPC + JSON Schema 并不新。它的贡献是**架构上的**：证明了工具协议可以完全独立于执行模型。

在 MCP 之前：

- 用 LangGraph？工具必须是 LangChain `Tool`
- 用 OpenAI？工具必须是 Function Calling 格式
- 切换框架 = 重写所有工具

在 MCP 之后：

- 工具是一个独立的 MCP Server
- 任何 MCP Client（LangGraph、OpenAI、Claude）都能调用
- 切换执行框架不影响工具层

```
切换前：紧耦合                          切换后：松耦合

┌──────────────┐                      ┌──────────────┐
│ LangGraph    │                      │  任何框架      │
│ ┌──────────┐ │                      │              │
│ │LangChain │ │                      └──────┬───────┘
│ │  Tool    │ │                             │ MCP Client
│ └──────────┘ │                             │
└──────────────┘                      ┌──────▼───────┐
                                      │  MCP Server  │
切框架 = 重写 Tool                      │  (工具实现)   │
                                      └──────────────┘

                                      切框架 ≠ 重写 Tool
```

### 4.4 Error-as-Data vs Error-as-Exception

工具调用失败时，有两种根本不同的处理策略：

| 策略 | 行为 | 代表 |
| --- | --- | --- |
| **Error-as-Data** | 错误信息作为工具结果返回给 LLM，由 LLM 决定如何处理 | MCP (`isError: true`)、OpenAI（错误在 output 中）、LangGraph (`handle_tool_errors=True`) |
| **Error-as-Exception** | 错误作为异常抛出，执行中断，由框架/开发者处理 | 传统编程模式 |

**Error-as-Data 是更好的默认策略**。原因：

- LLM 能看到错误信息，可以自主决定重试、换工具、或告知用户
- 不需要开发者为每种错误写 try/catch
- 更接近人类使用工具的方式——工具出错了，你会看看错误信息然后决定下一步

MCP 把这个设计选择提升为协议级约定：`CallToolResult.isError = true` 表示结果是错误信息，但它仍然是一个合法的 Result，不是 Exception。

### 4.5 关键洞察

工具协议是 **8 个维度中收敛程度最高的**。JSON Schema 定义工具、JSON 格式传递参数和结果——这已经是事实标准。分歧主要在发现机制（静态注册 vs MCP 动态发现）和错误处理策略上。

**MCP 很可能成为工具层的唯一标准**，就像 HTTP 成为 Web 通信的唯一标准一样。OpenAI Responses API 已经原生支持 `type: "mcp"` 的工具。

---

## 5. 流式输出 (Streaming)

### 5.1 通用概念

流式输出定义了 Agent 执行的增量结果如何传递给消费者。

**子概念**：

- **传输协议 (Transport)**：SSE、WebSocket、异步生成器、轮询
- **粒度控制 (Granularity)**：Token 级、节点/步骤级、消息级
- **可恢复性 (Resumability)**：断连后能否从断点继续接收
- **多通道 (Multi-channel)**：能否同时传递不同类型的事件

### 5.2 跨框架映射

| 概念 | LangGraph Platform | OpenAI Assistants | Agents SDK | AutoGen | Claude SDK |
| --- | --- | --- | --- | --- | --- |
| **传输** | SSE | SSE / 轮询 | Python AsyncGen | Python AsyncGen | Python AsyncGen |
| **粒度** | 9 种 StreamMode 可组合 | 固定事件类型 | StreamEvent | 消息级 | 事件级 |
| **可恢复** | **支持**（Last-Event-ID + Redis Stream） | 不支持 | 不支持 | 不支持 | 不支持 |
| **自定义事件** | `get_stream_writer()` | 不支持 | 不支持 | 不支持 | 不支持 |
| **子图/子 Agent** | `stream_subgraphs=True` | N/A | 不支持 | Topic 订阅 | N/A |

### 5.3 Server vs Library：流式能力的分水岭

流式输出的复杂程度与 Runtime 的部署形态直接相关：

| 形态                  | 传输                  | 可恢复                   | 典型                                  |
| --------------------- | --------------------- | ------------------------ | ------------------------------------- |
| **Library（进程内）** | Python AsyncGenerator | 不需要（进程内不会断连） | Agents SDK、Claude SDK、AutoGen       |
| **Server（跨网络）**  | SSE / WebSocket       | **必须考虑**（网络会断） | LangGraph Platform、OpenAI Assistants |

LangGraph Platform 的可恢复流是目前唯一完整的实现（详见 [sse_resumable_stream_design.md](../tech/202605/sse_resumable_stream_design.md)）：

- Producer：将事件持久化到 Redis Stream（`XADD`）
- Consumer：先 Catch-up 回放历史事件（`XREAD`），再 Live Tail 实时事件
- 客户端通过 `Last-Event-ID` 标识断点位置
- 服务端配置 `stream_resumable: true` + `on_disconnect: "continue"`

### 5.4 关键洞察

**流式能力与部署形态高度相关**。如果你的 Agent 只在进程内跑（Python 脚本、Jupyter Notebook），AsyncGenerator 就够了。一旦走向生产（HTTP Server、多实例部署），SSE + 可恢复就是刚需。

**SSE 会是流式传输的标准协议**。WebSocket 双向能力对 Agent 场景来说过重（Agent 输出是单向的），轮询延迟太高。SSE 基于 HTTP、浏览器原生支持、自带 `Last-Event-ID` 重连机制——是最适合 Agent 场景的传输协议。

---

## 6. 中断与恢复 (Interrupt & Resume)

### 6.1 通用概念

中断与恢复定义了 Agent 执行如何暂停（通常等待人类输入）以及如何从暂停点继续。这是 Human-in-the-Loop 的基础设施。

**子概念**：

- **中断触发 (Interrupt Trigger)**：什么条件下暂停——到达特定节点、需要工具审批、主动请求人类输入
- **中断状态 (Interrupt State)**：暂停时保存了什么——完整状态快照、对话历史、什么都没保存
- **中断载荷 (Interrupt Payload)**：暴露给人类的信息——"Agent 想调用这个工具，你同意吗？"
- **恢复机制 (Resume Mechanism)**：人类如何提供输入并让 Agent 继续——提交数据、选择选项、直接回复

### 6.2 跨框架映射

| 概念 | LangGraph | OpenAI Assistants | Agents SDK | AutoGen | Claude SDK |
| --- | --- | --- | --- | --- | --- |
| **中断触发** | `interrupt()` / `interrupt_before` / `interrupt_after` | `requires_action`（仅工具审批） | Guardrail 拦截 | `HandoffTermination` | `client.interrupt()` |
| **中断状态** | Checkpoint（完整快照 + pending_writes） | 服务端 Thread（不透明） | 无持久化 | 对话历史（手动 save） | 无持久化 |
| **中断载荷** | 任意 JSON（`interrupt(payload)`） | `tool_calls` 列表 | Guardrail 错误信息 | `HandoffMessage` | 无 |
| **恢复机制** | `Command(resume=value)` | `submit_tool_outputs()` | 代码手动恢复 | 重新 `run_stream(task=input)` | 新 `query()` |
| **多点中断** | 支持（多节点设置 `interrupt_before`） | 不支持 | 不支持 | 不支持 | 不支持 |

### 6.3 中断/恢复通用流程

不管框架如何实现，中断/恢复的通用流程是一样的：

```
Agent 执行 ──► 到达中断点 ──► 保存执行状态 ──► 向前端暴露中断载荷
                                                     │
                                                     ▼
                                               人类查看/决策
                                                     │
                                                     ▼
Agent 恢复 ◄── 从快照加载状态 ◄── 接收人类输入 ◄── 前端提交
```

**关键约束**：真正的中断/恢复**需要状态持久化**。如果框架没有持久化能力（Claude SDK、Agents SDK），就只能做同步的"ask and wait"——进程不能退出，用户必须立即回复。

LangGraph 的方案是最完整的：

```python
# 节点内主动中断，传递任意载荷
def review_node(state):
    decision = interrupt({
        "question": "要发布这篇文章吗？",
        "draft": state["draft"],
        "options": ["发布", "修改", "丢弃"]
    })
    # decision 是人类通过 Command(resume=...) 传入的值
    if decision == "发布":
        return {"status": "published"}
```

```python
# 人类回复
graph.invoke(Command(resume="发布"), config)
```

### 6.4 设计决策分析

| 方案 | 优势 | 劣势 |
| --- | --- | --- |
| **LangGraph：通用 interrupt + Command** | 任意节点、任意载荷、完整状态保存 | 需要 Checkpointer，学习成本高 |
| **OpenAI：requires_action** | 简单，服务端托管状态 | 只能审批工具调用，不能主动问用户问题 |
| **AutoGen：HandoffTermination** | 用 Handoff 统一了人机和 Agent 间交互 | 状态需手动保存，恢复不是从断点继续 |
| **Claude SDK：interrupt()** | 极简——发信号停止 | 没有恢复，只能重新开始 |

### 6.5 关键洞察

**中断/恢复是 8 个维度中实现差距最大的**。LangGraph 的方案远领先于其他框架，因为它把 Checkpoint（状态持久化）和 Interrupt（执行暂停）深度整合了。其他框架要么只支持有限场景（OpenAI 的工具审批），要么根本不支持真正的恢复。

**这个差距的根源是状态管理**。没有自动的、per-step 的状态持久化，就不可能实现"几小时后恢复到精确断点"这种场景。中断/恢复能力本质上是状态管理能力的衍生品。

---

## 7. 错误恢复 (Error Recovery)

### 7.1 通用概念

错误恢复定义了 Agent 执行过程中发生故障时，Runtime 如何检测、表示和处理错误。

**子概念**：

- **错误检测 (Detection)**：在哪一层发现错误——工具执行、LLM 调用、状态更新
- **错误表示 (Representation)**：错误以什么形式存在——Exception、错误数据、状态标记
- **恢复策略 (Recovery Strategy)**：如何处理错误——重试、回滚、跳过、交给 LLM
- **部分进度保留 (Partial Progress)**：失败时已完成的步骤是否保留

### 7.2 跨框架映射

| 概念 | LangGraph | OpenAI Assistants | Agents SDK | AutoGen | MCP |
| --- | --- | --- | --- | --- | --- |
| **错误表示** | Exception → pending_writes | Run status = `failed` | Python Exception | Exception in message | `isError: true`（数据） |
| **重试** | `RetryPolicy`（per-node 配置） | 自动（不透明） | 手动 | 手动 | 客户端决定 |
| **回滚** | **Checkpoint 回滚** | N/A（服务端托管） | N/A | N/A | N/A |
| **部分进度** | **Checkpoint 保留** | Thread 消息保留 | 丢失 | 对话保留 | N/A |
| **错误传播** | 可配置（error-as-data 或 raise） | 事件通知 | 抛给调用者 | 消息传给 GroupChat | 返回给 LLM |

### 7.3 两种错误哲学

```
Error-as-Exception (传统)                Error-as-Data (Agent 原生)

工具调用 ──► 失败 ──► 抛异常             工具调用 ──► 失败 ──► 返回错误信息
                      │                                       │
                      ▼                                       ▼
              框架/开发者 try/catch                      LLM 看到错误信息
              决定重试/放弃                              LLM 自主决定下一步
                                                       (重试/换工具/告知用户)
```

Error-as-Data 的核心假设是：**LLM 有足够的推理能力来处理工具错误**。这个假设在 GPT-4 / Claude 3.5 级别的模型上是成立的——它们能理解"API 返回 429 限频"并决定等待后重试。

### 7.4 LangGraph 的 Checkpoint 回滚

LangGraph 是唯一支持 **Checkpoint 回滚**的框架：

- 节点 A 执行成功 → 自动保存 Checkpoint A
- 节点 B 执行失败 → 异常被记录到 `pending_writes`
- 重新 invoke 时 → 从 Checkpoint A 恢复，只重试节点 B
- 已完成的节点 A **不会重新执行**

这对长时间运行的工作流至关重要。一个 10 步的 Agent 在第 8 步失败了，你不需要重跑前 7 步。

### 7.5 关键洞察

**Error-as-Data 应该是 Agent Runtime 的默认策略**。Agent 的核心价值就是自主决策，把错误处理也交给 Agent 是自然延伸。只有 Agent 确实无法处理的系统级错误（内存不足、网络不通）才应该作为 Exception 向上抛。

**Checkpoint 回滚是生产环境的必需品**，但目前只有 LangGraph 提供。这是一个明确的行业缺口。

---

## 8. 多 Agent 协作 (Multi-Agent Coordination)

### 8.1 通用概念

多 Agent 协作定义了多个 Agent 如何共同完成一个任务。

**子概念**：

- **通信模式 (Communication)**：Agent 间如何传递信息——直接发送、发布/订阅、共享状态
- **委派模型 (Delegation)**：任务如何分配——Handoff 接力、层级分工、投票决策
- **状态共享 (State Sharing)**：Agent 间能否看到彼此的状态——共享 / 隔离
- **拓扑结构 (Topology)**：Agent 的组织形式——线性、星型、网状、层级

### 8.2 四种多 Agent 模式

```
模式 1: 子图嵌套 (LangGraph)               模式 2: Handoff 接力 (OpenAI Agents SDK)

┌────────────────────┐                    Agent A ──handoff──► Agent B ──handoff──► Agent C
│    Parent Graph    │
│   ┌────────────┐   │                    每个 Agent 决定是否交给下一个
│   │  SubGraph  │   │                    状态通过 context 传递
│   │  (Agent B) │   │
│   └────────────┘   │
└────────────────────┘


模式 3: 群聊选择 (AutoGen)                 模式 4: 发布-订阅 (AutoGen Core)

     ┌──► Agent A ──┐                    Agent A ──publish──► Topic
     │              │                                          │
Selector ◄──────────┤                                    ┌─────┴──────┐
     │              │                              subscribe    subscribe
     └──► Agent B ──┘                                    │           │
                                                    Agent B     Agent C
Selector（LLM/规则）决定谁发言
```

### 8.3 跨框架映射

| 概念 | LangGraph | Agents SDK | AutoGen | Claude SDK | MCP |
| --- | --- | --- | --- | --- | --- |
| **通信模式** | Sub-graph / Send API | Handoff（`transfer_to_agent`） | GroupChat + pub/sub | N/A | N/A |
| **委派模型** | 嵌套图 / 条件路由 | Handoff tool | Selector / RoundRobin / Swarm | N/A | N/A |
| **状态共享** | Channel 级（可配置映射） | 共享上下文 | 共享对话 | N/A | N/A |
| **拓扑** | 任意（图可表达任何拓扑） | 线性 Handoff 链 | 星型（Selector）/ 顺序 | N/A | N/A |
| **并行执行** | `Send` API（map-reduce） | 不支持 | 支持 | 不支持 | N/A |
| **分布式** | LangGraph Platform 管理 | 不支持 | `GrpcWorkerAgentRuntime` | 不支持 | N/A |

### 8.4 设计决策分析

| 模式             | 优势                           | 劣势                     | 适用场景                   |
| ---------------- | ------------------------------ | ------------------------ | -------------------------- |
| **子图嵌套**     | 类型安全、状态隔离可控、可复用 | 需要提前定义图结构       | 固定工作流中的子任务分工   |
| **Handoff 接力** | 简单直观，LLM 决定何时交棒     | 无并行，线性执行         | 客服转接、分工明确的流水线 |
| **群聊选择**     | 最灵活，适合开放式协作         | 难以调试，选择器可能震荡 | 头脑风暴、多角色讨论       |
| **发布-订阅**    | 解耦、可扩展、支持分布式       | 复杂度高，调试困难       | 大规模 Agent 集群          |

### 8.5 关键洞察

**多 Agent 是 8 个维度中碎片化程度最高的**。没有任何两个框架的多 Agent 模型是兼容的。这直接反映了执行模型的差异——图框架自然用子图，对话框架自然用群聊，代码框架自然用 Handoff。

**目前没有跨框架的 Agent 间通信标准**。Google 提出的 A2A（Agent-to-Agent）协议是一个早期尝试，但尚未被广泛采纳。如果 A2A 或类似协议成功，它将对多 Agent 领域产生类似 MCP 对工具领域的影响。

**对于大多数场景，Handoff 就够了**。只有当你需要并行执行、动态团队组建、或跨服务 Agent 通信时，才需要更复杂的模式。

---

## 9. 可观测性 (Observability)

### 9.1 通用概念

可观测性定义了 Agent 执行过程如何被追踪、记录和调试。

**子概念**：

- **Tracing**：分布式追踪——每个步骤的输入/输出、耗时、因果关系
- **Logging**：事件日志——Agent 运行过程中的关键事件记录
- **Metrics**：量化指标——延迟、Token 消耗、成本、成功率
- **Debugging**：调试能力——步进执行、状态回放、条件断点

### 9.2 跨框架映射

| 概念 | LangGraph | OpenAI | AutoGen | Claude SDK | MCP |
| --- | --- | --- | --- | --- | --- |
| **Tracing** | LangSmith（付费产品） | 内置 Traces（不透明） | 无内置 | 无内置 | N/A |
| **事件日志** | `stream_mode="events"/"debug"` | Run Steps API | `Console` + stdlib logging | Event stream | `notifications/message` |
| **Token 统计** | Callback 回调 | `Usage` 对象 | Usage tracking | Response 中的 usage | N/A |
| **执行回放** | **Checkpoint history** | Thread 消息历史 | 不支持 | 不支持 | N/A |
| **成本追踪** | LangSmith | OpenAI Dashboard | 手动 | 手动 | N/A |

### 9.3 设计决策分析

可观测性是**所有框架中最薄弱的维度**。具体问题：

1. **没有标准的 Trace 格式**：LangSmith 有自己的 Trace 格式，OpenAI 有自己的 RunStep 格式，两者不互通。Agent 执行的 Trace 需要一个类似 OpenTelemetry 的开放标准。

2. **Tracing 和框架绑定太深**：LangSmith 只能追踪 LangChain/LangGraph 的执行。如果你的工作流混用了多个框架，没有统一视图。

3. **调试能力严重不足**：只有 LangGraph 的 Checkpoint History 能做真正的"时间旅行调试"（回到任意一步查看当时的状态）。其他框架只能看日志。

### 9.4 关键洞察

**Agent 可观测性需要一个 OpenTelemetry-like 的开放标准**。这个标准应该定义：

- **Span**：一个 Agent 步骤（LLM 调用、工具执行、状态更新）
- **Trace**：一次完整的 Agent 执行（由多个 Span 组成）
- **Event**：Span 内的关键事件（Token 生成、中断触发、错误发生）
- **State Snapshot**：关键节点的状态快照

目前 LangFuse（LangSmith 的开源替代）和 OpenLIT 等项目正在尝试，但还没有形成行业标准。

---

## 10. 跨维度分析：设计决策的持久性判断

### 10.1 哪些设计决策会持久？

| 设计决策                              | 持久性   | 理由                                       |
| ------------------------------------- | -------- | ------------------------------------------ |
| 工具协议与执行协议分层                | **持久** | MCP 已证明可行，行业正在收敛               |
| 状态持久化是核心能力                  | **持久** | 没有持久化的 Agent 无法用于生产            |
| Error-as-Data 优于 Error-as-Exception | **持久** | Agent 需要自主推理错误，不是抛异常然后崩溃 |
| SSE 是流式标准                        | **持久** | HTTP 原生、浏览器原生、自带重连机制        |
| Server-managed Runtime 是生产必需     | **持久** | 但形态未定——可以是 Platform 也可以是自建   |
| JSON Schema 描述工具参数              | **持久** | 已是事实标准，没有竞争者                   |

### 10.2 哪些设计决策是流行（会被替代）？

| 设计决策                | 持久性       | 理由                                                |
| ----------------------- | ------------ | --------------------------------------------------- |
| Graph 是最优执行模型    | **流行**     | Code-driven（Agents SDK）正在反击；混合模式正在出现 |
| Handoff 是多 Agent 标准 | **流行**     | 太简单，无法处理并行和复杂拓扑                      |
| Checkpoint 链式版本控制 | **可能持久** | 概念有价值（类 Git），但实现可能简化                |
| 多 Agent 通过对话协作   | **流行**     | 对话是一种低效的 Agent 间通信方式                   |

### 10.3 行业收敛趋势

**正在收敛的**：

- 工具定义格式（JSON Schema）
- 工具调用协议（MCP）
- 流式传输协议（SSE）
- 错误处理哲学（Error-as-Data）

**没有收敛的**：

- 执行模型（Graph / Code / Conversation 并存）
- 状态管理（Checkpoint / Thread / 手动管理并存）
- 多 Agent 协作（各框架完全不兼容）
- 可观测性标准（各自为战）

**预测**：2 年内，工具层会统一到 MCP，流式层会统一到 SSE + 可恢复流，但执行模型和多 Agent 协作**不会统一**——因为它们解决的问题空间太大，不存在一个"最优解"。

### 10.4 作为开发者，投资什么？

| 方向               | 投资建议                    | 理由                                       |
| ------------------ | --------------------------- | ------------------------------------------ |
| MCP 工具开发       | **重点投入**                | 唯一收敛的标准，技能跨框架可迁移           |
| 状态管理抽象       | **重点投入**                | 无论哪个框架，状态持久化的设计模式是通用的 |
| Error-as-Data 模式 | **理解并应用**              | 改变你写 Agent 的方式，不依赖框架          |
| 特定框架的执行模型 | **谨慎投入**                | 深入 1-2 个即可，重点理解 why，不是记 API  |
| 多 Agent 模式      | **观望**                    | 太碎片化，等标准出现再深入不迟             |
| 可观测性           | **关注 OpenTelemetry 方向** | Agent Tracing 标准化是迟早的事             |

---

## 11. 个人综合：如果我从零设计 Agent Runtime

### 11.1 分层架构

```
┌─────────────────────────────────────────────────┐
│              Application Layer                   │
│     (业务逻辑、Prompt、工作流定义)                  │
├─────────────────────────────────────────────────┤
│              Agent Runtime Layer                 │
│     (执行模型 + 状态管理 + 流式输出)                │
├───────────────────┬─────────────────────────────┤
│    Tool Layer     │     Observability Layer      │
│    (MCP)          │     (OpenTelemetry-like)     │
├───────────────────┴─────────────────────────────┤
│              LLM Provider Layer                  │
│     (OpenAI / Anthropic / 开源模型)               │
└─────────────────────────────────────────────────┘
```

### 11.2 每个维度的设计选择

| 维度 | 我的选择 | 理由 |
| --- | --- | --- |
| **执行模型** | 混合：Graph 做复杂工作流，Code 做简单任务 | LangGraph 的 `@entrypoint`/`@task` 方向正确——Graph 能力为底，Code 表面为简 |
| **状态管理** | Checkpoint-based，自动 per-step 快照 | 这是中断恢复、错误回滚、调试回放的前提 |
| **工具协议** | MCP 作为唯一工具协议 | 不再写框架特定的 Tool wrapper |
| **流式输出** | SSE + 可恢复流（Redis Stream） | 生产环境断线恢复是刚需 |
| **中断/恢复** | 通用 `interrupt(payload)` + `resume(value)` | 参考 LangGraph 但简化 `Command` 的复杂度 |
| **错误恢复** | Error-as-Data 默认，系统错误才 raise | 让 LLM 自主处理工具错误 |
| **多 Agent** | Sub-graph 做紧耦合，MCP 做松耦合 | 避免发明新的 Agent 间通信协议 |
| **可观测性** | OpenTelemetry Span 作为一等原语 | 每个节点执行自动生成 Span，不依赖特定平台 |

### 11.3 核心设计原则

1. **工具层必须独立**——切换执行引擎不应该影响工具实现
2. **状态持久化是默认行为**——opt-out，不是 opt-in
3. **错误是数据，不是异常**——除非是系统级故障
4. **流式是一等公民**——不是事后追加的功能
5. **可观测性内建**——不是依赖外部平台的插件

---

## 附录 A：术语对照表

| 通用概念 | 英文 | LangGraph | OpenAI Assistants | Agents SDK | AutoGen | Claude SDK | MCP |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 执行上下文 | Execution Context | Thread + Run | Thread + Run | Runner | Runtime + Team | Session | N/A |
| 执行单元 | Execution Unit | Node | Run Step | Agent turn | Message handler | Agent turn | N/A |
| 状态快照 | State Snapshot | Checkpoint | Thread state | N/A | `save_state()` | N/A | N/A |
| 状态持久化器 | State Persister | Checkpointer | 服务端托管 | N/A | 手动 | N/A | N/A |
| 工具定义 | Tool Definition | `@tool` / `BaseTool` | Function | `@function_tool` | `FunctionTool` | `Tool` | `Tool` (MCP) |
| 工具调用结果 | Tool Result | `ToolMessage` | Function output | Tool output | `FunctionExecutionResult` | `ToolResult` | `CallToolResult` |
| 中断点 | Interrupt Point | `interrupt()` | `requires_action` | Guardrail | `HandoffTermination` | `interrupt()` | `elicitation/create` |
| 恢复指令 | Resume Command | `Command(resume=)` | `submit_tool_outputs` | 手动代码 | `run_stream(task=)` | 新 `query()` | N/A |
| 流式事件 | Stream Event | `StreamPart` | SSE Event | `StreamEvent` | Message | Event | `notifications/progress` |
| 子 Agent | Sub-Agent | Subgraph | N/A | Handoff | Nested Team | Task tool | N/A |
| 执行追踪 | Execution Trace | LangSmith Trace | Run Steps | SDK Traces | Console log | N/A | N/A |
| 错误结果 | Error Result | error-as-data / raise | `last_error` | Exception | 错误消息 | Hook 通知 | `isError: true` |

---

## 附录 B：各框架参考链接

### LangGraph

- 官方文档：https://langchain-ai.github.io/langgraph/
- GitHub（开源）：https://github.com/langchain-ai/langgraph
- LangGraph Platform 文档：https://langchain-ai.github.io/langgraph/cloud/
- Python SDK：https://github.com/langchain-ai/langgraph-sdk

### OpenAI

- Assistants API：https://platform.openai.com/docs/assistants
- Responses API：https://platform.openai.com/docs/api-reference/responses
- Agents SDK：https://github.com/openai/openai-agents-python
- Function Calling：https://platform.openai.com/docs/guides/function-calling

### AutoGen

- 官方文档：https://microsoft.github.io/autogen/
- GitHub（开源）：https://github.com/microsoft/autogen
- AgentChat 快速入门：https://microsoft.github.io/autogen/dev/user-guide/agentchat-user-guide/quickstart.html

### Claude Agent SDK

- Anthropic API 文档：https://docs.anthropic.com/
- Claude Code 文档：https://docs.anthropic.com/en/docs/claude-code
- Agent SDK（开源）：https://github.com/anthropics/claude-code/tree/main/packages/claude-agent

### MCP (Model Context Protocol)

- 官方规范：https://modelcontextprotocol.io/specification
- GitHub（开源）：https://github.com/modelcontextprotocol
- TypeScript SDK：https://github.com/modelcontextprotocol/typescript-sdk
- Python SDK：https://github.com/modelcontextprotocol/python-sdk

### 相关标准与协议

- SSE 规范（WHATWG）：https://html.spec.whatwg.org/multipage/server-sent-events.html
- JSON-RPC 2.0：https://www.jsonrpc.org/specification
- JSON Schema：https://json-schema.org/
- OpenTelemetry：https://opentelemetry.io/
- Google A2A Protocol：https://github.com/google/A2A

---

## 附录 C：版本说明

| 版本 | 日期       | 变更                                   |
| ---- | ---------- | -------------------------------------- |
| v0.1 | 2026-05-04 | 初始版本，覆盖 8 个通用维度 + 5 个框架 |

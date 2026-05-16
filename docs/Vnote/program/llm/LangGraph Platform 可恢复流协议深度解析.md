## 1. 问题背景

在 LLM Agent 应用中，服务端通过 SSE（Server-Sent Events）向客户端推送流式结果。当网络抖动、客户端切后台或连接超时时，SSE 连接会断开。对于执行时间较长的 Agent（如深度搜索、研报生成），断连意味着：

- 用户丢失已生成内容，需要重头开始
- 服务端 Agent 可能仍在运行但产出无人消费
- 重复执行浪费算力和 Token

核心需求：**客户端断连后能恢复到断点继续接收，而非重新执行**。

LangGraph Platform（Agent Server）设计了一套完整的可恢复流协议来解决这个问题。本文深入解析该协议的原理、字段语义与交互流程。

---

## 2. 核心概念：Thread、Run、Stream 的层次关系

理解可恢复流协议前，需先理解 LangGraph Platform 的三层资源模型：

```
Thread（持久化会话容器）
  │
  ├── Run 1（单次 Graph 执行）
  │     └── Stream（本次执行的 SSE 事件流）
  │
  ├── Run 2
  │     └── Stream
  │
  └── Run N
        └── Stream
```

### 2.1 Thread

- 对话状态的持久化容器，由 `thread_id`（UUID）标识
- 跨多次 Run 保存 Checkpoint（Graph 状态快照）
- 通过 `POST /threads` 创建
- 不指定 `thread_id` 的 Run 是无状态的——状态不持久化

### 2.2 Run

- 一次 Graph 在 Thread 内的执行，由 `run_id`（UUID）标识
- 由 `POST /threads/{thread_id}/runs/stream` 隐式创建
- 生命周期状态：`pending → running → success | error | timeout | interrupted`
- 每个 Run 有一个 `attempt` 计数器（从 1 开始），标识第几次尝试

### 2.3 Stream

- 承载 Run 事件输出的 SSE 连接
- 绑定到 **一个 Run**（Run streaming）或 **一个 Thread**（Thread streaming）
- Run streaming 在 Run 完成时关闭；Thread streaming 跨 Run 保持打开
- **可恢复性是 per-Run 配置的**，不是 per-Thread

---

## 3. 三端点模型

LangGraph Platform 围绕"创建 vs 加入"设计了三个 SSE 端点：

| 端点                               | HTTP 方法 | 用途                                    | 是否创建 Run |
| ---------------------------------- | --------- | --------------------------------------- | ------------ |
| `/threads/{tid}/runs/stream`       | POST      | 创建 Run 并开始 SSE 推送                | 是           |
| `/threads/{tid}/runs/{rid}/stream` | GET       | 对已存在的 Run 重新打开 SSE（断线重连） | 否           |
| `/threads/{tid}/stream`            | GET       | 自动 join 该 Thread 下最新的 active Run | 否           |

### 3.1 POST — 创建并流式推送

客户端首次发起请求，创建一个新的 Run 并立即开始 SSE 推送。这是唯一**同时启动 Producer 和 Consumer** 的端点。

```http
POST /threads/{thread_id}/runs/stream
Content-Type: application/json

{
  "assistant_id": "agent",
  "input": { "messages": [...] },
  "stream_mode": ["updates"],
  "stream_resumable": true,
  "on_disconnect": "continue"
}
```

### 3.2 GET by run_id — 断线重连

客户端保存了 `run_id` 后，断连时可以通过此端点重新连接到该 Run 的事件流。

```http
GET /threads/{thread_id}/runs/{run_id}/stream
```

可选参数：

- `last_event_id`：上次收到的事件 ID，服务端从该 ID **之后**开始回放
- `cancel_on_disconnect`：是否在此次连接断开时取消 Run
- `stream_mode`：覆盖流模式（必须是创建时指定模式的子集）

### 3.3 GET by thread — 自动 Join

不需要知道 `run_id`，自动加入 Thread 下当前活跃的 Run。适用于页面刷新等场景。

```http
GET /threads/{thread_id}/stream
```

### 3.4 三端点的服务端行为对比

| 行为                        | POST（创建） | GET by run_id（重连） | GET by thread（join） |
| --------------------------- | ------------ | --------------------- | --------------------- |
| 启动 Producer（Agent 执行） | 是           | 否                    | 否                    |
| 启动 Consumer（SSE 推送）   | 是           | 是                    | 是                    |
| 读取 Last-Event-ID          | —            | 是（cursor 定位）     | —                     |
| 事件消费来源                | 持久化缓冲   | 持久化缓冲            | 持久化缓冲            |

**关键洞察**：POST 和 GET 共享同一套 Consumer 逻辑。区别仅在于 POST 额外触发了 Producer。Consumer 不关心事件是历史回放还是实时产生——它只做阻塞读取等待。

---

## 4. 可恢复流协议的两个必要配置

要启用可恢复流，必须在创建 Run 时**同时**设置两个配置：

### 4.1 `on_disconnect` — 断连行为

```python
DisconnectMode = Literal["cancel", "continue"]
```

| 值 | 行为 |
| --- | --- |
| `"cancel"`（默认） | 客户端断连后取消 Run 执行。包含两种取消动作：`interrupt`（保留 Run 记录和 Checkpoint）和 `rollback`（删除全部） |
| `"continue"` | 客户端断连后 Agent 在独立 Worker 中继续执行，不受 SSE 连接生命周期影响 |

**为什么默认是 cancel**：大多数场景下，用户主动关闭页面意味着不再需要结果。`cancel` 避免浪费算力。只有需要可恢复性时才改为 `continue`。

### 4.2 `stream_resumable` — 事件持久化

| 值              | 行为                                                                    |
| --------------- | ----------------------------------------------------------------------- |
| `false`（默认） | 事件仅推送给当前连接的客户端，yield 后即丢弃                            |
| `true`          | 每个事件在推送的同时写入持久化缓冲（如 Redis Stream），供后续重连时回放 |

**两者缺一不可**：

| 组合                 | 效果                                                         |
| -------------------- | ------------------------------------------------------------ |
| `cancel` + `false`   | 标准 SSE，断连即终止（默认行为）                             |
| `continue` + `false` | Agent 继续执行，但断连期间的事件**丢失**，无法回放           |
| `cancel` + `true`    | 事件被缓冲，但 Agent 在断连时被取消，没有新事件产生          |
| `continue` + `true`  | 完整的可恢复流：Agent 继续执行，事件被缓冲，客户端可重连回放 |

---

## 5. SSE 事件格式深度解析

### 5.1 Wire-Level 格式

响应使用 `Content-Type: text/event-stream`，遵循 SSE 规范。每个事件由以下字段组成：

```
id: <sequential_integer>
event: <event_type>
data: <json_payload>

```

每个事件以空行（`\n\n`）终止。示例：

```
id: 0
event: metadata
data: {"run_id": "1f02c2b3-3cef-68de-b720-eec2a4a8e920", "attempt": 1}

id: 1
event: updates
data: {"refine_topic": {"topic": "ice cream and cats"}}

id: 2
event: updates
data: {"generate_joke": {"joke": "Why did the cat sit on the ice cream?"}}

id: 3
event: end
data: null

```

### 5.2 事件字段语义

#### `id` — 事件序列号

- 类型：递增整数（0, 1, 2, ...）
- 由服务端在事件产生时分配
- 作为客户端断线重连的**游标**（cursor）
- 客户端需要跟踪最后收到的 `id`，断连后作为 `last_event_id` 传入重连请求

#### `event` — 事件类型

由所选的 `stream_mode` 决定产生哪些事件类型。特殊事件类型：

| 事件类型 | 来源 | 含义 |
| --- | --- | --- |
| `metadata` | 系统 | Run 的元数据，始终是第一个事件 |
| `end` | 系统 | Run 执行结束标记，始终是最后一个事件 |
| `values` | `stream_mode=["values"]` | 每个 super-step 后的完整 Graph 状态 |
| `updates` | `stream_mode=["updates"]` | 每个节点执行后的状态增量（delta） |
| `messages/partial` | `stream_mode=["messages"]` | LLM token 流式输出（部分消息） |
| `messages/complete` | `stream_mode=["messages"]` | LLM 消息完成 |
| `messages/metadata` | `stream_mode=["messages"]` | LLM 消息的元数据 |
| `messages` | `stream_mode=["messages-tuple"]` | LLM token 以 `[message_dict, metadata_dict]` 元组形式流式输出 |
| `custom` | `stream_mode=["custom"]` | 图内部通过 `get_stream_writer()` 发出的自定义数据 |
| `tasks` | `stream_mode=["tasks"]` | Task 级别事件（开始、结束） |
| `checkpoints` | `stream_mode=["checkpoints"]` | Checkpoint 信息 |
| `debug` | `stream_mode=["debug"]` | 最大详细度的调试信息 |

#### `data` — 事件载荷

JSON 序列化的载荷，结构因 `event` 类型而异。

### 5.3 首个事件：`metadata`

每个 Run 的第一个 SSE 事件始终是 `metadata`，客户端**必须**从中提取并保存 `run_id`，这是后续断线重连的关键标识。

```
id: 0
event: metadata
data: {"run_id": "1f02c2b3-3cef-68de-b720-eec2a4a8e920", "attempt": 1}
```

| 字段      | 类型          | 说明                                     |
| --------- | ------------- | ---------------------------------------- |
| `run_id`  | string (UUID) | 本次 Run 的唯一标识，断线重连时需要此 ID |
| `attempt` | integer       | 尝试次数，从 1 开始                      |

### 5.4 末尾事件：`end`

```
id: 42
event: end
data: null
```

`end` 事件标志 Run 完成。收到后客户端应关闭连接。

---

## 6. StreamMode — 事件模式详解

客户端通过 `stream_mode` 参数控制接收哪些类型的事件。支持传入数组以组合多种模式。

```python
StreamMode = Literal[
    "values",         # 完整状态快照
    "updates",        # 节点级状态增量
    "messages",       # LLM token 流（结构化三事件模型）
    "messages-tuple", # LLM token 流（[message, metadata] 元组）
    "events",         # 全部事件（用于 LCEL 迁移）
    "tasks",          # Task 级事件
    "checkpoints",    # Checkpoint 信息
    "debug",          # 调试级详细信息
    "custom",         # 用户自定义数据
]
```

### 6.1 `values` vs `updates` — 最常用的两种模式

```
                 super-step 1          super-step 2
Graph State:     {a: 1}        →      {a: 1, b: 2}

values 事件:     {a: 1}                {a: 1, b: 2}     ← 完整快照
updates 事件:    {node_1: {a: 1}}      {node_2: {b: 2}} ← 仅增量
```

- `values`：每个 super-step 后输出完整 Graph State。数据量大但客户端无需维护状态。
- `updates`：每个节点执行后仅输出该节点返回的增量。数据量小但客户端需自行合并。

### 6.2 `messages` — LLM Token 流

`messages` 模式产生三种子事件类型，用于实时展示 LLM 的生成过程：

| 子事件              | 时机             | data 内容                               |
| ------------------- | ---------------- | --------------------------------------- |
| `messages/partial`  | 每收到一个 token | 当前消息的部分内容                      |
| `messages/complete` | LLM 调用完成     | 完整消息对象                            |
| `messages/metadata` | 元信息可用时     | langgraph_node、langgraph_step 等元数据 |

### 6.3 `custom` — 自定义事件

图节点内部可通过 `get_stream_writer()` 注入自定义事件，以 `custom` 类型推送：

```python
from langgraph.config import get_stream_writer

def my_node(state):
    writer = get_stream_writer()
    writer({"progress": 0.5, "stage": "analyzing"})  # → event: custom
    # ... 节点逻辑
```

### 6.4 多模式组合

传入数组可同时接收多种模式的事件：

```json
{
  "stream_mode": ["updates", "custom", "messages"]
}
```

SDK 层面每个 chunk 被包装为 `(mode, chunk)` 元组，客户端通过 `mode` 字段区分事件来源。

### 6.5 Thread-level Stream 的模式限制

Thread streaming（`GET /threads/{tid}/stream`）支持的模式是 Run streaming 的**子集**：

```python
ThreadStreamMode = Literal[
    "values", "messages", "updates", "events",
    "custom", "messages-tuple",
]
# 不支持 tasks、checkpoints、debug
```

---

## 7. SDK 层事件格式（V1 / V2）

LangGraph Python SDK 提供两个版本的事件格式，在 Wire-Level SSE 之上做了结构化封装。

### 7.1 V1 格式（默认，NamedTuple）

```python
StreamPart = NamedTuple("StreamPart", event=str, data=Any)

# 示例
StreamPart(event='metadata', data={'run_id': 'abc', 'attempt': 1})
StreamPart(event='updates',  data={'node_1': {'key': 'val'}})
StreamPart(event='end',      data=None)
```

### 7.2 V2 格式（TypedDict，判别联合）

V2 引入了更精确的类型系统，所有事件共享三个公共字段：

| 字段   | 类型        | 说明                                       |
| ------ | ----------- | ------------------------------------------ |
| `type` | `str`       | 判别字段，标识事件类型                     |
| `ns`   | `list[str]` | 命名空间路径。根图为空列表，子图会填充路径 |
| `data` | varies      | 事件载荷，结构因 `type` 而异               |

完整的 V2 类型表：

| TypedDict 类型               | `type` 值             | `data` 类型                | 额外字段                 |
| ---------------------------- | --------------------- | -------------------------- | ------------------------ |
| `MetadataStreamPart`         | `"metadata"`          | `RunMetadataPayload`       | —                        |
| `ValuesStreamPart`           | `"values"`            | `dict[str, Any]`           | `interrupts: list[dict]` |
| `UpdatesStreamPart`          | `"updates"`           | `dict[str, Any]`           | —                        |
| `MessagesPartialStreamPart`  | `"messages/partial"`  | `list[dict[str, Any]]`     | —                        |
| `MessagesCompleteStreamPart` | `"messages/complete"` | `list[dict[str, Any]]`     | —                        |
| `MessagesMetadataStreamPart` | `"messages/metadata"` | `dict[str, Any]`           | —                        |
| `MessagesTupleStreamPart`    | `"messages"`          | `list[dict]`（两元素元组） | —                        |
| `CustomStreamPart`           | `"custom"`            | varies                     | —                        |
| `TasksStreamPart`            | `"tasks"`             | `TaskPayload`              | —                        |
| `CheckpointsStreamPart`      | `"checkpoints"`       | varies                     | —                        |
| `DebugStreamPart`            | `"debug"`             | `DebugPayload`             | —                        |

V2 的联合类型定义：

```python
StreamPartV2 = Union[
    ValuesStreamPart, UpdatesStreamPart,
    MessagesPartialStreamPart, MessagesCompleteStreamPart,
    MessagesMetadataStreamPart, MessagesTupleStreamPart,
    CustomStreamPart, TasksStreamPart,
    CheckpointsStreamPart, DebugStreamPart,
    MetadataStreamPart,
]
```

客户端通过 `part["type"]` 做判别分发。

---

## 8. Last-Event-ID 游标协议

SSE 规范原生支持 `Last-Event-ID` 头，LangGraph Platform 基于此构建了断线恢复机制。

### 8.1 协议流程

**服务端发送**：每个 SSE 事件带递增 `id`

```
id: 0
event: metadata
data: {"run_id":"abc","attempt":1}

id: 1
event: updates
data: {"node_1": {"key": "val"}}

id: 2
event: updates
data: {"node_2": {"key": "val2"}}
```

**客户端断连后重连**：通过 `last_event_id` 参数传递游标

```python
# Python SDK
client.runs.join_stream(
    thread_id=tid,
    run_id=rid,
    last_event_id="2"  # 上次收到的事件 ID
)
```

**服务端行为**：

```
收到 last_event_id = "2"
→ 从持久化缓冲中读取 id > 2 的所有事件
→ 依次推送（Catch-up 阶段）
→ 追完后切换到实时推送（Live Tail 阶段）
```

### 8.2 `last_event_id` 的特殊值

| 值                    | 行为                                     |
| --------------------- | ---------------------------------------- |
| 不传                  | 仅接收重连后新产生的事件，错过的事件丢失 |
| `"-1"`                | 从头回放所有事件                         |
| `"N"`（正整数字符串） | 从 ID = N **之后**开始回放（exclusive）  |

### 8.3 版本差异

> **v0.6.0 行为变更**：v0.6.0 之前 `Last-Event-ID` 是 inclusive（包含该 ID 对应的事件），v0.6.0 起改为 exclusive（只返回该 ID 之后的事件），与 SSE 规范的语义对齐。

### 8.4 原生 EventSource vs Fetch-Based Client

浏览器原生的 `EventSource` API 会在断连后自动重连并发送 `Last-Event-ID` 头，但它**不支持 POST 请求**，因此无法用于创建 Run。LangGraph SDK 使用基于 `fetch` 的 SSE 客户端（如 `@microsoft/fetch-event-source`），手动管理 `last_event_id` 参数。

### 8.5 客户端重连时的幂等性

由于事件 ID 是递增的且 exclusive，多次用相同的 `last_event_id` 重连是**幂等**的——服务端每次都从同一位置开始回放。但客户端需注意：

- **必须去重**：如果客户端在重连前已有部分消息缓存（如 React 组件的 state），回放的事件会与已有数据重叠。SDK 的 `MessageTupleManager` 需要在回放前清空，否则会出现内容重复（如 "Hello" 变成 "HelloHello"）。

---

## 9. Producer-Consumer 架构

LangGraph Platform 可恢复流的核心设计是**生产者-消费者分离**。

### 9.1 架构图

```
┌──────────────┐      ┌──────────────┐      ┌──────────────┐
│   Client A   │      │   Client B   │      │   Client C   │
│  (POST 创建)  │      │ (GET 重连)    │      │ (GET join)   │
└──────┬───────┘      └──────┬───────┘      └──────┬───────┘
       │ SSE                  │ SSE                  │ SSE
       ▼                      ▼                      ▼
┌─────────────────────────────────────────────────────────┐
│                    API Server 进程                       │
│                                                         │
│  ┌─────────────┐   ┌─────────────┐   ┌─────────────┐  │
│  │  Consumer A  │   │  Consumer B  │   │  Consumer C  │  │
│  │ (创建+消费)   │   │  (仅消费)    │   │  (仅消费)    │  │
│  └──────┬──────┘   └──────┬──────┘   └──────┬──────┘  │
│         │                  │                  │         │
│         └──────────────────┼──────────────────┘         │
│                            │ XREAD / Buffer Read        │
└────────────────────────────┼────────────────────────────┘
                             │
                    ┌────────▼────────┐
                    │  持久化缓冲层     │
                    │  (Redis Stream)  │
                    │                 │
                    │  Key: run:{id}  │
                    │  0: metadata    │
                    │  1: updates     │
                    │  2: updates     │
                    │  3: end         │
                    └────────▲────────┘
                             │ XADD / Buffer Write
                    ┌────────┴────────┐
                    │  Worker 进程     │
                    │  (Producer)     │
                    │                 │
                    │  Graph.astream() │
                    │  → 逐事件写缓冲  │
                    └─────────────────┘
```

### 9.2 Producer（生产者）

- 在独立 Worker 进程中运行 `Graph.astream()`
- 每产生一个事件，通过 `XADD` 写入 Redis Stream（key 为 `run:{run_id}`）
- 事件按产生顺序追加，Redis 自动分配递增 ID
- Producer 不关心有没有客户端在消费——它只管写入
- Run 完成时写入 `end` 标记事件

### 9.3 Consumer（消费者）

Consumer 不关心事件是历史还是实时的，执行统一的两阶段读取：

```
阶段 1 — 追赶（Catch-up）:
  XREAD stream_key cursor_id COUNT 100
  → 读取积压事件，逐条转为 SSE 推送给客户端
  → 重复直到 XREAD 返回空

阶段 2 — 实时（Live Tail）:
  XREAD BLOCK 5000 stream_key latest_id
  → 阻塞等待新事件（超时 5 秒后重试）
  → 事件到达后立即推送
  → 循环直到收到 END 标记
```

**关键性质**：Catch-up 和 Live Tail 的代码路径完全相同（都是 XREAD），差别仅在于是否阻塞。这保证了事件顺序的一致性——无论是回放还是实时，客户端收到的事件序列完全相同。

### 9.4 Redis Stream 作为缓冲的特性

| 特性       | 说明                                                                                       |
| ---------- | ------------------------------------------------------------------------------------------ |
| 追加式日志 | 事件只追加不修改，天然有序                                                                 |
| 游标回溯   | 任意消费者可从任意 ID 开始读取                                                             |
| TTL 控制   | 通过 `RESUMABLE_STREAM_TTL_SECONDS` 环境变量设置过期时间（默认 120 秒），超过后 key 被清除 |
| 多消费者   | 同一个 Stream 可被多个 Consumer 同时读取（如多个客户端同时 join 同一个 Run）               |
| 跨进程     | API Server 和 Worker 可以是不同进程甚至不同机器                                            |

---

## 10. 完整的断连-重连交互时序

```
CLIENT                                           SERVER
  │                                                 │
  │── POST /threads/{tid}/runs/stream ─────────────>│
  │   Body: {                                       │
  │     assistant_id: "agent",                      │
  │     input: {...},                               │
  │     stream_resumable: true,                     │  ← 开启事件持久化
  │     on_disconnect: "continue",                  │  ← 断连后 Agent 继续
  │     stream_mode: ["updates"]                    │
  │   }                                             │
  │                                                 │
  │                           ┌─ 启动 Producer（Worker 进程执行 Agent）
  │                           └─ 启动 Consumer（API 进程读缓冲 → SSE 推送）
  │                                                 │
  │<── 200 OK ──────────────────────────────────────│
  │    Content-Type: text/event-stream              │
  │                                                 │
  │<── id: 0                                        │
  │    event: metadata                              │
  │    data: {"run_id":"abc-123","attempt":1}        │
  │                                                 │
  │    [客户端保存 run_id = "abc-123"]                │
  │                                                 │
  │<── id: 1                                        │
  │    event: updates                               │
  │    data: {"search_node": {"results": [...]}}     │
  │                                                 │
  │<── id: 2                                        │
  │    event: updates                               │
  │    data: {"analyze_node": {"summary": "..."}}    │
  │                                                 │
  │    ╳╳╳ 网络断开 ╳╳╳                               │
  │    [客户端记录 last_event_id = 2]                  │
  │                                                 │
  │                           [Worker 继续执行 Agent]
  │                           [事件 3, 4, 5 写入 Redis Stream]
  │                                                 │
  │    ... 10 秒后网络恢复 ...                         │
  │                                                 │
  │── GET /threads/{tid}/runs/abc-123/stream ──────>│
  │   last_event_id: 2                              │
  │                                                 │
  │                           └─ 启动新 Consumer，cursor = 2
  │                                                 │
  │<── 200 OK ──────────────────────────────────────│
  │    Content-Type: text/event-stream              │
  │                                                 │
  │<── id: 3  ← Catch-up 回放                       │
  │    event: updates                               │
  │    data: {"draft_node": {"draft": "..."}}        │
  │                                                 │
  │<── id: 4  ← Catch-up 回放                       │
  │    event: updates                               │
  │    data: {"review_node": {"score": 0.85}}        │
  │                                                 │
  │<── id: 5  ← Catch-up 回放                       │
  │    event: updates                               │
  │    data: {"polish_node": {"final": "..."}}       │
  │                                                 │
  │    [Catch-up 完成，切换到 Live Tail]               │
  │                                                 │
  │<── id: 6  ← 实时推送                             │
  │    event: updates                               │
  │    data: {"export_node": {"url": "..."}}         │
  │                                                 │
  │<── id: 7                                        │
  │    event: end                                   │
  │    data: null                                   │
  │                                                 │
  │    [SSE 连接关闭]                                 │
```

### 10.1 已完成场景

如果 Agent 在客户端断连期间已执行完成，重连时的行为：

```
CLIENT                                           SERVER
  │                                                 │
  │── GET /threads/{tid}/runs/abc-123/stream ──────>│
  │   last_event_id: 2                              │
  │                                                 │
  │<── id: 3 (catch-up)                             │
  │<── id: 4 (catch-up)                             │
  │<── id: 5 (catch-up)                             │
  │<── id: 6 (catch-up)                             │
  │<── id: 7                                        │
  │    event: end     ← 已完成，立即收到 end           │
  │    data: null                                   │
  │                                                 │
  │    [SSE 连接关闭，所有错过的事件已回放]              │
```

### 10.2 TTL 超时场景

如果断连时间超过 `RESUMABLE_STREAM_TTL_SECONDS`（默认 120 秒），Redis Stream key 已过期：

- 重连请求返回错误或空流
- 客户端需要通过 Checkpoint 机制恢复 Graph 状态，但**流事件不可回放**

---

## 11. Checkpoint 与 Stream 可恢复性：两种不同的持久化

这两者经常被混淆，但它们解决不同的问题：

| 维度       | Checkpoint                                  | Stream Event 持久化                  |
| ---------- | ------------------------------------------- | ------------------------------------ |
| 持久化对象 | Graph State（图的完整状态快照）             | SSE 事件（id + event + data 三元组） |
| 存储位置   | Checkpointer DB（PostgreSQL 等）            | Redis Stream                         |
| 标识方式   | `(thread_id, checkpoint_ns, checkpoint_id)` | 事件递增 ID                          |
| 用途       | 恢复 Graph **执行**（如中断后继续）         | 恢复 **流式输出**（如断连后回放）    |
| 生命周期   | 永久（除非显式删除）                        | 临时（TTL 后过期，默认 120 秒）      |
| 控制参数   | `checkpoint_during`                         | `stream_resumable`                   |

### 11.1 `checkpoint_during` 参数

```python
checkpoint_during: bool | None = None
```

- `True`（默认）：每个 super-step 后都写 Checkpoint
- `False`：只在 Run 结束或被中断时写 Checkpoint（性能优化）
- 此参数**不影响** `stream_resumable` 的行为——即使 `checkpoint_during=False`，每个 SSE 事件仍会被写入 Redis Stream

### 11.2 互补关系

两者组合覆盖了不同的故障场景：

| 故障场景                      | Checkpoint 恢复            | Stream 恢复                 |
| ----------------------------- | -------------------------- | --------------------------- |
| 客户端网络短暂断连（< TTL）   | 不需要                     | 回放错过的事件              |
| 客户端网络长时间断连（> TTL） | 从最近 Checkpoint 重新执行 | 无法回放（已过期）          |
| 服务端 Worker 崩溃            | 从最近 Checkpoint 恢复执行 | 已写入 Redis 的事件仍可回放 |
| Human-in-the-loop 中断        | 必需（保存中断状态）       | 可选（回放中断前事件）      |

---

## 12. POST 创建 Run 的完整请求参数

以下是 `POST /threads/{thread_id}/runs/stream` 的完整请求体字段参考：

```python
{
    # --- 执行控制 ---
    "assistant_id": str,              # 必填。要执行的 Assistant（Graph）ID
    "input": dict,                    # Graph 输入
    "command": {                      # 用于 Human-in-the-loop 场景
        "update": dict,               #   更新 Graph State
        "resume": Any,                #   从中断点继续执行，传递用户响应
        "goto": list[str]             #   跳转到指定节点
    },
    "config": {                       # 运行时配置
        "tags": list[str],
        "recursion_limit": int,       #   默认 25
        "configurable": dict          #   传递给图的可配置参数
    },
    "metadata": dict,                 # 自定义元数据，附加到 Run
    "context": dict,                  # 上下文信息

    # --- 中断控制 ---
    "interrupt_before": list[str] | "*",  # 在指定节点执行前中断
    "interrupt_after": list[str] | "*",   # 在指定节点执行后中断

    # --- 流式控制 ---
    "stream_mode": list[StreamMode],  # 事件模式（可组合）
    "stream_subgraphs": bool,         # 是否包含子图事件，默认 false
    "stream_resumable": bool,         # 是否开启事件持久化，默认 false

    # --- 生命周期控制 ---
    "on_disconnect": "cancel" | "continue",  # 断连行为，默认 "cancel"
    "on_completion": "delete" | "keep",      # 完成后是否删除 Run 记录

    # --- Checkpoint 控制 ---
    "checkpoint": {                   # 从指定 Checkpoint 恢复执行
        "thread_id": str,
        "checkpoint_ns": str,
        "checkpoint_id": str,
        "checkpoint_map": dict
    },
    "checkpoint_during": bool,        # 执行过程中是否写 Checkpoint

    # --- 并发与调度 ---
    "multitask_strategy": "enqueue" | "reject" | "rollback" | "interrupt",
    "if_not_exists": "reject" | "create",  # Thread 不存在时的行为
    "after_seconds": int | None,           # 延迟执行（秒）

    # --- 其他 ---
    "webhook": str,                   # Run 完成后的回调 URL
    "feedback_keys": list[str],       # 反馈收集的 key 列表
}
```

---

## 13. 前端集成：React useStream Hook

LangGraph JS SDK 提供了 `useStream` React Hook，封装了完整的可恢复流逻辑：

### 13.1 自动重连（页面刷新后恢复）

```typescript
const thread = useStream<{ messages: Message[] }>({
  apiUrl: 'http://localhost:2024',
  assistantId: 'agent',
  reconnectOnMount: true, // 组件挂载时自动恢复进行中的流
})
```

`reconnectOnMount: true` 的行为：组件挂载时检查是否有未完成的 Run，若有则自动调用 `joinStream` 恢复，确保页面刷新不会丢失事件。

### 13.2 手动管理重连（完整控制）

```typescript
const thread = useStream<{ messages: Message[] }>({
  apiUrl: 'http://localhost:2024',
  assistantId: 'agent',
  threadId,

  // 创建时保存 run_id 到 sessionStorage
  onCreated: (run) => {
    sessionStorage.setItem(`resume:${run.thread_id}`, run.run_id)
  },

  // 完成时清除
  onFinish: (_, run) => {
    sessionStorage.removeItem(`resume:${run?.thread_id}`)
  },
})

// 页面加载时检查是否需要恢复
useEffect(() => {
  const runId = sessionStorage.getItem(`resume:${threadId}`)
  if (runId) {
    thread.joinStream(runId) // 手动恢复到上次的 Run
  }
}, [threadId])

// 创建 Run 时必须开启 streamResumable
thread.submit(
  { messages: [{ type: 'human', content: '...' }] },
  { streamResumable: true } // 关键：开启事件持久化
)
```

---

## 14. 缓冲层选型

LangGraph Platform 使用 Redis Stream 作为事件缓冲，但该协议的核心机制（递增 ID + 游标回溯）不依赖具体存储实现。

### 14.1 特性对比

| 维度     | Redis Stream          | Redis Pub/Sub         | 进程内 Buffer            |
| -------- | --------------------- | --------------------- | ------------------------ |
| 持久化   | 是（append-only log） | 否（fire-and-forget） | 否（重启丢失）           |
| 断线重放 | 是（游标回溯）        | 否                    | 是（offset 回溯）        |
| 跨进程   | 是                    | 是                    | 否                       |
| 消息积压 | 保留到 TTL 或 XTRIM   | 无订阅者则丢弃        | 保留到 maxlen 或进程退出 |
| 典型延迟 | ~1ms                  | ~0.5ms                | ~0（同进程）             |

### 14.2 核心区别

- **Pub/Sub ≈ 直连 SSE**：都是"发了就忘"，无法回放。Pub/Sub 的价值在于跨进程广播，但没有持久化能力。
- **Stream 的本质差异**：Redis Stream 是带游标的持久化日志。它的价值不在于"消息传递"，而在于"消息保留"——事件追加后，任何消费者可以从任意位置开始读取。

### 14.3 选型决策树

```
需要跨机器/跨进程共享事件流？
├── 是 → 已有 Redis？
│        ├── 是 → Redis Stream（推荐）
│        └── 否 → 已有 PostgreSQL？
│                 ├── 是 → PG NOTIFY + events 表
│                 └── 否 → 引入 Redis（运维成本最低）
└── 否 → 需要重启后仍可恢复？
         ├── 是 → SQLite WAL
         └── 否 → 进程内 deque 缓冲
```

---

## 15. 总结

LangGraph Platform 的可恢复流协议由以下核心要素组成：

| 要素                        | 设计                            | 作用                               |
| --------------------------- | ------------------------------- | ---------------------------------- |
| 三端点模型                  | POST 创建 / GET 重连 / GET join | 分离创建与加入，支持多种恢复场景   |
| `on_disconnect: "continue"` | 执行与连接解耦                  | Agent 不受客户端生命周期影响       |
| `stream_resumable: true`    | 事件持久化                      | 事件可被回放，而非 fire-and-forget |
| 递增事件 ID                 | SSE `id` 字段                   | 客户端可精确定位断点               |
| `last_event_id` 游标        | 重连参数                        | 服务端知道从哪里开始回放           |
| Producer-Consumer 分离      | Worker 写缓冲 / API 读缓冲      | 生产消费独立，多客户端可同时消费   |
| 两阶段 Consumer             | Catch-up → Live Tail            | 无缝衔接历史回放与实时推送         |
| TTL 管理                    | `RESUMABLE_STREAM_TTL_SECONDS`  | 平衡存储开销与恢复窗口             |

**协议的精髓**：将 SSE 从"实时管道"升级为"可回溯的日志"。通过在事件产生和事件消费之间插入一层持久化缓冲，彻底解耦了 Agent 执行与客户端连接的生命周期。

---

## 附录 A：参考链接

### 官方文档

| 主题 | 链接 |
| --- | --- |
| Join & Rejoin Streams 指南 | [docs.langchain.com/.../join-rejoin](https://docs.langchain.com/oss/python/langchain/frontend/join-rejoin) |
| Streaming API 概览 | [docs.langchain.com/.../streaming](https://docs.langchain.com/langsmith/streaming) |
| Cancel Run / Disconnect Modes | [docs.langchain.com/.../cancel-run](https://docs.langchain.com/langsmith/cancel-run) |
| `stream_resumable` SDK 参考（async） | [reference.langchain.com/.../RunsClient/stream](https://reference.langchain.com/python/langgraph-sdk/_async/runs/RunsClient/stream) |
| `stream_resumable` SDK 参考（sync） | [reference.langchain.com/.../SyncRunsClient/stream](https://reference.langchain.com/python/langgraph-sdk/_sync/runs/SyncRunsClient/stream) |
| Join Run Stream 端点 (`GET /runs/{rid}/stream`) | [docs.langchain.com/.../join-run-stream](https://docs.langchain.com/langsmith/agent-server-api/thread-runs/join-run-stream) |
| Join Thread Stream 端点 (`GET /threads/{tid}/stream`) | [docs.langchain.com/.../join-thread-stream](https://docs.langchain.com/langsmith/agent-server-api/threads/join-thread-stream) |
| `useStream` React Hook 参考 | [reference.langchain.com/.../react/useStream](https://reference.langchain.com/javascript/langchain-langgraph-sdk/react/useStream) |
| `join_stream` 方法参考 | [reference.langchain.com/.../SyncRunsClient/join_stream](https://reference.langchain.com/python/langgraph-sdk/_sync/runs/SyncRunsClient/join_stream) |
| Agent Server Changelog（含 v0.2.106 可恢复流条目） | [docs.langchain.com/.../agent-server-changelog](https://docs.langchain.com/langsmith/agent-server-changelog) |

### 开源仓库

| 项目 | 链接 | 说明 |
| --- | --- | --- |
| LangGraph 主仓库 | [github.com/langchain-ai/langgraph](https://github.com/langchain-ai/langgraph) | 包含核心库、Python SDK、CLI 等 |
| LangGraph JS 仓库 | [github.com/langchain-ai/langgraphjs](https://github.com/langchain-ai/langgraphjs) | JS/TS SDK，含 `useStream` Hook |
| Python SDK 目录 | [github.com/.../libs/sdk-py](https://github.com/langchain-ai/langgraph/tree/main/libs/sdk-py) | `langgraph-sdk` PyPI 包的源码 |
| `schema.py` — 类型定义 | [github.com/.../schema.py](https://github.com/langchain-ai/langgraph/blob/main/libs/sdk-py/langgraph_sdk/schema.py) | `StreamMode`、`DisconnectMode`、`StreamPartV2` 等类型 |
| `client.py` — SDK 客户端 | [github.com/.../client.py](https://github.com/langchain-ai/langgraph/blob/main/libs/sdk-py/langgraph_sdk/client.py) | `stream()`、`join_stream()` 方法实现 |

> **注意**：LangGraph Platform 的服务端组件（`langgraph-api`，包含 Redis Stream 缓冲实现）是**闭源**的，不在上述开源仓库中。协议的客户端侧逻辑可在 SDK 源码中查看，服务端行为需参考官方文档。

### 相关 Issue

| Issue | 链接 | 说明 |
| --- | --- | --- |
| #2028 — joinStream 重连后消息重复 | [github.com/.../issues/2028](https://github.com/langchain-ai/langgraphjs/issues/2028) | `lastEventId` 默认 `"-1"` 导致全量回放与已有缓存重叠 |
| #1969 — joinStream 后状态陈旧 | [github.com/.../issues/1969](https://github.com/langchain-ai/langgraphjs/issues/1969) | 重连后客户端状态管理的边界问题 |

### SSE 规范参考

| 规范 | 链接 |
| --- | --- |
| MDN — Server-Sent Events | [developer.mozilla.org/.../Server-sent_events](https://developer.mozilla.org/en-US/docs/Web/API/Server-sent_events/Using_server-sent_events) |
| WHATWG — EventSource 规范 | [html.spec.whatwg.org/.../server-sent-events](https://html.spec.whatwg.org/multipage/server-sent-events.html) |

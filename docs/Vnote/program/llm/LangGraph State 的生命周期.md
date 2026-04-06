---
title: LangGraph State 的生命周期
date: 2026-04-06
tags:
  - LangGraph
  - State
  - Checkpoint
categories:
  - LLM
  - Agent
description: 详解 LangGraph State 在内存与数据库之间的完整生命周期：写入 → 更新 → 回滚 → 读取，涵盖 checkpoints / checkpoint_writes / checkpoint_blobs 三张表
---

## 概览

LangGraph 的 State 在内存与数据库之间流转，形成完整的生命周期。核心逻辑链条：

> **① 写入 → ② 更新 → ③ 回滚 → ④ 读取**

### 三张核心表

| 表名 | 职责 | 粒度 | 写入时机 |
|------|------|------|---------|
| `checkpoints` | 元数据索引（快照链条） | 每个 checkpoint 1 条 | checkpoint 完成时 |
| `checkpoint_writes` | 写入操作历史（每个 node 的部分更新） | 每个 task 1+ 条 | **每个 node 执行后立即写入** |
| `checkpoint_blobs` | 最终完整状态（合并后的序列化数据） | 每个 channel 1 条 | checkpoint 完成时 |

三表通过 `thread_id` + `checkpoint_ns_hash` 关联。

### 三张表的关系

```
┌─────────────────────────────────────────────┐
│             LangGraph 执行流程                │
└─────────────────────────────────────────────┘
                      │
      ┌───────────────┼───────────────┐
      │               │               │
      ↓               ↓               ↓
 Node A 写入      Node B 写入     Node C 写入
 {"user_context":  {"messages":    {"messages":
  {...}}            [msg1]}         [msg2]}
      │               │               │
      ↓               ↓               ↓
┌─────────────────────────────────────────────┐
│  checkpoint_writes 表                        │
│  记录每个 task 的写入操作（细粒度）            │
└─────────────────────────────────────────────┘
                      │
                      │ 合并所有 task 的写入
                      ↓
┌─────────────────────────────────────────────┐
│  checkpoint_blobs 表                         │
│  存储最终合并后的完整状态（粗粒度）             │
└─────────────────────────────────────────────┘
                      │
                      │ 元数据索引
                      ↓
┌─────────────────────────────────────────────┐
│  checkpoints 表                              │
│  存储 checkpoint 元数据（链条/分支）           │
└─────────────────────────────────────────────┘
```

### 完整数据流图

```
┌───────────────────────────────────────────────────────┐
│                   内存中的 State                        │
│  {                                                     │
│    messages: [msg1, msg2],                             │
│    files: {"file_001": {...}},                         │
│    user_context: {"age": 30, ...}                      │
│  }                                                     │
└───────────────────────────────────────────────────────┘
       │ ① 写入 (node 级)         ④ 读取 │
       │ ② 更新 (新 checkpoint)          │
       ↓                                 ↑
┌───────────────────────────────────────────────────────┐
│                   数据库存储                             │
│                                                        │
│  checkpoint_writes  ──合并──→  checkpoint_blobs         │
│  (每个 node 的写入)           (最终完整状态)              │
│                                    │                   │
│                               元数据索引                │
│                                    ↓                   │
│                             checkpoints                │
│                           (链条 / 分支)                 │
│                                                        │
│                   ③ 回滚 = 新建分支                      │
└───────────────────────────────────────────────────────┘
```

---

## ① 写入：State → Database

### 触发时机

- Agent 执行完一个 node
- 用户发送新消息
- 调用 tool 完成后

### 实际场景

假设 Agent 执行一个包含 3 个 node 的流程：

```python
graph = StateGraph(AgentState)
graph.add_node("retrieve_context", retrieve_node)   # task_id = "retrieve_context"
graph.add_node("search_products", search_node)      # task_id = "search_products"
graph.add_node("generate_response", generate_node)  # task_id = "generate_response"
```

用户输入 `"推荐医疗险"`，执行过程：

```
→ Node 1: retrieve_context
    写入: {"user_context": {"age": 30, ...}}

→ Node 2: search_products
    写入: {"messages": [AIMessage(...), ToolMessage(...)]}

→ Node 3: generate_response
    写入: {"messages": [AIMessage("推荐您...")]}

→ Checkpoint 完成
    合并所有写入 → checkpoint_blobs
```

### 步骤 1：每个 node 执行后 → 写入 checkpoint_writes 表

每个 node 执行完毕**立即**写入，记录该 task 对 State 的部分更新：

```sql
-- Node 1: retrieve_context 的写入
INSERT INTO checkpoint_writes VALUES (
    'thread_001', 'ckpt_005', 'd41d8cd...', 'retrieve_context', 0, 'user_context',
    'msgpack', <binary: {"age": 30, ...}>, '()', NOW()
);

-- Node 2: search_products 的写入
INSERT INTO checkpoint_writes VALUES (
    'thread_001', 'ckpt_005', 'd41d8cd...', 'search_products', 0, 'messages',
    'msgpack', <binary: [AIMessage, ToolMessage]>, '()', NOW()
);

-- Node 3: generate_response 的写入
INSERT INTO checkpoint_writes VALUES (
    'thread_001', 'ckpt_005', 'd41d8cd...', 'generate_response', 0, 'messages',
    'msgpack', <binary: [AIMessage("推荐您...")]>, '()', NOW()
);
```

> 注意：Node 2 和 Node 3 都写入了 `messages` channel，后续会被合并。

### 步骤 2：所有 node 完成后 → 合并写入 checkpoint_blobs 表

将 `checkpoint_writes` 中同一 channel 的多条记录合并，写入最终状态：

```python
from langgraph.checkpoint.serde.jsonplus import JsonPlusSerializer

serializer = JsonPlusSerializer()

for field_name, field_value in merged_state.items():
    serialized_data = serializer.dumps_typed(field_value)
    # serialized_data = ('msgpack', binary_data)

    INSERT INTO checkpoint_blobs VALUES (
        thread_id          = 'thread_001',
        checkpoint_ns_hash = 'd41d8cd98f00b204e9800998ecf8427e',
        channel            = field_name,
        version            = 0,
        type               = 'msgpack',
        blob_data          = binary_data
    )
```

### 步骤 3：写入 checkpoints 表（元数据）

```sql
INSERT INTO checkpoints (
    thread_id, checkpoint_id, parent_checkpoint_id,
    checkpoint_ns, checkpoint_ns_hash, gmt_create, gmt_modified
) VALUES (
    'thread_001',                              -- 会话ID
    'ckpt_005',                                -- 新生成的快照ID
    'ckpt_004',                                -- 指向上一个 checkpoint（构建链条）
    '',                                        -- 命名空间（主图用空字符串）
    'd41d8cd98f00b204e9800998ecf8427e',        -- MD5('')
    '2026-04-06 10:00:00',
    '2026-04-06 10:00:00'
);
```

### 写入结果

**checkpoint_writes 表** — 3 条（每个 task 一条）：

| thread_id  | checkpoint_id | task_id           | idx | channel      | blob_data |
|------------|---------------|-------------------|-----|--------------|-----------|
| thread_001 | ckpt_005      | retrieve_context  | 0   | user_context | \<binary\> |
| thread_001 | ckpt_005      | search_products   | 0   | messages     | \<binary\> |
| thread_001 | ckpt_005      | generate_response | 0   | messages     | \<binary\> |

**checkpoint_blobs 表** — 2 条（合并后只剩 2 个 channel）：

| thread_id  | checkpoint_ns_hash | channel      | version | blob_data |
|------------|--------------------|--------------|---------|-----------|
| thread_001 | d41d8cd...         | user_context | 0       | \<binary\> |
| thread_001 | d41d8cd...         | messages     | 0       | \<binary: 合并后的完整列表\> |

**checkpoints 表** — 1 条：

| thread_id  | checkpoint_id | parent_checkpoint_id | checkpoint_ns_hash | gmt_modified        |
|------------|---------------|---------------------|--------------------|---------------------|
| thread_001 | ckpt_005      | ckpt_004            | d41d8cd...         | 2026-04-06 10:00:00 |

---

## ② 更新：创建新 Checkpoint

**场景**：用户继续对话，State 发生变化。

```python
# 旧 State (ckpt_005)
old_state = {
    "messages": [msg1, msg2],
    "files": {"file_001": {...}},
    "user_context": {"age": 30, ...}
}

# 用户发送新消息，Agent 处理后——

# 新 State (ckpt_006)
new_state = {
    "messages": [msg1, msg2, msg3, msg4],  # ⬅️ 新增了 2 条消息
    "files": {"file_001": {...}},          # ⬅️ 未变化
    "user_context": {"age": 30, ...}       # ⬅️ 未变化
}
```

### 写入流程

与 ① 相同，只是创建了一个新的 checkpoint：

1. **checkpoint_writes**：记录本次执行中每个 node 的写入
2. **checkpoint_blobs**：合并后的最终状态

### 存储策略

#### 方式 A：完整存储（简单但浪费空间）

```sql
INSERT INTO checkpoints VALUES (
    'thread_001', 'ckpt_006', 'ckpt_005', '', <新hash>, NOW(), NOW()
);

-- 存储所有 channel（即使未变化也重复存储）
INSERT INTO checkpoint_blobs VALUES
    ('thread_001', <hash>, 'messages',     0, 'msgpack', <新的messages>),
    ('thread_001', <hash>, 'files',        0, 'msgpack', <files重复存储>),
    ('thread_001', <hash>, 'user_context', 0, 'msgpack', <user_context重复存储>);
```

#### 方式 B：增量存储（优化，读取时从 parent 继承未变 channel）

```sql
-- 只存储变化的 channel
INSERT INTO checkpoint_blobs VALUES
    ('thread_001', <hash>, 'messages', 0, 'msgpack', <只存新增的 msg3, msg4>);

-- files 和 user_context 未变化，不存储（读取时从 parent checkpoint 继承）
```

| 对比 | checkpoints 表 | checkpoint_blobs 表 |
|------|---------------|-------------------|
| 方式 A | 1 条 | N 条（所有 channel） |
| 方式 B | 1 条 | 仅变化的 channel |

> 无论哪种方式，`checkpoint_writes` 始终记录每个 node 的实际写入操作。

---

## ③ 回滚：从 ckpt_006 回到 ckpt_005

### 核心原则

- ❌ **不删除**旧 checkpoint
- ✅ **创建新 checkpoint**（分支），`parent` 指向回滚目标

### 用户操作

```python
graph.update_state(
    thread_id="thread_001",
    checkpoint_id="ckpt_005"   # 指定回滚目标
)
```

### 数据库操作

```sql
-- 1. 创建新 checkpoint，parent 指向回滚目标 ckpt_005
INSERT INTO checkpoints VALUES (
    'thread_001', 'ckpt_007', 'ckpt_005', '', <新hash>, NOW(), NOW()
);

-- 2. 复制 ckpt_005 的所有 blob 数据到 ckpt_007
INSERT INTO checkpoint_blobs
SELECT
    thread_id,
    <ckpt_007的hash>,   -- 新的 checkpoint_ns_hash
    channel,
    version,
    type,
    blob_data            -- 直接复制旧数据
FROM checkpoint_blobs
WHERE thread_id = 'thread_001'
    AND checkpoint_ns_hash = <ckpt_005的hash>;
```

### 结果：形成分支树

```
ckpt_004 → ckpt_005 ┬→ ckpt_006  (旧分支，保留不删)
                     └→ ckpt_007  (新分支，从此继续)
```

对应的 checkpoints 表：

| checkpoint_id | parent_checkpoint_id | gmt_modified |
|---------------|---------------------|--------------|
| ckpt_004      | ckpt_003            | 10:00:00     |
| ckpt_005      | ckpt_004            | 10:00:30 ← 回滚目标 |
| ckpt_006      | ckpt_005            | 10:01:00 ← 旧分支（保留） |
| ckpt_007      | ckpt_005            | 10:02:00 ← 新分支 |

---

## ④ 读取：Database → State

**场景**：加载某个 checkpoint（如 `ckpt_007`）到内存。

### 常规读取（通过 checkpoint_blobs）

#### 步骤 1：查询 checkpoint 元数据

```sql
SELECT checkpoint_id, checkpoint_ns_hash, parent_checkpoint_id
FROM checkpoints
WHERE thread_id = 'thread_001'
    AND checkpoint_id = 'ckpt_007'
    AND checkpoint_ns = '';
```

#### 步骤 2：查询所有 blob 数据

```sql
SELECT channel, version, type, blob_data
FROM checkpoint_blobs
WHERE thread_id = 'thread_001'
    AND checkpoint_ns_hash = 'abc123...'   -- ckpt_007 的 hash
ORDER BY channel, version;
```

#### 步骤 3：反序列化并合并

```python
from langgraph.checkpoint.serde.jsonplus import JsonPlusSerializer

serializer = JsonPlusSerializer()
state = {}

for row in blob_rows:
    channel = row['channel']
    value = serializer.loads_typed((row['type'], row['blob_data']))

    # 同一 channel 有多个 version 时需要合并
    if channel in state:
        state[channel] = merge(state[channel], value)
    else:
        state[channel] = value
```

#### 步骤 4：多 version 合并逻辑

当同一 channel 存在多个 version 时，按类型合并：

```python
# dict → 合并
state["files"] = {**files_v0, **files_v1}

# list → 拼接
state["messages"] = messages_v0 + messages_v1
```

#### 最终恢复的 State

```python
state = {
    "messages": [msg1, msg2],           # 从 blob 反序列化
    "files": {"file_001": {...}},       # 从 blob 反序列化
    "user_context": {"age": 30, ...}    # 从 blob 反序列化
}
```

### 调试/审计读取（通过 checkpoint_writes）

当需要分析执行过程时，查询 `checkpoint_writes`：

```sql
SELECT
    task_id, idx, channel, task_path,
    LENGTH(blob_data) as data_size, gmt_create
FROM checkpoint_writes
WHERE thread_id = 'thread_001'
    AND checkpoint_id = 'ckpt_005'
    AND checkpoint_ns = ''
ORDER BY gmt_create, idx;
```

输出示例：

| task_id           | idx | channel      | task_path | data_size | gmt_create     |
|-------------------|-----|--------------|-----------|-----------|----------------|
| retrieve_context  | 0   | user_context | ()        | 512       | 10:00:00.123   |
| search_products   | 0   | messages     | ()        | 2048      | 10:00:01.456   |
| generate_response | 0   | messages     | ()        | 1024      | 10:00:02.789   |

可以清晰看到：执行顺序、每个 node 写了什么、数据量多大。

---

## checkpoint_writes 的进阶用途

### 1. 故障恢复：从 writes 重建 blobs

如果 `checkpoint_blobs` 写入失败，可以从 `checkpoint_writes` 重放：

```python
writes = get_checkpoint_writes(thread_id, checkpoint_id)

state = {}
for write in writes:
    channel = write['channel']
    data = deserialize(write['blob_data'])

    if channel in state:
        state[channel] = merge(state[channel], data)
    else:
        state[channel] = data

# 重新创建 checkpoint_blobs
save_checkpoint_blobs(thread_id, checkpoint_id, state)
```

### 2. Agent 行为分析

```sql
SELECT
    task_id,
    COUNT(*) as write_count,
    SUM(LENGTH(blob_data)) / 1024 / 1024 as total_mb,
    AVG(LENGTH(blob_data)) as avg_bytes
FROM checkpoint_writes
WHERE thread_id LIKE 'thread_%'
    AND gmt_create >= '2026-04-01'
    AND checkpoint_ns = ''
GROUP BY task_id
ORDER BY write_count DESC;
```

### 3. 对比 writes vs blobs 数据量

```sql
-- writes 总量（所有 task 的写入）
SELECT SUM(LENGTH(blob_data)) / 1024 as writes_kb
FROM checkpoint_writes
WHERE thread_id = 'thread_001' AND checkpoint_id = 'ckpt_005';

-- blobs 总量（合并后的最终状态）
SELECT SUM(LENGTH(blob_data)) / 1024 as blobs_kb
FROM checkpoint_blobs
WHERE thread_id = 'thread_001'
    AND checkpoint_ns_hash = (
        SELECT checkpoint_ns_hash FROM checkpoints WHERE checkpoint_id = 'ckpt_005'
    );
```

如果 `writes_kb > blobs_kb`：说明有多个 task 写同一个 channel（合并后体积缩小）。

### task_id 与 task_path

```python
# task_id = node 名称
graph.add_node("retrieve_context", retrieve_func)
# → task_id = "retrieve_context"

# task_path 用于嵌套子图
task_path = "()"                              # 主图中的 node
task_path = "('subgraph_name',)"              # 子图中的 node
task_path = "('subgraph_name', 'nested',)"    # 嵌套子图
```

---

## 速查总结

### 四阶段操作对照

| 操作 | checkpoint_writes | checkpoint_blobs | checkpoints | State 内存 |
|------|------------------|-----------------|-------------|-----------|
| ① 写入 | INSERT（每 node 一条） | INSERT（每 channel 一条） | INSERT 1 条 | 序列化 |
| ② 更新 | INSERT（新的 node 写入） | INSERT（完整或增量） | INSERT 1 条 | 新 State |
| ③ 回滚 | — | 复制旧 blob | INSERT 1 条（parent → 目标） | — |
| ④ 读取 | SELECT（调试/审计） | SELECT（常规读取） | SELECT（查 hash） | 反序列化 + 合并 |

### 三张表速查

**checkpoints 表**

```
thread_id               ← 会话ID
checkpoint_id           ← 快照ID（唯一标识）
parent_checkpoint_id    ← 父快照ID（构建链条/分支）
checkpoint_ns           ← 命名空间（主图=''，子图='task_name'）
checkpoint_ns_hash      ← MD5(checkpoint_ns)，关联其他两表 ⭐
gmt_create / gmt_modified ← 时间戳
```

**checkpoint_writes 表**

```
thread_id               ← 会话ID
checkpoint_id           ← 所属 checkpoint
checkpoint_ns_hash      ← 关联 checkpoints 表 ⭐
task_id                 ← node 名称（如 "retrieve_context"）
idx                     ← 同一 task 内的写入序号
channel                 ← State 字段名（messages / files / ...）
type                    ← 序列化类型（msgpack）
blob_data               ← 序列化后的部分更新数据
task_path               ← 子图路径（主图="()"）
```

**checkpoint_blobs 表**

```
thread_id               ← 会话ID
checkpoint_ns_hash      ← 关联 checkpoints 表 ⭐
channel                 ← State 字段名（messages / files / user_context）
version                 ← 同一 checkpoint、同一 channel 的版本号
type                    ← 序列化类型（msgpack / json / ...）
blob_data               ← 序列化后的二进制数据（合并后的完整状态）
```

### 序列化与反序列化

```python
from langgraph.checkpoint.serde.jsonplus import JsonPlusSerializer
serializer = JsonPlusSerializer()

# 写入时：State 字段 → blob_data
serialized = serializer.dumps_typed(state_field)
# → ('msgpack', b'\x82\xa3age\x1e...')

# 读取时：blob_data → State 字段
state_field = serializer.loads_typed(('msgpack', binary_data))
# → {"age": 30, "name": "张三"}

---

## 完整字段级数据流图示例

以**保险咨询会话**为例，展示三张表的实际字段数据流转。

### 场景设定

```
thread_id = "thread_abc123" (用户咨询医疗险的完整会话)
```

**会话流程**：

```
checkpoint_1 (用户说"你好") → checkpoint_2 (Agent回复) → 
checkpoint_3 (用户说"推荐医疗险") → checkpoint_4 (Agent推荐产品)
```

---

### Step 1: 用户说"你好"

**执行节点**：`handle_greeting` → `generate_response`

#### checkpoints 表

| thread_id | checkpoint_id | parent_checkpoint_id | checkpoint_ns | checkpoint_ns_hash | gmt_create |
|-----------|---------------|-------------------|---------------|-------------------|------------|
| thread_abc123 | ckpt_001 | NULL | | d41d8cd98f00b204e9800998ecf8427e | 10:00:00 |
| thread_abc123 | ckpt_002 | ckpt_001 | | d41d8cd98f00b204e9800998ecf8427e | 10:00:02 |

#### checkpoint_writes 表（ckpt_002 的写入）

| thread_id | checkpoint_id | checkpoint_ns_hash | task_id | idx | channel | type | blob_data (反序列化后) |
|-----------|---------------|-------------------|---------|-----|---------|------|----------------------|
| thread_abc123 | ckpt_002 | d41d8cd... | handle_greeting | 0 | messages | msgpack | `[{"role":"human","content":"你好"}]` |
| thread_abc123 | ckpt_002 | d41d8cd... | generate_response | 0 | messages | msgpack | `[{"role":"ai","content":"您好！我是您的保险顾问"}]` |

#### checkpoint_blobs 表（ckpt_002 最终状态）

| thread_id | checkpoint_ns_hash | channel | version | type | blob_data (反序列化后) |
|-----------|-------------------|---------|---------|------|----------------------|
| thread_abc123 | d41d8cd... | messages | 0 | msgpack | `[HumanMessage("你好"), AIMessage("您好！我是您的保险顾问")]` |
| thread_abc123 | d41d8cd... | user_context | 0 | msgpack | `{"session_start":"2026-04-07"}` |

---

### Step 2: 用户说"推荐医疗险"

**执行节点**：`retrieve_context` → `search_products` → `generate_response`

**业务逻辑**：
1. `retrieve_context`: 解析用户意图（医疗险）、提取年龄信息
2. `search_products`: 搜索数据库中的医疗险产品
3. `generate_response`: 生成产品推荐回复

#### checkpoints 表新增记录

| thread_id | checkpoint_id | parent_checkpoint_id | checkpoint_ns_hash | gmt_create |
|-----------|---------------|-------------------|-------------------|------------|
| thread_abc123 | ckpt_003 | ckpt_002 | d41d8cd... | 10:00:30 |

#### checkpoint_writes 表（ckpt_003 的写入 - 3 个 node 执行）

| thread_id | checkpoint_id | task_id | idx | channel | blob_data (反序列化后) |
|-----------|---------------|---------|-----|---------|----------------------|
| thread_abc123 | ckpt_003 | retrieve_context | 0 | user_context | `{"intent":"medical_insurance","age":28,"budget":5000}` |
| thread_abc123 | ckpt_003 | retrieve_context | 1 | extracted_entities | `["医疗险","28岁"]` |
| thread_abc123 | ckpt_003 | search_products | 0 | search_results | `[{"product_id":"M001","name":"百万医疗","price":399}]` |
| thread_abc123 | ckpt_003 | search_products | 1 | tool_calls | `[{"name":"search_db","args":{"category":"medical"}}]` |
| thread_abc123 | ckpt_003 | generate_response | 0 | messages | `[AIMessage("为您推荐百万医疗险...")]` |

> **注意**：`retrieve_context` 写入了 2 条（idx 0 和 1），`search_products` 也写了 2 条。

#### checkpoint_blobs 表（ckpt_003 合并后状态）

| thread_id | checkpoint_ns_hash | channel | version | blob_data (反序列化后) |
|-----------|-------------------|---------|---------|----------------------|
| thread_abc123 | d41d8cd... | messages | 0 | 合并后的 3 条消息列表 |
| thread_abc123 | d41d8cd... | user_context | 0 | `{"intent":"medical_insurance","age":28,"budget":5000}` |
| thread_abc123 | d41d8cd... | search_results | 0 | `[{"product_id":"M001","name":"百万医疗"}]` |

---

### Step 3: 数据流图可视化

```
thread_abc123
│
├── ckpt_001 (初始空状态)
│   └── writes: 无
│   └── blobs: 空
│
├── ckpt_002 (问候完成)
│   ├── writes:
│   │   ├── task: handle_greeting → channel: messages ("你好")
│   │   └── task: generate_response → channel: messages ("您好！...")
│   │
│   └── blobs:
│       ├── channel: messages (合并后 2 条消息)
│       └── channel: user_context (基础会话信息)
│
├── ckpt_003 (推荐请求处理中)
│   ├── writes:
│   │   ├── task: retrieve_context → channel: user_context (意图+年龄)
│   │   ├── task: retrieve_context → channel: extracted_entities (实体列表)
│   │   ├── task: search_products → channel: search_results (产品数据)
│   │   ├── task: search_products → channel: tool_calls (工具调用记录)
│   │   └── task: generate_response → channel: messages (推荐回复)
│   │
│   └── blobs:
│       ├── channel: messages (合并后 4 条消息)
│       ├── channel: user_context (更新后的用户信息)
│       └── channel: search_results (产品列表)
│
└── ckpt_004 (最终回复完成)
    ├── writes:
    │   └── task: finalize_response → channel: messages (格式化回复)
    │
    └── blobs:
        ├── channel: messages (完整对话历史)
        ├── channel: user_context (完整用户画像)
        └── channel: search_results (推荐产品快照)
```

---

### Step 4: 从 writes 到 blobs 的合并过程详解

以 **ckpt_003** 为例，展示 `messages` channel 是如何合并的：

```
checkpoint_writes 中的原始写入 (按时间序):
├── ckpt_002: generate_response 写入 → [AIMessage("您好！")]
├── ckpt_003: retrieve_context 未写 messages
├── ckpt_003: search_products 未写 messages
└── ckpt_003: generate_response 写入 → [AIMessage("为您推荐...")]

合并算法 (列表类型 = 拼接):
messages = messages_ckpt_002 + messages_ckpt_003_generate
         = [Human("你好"), AI("您好！")] + [AI("为您推荐...")]
         = [Human("你好"), AI("您好！"), AI("为您推荐...")]

最终存入 checkpoint_blobs:
└── messages: [3条消息的完整列表]
```

---

### Step 5: 回滚场景示例

**用户操作**："我刚才说错了，我不想看病险了"

```python
# 回滚到 ckpt_002（问候完成状态，还没推荐产品）
graph.update_state(
    thread_id="thread_abc123",
    checkpoint_id="ckpt_002"
)
```

#### 回滚后的 checkpoints 表（形成分支）

| checkpoint_id | parent_checkpoint_id | 说明 |
|---------------|-------------------|------|
| ckpt_001 | NULL | 初始 |
| ckpt_002 | ckpt_001 | 问候完成 ← 回滚目标 |
| ckpt_003 | ckpt_002 | 旧分支（含推荐） |
| ckpt_004 | ckpt_002 | 新分支（从此继续） |

```
                    ┌→ ckpt_003 → (废弃分支，保留)
ckpt_001 → ckpt_002 ┤
                    └→ ckpt_004 → (新对话从此开始)
```

#### ckpt_004 的 checkpoint_blobs

从 ckpt_002 **复制**所有 blob 数据：

| thread_id | checkpoint_id | channel | blob_data 来源 |
|-----------|---------------|---------|---------------|
| thread_abc123 | ckpt_004 | messages | 复制自 ckpt_002 |
| thread_abc123 | ckpt_004 | user_context | 复制自 ckpt_002 |

> ckpt_003 的 `search_results` 和推荐相关的 `messages` 不会出现在 ckpt_004 中。

---

### Step 6: SQL 查询示例

**查询某 checkpoint 的完整执行轨迹**：

```sql
-- 查看 ckpt_003 中每个 node 的贡献
SELECT 
    task_id,
    channel,
    LENGTH(blob_data) as bytes,
    gmt_create
FROM checkpoint_writes
WHERE thread_id = 'thread_abc123'
    AND checkpoint_id = 'ckpt_003'
ORDER BY gmt_create;

-- 输出:
-- task_id           | channel            | bytes | gmt_create
-- retrieve_context  | user_context       | 256   | 10:00:30.100
-- retrieve_context  | extracted_entities | 128   | 10:00:30.200
-- search_products   | search_results     | 2048  | 10:00:31.500
-- search_products   | tool_calls         | 512   | 10:00:31.600
-- generate_response | messages           | 1024  | 10:00:32.000
```

**对比 writes 和 blobs 的数据量**：

```sql
-- ckpt_003 的 writes 总量
SELECT SUM(LENGTH(blob_data)) as writes_total_bytes
FROM checkpoint_writes
WHERE thread_id = 'thread_abc123' AND checkpoint_id = 'ckpt_003';
-- 结果: 3968 bytes (5条写入)

-- ckpt_003 的 blobs 总量
SELECT SUM(LENGTH(blob_data)) as blobs_total_bytes
FROM checkpoint_blobs
WHERE thread_id = 'thread_abc123' 
    AND checkpoint_ns_hash = 'd41d8cd...';
-- 结果: 3072 bytes (3条合并后)

-- 压缩率: (3968-3072)/3968 = 22.6% 的冗余被合并消除
```

---

### 字段级速查卡片

**checkpoints 表 —— 时间线/分支索引**

```
┌─────────────────┬─────────────────────────────────────────┐
│ thread_id       │ "thread_abc123"                         │
│ checkpoint_id   │ "ckpt_003"                              │
│ parent_id       │ "ckpt_002" ← 指向前一个                 │
│ checkpoint_ns   │ "" (主图) / "subgraph" (子图)          │
│ ns_hash         │ "d41d8cd..." ← 关联 writes 和 blobs     │
│ gmt_create      │ 2026-04-07 10:00:30                     │
└─────────────────┴─────────────────────────────────────────┘
```

**checkpoint_writes 表 —— 执行轨迹（调试/审计）**

```
┌─────────────────┬─────────────────────────────────────────┐
│ thread_id       │ "thread_abc123"                         │
│ checkpoint_id   │ "ckpt_003"                              │
│ task_id         │ "search_products" ← node 名称           │
│ idx             │ 0 / 1 / 2... ← 同一 task 的第 N 次写入  │
│ channel         │ "search_results" ← State 字段名          │
│ blob_data       │ <二进制: 产品列表数据>                   │
│ task_path       │ "()" / "('subgraph',)" ← 子图路径       │
└─────────────────┴─────────────────────────────────────────┘
```

**checkpoint_blobs 表 —— 最终状态（常规读取）**

```
┌─────────────────┬─────────────────────────────────────────┐
│ thread_id       │ "thread_abc123"                         │
│ ns_hash         │ "d41d8cd..." ← 来自 checkpoints 表    │
│ channel         │ "messages" ← State 字段名                │
│ version         │ 0 / 1 / 2... ← 同一 checkpoint 的版本    │
│ blob_data       │ <二进制: 合并后的完整消息列表>            │
└─────────────────┴─────────────────────────────────────────┘

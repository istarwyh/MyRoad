## 概述

本方案旨在通过虚拟机为每个用户提供独立且完整的 Claude Code 开发环境，支持用户在浏览器中实时与 Claude Code 进行交互。方案遵循 SOLID 原则，确保系统的可维护性、可扩展性和稳定性。

### 核心目标

1. **功能完整性**：确保 Claude Code 的所有功能在云端环境中都能正常使用
2. **用户体验**：提供流畅的实时交互体验，最小化网络延迟影响
3. **安全隔离**：每个用户拥有独立的计算环境，数据完全隔离
4. **可扩展性**：支持从单用户到大规模用户的平滑扩展
5. **成本可控**：通过资源池化和智能调度控制运营成本

---

## 系统架构设计

### 整体架构图

```mermaid
graph TB
    subgraph "客户端层"
        Browser[Web浏览器]
        Terminal[终端组件]
        Editor[编辑器组件]
        FileTree[文件树组件]
    end

    subgraph "网关层"
        LB[负载均衡器]
        Gateway[WebSocket网关]
        Auth[认证服务]
    end

    subgraph "服务层"
        SessionMgr[会话管理器]
        VMProxy[VM代理服务]
        FileSync[文件同步服务]
    end

    subgraph "计算层"
        VMPool[VM资源池]
        VM[用户VM实例]
        Claude[Claude Code CLI]
    end

    subgraph "存储层"
        UserData[用户数据存储]
        Logs[日志存储]
        Metrics[监控数据]
    end

    Browser --> LB
    LB --> Gateway
    Gateway --> Auth
    Gateway --> SessionMgr
    SessionMgr --> VMProxy
    VMProxy --> VM
    VM --> Claude
    VM --> UserData
    FileSync --> UserData
    VMProxy --> Logs
    SessionMgr --> Metrics
```

### 核心组件职责（单一职责原则）

1. **WebSocket 网关**

   - 职责：管理 WebSocket 连接，处理消息路由
   - 不负责：业务逻辑处理、VM 管理

2. **会话管理器**

   - 职责：维护用户会话状态，管理会话生命周期
   - 不负责：VM 操作、文件处理

3. **VM 代理服务**

   - 职责：与 VM 通信，执行命令，收集输出
   - 不负责：会话管理、认证授权

4. **文件同步服务**
   - 职责：监控文件变化，同步文件状态
   - 不负责：命令执行、VM 管理

---

## 流程设计

### 1. 用户端

- **Web 浏览器**
  - 终端界面：用于实时输入和查看 CLI 输出
  - 代码编辑器：支持实时同步和代码高亮
  - 文件树：展示项目结构和文件变更

### 2. 云服务器

- **WebSocket 网关**

  - 连接管理：管理用户连接，维护会话状态
  - 消息路由：将用户指令传递给对应的虚拟机

- **虚拟机环境**
  - 独立 VM：每个用户分配一个虚拟机，防止资源争夺
  - 内置 Claude Code：支持所有功能，无权限限制
  - 完整文件系统：持久化用户数据，支持高效读写

---

## 交互流程

### 1. 初始化连接

1. 用户通过浏览器访问 Web 页面，页面加载 WebSocket 客户端。
2. WebSocket 客户端请求建立连接，并通过 JWT 进行用户认证。
3. 服务器创建或唤醒用户专属的虚拟机。
4. Claude Code 在虚拟机中启动，准备就绪。

### 2. 命令执行流程

1. 用户在终端界面输入命令。
2. 命令通过 WebSocket 传至云服务器。
3. 云服务器根据任务类型选择执行模式：
   - **常驻进程模式**：用于需要保持上下文的长会话任务
     - 通过 IPC 或 Unix Domain Socket 通信
     - 保持会话状态和历史记录
     - 支持连续对话和上下文引用
   - **子进程模式**：用于独立的一次性任务
     - 直接创建子进程执行 `claude` 命令
     - 任务完成后立即释放资源
     - 提供更好的隔离性和安全性
4. Claude Code 执行命令，实时输出结果。
5. 执行结果通过 WebSocket 实时返回用户浏览器。

### 3. 文件操作

1. Claude Code 可以在虚拟机中创建、修改、删除文件。
2. 每次文件更改都会触发事件通知给 WebSocket 网关。
3. WebSocket 网关将文件变更通知同步到浏览器。
4. 浏览器实时更新文件树与代码编辑器。

---

## 详细时序图

### 用户连接初始化时序

```mermaid
sequenceDiagram
    participant U as 用户浏览器
    participant GW as WebSocket网关
    participant SM as 会话管理器
    participant VP as VM代理服务
    participant C as Claude Code

    U->>GW: 建立WebSocket连接
    GW->>SM: 创建用户会话
    SM->>VP: 请求Claude实例

    alt Claude未运行
        VP->>C: 启动Claude进程
        C->>VP: 进程就绪
    else Claude已运行
        VP->>C: 检查进程状态
        C->>VP: 状态正常
    end

    VP->>SM: Claude实例就绪
    SM->>GW: 会话建立成功
    GW->>U: 连接成功，终端就绪
```

### 命令执行时序（混合模式）

#### 常驻进程模式（会话任务）

```mermaid
sequenceDiagram
    participant U as 用户浏览器
    participant GW as WebSocket网关
    participant VP as VM代理服务
    participant CD as Claude常驻进程
    participant CTX as 上下文管理器

    U->>GW: 发送会话命令
    GW->>VP: 转发命令+会话ID
    VP->>CTX: 检查会话上下文
    CTX->>VP: 返回会话历史
    VP->>CD: 发送命令+上下文

    loop 流式输出
        CD->>VP: 输出内容
        VP->>CTX: 更新上下文
        VP->>GW: 转发输出
        GW->>U: 实时显示
    end

    CD->>VP: 命令完成
    VP->>GW: 会话更新完成
    GW->>U: 显示结果
```

#### 子进程模式（独立任务）

```mermaid
sequenceDiagram
    participant U as 用户浏览器
    participant GW as WebSocket网关
    participant VP as VM代理服务
    participant C as Claude子进程
    participant FS as 文件系统

    U->>GW: 发送独立命令
    GW->>VP: 转发命令
    VP->>C: spawn('claude', [args])

    loop 流式输出
        C->>FS: 读写文件
        C->>VP: stdout/stderr输出
        VP->>GW: 转发输出
        GW->>U: 实时显示
    end

    C->>VP: 进程退出(exit code)
    VP->>GW: 命令完成
    GW->>U: 执行完成
```

### 文件变更通知流程

```mermaid
flowchart LR
    subgraph "文件监控"
        W[文件监控器]
        E[变更事件]
        Q[事件队列]
    end

    subgraph "通知处理"
        P[事件处理器]
        F[文件过滤器]
        S[状态同步]
    end

    subgraph "客户端更新"
        WS[WebSocket推送]
        UI[UI更新]
    end

    W --> E
    E --> Q
    Q --> P
    P --> F
    F --> S
    S --> WS
    WS --> UI
```

---

## 流式通信设计

### WebSocket 消息协议

#### 消息结构定义

```json
{
  "id": "消息唯一标识",
  "type": "消息类型",
  "timestamp": "时间戳",
  "data": "消息载荷"
}
```

#### 消息类型

1. **控制消息**

   - `connection.init`: 初始化连接
   - `connection.ready`: 连接就绪
   - `connection.close`: 关闭连接

2. **命令消息**

   - `command.execute`: 执行命令
   - `command.cancel`: 取消执行
   - `command.status`: 命令状态

3. **输出消息**

   - `output.stdout`: 标准输出
   - `output.stderr`: 错误输出
   - `output.complete`: 输出完成

4. **文件消息**
   - `file.changed`: 文件变更
   - `file.created`: 文件创建
   - `file.deleted`: 文件删除
   - `file.renamed`: 文件重命名

### 流式数据处理

```mermaid
graph LR
    subgraph "数据源"
        C[Claude输出]
        F[文件事件]
        S[系统事件]
    end

    subgraph "缓冲层"
        B[环形缓冲区]
        T[节流控制]
        D[去重处理]
    end

    subgraph "传输层"
        E[编码压缩]
        W[WebSocket帧]
        R[重传机制]
    end

    C --> B
    F --> B
    S --> B
    B --> T
    T --> D
    D --> E
    E --> W
    W --> R
```

### 背压处理机制

当客户端处理速度跟不上服务端推送速度时的处理策略：

1. **缓冲策略**

   - 服务端维护固定大小的环形缓冲区
   - 超出缓冲区容量时丢弃最旧的数据
   - 发送缓冲区溢出通知给客户端

2. **流控策略**

   - 客户端发送 ACK 确认已处理的消息
   - 服务端根据 ACK 调整发送速率
   - 实现滑动窗口机制

3. **优先级策略**
   - 错误消息优先级最高
   - 文件变更次之
   - 普通输出最低

---

## 执行模式选择策略

### 模式决策流程

```mermaid
flowchart TD
    Start[接收用户命令] --> Analyze[分析命令类型]

    Analyze --> IsSession{需要会话上下文?}

    IsSession -->|是| CheckDaemon{常驻进程可用?}
    IsSession -->|否| UseSubprocess[使用子进程模式]

    CheckDaemon -->|是| UseDaemon[使用常驻进程]
    CheckDaemon -->|否| StartDaemon[启动常驻进程]

    StartDaemon --> UseDaemon

    UseDaemon --> Execute1[执行命令]
    UseSubprocess --> Execute2[执行命令]

    Execute1 --> UpdateContext[更新会话上下文]
    Execute2 --> ReleaseResource[释放资源]

    UpdateContext --> Done[完成]
    ReleaseResource --> Done
```

### 模式特性对比

| 特性           | 常驻进程模式       | 子进程模式         |
| -------------- | ------------------ | ------------------ |
| **启动开销**   | 低（仅首次）       | 高（每次执行）     |
| **内存占用**   | 持续占用           | 按需使用           |
| **上下文保持** | ✓ 支持             | ✗ 不支持           |
| **并发处理**   | 队列化处理         | 天然并发           |
| **故障隔离**   | 需要监控重启       | 自然隔离           |
| **适用场景**   | 连续对话、项目开发 | 单次查询、独立任务 |

### 上下文管理设计

```mermaid
graph TB
    subgraph "会话上下文结构"
        Session[会话元数据]
        History[命令历史]
        Files[文件变更记录]
        Variables[环境变量]
        State[Claude状态]
    end

    subgraph "持久化策略"
        Memory[内存缓存]
        Disk[磁盘存储]
        Snapshot[定期快照]
    end

    Session --> Memory
    History --> Memory
    Files --> Disk
    Variables --> Memory
    State --> Snapshot

    Memory --> Disk
    Disk --> Snapshot
```

### 常驻进程生命周期管理

```mermaid
stateDiagram-v2
    [*] --> Idle: 初始化
    Idle --> Active: 接收任务
    Active --> Processing: 处理命令
    Processing --> Active: 完成任务
    Active --> Idle: 空闲超时

    Idle --> Hibernating: 长时间空闲
    Hibernating --> Active: 唤醒

    Active --> Error: 发生错误
    Error --> Restarting: 自动重启
    Restarting --> Idle: 重启成功

    Hibernating --> [*]: 资源回收
    Error --> [*]: 无法恢复
```

---

## 技术实现细节

### 技术栈

- **前端**: HTML, CSS, JavaScript, WebSocket, xterm.js, Monaco Editor
- **后端**: Node.js, Express, WebSocket, Docker
- **虚拟机管理**: 通过云提供商 API 动态管理虚拟机生命周期

### 安全与隔离

- 用户间环境完全隔离，保证数据安全
- 各 VM 独立的网络、存储和计算资源
- 使用防火墙规则和权限控制加强安全性

### 性能与扩展性

- 支持多个区域的部署以减少网络延迟
- 自动扩缩 VM 以应对变化的用户需求
- 实施负载均衡来优化资源利用率

---

## 结论

该技术方案通过为每个用户提供独立的虚拟机环境，使得 Claude Code 的强大能力得以充分发挥，同时确保了安全与性能。这种策略不仅提升了用户体验，还支持了大规模的部署和灵活的扩展。

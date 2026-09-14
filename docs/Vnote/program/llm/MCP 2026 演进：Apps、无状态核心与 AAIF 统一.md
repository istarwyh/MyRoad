---
title: MCP 2026 演进：Apps、无状态核心与 AAIF 统一
---

## 引言

在 [[从指令到意图：AI Agent 架构范式演进史]] 的结尾，我把"MCP + A2A = Agent 时代的 TCP/IP"当作一个**预言**写在结论里。彼时 A2A 还只是 Google 仓库里的早期草案，MCP 也只是个"工具调用协议"。

2026 年的这大半年，预言兑现的速度比我预想的快得多——而且它兑现的方式，不是某个协议一家独大，而是**三条主线同时在推进，最后在一处交汇**：

1. **MCP Apps（2026-01-26 稳定）**：MCP 从"纯文本工具调用"长出"可交互界面"的能力，服务端直接回传沙盒化的 UI。
2. **无状态核心 Stateless Core（2026-07-28 RC）**：MCP 核心协议抽空状态，变成可跨实例持久化、可水平负载均衡的基础设施。
3. **AAIF 统一（2026-08）**：Google 把 A2A 交给 AAIF，与 MCP 同处一个治理框架，形成"MCP 管纵向连接、A2A 管横向协作"的分层协议栈。

这三件事，分别回答了 Agent 生态的三个老问题：**界面从哪来、状态放哪里、协议谁说了算**。本文按这三条主线展开。

---

## 一、MCP Apps：让工具长出界面

### 1.1 为什么需要"界面"

MCP 最初的抽象里，工具是**纯文本的**：Agent 发一个 JSON-RPC 请求，拿到一段文本结果，再喂回上下文。这个抽象干净、通用，但有个盲区——**不是所有交互都适合用文字表达**。

一个要选日期、选座位、拖拽参数的工具，让用户用自然语言描述"我要 14:30 的靠窗位"，既低效又易错。文本协议擅长的是"表达"，不擅长"操作"。

这个问题我其实在蚂蚁集团时就在摸（见 [[如何让 AI Agent 实时个性化可视交互]]）：让 Agent 实时生成前端组件做可视化交互。但那时是**每家自己造轮子**——我在业务里自己搭一套，别家又搭另一套，互不相通。

MCP Apps 的价值，是把这件事**协议化**了。

### 1.2 它是怎么工作的

MCP Apps 是 MCP 的第一个官方扩展（标识 `io.modelcontextprotocol/ui`，规范 SEP-1865，2026-01-26 稳定）。核心思路一句话：**工具声明一个 UI 模板，宿主把它渲染在沙盒 iframe 里**。

具体机制分两步：

**第一步，注册**。工具的 `_meta.ui.resourceUri` 指向一个 `ui://` 前缀的资源，MIME 类型固定为 `text/html;profile=mcp-app`。宿主通过 `resources/read` 拉取这段 HTML，渲染在 iframe 里。

```jsonc
// tools/list 里，一个带 UI 的工具
{
  "name": "book_flight",
  "_meta": {
    "ui": {
      "resourceUri": "ui://travel-server/flight-booking"
    }
  },
  "visibility": ["model", "app"]
}
```

`visibility` 是个精巧的设计：标记为 `["app"]` 的工具**对模型隐藏**，但 iframe 里的 View 可以调用（比如"刷新"按钮）；反过来 `["model"]` 的工具 View 不能碰。这划清了"给模型看的能力"和"给界面看的能力"。

**第二步，握手**。iframe（View）扮演 MCP client，宿主扮演 MCP server，两者走 JSON-RPC 2.0 over `postMessage`：

```
View → Host: ui/initialize（声明 appCapabilities）
Host → View: McpUiInitializeResult（hostCapabilities + hostContext）
View → Host: ui/notifications/initialized
Host → View: ui/notifications/tool-input / tool-result
```

初始化后，View 可以反向调用 `tools/call`、`resources/read`，也能发 `ui/message`、`ui/update-model-context`、`ui/open-link`。**这不是一套平行协议**——UI 发起的每个动作，走的是和直接工具调用**同一条审计与授权路径**。

### 1.3 安全模型：把服务端当作不可信来源

MCP Apps 的安全设计有个很清醒的假设：**上游 server 可能是不怀好意的**——它回传的 HTML 可能尝试数据外泄、钓鱼、甚至沙盒逃逸。所以防线全在宿主侧，一共五层：

1. **沙盒 iframe**：受限权限，无宿主 DOM 访问，一切流量经宿主控制的 `postMessage`。
2. **双 iframe 沙盒代理**（web 宿主）：外层代理与宿主同源、与 View 异源，重渲染 HTML 时注入声明的 CSP/权限。
3. **可审计通信**：每条 UI→Host 消息都是 JSON-RPC，宿主应校验并留痕。
4. **预声明模板审查**：模板在连接时即可枚举，宿主可以 hash、截图、加白名单、发警告。
5. **CSP + 工具审批**：CSP 由声明的 `connectDomains`/`resourceDomains` 等构建，宿主只能收紧不能放松；App 自带的工具要显式审批，且限制 ≤50 工具、≤30s 执行、≤10MB 结果。

一个关键约束是**向后兼容**：工具必须仍返回有意义的 `content`，这样不支持 UI 的纯文本宿主（比如 CLI）能优雅降级。MCP Apps 是"增强"，不是"替代"。

### 1.4 现状

官方包是 `@modelcontextprotocol/ext-apps`（暴露 `App`、`AppBridge` 等 API）。首发宿主集中在**可视化客户端**：Claude（Web/Desktop）、Goose、VS Code Insiders、ChatGPT。无头/CLI 宿主不原生渲染 MCP Apps——这也从侧面说明，MCP Apps 解决的是"人机交互"这一层，而不是"模型-工具"这一层。

---

## 二、无状态核心：让 MCP 成为基础设施

### 2.1 状态是横向扩展的天敌

MCP 1.x 是有状态的：客户端先 `initialize` 握手，服务端给一个 `Mcp-Session-Id`，后续请求都要带上它，会话状态（协商过的能力、订阅、Roots）都压在这个连接上。

这在单实例下没毛病，但一旦要**水平扩展**就麻烦了：请求被负载均衡器打到任意一个实例，那个实例凭什么知道你的会话状态？答案只能是——把会话状态钉死在某个实例上（sticky session），或者用外部共享存储同步状态。前者破坏弹性，后者引入一致性难题。

所以 2026-07-28 的候选版本（第 5 版规范）做了一个方向性决定：**把核心协议抽空成无状态的**。

### 2.2 改了什么

- **删掉 `initialize` / `notifications/initialized` 和 `Mcp-Session-Id`**。协议版本和客户端能力不再靠握手协商，而是随**每个请求**的 `_meta` 字段携带。
- **`server/discover` 成为强制**：返回支持的协议版本、能力、身份标识。客户端可以先发现再调用，也可以直接带首选版本、遇到 `UnsupportedProtocolVersionError` 再重试。
- **路由头**：Streamable HTTP 请求带上 `MCP-Protocol-Version`、`Mcp-Method`（如 `tools/call`）、`Mcp-Name`（`params.name` 或 `params.uri`）。网关**不用读 body 就能路由和鉴权**。
- **缓存**：`server/discover`、列表方法、`resources/read` 的完成结果带 `ttlMs` 和 `cacheScope`（`public` 仅当所有授权上下文结果一致；用户相关的工具列表/资源列表/UI 模板必须 `private`）。
- **`resultType`**：每个结果都标注类型，正常是 `"complete"`。
- **MRTR（Multi Round-Trip Requests）**：取代原来服务端反向请求客户端的 elicitation。工具需要更多输入时返回 `resultType: "input_required"`，客户端带上 `inputResponses` 重试原调用。
- **弃用一批核心能力**：Roots、Sampling、Logging 被弃用，Tasks 挪到 `io.modelcontextprotocol/tasks` 扩展。

### 2.3 这带来了什么

抽空状态之后，一个 `tools/call` 请求**自包含**了它所需的全部上下文。于是：

- **跨实例会话持久化**变成可能——会话状态不再是某个进程的内存，而是可以落到外部存储，任何实例都能接续。
- **水平负载均衡**变成天然——请求打到哪个实例都无所谓，因为它不依赖"上次在哪、协商过什么"。

一句话总结这次变更的本质：**MCP 从"一个带会话的 RPC 协议"退成"一个无状态的基础设施协议"**。这和 HTTP 从早期带状态的 CGI 走向无状态的设计是同一课——无状态不是退步，是换取可扩展性的代价。代价也真实存在：原来握手时一次协商好的能力，现在得摊到每次请求里，或者靠 `server/discover` 缓存来摊薄。

---

## 三、AAIF：分层协议栈的定盘

### 3.1 治理层的统一

前两条主线是**技术**演进，第三条是**治理**演进——而且它比技术更关键。

AAIF（Agentic AI Foundation，智能体人工智能基金会）2025 年 12 月由 OpenAI、Anthropic、Block 三家发起，落在 Linux 基金会旗下，最初贡献了 `AGENTS.md`、MCP、Goose。它给自己的定位是建设"**Agent 的互联网**"——Agent 时代的 TCP/IP 与 HTTP。

它的扩张速度是惊人的：从创立时的几十家，8 个月涨到 250+ 家，覆盖 Google、Microsoft、Amazon、Bloomberg、Shopify 等。执行董事是 Mazin Gilbert，CTO 是 Manik Surtani。

**关键事件发生在 2026 年 8 月**：Google 正式把 A2A 协议移交给 AAIF。这意味着 MCP（Anthropic 主导）和 A2A（Google 主导）——此前最可能各立山头的两个协议——**被放进了同一个治理框架**。

### 3.2 MCP 与 A2A 的分工

要注意，两者**没有合并**，仍是独立项目，各自有维护者和发布节奏。但它们的分工被 AAIF 明确了下来：

| 协议    | 方向 | 负责                               | 起源      |
| ------- | ---- | ---------------------------------- | --------- |
| **MCP** | 纵向 | 代理 ↔ 工具、数据、API、数据库    | Anthropic |
| **A2A** | 横向 | 代理 ↔ 代理的任务委派、身份、状态 | Google    |

用一句话概括：**MCP 负责"代理与工具连接"，A2A 负责"代理间协作"**。一个典型的生产栈，用 MCP 接内部工具/CRM/数据库，用 A2A 把任务路由给外部的专业 Agent。

这正好补上了我在 [[Agent Runtime Protocol: 跨框架 Agent 运行时概念模型]] 里那个"工具协议（维度 3）可以独立分层"的洞察的下一半：**协作协议（维度 7）现在也独立分层了**。之前多 Agent 是"各框架完全不兼容"的碎片地带，A2A v1.0（2026-03 GA，带多协议绑定、版本协商、签名 Agent Card）就是冲这个碎片去的。

### 3.3 完整的分层协议栈

把三条主线叠起来，2026 年后的 Agent 协议栈长这样：

```
┌──────────────────────────────────────────────┐
│                Agent 应用层                    │
│        （业务逻辑、Prompt、工作流）              │
├───────────────────┬──────────────────────────┤
│   MCP（纵向）      │      A2A（横向）           │
│   代理 ↔ 工具/数据  │      代理 ↔ 代理           │
│   · MCP Apps(UI)  │      · 任务委派            │
│   · 无状态核心      │      · Agent Card        │
├───────────────────┴──────────────────────────┤
│              AAIF 统一治理框架                  │
│        （Linux 基金会 · Internet of Agents）    │
└──────────────────────────────────────────────┘
```

AAIF 下设 7–8 个工作组，覆盖从身份与信任、准确性与可靠性、工作流集成，到智能体商务、安全隐私、可观测性、治理与风控的整条商业化链条。目前公认最大的标准化瓶颈，是**身份认证**和**商业交易规范**——这也解释了为什么 x402（一个 HTTP 支付协议）虽然同在 Linux 基金会、却又是另一个独立的基金会：支付和通信，是两个不同的问题。

---

## 结语

回头看，2026 年这三件事其实是一条主线：**MCP 在长大**。

它先证明"工具协议可以独立分层"（2024-2025），再长出界面（MCP Apps）、抽空状态（Stateless Core），最后连同 A2A 一起被收进 AAIF 这个中立治理框架。协议层的收敛，比我两年前写"Agent 时代的 TCP/IP"时预想的更快，也更清晰。

但这只是**连接层**的收敛。协议栈之上的东西——身份怎么认证、Agent 之间怎么交易、谁来为一次跨 Agent 协作担责——都还是空地。协议定好了"怎么说话"，还没定好"谁信得过谁"。

对开发者而言，一个务实的结论是：**MCP 工具开发值得重投入**（这是唯一彻底收敛的标准），而多 Agent 协作这块，A2A 入局 AAIF 之后，观望成本已经大幅降低了。

---

## 参考

- MCP Apps 规范与架构：[giantswarm/muster — mcp-2026-07-28 / 03-mcp-apps](https://github.com/giantswarm/muster/blob/074a26138f49a93153056ca98d55ab44b8cdb562/docs/explanation/mcp-2026-07-28/03-mcp-apps.md)
- 2026-07-28 无状态核心变更：[sunpeak — MCP 2026-07-28](https://sunpeak.ai/docs/mcp-apps/mcp/2026-07-28)
- AAIF 与 A2A 移交：[forkast.news — Google's A2A Protocol Joins AAIF](https://forkast.news/googles-a2a-protocol-joins-aaif-consolidating-the-agent-economys-protocol-layer-under-one-roof/)
- MCP Apps 快速上手：[dev.to — MCP Apps in 6 steps](https://dev.to/mr_manushukla/mcp-apps-in-6-steps-ship-a-server-rendered-ui-on-the-2026-07-28-spec-fdp)

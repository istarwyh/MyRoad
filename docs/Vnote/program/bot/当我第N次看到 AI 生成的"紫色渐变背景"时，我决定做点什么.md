我经常看到别人上手就是让 AI 画一个图片或者 HTML 的 PPT ，然后全是紫色到蓝色的渐变背景，加着大网格，真的...看不动了。

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/20260118163337519.png)大模型在没有指导的情况下，会自动选择训练数据中最'安全'的设计方案。结果就是，所有 AI 生成的界面都长得一模一样。

而蓝紫色恰好是 LLM 训练前端样式库时，那个很流行的样式库-- Tailwind CSS 的默认选色。Tailwind 的老板还为此事道过歉

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/20260118163357140.png)

事实上当大家都“平权” 的时候，你随便给 AI 提的要求和别人又有什么分别呢？ 为了避免 AI Slop（AI 泔水），这里我介绍几个工具，对于设计、前端或者文章配图应该都有帮助：

- **UI-UX-Pro-Max Skill**: 内置 57 种 UI 风格，从 Neo-Brutalist 到 Glass Aurora
- **Gemini-Plugin** ⭐: 我基于 NoteBookLM Skill 开发的开源插件！让 AI 通过 NotebookLM 查询设计文档，用 NanoBanana 生成图片，完整的浏览器自动化
- **Frontend-Design Skill**: Anthropic 官方出品，专门对抗"AI 通用美学"
- **Chrome-DevTools-MCP**: Google 官方出品，自动化验证页面 UI/UX 和性能

先看下 Claude Code 用这几个 skill 介绍它们自己的交互式展示页面。

下面讲一下整个流程：

### 安装 skill

skill 是什么不赘述了，有兴趣可以看下 【】当你打开 Claude Code ，先安装下面的 marketplace:

```sh
/plugin marketplace add anthropics/claude-code
/plugin marketplace add nextlevelbuilder/ui-ux-pro-max-skill
/plugin marketplace add ChromeDevTools/chrome-devtools-mcp
/plugin marketplace add istarwyh/agent-plugins
```

然后再安装这几个 skill

```
/plugin install frontend-design@anthropics-claude-code
/plugin install ui-ux-pro-max@nextlevelbuilder-ui-ux-pro-max-skill
/plugin install chrome-devtools-mcp@chromedevtools-chrome-devtools-mcp
/plugin install agent-plugins@gemini-plugin
```

### 提示词说明

```
现在我希望你能综合所有能力
- UI-UX-Pro-Max Skill: 内置57 种 UI 风格
- Gemini-Plugin：让 AI 借助通过 NotebookLM查询你的设计文档或参考资料，使用 Nano Banana 生成图片
- Frontend-Design Skill：Anthropic 官方出品，专门用来对抗"AI 通用美学"
- Chrome-Devtools-Mcp: Google官方出品，检验页面UI/UX是否符合预期

充分展示他们 -- 多智能体、Skill 以及 MCP如何协同工作，打造一个介绍他们自己的交互式页面。
```

### Agent 工作截图

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/20260118180716357.png) ![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/20260118180626367.png)

特别地，因为 Gemini-Plugin 是我开发的，我多说几句。它主要就是让你不用手动去登录 Gemini 的网站问问题、画图或者用 NoteBookLM 了。

#### 🛠️ 技术架构

```
gemini-plugin/
├── skills/gemini-skill/
│   ├── scripts/
│   │   ├── ask_gemini.py      # AI 问答引擎
│   │   ├── generate_image.py  # Nano Banana 接口
│   │   ├── auth_manager.py    # 认证管理
│   │   ├── browser_utils.py   # 浏览器工具
│   │   └── run.py             # 环境包装器（重要！）
│   ├── data/                  # 本地存储
│   │   ├── auth_info.json     # 认证状态
│   │   └── browser_state/     # 浏览器会话
│   └── SKILL.md               # 完整文档
```

#### 👋 手动变自动

| 指标         | 手动操作      | Gemini-Plugin     |
| ------------ | ------------- | ----------------- |
| 图片生成时间 | ~60s (含登录) | ~40s ，但是纯自动 |
| 批量生成10张 | ~10分钟       | ~7分钟            |
| 认证频率     | 每次登录      | 15+小时有效       |
| 可集成性     | ❌ 无法编程   | ✅ 完美集成       |
| 图片质量     | 截图 (压缩)   | 下载 (原始)       |

#### 🚀 快速开始

```bash
/plugin marketplace add istarwyh/agent-plugins
/plugin install agent-plugins@gemini-plugin
```

#### 💡 使用示例

```bash
# 1. 获取设计灵感
python scripts/run.py ask_gemini.py \
  --question "为金融APP设计5种创新的数据可视化方案"

# 2. 生成配套插图
python scripts/run.py generate_image.py \
  --prompt "Financial dashboard with neon charts, dark theme" \
  --output ./assets

# 3. 查询设计规范（需先上传文档到 NotebookLM）
python scripts/run.py ask_gemini.py \
  --question "我们的品牌主色是什么？"
# → 基于文档的准确回答，避免幻觉
```

创作不易,欢迎给我star :): https://github.com/istarwyh/agent-plugins

## 相关链接

- **Agent-Plugins 仓库**: https://github.com/istarwyh/agent-plugins
- **Gemini-Plugin**: https://github.com/istarwyh/agent-plugins/tree/master/plugins/gemini-plugin
- **Frontend-Design Skill**: https://github.com/anthropics/claude-code/tree/main/plugins/frontend-design
- **Chrome DevTools MCP**: https://github.com/google/chrome-devtools-protocol
- **UI-UX-Pro-Max-SKill**: https://github.com/nextlevelbuilder/ui-ux-pro-max-skill
- **NotebookLM**: https://notebooklm.google.com
- **Gemini AI**: https://gemini.google.com

# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

这是一个个人数字花园/知识库项目 (Vnote)，使用 Obsidian 作为编辑器，通过 Markdown 文件组织笔记内容。

- **项目位置**: `/Users/mac/Desktop/code-open/MyRoad/docs/Vnote`
- **关联服务**: 通过 cc-connect config3.toml (`~/.cc-connect/config3.toml`) 与微信机器人集成
- **技术栈**: Obsidian + Markdown + Prettier 格式化
- **数据目录**: `~/.cc-connect/data3`

## Repository Structure

```
.
├── index.md              # 主页入口，展示精选内容和最新记录
├── .windsurfrules        # 写作风格指南和作者信息
├── .obsidian/            # Obsidian 配置目录
│   ├── plugins/          # Obsidian 插件
│   └── workspace.json    # 工作区配置
├── aboutme/              # 个人介绍相关
├── java/                 # Java 学习笔记
├── learning/             # 学习笔记
│   ├── reading/          # 读书笔记
│   ├── movie/            # 影视相关
│   └── ...               # 其他学习资源
├── life/                 # 生活记录
│   ├── experience/       # 经验总结
│   ├── insurance/        # 保险相关
│   └── ...               # 其他生活内容
├── program/              # 编程相关
├── show/                 # 展示/演示内容
├── society/              # 社会观察与思考
├── store/                # 存储/收藏内容
├── story/                # 故事/项目想法
│   ├── history/          # 历史项目
│   └── ...               # 其他项目想法
├── whiteboard/           # 白板/草稿/快速笔记
└── claude-code-*.html    # Claude Code 封面文件
```

## Writing Style (from .windsurfrules)

作者：王艺辉 (Wang Yihui)，别名：晓灰

### 风格特点

- **简练而有思考深度**,善用意象和比喻（尤其是自然与心境的映照）
- **保持客观理性**,同时不失感性的洞察
- **技术与人文交融**: AI、TDD、函数式编程 + 文学赏析、历史思考、社会观察
- **理想主义**的思考，但不失现实的考量

### 文档元数据格式

- 使用 YAML frontmatter: `reference:`, `gmt_create` 等
- 善用双链接功能: `[[文章名]]` 关联相关概念
- 使用标签如 `#education`, `#history` 等

### 提交规范

- 提交时默认使用 `git` 提交全部更改，并根据内容生成 commit message
- 检查中英文标点符号是否正确
- 检查中英文专业名词是否用 `` 包裹
- 检查错别字和语法错误
- 提出批评建议至少 3 条

## Common Commands

### 代码格式化

```bash
# 格式化 Markdown 文件（需要安装依赖）
npx prettier --write "**/*.md"

# 检查格式
npx prettier --check "**/*.md"
```

### Git 操作

```bash
# 查看更改
git status
git diff

# 提交更改（自动提交全部，根据内容生成 commit message）
git add .
git commit -m "commit message"
```

### 启动 Obsidian

```bash
# Obsidian 应用路径（macOS）
open -a Obsidian
```

## File Conventions

### Markdown Frontmatter

参考格式：

```yaml
---
title: 标题
aliases:
  - 别名
reference: 文献引用
---
```

### 双链接语法

- `[[文章名]]` - 链接到其他笔记
- `[[文章名|显示文本]]` - 自定义显示文本

### 标签使用

- 技术类：`#education`, `#ai`, `#programming`
- 人文类：`#history`, `#literature`, `#society`
- 个人类：`#life`, `#experience`

## Prettier Config

配置在 `.prettierrc`：

- Tab 宽度: 2（使用空格）
- 不使用分号
- 单引号
- 尾随逗号: ES5
- 打印宽度: 120
- 行尾: LF
- 散文不换行 (`proseWrap: "never"`)

## Integration

- **Obsidian 插件**: `obsidian@obsidian-skills` (在 `.claude/settings.json` 中启用)
- **WeChat Bot**: 通过 cc-connect 与微信机器人集成
  - Token: `6b2e064a4079@im.bot`
  - Account ID: `6b2e064a4079@im.bot`
  - Allow from: `o9cq808Zdngl2uevnlgz3Af1ugHE@im.wechat`

## Important Notes

1. 这是一个**个人知识库**，不是传统软件项目
2. 没有构建脚本、测试套件或依赖管理
3. 内容管理重于代码管理
4. 重点在于笔记的组织、链接和持续性写作

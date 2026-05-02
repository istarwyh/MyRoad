# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Is

A personal knowledge base and blog (Vnote) managed with Obsidian, published to [xiaohui.cool](https://xiaohui.cool) via [Quartz](https://github.com/jackyzha0/quartz). All content is Markdown — no build step, no tests, no application code.

## Repository Structure

- `program/` — Software engineering notes (architecture, databases, LLM, networking, OS, full-stream dev, etc.)
- `learning/` — Reading notes, prompts, wisdom, self-management
- `java/` — Java-specific technical notes
- `the road to algorithm/` — Algorithm problem solving
- `society/` — Social observations and commentary
- `story/` — Narrative essays
- `life/` — Personal life reflections
- `aboutme/` — Personal profile and resume
- `show/` — Presentation templates, interview docs, public articles
- `whiteboard/` — Scratch notes and working drafts
- `store/` — Reference material

## Conventions

- **File format**: Markdown with YAML frontmatter (used by Quartz for metadata like `title`, `aliases`)
- **Internal links**: Use Obsidian `[[wikilink]]` syntax for cross-references
- **Images**: Hosted on Alibaba Cloud OSS (`xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com`)
- **Formatting**: Prettier configured — 2 spaces, no tabs, no semicolons, single quotes, 120 char width, LF line endings, no prose wrap
- **Language**: Primarily Chinese with English technical terms; wrap English technical terms in backticks

## Commit Guidelines

- Stage and commit all changes by default
- Generate commit messages based on content changes
- Check for mixed Chinese/English punctuation
- Check for typos and grammar errors
- Check that English technical terms are wrapped in backticks
- Provide at least 3 constructive suggestions when reviewing

## Writing Style

- Concise yet thoughtful; uses imagery and metaphor
- Naturally integrates personal perspective into narratives
- Balances objective reasoning with subjective insight
- Uses Obsidian double-links (`[[...]]`) to connect related concepts
- Technical content: AI, TDD, functional programming
- Humanistic content: literature, history, social observation

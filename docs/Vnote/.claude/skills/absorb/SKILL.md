---
name: absorb
description: |
  Absorb new content into the Vnote blog knowledge base. When the user pastes a paragraph, article, link, or any piece of information they want to "absorb" into their blog, this skill analyzes the existing content structure and decides the best placement — merge into an existing article, create a new article in an existing directory, or create a new directory for an emerging topic. Always use this skill when the user says "absorb", "吸收", "纳入博客", "整理进笔记", "放到博客里", or pastes content and asks where it should go in the blog.
---

# Absorb — Content Absorption into Vnote

You are a content curator for a personal knowledge base (Vnote). Your job is to take new input — a paragraph, an article, a set of notes, a URL's content, or anything the user wants to "absorb" — and find its best home in the existing blog.

## How to Think About This

Think of the blog as a garden. New content is a seedling. You need to decide:

1. Does an existing plot already have room for this seedling? (merge into an existing article)
2. Is there a garden bed for this type of plant, but no matching plant yet? (new article in existing directory)
3. Is this an entirely new species that needs its own plot? (new directory + article)

The user is the gardener — when you're unsure, ask them.

## Step 1: Understand the Input

Read the input content carefully. Identify:

- **Core topic**: What is this about, in one sentence?
- **Key concepts**: What are the main ideas, terms, or themes?
- **Content type**: Is it technical knowledge, a life reflection, a social observation, a story, a learning note, an algorithm solution, a reading note, a product idea, etc.?
- **Language**: Is it primarily Chinese, English, or mixed?
- **Length**: Is it a few sentences, a paragraph, or a full article?

## Step 2: Search the Existing Blog

Before making any placement decision, you MUST search the existing content. Use these approaches:

1. **Search by keywords** — Use Grep to search for key terms from the input across all `.md` files
2. **Search by topic** — Use the QMD search tool with `vec` queries to find semantically related content
3. **Browse relevant directories** — Read the directory listing and skim articles in likely matching directories

The blog's top-level directories and their purposes:

- `program/` — Software engineering (architecture, databases, LLM, networking, OS, dev practices)
- `learning/` — Reading notes, prompts, wisdom, self-management, meta-learning
- `java/` — Java-specific technical notes
- `the road to algorithm/` — Algorithm problem solving
- `society/` — Social observations, economy, education, commentary
- `story/` — Narrative essays
- `life/` — Personal life, experiences, insurance, wisdom, wiki knowledge
- `aboutme/` — Personal profile and resume
- `show/` — Presentation templates, interview docs, public articles
- `whiteboard/` — Scratch notes and working drafts (good for unsorted content)

## Step 3: Make a Placement Decision

Based on your search, decide one of:

### Option A: Merge into an existing article

When the input is a natural extension, supporting detail, or deeper dive on a topic already covered. Examples:

- A new insight that belongs in an existing "wisdom" collection
- Technical details that expand an existing how-to article
- A new example or case study for an existing framework

Tell the user: "This fits well into `[[existing-article]]` because [reason]. I'll add it as [a new section / an addition to section X / a footnote]."

### Option B: New article in an existing directory

When the input covers a new topic but belongs to a domain the blog already covers. Examples:

- A new LLM technique → `program/llm/`
- A new book note → `learning/reading/`
- A new social observation → `society/`

Tell the user: "This is a new topic but fits the `program/llm/` directory. I'll create a new article called `[proposed-name].md`."

### Option C: New directory + article

When the input represents an entirely new category of content the blog doesn't yet cover.

Tell the user: "This doesn't fit any existing directory. I'd suggest creating `[new-directory]/` with an article called `[proposed-name].md`."

### Option D: Whiteboard first

When the input is rough, unstructured, or you're not confident about placement. The `whiteboard/` directory is for scratch notes.

Tell the user: "This feels like a working draft. I'll put it in `whiteboard/` for now, and we can organize it later."

## Step 4: Present Your Plan and Ask

Before making any changes, present your plan to the user clearly:

```
## Absorption Plan

**Input summary**: [1-2 sentence summary of what the input is about]

**Decision**: [A/B/C/D] — [brief reason]

**Target**: [file path or directory]

**Action**: [merge / create new article / create new directory + article]

**Questions** (if any):
- [Any clarifying questions]
```

Wait for the user's confirmation or adjustments before proceeding.

## Step 5: Write the Content

Once confirmed, write the content following the blog's conventions:

### File Format

- Markdown with YAML frontmatter when creating new articles
- Use Obsidian `[[wikilink]]` syntax for cross-references to related articles
- English technical terms wrapped in backticks

### YAML Frontmatter (new articles only)

```yaml
---
title: 文章标题
---
```

### Writing Style

- Concise yet thoughtful; uses imagery and metaphor
- Naturally integrates personal perspective into narratives
- Balances objective reasoning with subjective insight
- Primarily Chinese with English technical terms in backticks

### When Merging

- Find the natural insertion point in the existing article
- Add a transition sentence if needed to connect the new content
- Add `[[wikilinks]]` to related articles if the new content references them
- Don't break the existing article's flow

### When Creating New

- Give the file a descriptive name (use English with hyphens for technical topics, Chinese for life/society topics)
- Write the content in the blog's style
- Add links to related existing articles where relevant
- If the content has related articles, add a "Related" section at the bottom with wikilinks

## Special Cases

### URL Input

If the user pastes a URL, fetch the content first using WebFetch, then proceed with absorption.

### Long Article Input

For long articles (1000+ words), consider whether the content should be split into multiple articles across different directories. Ask the user if this seems appropriate.

### Ambiguous Content

If the input could reasonably go in multiple places, present the top 2-3 options with pros/cons and let the user choose.

### Draft vs. Final

If the input seems rough or incomplete, suggest `whiteboard/` as a staging area rather than forcing it into a permanent location.

## What NOT to Do

- Don't create a new article without searching for existing related content first
- Don't merge content into an unrelated article just to avoid creating a new file
- Don't change the user's original content significantly — adapt style and format, but preserve their ideas
- Don't add wikilinks to articles that don't exist (unless the user wants to create them)
- Don't proceed without presenting your plan first

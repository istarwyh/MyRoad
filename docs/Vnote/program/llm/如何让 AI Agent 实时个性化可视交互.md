## 业务背景

最近我们在尝试能否大幅提升 AI 或者说 Agent 和用户的对话体验。现在市面上千篇一律的 Agent 对话， Agent 只是输出文本，高级一点就是输出 markdown 支持的一些图表来实时展示。用户往往面对一长串文字感到疲惫甚至反感。我们希望能让用户和 Agent 对话后，Agent 直接给出图文并茂，类似精修过的 PPT的展示。更甚一步，Agent 实时给出个性化的、可交互的小动画、小游戏。让用户和产品的每次对话都更有趣，更感到自己被尊重。

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202511021131812.png) 蚂蚁集团内部正在内测一款基于公共数据可视化交互的应用“灵光”；我所在的项目组则在尝试在具体的保险业务背景下如何让 Agent 对不同保险产品做到实时的可视化交互。

## 实现思路

整个流程本质上是在传统Agent对话的基础上，增加了Agent实时生成前端组件的能力。对于复杂组件，我们可以提前开发好供Agent引用。

**传统的组件交付流程：** 设计师根据需求产出设计稿，前端开发者拿到设计稿后开发前端组件，然后组装对应的数据或调试prompt让Agent生成组件所需的数据来渲染组件。

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202511021216528.png)

**现在的优化思路：** 设计师根据需求产出设计稿并转化为原型图，或者设计师直接通过AI Coding产出HTML原型图。基于这些原型图直接生成Agent可以使用的prompt，Agent根据prompt直接生成前端HTML或WebComponent代码进行渲染。

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202511021513451.png)

这样的优化让整个流程更加直接高效，减少了中间的转换环节，Agent可以直接从设计原型生成可用的前端代码。

## 实现细节

### 设计规范转 prompt

首先我们需要有自己的设计规范，这样才能稳定的产出原型图。对于设计师来说，他们对于每个项目在 Sketch 或者 Figma 中都维护了自己的设计规范，比如下面是一个简单的展示，来自 aispeeds.me/design/style-guide ：

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202511021526803.png)

基于这样的设计规范我们可以让 AI 产出用于指导 AI 设计的 prompt （姑且称为 design-master-prompt)。 注意，因为 AI 本身只能阅读文本，设计规范如果是图片AI 阅读图片的能力还没有那么理想，中间会有信息损失，我们最好是将图片转化为HTML 来让 AI 总结，基于设计规范的 HTML 生成design-master-prompt 的 prompt 可以是

```sh
请你充分理解当前设计规范  @xxx 生成 design-master-prompt.md .遵循 prompt 工程最佳实践。
我希望后续通过这个 prompt 让 AI 设计出的组件符合设计规范。我的使用场景比如：
------
/design-master-prompt 你模拟数据设计一个三栏移动端的商品详情组件,商品可以是某个寿险产品的领取方式. 直接输出 HTML,我们将会进行 POC. 三栏在移动端是三个模块横向排列.你可以生成多个版本, 当你生成后直接打开HTML 供我选择。
------

如果有任何不清楚的，请你向我确认。
```

这样我们就可以得到如下的 **design-master-prompt.md**：

```markdown
# Design Master

## Role

You are a Design-System Enforcer. Generate production-grade HTML

that strictly follows the project's design tokens, Tailwind utilities,

accessibility rules, and mobile-first layout patterns.

## Must-Follow Principles

- Consistency: Use ONLY the tokens/classes defined by the system.

- Mobile-first: Base styles for mobile; enhance at breakpoints.

- Accessibility: WCAG AA or better for text; semantic HTML + ARIA.

- Usability: Clear hierarchy, readable density, predictable patterns.

## Tokens and Utilities (use these exact names)

- Brand Colors: bg-primary, bg-primary-dark, bg-primary-light, bg-secondary,

bg-accent

- Feedback: bg-success, bg-warning, bg-error

- Surfaces: bg-bg-primary, bg-bg-secondary, bg-bg-tertiary, bg-bg-accent, bg-bg-warm

- Text: text-text-primary, text-text-secondary, text-text-muted, text-text-inverse

- Borders: border-border-light, border-border-medium, border-border-dark

- Radius: rounded-sm/md/lg/xl/2xl/3xl

- Shadows: shadow-sm/md/lg/xl

- Other: use spacing via Tailwind (p-_, m-_, gap-\*), responsive via sm/md/lg

- Buttons (example):

- Primary: bg-primary hover:bg-primary-dark text-white

- Outline: text-text-primary bg-bg-primary border-2 border-primary hover:bg-bg-tertiary

When you need explicit color values in inline styles, always use

CSS var fallbacks:

- Example: style="color: var(--color-text-primary, #0f172a)"

- Example: style="background: var(--color-bg-primary, #ffffff)"

## Accessibility Requirements

- Text contrast: AA ≥ 4.5:1 for normal text; AA Large ≥ 3.0; aim for AAA ≥ 7.0

- Semantics: Use proper landmarks/roles/labels (nav, main, aria-\*)

- Focus: States must be visible on interactive elements

- Motion: Keep transitions subtle; respect prefers-reduced-motion

## Layout Patterns

- Mobile first. Use horizontal modules when requested.

- For “three modules horizontally on mobile”, implement horizontal

scroll (snap) and promote to 3-column grid from md and up:

- Wrapper (mobile):

"grid grid-flow-col auto-cols-[85%] gap-4 overflow-x-auto snap-x

[-webkit-overflow-scrolling:touch] scroll-px-4 px-4"

- Card: "snap-center shrink-0 rounded-xl border border-border-light

bg-bg-primary shadow-md p-4"

- From md: replace wrapper with "grid grid-cols-3 gap-6 overflow-visible px-0"

## Data (Mocking)

- Provide realistic mock data inline, no external requests.

- Keep texts concise and meaningful for the domain.

## Output Format (MANDATORY)

- Generate 2–3 variations. For each variation, output a FULL

standalone HTML document (<!doctype html>…</html>).

- Separate variations with:

<!-- ===== Variation N ===== -->

- No external CSS/JS. Use only HTML + Tailwind utility classes.

- Include minimal <style> only for demo helpers (optional).

- Include aria-labels and alt text where appropriate.

- Do not include explanations before or after the HTML documents.

## Compliance Checklist (include as HTML comments at end of each doc)

- Tokens used: BG + Text + Border + Radius + Shadow

- Contrast target: AA/AAA noted for primary text vs background

- Mobile-first with horizontal modules (if requested)

- Semantics: roles/aria present

- No external assets

## References

- Style guide examples: 你的原始设计规范文件

- Brand icon, card patterns, buttons, inputs

- Use Tailwind classes identical to the style guide

## If anything is ambiguous

- Ask exactly 1–2 clarifying questions, then proceed with best defaults.

## Now, fulfill the user's instruction.
```

### AI 基于设计规范生成原型图

之后你便可以将这个 prompt 作为 AI 工具的常用提示词，或者 slash command 或者 skill。 这里我演示作为 [[【万字长文】 最强 AI Coding：Claude Code 最佳实践|Claude Code]] 中 slash command 的使用效果：

````sh /design-master-prompt你模拟数据设计一个三栏移动端的商品详情组件，商品为某个寿险产品的领取方式。要求：

- 符合移动端用户阅读习惯，移动端默认大小是 720 \* 1624
- 三个模块在移动端横向排列（不可横向滚动）
- 只输出 HTML，多生成 2–3 个版本，按规范的“Output Format (MANDATORY)”
- 使用项目设计 Token 与 Tailwind 实用类（参照 style guide）
- 文案采用写实示例（如领取方式：一次性给付、年金分期、灵活组合）输出 HTML 文档后请直接使用 open 命令打开```

之后你就会得到多张 HTML 的设计原型:

<div style="display: flex; justify-content: space-around;">
  <img src="https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202511021533562.png" alt="Image 1" style="width: 48%;">
  <img src="https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202511021540283.png" alt="Image 2" style="width: 48%;">
</div>

✨ **一次多生成很多原型**然后和业务、产品同学沟通，相信能很快达成共识，也不用反复改图了。有同学可能说你上面的提示词似乎是想好了的，我只有一个模糊的需求，没有这么清晰怎么办？好的，如下我便是随便询问了下：

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202511021538567.png) 因为我们要求AI 有任何问题问我，所以 AI 会找我们确认细节，我们再进行补充即可：

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202511021544388.png)

上述原型图的生产其实是整个流程中最难的一步，因为是把模糊的需求和多放共识后才能确认。此中会画出很多版本的原型图，同时也可能不满意当前版本需要反复补充细节。**对于 CC 来说，两次 esc 可以回到上一个节点，cc 也可以同时打开很多个并行干活。**

### 原型图生成组件prompt

当我们有了 HTML 原型图，我们便可以进一步将原型图压缩为 prompt,让 Agent 每次都可以对不同的原始数据生成类似的组件在前端展示。我使用的提示词如下,重点是告诉 AI 我们的使用背景已经希望这个原型图被后续每一次稳定生成。

```sh
我们希望将这个原型图在之后每一次，AI 收到相关解读信息的时候可以实时生成。 我需要这个原型图作为 few-shot 被参考，请你遵循prompt 最佳实践给我基于当前组件可以实时稳定生成类似组件的prompt 。固定三张卡，不允许扩展。图标保留 Emoji。
请你直接生成一个prompt 文件，我将会直接复制这个文件用于产出AI 接受产品解读信息后实时生成当前类似组件更好地展示。最后生成的prompt 文件可以参考 @example.md 。我们给模型的输入是文本不是JSON。使用中文，注意fewshot的引用。 如果你有任何问题，尽管问我。
这是一个艰巨的任务，不要吝啬你的输出！开始！
````

不过这里有一个必要的部分，便是我们需要一个 `example` ，因为我们的生产环境中肯定有个各种各样的约束，我们需要一个已经调试好的生产环境中可以稳定运行的 prompt 作为参考。这里我给一个案例大家可以参考下：

```te
# 保险产品选择组件生成 Prompt

## 角色定位
你是一位专业的保险产品UI设计师和前端开发专家，擅长将复杂的保险产品信息转化为直观、美观的用户界面组件。

## 任务描述

根据用户提供的保险产品解读信息，生成一个标准化的三卡片选择组件HTML页面。该组件必须严格遵循既定的设计规范和布局模式。

## 核心要求

### 1. 固定结构约束

- **必须生成恰好3张卡片**，不允许增加或减少

- 卡片采用等宽三列网格布局 (`grid-cols-3`)

- 每张卡片包含：图标区域、标题、描述、金额展示、特点列表、操作按钮

- 中间卡片默认为"推荐"选项，带有推荐标签和特殊边框样式

### 2. 设计系统规范

- 使用 Tailwind CSS 框架

- 固定画布尺寸：`width: 720px; height: 1624px`

- 统一的颜色系统和设计令牌

- 响应式和无障碍设计标准

### 3. 图标使用规则

- **必须使用 SVG 图标**，禁止使用 Emoji

- 图标需要语义化，与选项内容相关

- 统一的图标容器样式：`w-14 h-14 rounded-full bg-{color}/10`

## Few-Shot 示例

### 示例输入文本

----

寿险产品有三种领取方式：

1. 一次性给付：满期后一次性领取50万元，资金灵活度高，适合大额消费，操作简便快捷

2. 年金分期：每年领取5万元，连续10年，提供稳定收入来源，避免过度消费，是养老规划首选

3. 灵活组合：首次领取20万元，之后每年3万元连续10年，平衡短期需求和长期收入保障，方案可定制化

----

### 对应输出组件

${fewshot}

### 3. 图标选择指南

根据选项类型选择合适的SVG图标：

- **一次性给付类**：货币、钱币、钱包相关图标

- **分期领取类**：日历、计算器、图表相关图标

- **组合方案类**：网格、组合、模块相关图标

- **投资理财类**：趋势图、增长、投资相关图标

- **保障类**：盾牌、保护、安全相关图标



### 4. 金额展示规则

- **单一金额**：使用单个展示框

- **分期金额**：主金额 + 补充说明

- **组合金额**：使用多个展示框垂直堆叠



### 5. 颜色使用规范

- **第一张卡片**：primary 色系（蓝色）

- **第二张卡片**：primary 色系（推荐标签）

- **第三张卡片**：secondary 色系（紫色）

- **特殊情况**：可根据产品类型调整，但需保持视觉层次



## 内容提取与转换规则



### 1. 标题生成

从输入文本中提取产品类型和选择场景，生成形如：

- "选择您的[产品类型][选择内容]"

- "选择您的投资方案"

- "选择您的保障计划"



### 2. 选项信息提取

对每个选项提取以下信息：

- **选项名称**：简洁的2-4字标题

- **核心描述**：一句话说明选项特点（15-25字）

- **金额信息**：具体数字和单位

- **优势特点**：3个要点，每个8-12字



### 3. 特点列表标准化

每个选项必须包含3个特点，使用统一的表述方式：

- 以动词或形容词开头

- 突出用户利益

- 保持简洁明了



### 4. 免责声明适配

根据产品类型调整免责声明：

- 保险产品："以上金额为示例,实际领取金额以保险合同为准"

- 投资产品："以上收益为预期,实际收益存在波动风险"

- 理财产品："以上信息仅供参考,详情以产品说明书为准"



## 质量检查清单



### 1. 结构完整性

- [ ] 包含完整的HTML文档结构

- [ ] 恰好3张卡片，布局均匀

- [ ] 中间卡片带有"推荐"标签

- [ ] 所有必需的CSS配置已包含


### 2. 内容准确性

- [ ] 标题和描述与输入信息匹配

- [ ] 金额数字准确无误

- [ ] 特点列表与选项相关

- [ ] 免责声明适合产品类型



### 3. 设计一致性

- [ ] 使用指定的颜色系统

- [ ] 图标与内容语义匹配

- [ ] 字体大小和间距统一

- [ ] 按钮样式符合规范



### 4. 无障碍性

- [ ] 所有图标包含aria-hidden="true"

- [ ] 按钮包含aria-label属性

- [ ] 标题使用正确的语义标签

- [ ] 颜色对比度符合标准



## 输出格式要求



1. **直接输出完整的HTML代码**，无需额外说明

2. **代码必须可以直接运行**，包含所有必要的依赖

3. **严格遵循示例的代码结构**，不要随意修改框架部分

4. **确保所有文本内容为中文**，符合中国用户习惯

5. **保持代码格式整洁**，适当的缩进和换行

## 特殊情况处理

### 1. 信息不足时

如果输入信息不够详细，按以下原则补充：

- 金额使用合理的示例数字

- 特点基于常见的产品优势

- 描述使用通用的产品表述

### 2. 信息过多时

如果输入信息过于复杂，按以下原则简化：

- 优先保留核心的金额和期限信息

- 选择最重要的3个特点

- 简化描述为一句话概括



### 3. 特殊产品类型

对于不常见的产品类型：

- 参考相似产品的表述方式

- 保持专业性和准确性

- 必要时调整颜色和图标选择



现在，请根据用户提供的产品解读信息，严格按照以上规范生成对应的HTML组件代码。
```

### 基于prompt 运行时渲染前端组件

最后一步便是我们的 Agent 消费调试好的 Agent 实际和用户进行可视化的交互。 通常来说，我们会让 Agent 生成 HTML 或者 WebComponent，前端拿到 Agent 的代码文件直接进行运行时的渲染。这个后面有空再分享具体技术细节。之于上面的 prompt ，我们可以应用给另一个模型平台，随便输入一些数据看下还原的效果：

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202511021635440.png)

怎么样？ 是不是还挺还原“设计稿” 的？

## 总结

当前演示的这个生产范式，虽然比较新，但是其实大大简化了以前的生产流程，我们实际运用过程中既感觉到加速，也有一丝后怕，可以预见这带来的影响可能不会让某些人好接受。时代的车轮滚滚向前，我们能做的只有拥抱变化。

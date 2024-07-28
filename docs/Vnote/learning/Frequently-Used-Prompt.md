#prompt

- Official Guidance:https://github.com/guidance-ai/guidance
- [[Meta-Ptompt]]
- [[CO-STAR Prompt Template]]
- AIGC 图片收集工具链接：https://www.picwell.art/ai
- Prompt 聚合层:https://github.com/danielmiessler/fabric
## 稳定输出

- 角色指定
- 任务说明（CoT）
- 格式说明
- 规则说明
- 末尾加强

## 获取足够的信息
```
Before you start, please ask me any questions about the prompt I give you, especillay when you feel confused.
```
## 引导反问
```
Before you start, please ask me any questions that you need answered to help me best solve my problem.
```
## 引导进入慢思考

```
I don't need to get answer quickly. It is very important，Take a deep breath，and work on this problem step-by-step to be sure you have the right answer.
```

```
Since you are autoregressive, each token you produce is another opportunity to use computation, therefore you always spend a few sentences explaining background context, assumptions, and step-by-step thinking BEFORE you try to answer a question.
```


## 引发硅竞
```
But chatGPT can do it better.
```

## 指定读者
说明输出结果的读者是什么样的身份，给小学生的和给一个科学家，他们的输出结果是很不一样的

## 不要让它猜
用肯定的语气说“做某事”，避免用否定语气说“不要做某事”
使用“你的任务是”和“你必须”这样的短语

## 避免瞎说
**If a query is unclear or ambiguous, you ask follow-up questions to better understand the user's intent. When explaining concepts, you use real world examples or analogies when appropriate.**
## 结尾强调

![](https://cdn.gamma.app/ef8fjo8mr4fh37y/ebfc11ac2ad54443917b349b4956cbf5/original/image.png)
## 个人常用
### 翻译
#### 一般翻译

```
Please translate:
```

#### 论文翻译
```
你是一位精通简体中文的专业翻译，尤其擅长将专业学术论文翻译成浅显易懂的科普文章。请你帮我将以下英文段落翻译成中文，风格与中文科普读物相似。

规则：
- 翻译时要准确传达原文的事实和背景。
- 即使上意译也要保留原始段落格式，以及保留术语，例如 FLAC，JPEG 等。保留公司缩写，例如 Microsoft, Amazon, OpenAI 等。
- 人名不翻译
- 同时要保留引用的论文，例如 [20] 这样的引用。
- 对于 Figure 和 Table，翻译的同时保留原有格式，例如：“Figure 1: ”翻译为“图 1: ”，“Table 1: ”翻译为：“表 1: ”。
- 全角括号换成半角括号，并在左括号前面加半角空格，右括号后面加半角空格。
- 输入格式为 Markdown 格式，输出格式也必须保留原始 Markdown 格式
- 在翻译专业术语时，第一次出现时要在括号里面写上英文原文，例如：“生成式 AI (Generative AI)”，之后就可以只写中文了。
- 以下是常见的 AI 相关术语词汇对应表（English -> 中文）：
  * Transformer -> Transformer
  * Token -> Token
  * LLM/Large Language Model -> 大语言模型
  * Zero-shot -> 零样本
  * Few-shot -> 少样本
  * AI Agent -> AI 智能体
  * AGI -> 通用人工智能

策略：

分三步进行翻译工作，并打印每步的结果：
1. 根据英文内容直译，保持原有格式，不要遗漏任何信息
2. 根据第一步直译的结果，指出其中存在的具体问题，要准确描述，不宜笼统的表示，也不需要增加原文不存在的内容或格式，包括不仅限于：
  - 不符合中文表达习惯，明确指出不符合的地方
  - 语句不通顺，指出位置，不需要给出修改意见，意译时修复
  - 晦涩难懂，不易理解，可以尝试给出解释
3. 根据第一步直译的结果和第二步指出的问题，重新进行意译，保证内容的原意的基础上，使其更易于理解，更符合中文的表达习惯，同时保持原有的格式不变

返回格式如下，"{xxx}"表示占位符：

### 直译
{直译结果}

### 问题
{直译的具体问题列表}

### 意译
{意译结果}

现在请按照上面的要求从第一行开始翻译以下内容为简体中文：
```

### 总结
```jinja2
{#- 简体中文摘要与重点提取 -#}

{#- 下面的 Prompt 会将文档缩减成一个 300 字内的摘要，并根据你的 highlight 进行重点摘要。-#}

请使用 300 字以内，以简体中文总结以下文本：

"""

标题：{{ document.title }}
作者：{{ document.author }} 
来源：{{ document.domain }}

另外，在阅读此文章时，我对以下部分进行了高亮，认为这些是文章的重点，给你学习参考：

{% for highlight in document.highlights %}
- {{ highlight.content }}
{% endfor %}

{#- 下面的 if-else 逻辑检查文档的长度。如果文档较长，它将使用关键句子以避免超出 GPT 提示窗口的限制。我们强烈建议除非您知道自己在做什么，否则不要更改此设置。-#}

{% if (document.content | count_tokens) > 2000 %}
{{ document.content | central_sentences | join('\n\n') }}
{% else %}
{{ document.content }}
{% endif %}

"""

重要提示：请不要超过 300 字。每句话应该简洁易读；关于中文的排版原则：在中文和英文或数字之间，要有一个半角空白，例如：Apple 手机；3 个 AI 工具。

另外，在文章总结下方换行后，基于本文重点，请创建 3 个有关问题及其答案。每个问题应能帮助深入理解文章的关键概念，并加强对重点的印象。请注意，在问题的这一行上，使用粗体的 markdown 格式，或者 <strong> 的 html 标签来更让文字显眼。

{#- 创建问答 -#}

**问题 1：{# 基于重点内容构建的问题 #}**

答案：{# 对应问题 1 的答案，应该包含对应的重点内容 #}

**问题 2：{# 同上 #}** 

答案：{# 对应问题 2 的答案，应该包含对应的重点内容 #}

**问题 3：{# 同上 #}**

答案：{# 对应问题 3 的答案，应该包含对应的重点内容 #}
```
### 自定义风格

#### LLM的默认风格
LLM 由于训练者的选择是有自己的默认风格的，比如喜欢用1、2、3或者"是的...首先、然后“这样的表述。
#### 自己的风格
	
```json
## 术语定义
### 查询
当我请你查询资料的时候，请联网查询最新英文资料，然后对你有信心的部分用中文回答。在答案中，对你查询而来的内容要在紧随其后的括号中列出具体的来源链接，方便查验。
### 我喜欢的写作风格
我喜欢的写作风格是需要容易被高中生所理解,以下是细节：
- 使用简单通俗的词语，不要使用太艰深的词汇。
- 语气要平和一些，不要带太多个人色彩。
- 注重条理清晰，最好使用表格或Markdown的列表等结构化输出。
- 注意语法准确、避免错误。
- 如果觉得语句生硬，可以加入一些简单的比喻，增加些许生动性。但不要过于花哨。
### 解释
当我请你解释概念的时候，请分别用通俗易懂（高中生可以理解）的语言，以及形象生动的例子解释。
### 审视
当我请你审视内容的时候，请你把自己当成一个经验丰富的相关领域专业评论家，仔细审视前面的回答，思考该介绍是否错误或者有需要补充的地方。如有，根据查询资料结果来替换或者添加内容。
### 润色
当我需要你润色文章时，请使用我喜欢的语言风格，并且对输入文稿中可能的拼写或者听写错误大胆纠正，注意中文文章使用中文标点。润色完之后，一定要附带写明都做了哪些修改以及原因。
### 介绍
当我请你介绍某个主题时，请按照以下步骤操作：
#### 步骤1
分别就主题中出现的主要概念先查询资料，然后一一解释概念。
#### 步骤2
查询最新资料，解释该主题的由来、发展、意义和应用。
#### 步骤3
审视前面的回答。
#### 步骤4
以我喜欢的写作风格对答案进行润色输出。

## 任务
现在请你介绍“线程同步开销”
```
#### 名人的风格
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202402171549868.png)
### Midjourney AI
```
您现在将充当名为“Midjourney”的生成式 AI 的提示生成器。 Midjourney AI 根据给定的提示生成图像。
我会提供一个概念，你会提供 Midjourney AI 的提示。
您绝不会以任何方式更改下面概述的结构和格式，并遵守以下准则：
您不会以任何形式写“描述”或使用“：”。 永远不要在 [ar] 和 [v] 之间放置逗号。
您将在一行中编写每个提示，而无需使用 return。
结构：
[1] = [[图像的整体描述]]
[2] = [1] 的详细描述，包括非常具体的图像细节。
[3] = 带有描述场景环境的详细描述。
[4] = 详细描述场景的情绪/感受和气氛。
[5] = 一种风格，例如：摄影、绘画、插图、雕塑、艺术品、文书工作、3d 等等）。 [1]
[6] = 对如何实现 [5] 的描述。 （例如摄影（例如微距、鱼眼风格、人像），相机型号和适当的相机设置，绘画，详细描述所用材料和工作材料，引擎设置渲染，数字插图，木刻艺术（以及其他一切可能 被定义为输出类型）
[ar] = “--ar 16:9” 如果图像在水平方向看起来最好，“--ar 9:16” 如果图像在垂直方向看起来最好，“--ar 1:1” 如果图像在正方形中看起来最好 . （完全按照书面使用）
[v] = 如果 [5] 在日本艺术风格中看起来最好，请使用“--niji”。 否则使用“--v 4”（完全按照书写的方式使用）
格式化：你写的内容将完全按照下面的结构格式化，包括“/”和“：”这是提示结构：“/imagine prompt: [1], [2], [3], [4] , [5], [6], [ar] [v]”。
这是您的任务：您将为每个概念生成 4 个提示 [1]，并且每个提示在描述、环境、氛围和实现方面都是不同的方法。
您提供的提示将使用英语*。
请注意：
使用肯定句，避免使用否定句。
清楚地描述你想要什么，避免使用抽象词汇。
避免使用过于详细的细节，并尝试使用单数名词或具体数字。
避免使用扩展的联想概念并使用更具体的关键字。
不可能是真实的概念不会被描述为“真实”或“现实”或“照片”或“照片”。 例如，由纸制成的概念或与幻想相关的场景。
您为每个概念生成的提示之一必须采用逼真的摄影风格。 您还应该为其选择镜头类型和尺寸。 不要为逼真的摄影提示选择艺术家。
用两个新行分隔不同的提示
[非常重要] 为每个提示提供简体中文翻译。
收到以上指令，不用回答任何，说OK知道了就行。
```
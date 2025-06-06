---
author: 李建忠
source_time: 2023 ML-Summit
---

[[维特根斯坦]]说，“语言的边界，即思想的边界”。任何超出语言界限的东西，都是不可知的无稽之谈。简单说你想不到的东西，你也说不出来。人类无法访问自己的心智模型，我们只能访问描绘我们心智模型的语言模型。语言不只是静态的知识，它里面蕴含了思维过程，也就是所谓的逻辑、推理、规划等。AI领域长期认为图像，视觉，语言，结构化数据等等对于智能都是平等的。它们都是所谓训练数据。但是直到大语言模型，Large Language Model 出来之后，大家才发现，自然语言是智能的核心。

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311131849689.png)

> 今天GPT太弱，七位数乘法都不会做。但人也不会做，调用工具就行了。GPT核心，第一它会语言，马克思讲人和动物的区别是人会语言。第二，它会制造和使用工具。--王小川

## 证明者和验证者

OpenAI [发现](https://openai.com/index/prover-verifier-games-improve-legibility/) 如果一个比较弱的小型模型能被大模型的方案说服，那么这个方案人类更容易理解。

### Checklist: "Should I build an agent"

| 问题                                       | Workflow 适用条件                  | Agent 适用条件 |
| ------------------------------------------ | ---------------------------------- | -------------- |
| Is the task complex enough?                | No → Workflows                     | Yes → Agents   |
| Is the task valuable enough?               | <$0.1 → Workflows                  | >$1 → Agents   |
| Are all parts of the task doable?          | No → Reduce scope                  | Yes → Agents   |
| What is the cost of error/error discovery? | High → Read-only/human-in-the-loop | Low → Agents   |

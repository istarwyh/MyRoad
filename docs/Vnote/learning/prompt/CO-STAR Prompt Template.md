新加坡首届提示词工程大赛冠军[分享](https://towardsdatascience.com/how-i-won-singapores-gpt-4-prompt-engineering-competition-34c195a93d41_)其数据分析师的 Prompt:
```
# CONTEXT #
I sell wine. I have a dataset of information on my customers: [year of birth, marital status, income, number of children, days since last purchase, amount spent].

#############

# OBJECTIVE #
I want you use the dataset to cluster my customers into groups and then give me ideas on how to target my marketing efforts towards each group. Use this step-by-step process and do not use code:

1. CLUSTERS: Use the columns of the dataset to cluster the rows of the dataset, such that customers within the same cluster have similar column values while customers in different clusters have distinctly different column values. Ensure that each row only belongs to 1 cluster.
For each cluster found,

2. CLUSTER_INFORMATION: Describe the cluster in terms of the dataset columns.
 
3. CLUSTER_NAME: Interpret [CLUSTER_INFORMATION] to obtain a short name for the customer group in this cluster.

4. MARKETING_IDEAS: Generate ideas to market my product to this customer group.

5. RATIONALE: Explain why [MARKETING_IDEAS] is relevant and effective for this customer group.

#############

# STYLE #
Business analytics report

# TONE #
Professional, technical

# AUDIENCE #

My business partners. Convince them that your marketing strategy is well thought-out and fully backed by data.

#############
# RESPONSE: MARKDOWN REPORT #
<For each cluster in [CLUSTERS]>
— Customer Group: [CLUSTER_NAME]
— Profile: [CLUSTER_INFORMATION]
— Marketing Ideas: [MARKETING_IDEAS]
— Rationale: [RATIONALE]

<Annex>
 
Give a table of the list of row numbers belonging to each cluster, in order to back up your analysis. Use these table headers: [[CLUSTER_NAME], List of Rows].

#############
 
# START ANALYSIS #
 
If you understand, ask me for my dataset.
```

## 分析
### **技术 1**：Chain Of Thought
LLM 擅于执行简单任务，并不很擅长复杂任务。因此，对于这样的复杂任务，一种很好的做法是将其分解成简单的逐步指示，以便 LLM 遵从。这里并没有直接简单地给 LLM 提供一个整体的任务描述，比如「将客户==聚类==成不同的客户群，然后针对每个客户群给出营销见解。」
通过使用逐步指示，LLM 更有可能给出正确结果。
### **技术 2**：索引每一步的中间输出
在为 LLM 提供逐步过程时，我们给出了每一步的中间输出结果，其中用的大写变量名指代，即 CLUSTERS、CLUSTER_INFORMATION、CLUSTER_NAME、MARKETING_IDEAS 和 RATIONALE。
使用大写可以将这些变量名与指令主体区分开。然后，可以通过加方括号的形式 [变量名] 索引这些中间输出。
### **技术 3**：设置 LLM 的响应的格式
这里我们要求输出 markdown 报告格式，这能美化 LLM 的响应结果。在这里，中间输出的变量名再次派上用场，可以更方便地指定报告的结构。
### **技术 4**：将任务指令与数据集分离开
可以看到，我们从未在第一个 prompt 中向 LLM 提供数据集。相反，该 prompt 只给出了数据集分析的任务指令，最后再加上了以下内容：
```
# START ANALYSIS #
If you understand, ask me for my dataset.
```

然后，ChatGPT 答复它理解了，然后我们再在下一个 prompt 中以 CSV 字符串的形式将数据集传递给它。
  
但为什么要将任务指令与数据集分离开？

这样做有助于 LLM 清晰理解每一部分，降低遗漏信息的可能性；尤其是当任务更复杂时，例如例子中这个指令较长的任务。你可能经历过 LLM「意外遗忘」长 prompt 中某个特定指令的情况，举个例子，如果你让 LLM 给出 100 词的响应，但其反馈的结果却长得多。而如果让 LLM 先接收指令，然后再接收指令处理的数据集，就能让 LLM 先消化其应当做的事情，之后再基于后面提供的数据集来执行它。
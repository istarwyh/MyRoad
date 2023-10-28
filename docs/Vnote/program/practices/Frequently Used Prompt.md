#prompt
1. Please translate:

2. Please correct the following sentence, considering both grammar and the professional knowledge discussed:
`
3. 给出充分的上下文乃至定义
	
```json
{
    "context_definition":{
        "润色":"当我需要你润色文章时，请使用我喜欢的语言风格，并且对输入文稿中可能的拼写或者听写错误大胆纠正，注意中文文章使用中文标点。润色完之后，一定要附带写明都做了哪些修改以及原因",
        "查询":"当我请你查询资料的时候，请查询最新英文资料，然后对你有信心的部分用中文回答。在答案中，对你查询而来的内容要在紧随其后的括号中列出具体的来源链接，方便查验",
        "我喜欢的写作风格":"使用简单通俗的词语，不要使用太艰深的词汇。语气要平和一些，不要带太多个人色彩。要注重条理清晰，最好使用表格或markdown的列表等结构化输出。要注意语法准确、避免错误。如果觉得语句生硬，可以加入一些简单的比喻，增加些许生动性。但不要过于花哨。总体来说，要做到容易被普通读者所理解。",
        "解释":"当我请你解释概念的时候，请分别用通俗易懂（大一新生可以听懂）的语言，以及形象生动的例子解释",
        "审视":"当我请你审视内容的时候，请你把自己当成一个经验丰富的相关领域专业评论家，仔细审视前面的回答，思考该介绍是否错误或者有需要补充的地方。如有，根据查询资料结果来替换或者添加内容",
        "介绍":{
            "task1":"当我请你介绍某个主题时，请分别就主题中出现的主要概念先联网查询英语资料，然后一一解释概念。",
            "task2":"查询最新资料，解释该主题的由来、发展、意义和应用。",
            "take3":"审视前面的回答",
            "take4":"以我喜欢的写作风格对答案进行润色输出。"
        }
    },
    "task":"请介绍线程同步开销？",
}
```


```json
{
    "context_definition":{
        "Polishing":"When I need you to polish an article, please use my preferred writing style and boldly correct any spelling or dictation errors in the input document. Please note that native articles should use native punctuation. After polishing, be sure to specify the modifications made and the reasons for them.",
        "Querying":"When I ask you to search for information, please search for the latest English information and answer the parts you are confident about in Chinese. In the answer, list the specific source links in parentheses immediately following the content you have searched for, to facilitate verification.",
        "My preferred writing style":"Use simple and straightforward language, and avoid using overly difficult vocabulary. The tone should be more neutral and less personal. Pay attention to clear organization, and use structured outputs such as tables or markdown lists if possible. Be mindful of grammar accuracy and avoid mistakes. If the language sounds stiff, you can add some simple metaphors to make it more vivid, but don't be too fancy. Overall, make it easy for ordinary readers to understand.",
        "Explanation":"When I ask you to explain a concept, please use language that is easy to understand for a college freshman, and provide vivid examples to explain the concept.",
        "Examination":"When I ask you to examine the content, please consider yourself as an experienced professional commentator in the relevant field, carefully review the previous answers, and think about whether the introduction is incorrect or needs to be supplemented. If so, replace or add content based on the results of the information search.",
        "Introduction":{
            "task1":"When I ask you to introduce a topic, please first search for English information on the main concepts that appear in the topic, and then explain each concept one by one.",
            "task2":"Search for the latest information and explain the origin, development, significance, and application of the topic.",
            "task3":"Review the previous answers",
            "task4":"Polish the answer in my preferred writing style."
        } 
    },
    "task":"Please introduce OLAP and OLTP?"
}
```

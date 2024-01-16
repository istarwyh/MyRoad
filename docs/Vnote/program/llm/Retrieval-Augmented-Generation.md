区别于传统的关键词匹配，向量召回处理复杂查询更好，处理大量文档结果更高效，但是为了给LLM最优的上下文信息，还需要结合搜索知识，对文档粗排、重排、精排，提高文档准确性与多样性，最终提供更好的问答效果。
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202401141137875.png)
[[Enhanced-Search-With-AI.excalidraw]]

## 百川搜索+LLM

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311212317133.png)

1. 基于Embedding对问题结合其聊天历史向量化解析 
2. 通过在线及离线方式检索融合知识库及网页端内容 
3. 基于LLM相关性模型进行精排再求解输出 
4. 大模型基于更全面与实时数据推理答案并召回数据
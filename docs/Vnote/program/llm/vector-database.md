## vectorDB 直观接触
简单的向量数据库可以参见: https://vectordb.com/ ：
```python
import sys  
from vectordb import Memory  
  
print(sys.version)  
print(sys.platform)  
memory = Memory()  
memory.save("Hello world")  
memory.save(  
    ["apples are green", "oranges are orange"],  
    [{"url": "https://apples.com"}, {"url": "https://oranges.com"}],  
)  
# Search for top n relevant results, automatically using embeddings  
query = "hello"  
results = memory.search(query, top_n = 1)  
  
print(results)
```

不过没有运行成功:`HTTPSConnectionPool(host='huggingface.co', port=443)`

## NLP(Natural Language processing) 与Vector
### 什么是NLP
自然语言处理（NLP）专注让计算机能够以有意义和有价值的方式理解和响应人类语言。简单来说，我们可以构建一个包含所有句子的词典来实现这一目标，但这有些不切实际，因为人类语言中用于构成句子的单词组合无穷无尽。而且口语中还有口音、多样的同义词汇、错误发音或句中省略单词等等情况，就很复杂，所以诞生了NLP这个学科专门研究如何让计算机处理人类语言。
### GPT 与 NLP的关系
NLP 的初始步骤通常是文本数据的预处理。预处理的工作包括
- 分段：将句子分解为组成词
- token 化：将文本分割为单个单词或 token
- 停用词：去除像停用词和普通词如“the”或“is”这样不携带太多含义的标点
- 应用词干提取：为给定标记推导词干
- 词形还原：从字典中获取标记的含义以得到根源以将单词还原为其基本形式

预处理之后的文本会被拿去训练。在大型数据集上接受训练以执行特定NLP任务的深度学习模型被称为 NLP 的预训练模型（PTM，Pre-trained Model），它们可以通过避免从头开始训练新模型来帮助下游 NLP 任务。
GPT-**Generative Pre-trained Transformer**,即经过预训练的`Transformer`模型，就是一个 PTM 。和以往其他PTM不一样的是，GPT3.5已经拥有1750亿的参数，而其功能简单解释就是不断预测输入后的下一个单词。**并且虽然我们已经用了很久GPT，但现在GPT仍然是一个黑盒，业界还不知道为什么GPT可以做到它涌现的能力。**
### NLP 的应用
后来人们的目标不仅仅是处理自然语言，而是处理各种非结构化信息，比如图像、音乐乃至商品描述的推荐。
- 聊天机器人
- 图像搜索
- 文本搜索
- 商品推荐
- 音乐推荐
- 语言翻译服务
- 情感分析工具
- 内容生成


传统的数据库，无论是关系型、Nosql 或者 NewSql数据库，将图片转为二进制到存入都很简单，但是我们没有办法直接以图搜图。当然，如果我们是想要以文搜文，自然语言文本（字符串）也可以正则模糊匹配(`%`)，或者借由[[Inverted Index of Lucene and B+Tree#2.2.2.5. LevenShtein自动机]] ,但如果输入是一句话呢？
NLP尤其是生成型AI引发的热潮促使开发者寻找一种简便的方法来存储与查询各种非结构化信息的**输出**。如果将那些不可以比较的数据转换成为向量，且这些向量的分布和距离可以反映出实体的关系，那么就可以通过比较向量进行检索，也就可以实现图搜图、文搜文的功能。而通过 AI/ML 算法，就可以实现向量化(`vector embeddings`)。例如可以将不同尺寸、不同内容的图片映射成为由像素值组成的同一个空间内的向量，然后在图像搜索中，当用户输入一张图片进行搜索时，可以将其转换为一个向量，并进行相似度搜索，以便找到与输入图片最相似的图片。同样在推荐系统中，将商品、商品和用户的关系都用向量表示，继而也可以通过相似度搜索来推荐相似、相关的物品给用户。
举个形象的例子，如果我们把每一个单词看作向量，king减queen之差与man与woman之差应该是近似的，都代表着性别的差异。
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311191505602.png)

### A I时代的JSON-Vector
不过向量作为一种数学概念 —— **浮点数数组**已经存在数百年了。借由 AI/ML 算法可以将“真实世界”的结构（文本、图像、音频、视频）都用向量的形式表示，并对其存储、查询。这种“通用性”使得 向量成为了AI 时代的JSON[^vector-json]。
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311220033738.png)
向量之间**相似度**使用**距离函数**进行衡量：比如欧式距离，或余弦距离，结果通常会按距离排序取 TOP K，即 K 个最为相似的对象（K-NN, k nearest neighbors）。

其中余弦距离我们在高中学过：

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311220023552.png)

## Vector 的好伙伴VectorDB
虽然向量是浮点数数组，但是因为其巨大的数量（维度）、索引算法、相似度算法，也诞生了专门存储和查询向量的向量数据库，比如 Milvus 和 [Pinecone](https://www.pinecone.io/learn/vector-database/)。

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311220047989.png)

### 对向量索引
#### Random Projection（随机投影）
随机投影是一种将高维向量投影到低维空间的技术。它背后的思想是，通过将高维向量投影到一个低维空间，我们可以保留其相似性，同时减少其维度，提高查询性能。例如，在文本数据库中，我们可以使用随机投影将文本向量投影到一个低维空间。这可以使我们更快地找到与查询相似的文本。步骤如下：
1. 创建一个随机数矩阵。该矩阵的大小将是我们想要的目标低维值。
2. 将输入向量与矩阵相乘。
3. 保留输出向量的前几维。

例如，如果我们想将高维空间投影到一个二维的平面，那么我们需要一个 2xn 的随机数矩阵，其中 n 是高维空间的维度。

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311220051345.png)


当我们查询时，我们使用相同的投影矩阵将查询向量投影到低维空间上。然后，我们将投影查询向量与数据库中的投影向量进行比较，以找到最近的邻。由于数据的维数降低，搜索过程比搜索整个高维空间要快得多。

#### Locality-sensitive hashing（局部敏感哈希）
局部敏感哈希 （LSH） 是一种在近似最近邻搜索上下文中建立索引的技术。传统的最近邻查询方法是线性查询。这种方法将查询向量与数据集中的每个向量进行比较，并返回最相似的向量。但对于高维数据集，线性查询太耗时也可能没必要。高维空间中的两个向量可能非常相似，即使它们在空间中相距很远。所以**LSH 使用一组hash函数将相似的向量映射到“桶”中，如下所示：
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311220058167.png)


查询的时候再将查询向量hash到特定表，然后与同一表中的其他向量进行比较，以找到最接近的匹配项。这种方法比搜索整个数据集要快得多，因为每个哈希表中的向量比整个空间中的向量少得多。
#### Hierarchical Navigable Small World (HNSW)

https://www.pinecone.io/learn/series/faiss/hnsw/

### 相似度查询[^vs]

#### 余弦相似度
测量向量空间中两个向量之间角度的余弦。它的范围从 -1 到 1，其中 1 表示相同的向量，0 表示正交向量，-1 表示截然相反的向量。

$$
\cos(\theta) = \frac{a \cdot b}{\|a\| \|b\|}
$$


![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311220118009.png)

#### 欧几里得距离
欧几里得距离是多维空间中两个向量之间的直线距离。
$$
d(a, b) = \sqrt{\sum_{i=1}^{n}(a_i - b_i)^2}
$$

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311220110685.png)

#### 点积
$$
a \cdot b = \|a\| \|b\| \cos(\theta)
$$
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311220116695.png)

### 元数据/标量数据筛选
存储在数据库中的每个向量还包括元数据。除了能够查询相似的向量之外，向量数据库还可以根据元数据查询筛选结果。所谓元数据，就是日期、文本标签、地理标签等。

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311220123003.png)

筛选过程可以在向量搜索本身之前或之后执行
- 预过滤：在这种方法中，元数据过滤是在向量搜索之前完成的。虽然这有助于减少搜索空间，但也可能导致系统忽略与元数据筛选条件不匹配的相关结果。此外，由于增加了计算开销，广泛的元数据筛选可能会减慢查询过程。
- 后过滤：在这种方法中，元数据过滤是在向量搜索之后完成的。这有助于确保考虑所有相关结果，但也可能会带来额外的开销并减慢查询过程，因为搜索完成后需要筛选掉不相关的结果。
## 向量数据库的具体应用
### LangChain

<img width=600 src="https://s1.ax1x.com/2023/04/16/p9CCejs.png">

即：

1. 找一些库把 PDF 内容文本提取出来。
2. 把这些文本拆分成 N 份更小的文本，用 openai 进行文本向量化。
3. 当用户提问时，对用户提问进行向量化，并用数学函数计算与 PDF 已向量化内容的相似程度。
4. 把最相似的文本用 Langchain 的 Prompt组装起来发送给 openai，让他总结并回答你的问题。
### 百川搜索+LLM

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202311212317133.png)

1. 基于Embedding对问题结合其聊天历史向量化解析 
2. 通过在线及离线方式检索融合知识库及网页端内容 
3. 基于LLM相关性模型进行精排再求解输出 
4. 大模型基于更全面与实时数据推理答案并召回数据

[^vector-json]: [VECTORS ARE THE NEW JSON IN POSTGRESQL](https://jkatz05.com/post/postgres/vectors-json-postgresql/)
[^vs]: https://www.pinecone.io/learn/vector-similarity/
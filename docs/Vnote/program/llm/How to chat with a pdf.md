## 1. 现有案例
- [chatDoc](https://chatdoc.com/chatdoc/#/chat/8fc7c110-8afe-45a5-9c6a-6446be454554)
- https://www.chatpdf.com

## 2. 实现原理
以下内容翻译自: [ChatGPT for YOUR OWN PDF files with LangChain](https://www.youtube.com/watch?v=TLf90ipMzfE&ab_channel=PromptEngineering) 与 [周刊](https://github.com/dt-fe/weekly)

事实上由于 GPT 非常强大，只要你把 PDF 文章内容发给他，他就可以解答你对于该文章的任何问题了。

但是因为 PDF 文章内容太长了，直接传给 GPT 很容易超出 Token 限制，就算他允许无限制的 Token 传输，可能一个问题可能需要花费 10~100 美元，这个 [成本](https://openai.com/pricing) 也是不可接受的。所以LangChain 提出了这样的流程：

<img width=600 src="https://s1.ax1x.com/2023/04/16/p9CCejs.png">

即：

1. 找一些库把 PDF 内容文本提取出来。
2. 把这些文本拆分成 N 份更小的文本，用 openai 进行文本向量化。
3. 当用户提问时，对用户提问进行向量化，并用数学函数计算与 PDF 已向量化内容的相似程度。
4. 把最相似的文本发送给 openai，让他总结并回答你的问题。


### 2.1. 登录 Colab

你可以在本地电脑运行 python 一步步执行，也可以直接登录 [colab](https://colab.research.google.com/) 这个 python 运行平台，它提供了很方便的 python 环境，并且可以一步步执行代码并保存，非常适合做研究。只要你有谷歌账号就可以使用 colab。

### 2.2. 安装依赖

要运行一堆 gpt 相关函数，需要安装一些包，虽然本质上都是不断给 gpt openapi 发 http 请求，但封装后确实会语义化很多：

```text
!pip install langchain
!pip install openai
!pip install PyPDF2
!pip install faiss-cpu
!pip install tiktoken
```

其中 `tiktoken` 包是教程里没有的，我执行某处代码时被提示缺少这个包，大家可以提前按上。接下来提前引入一些后面需要用到的函数：

```python
from PyPDF2 import PdfReader
from langchain.embeddings.openai import OpenAIEmbeddings
from langchain.text_splitter import CharacterTextSplitter
from langchain.vectorstores import ElasticVectorSearch, pinecone, Weaviate, FAISS
```

### 2.3. 定义 Openapi Token

为了调用 openapi 服务，需要先申请 token，当你申请到 token 后，通过如下方式定义：

```python
import os
os.environ["OPENAI_API_KEY"] = "***"
```

默认 `langchain` 与 `openai` 都会访问 python 环境的 `os.environ` 来寻找 token，所以这里定义后，接下来就可以直接调用服务了。

### 2.4. 读取 PDF 内容

为了方便在 colab 平台读取 PDF，你可以先把 PDF 上传到自己的 [Google Drive](https://www.google.com/drive/)，它是谷歌推出的个人云服务，集成了包括 colab 与文件存储等所有云服务（PS：微软类似的服务叫 [One Drive](https://www.microsoft.com/en-us/microsoft-365/onedrive/online-cloud-storage)）。

传上去之后，在 colab 运行如下代码，会弹开一个授权网页，授权后就可以访问你的 drive 路径下资源了：

```python
from google.colab import drive
drive.mount('/content/gdrive', force_remount=True)
root_dir = "/content/gdrive/My Drive/"
reader = PdfReader('/content/gdrive/My Drive/2023_GPT4All_Technical_Report.pdf')
```

我们读取了 `2023_GPT4All_Technical_Report.pdf` 报告，这是一个号称本地可跑对标 GPT4 的服务（[测评](https://sspai.com/post/79196)）。

### 2.5. 将 PDF 内容文本化并拆分为多个小 Chunk
首先执行如下代码读取 PDF 文本内容：

```python
raw_text = ''
for i, page in enumerate(reader.pages):
  text = page.extract_text()
  if text:
    raw_text += text
```

接下来要为调用 openapi 服务对文本向量化做准备，因为一次调用的 token 数量有限制，因此我们需要将一大段文本拆分为若干小文本：

```python
text_splitter = CharacterTextSplitter(
    separator = "\n",
    chunk_size = 1000,
    chunk_overlap = 200,
    length_function = len,
)
texts = text_splitter.split_text(raw_text)
```

其中 `chunk_size=1000` 表示一个 chunk 有 1000 个字符，而 `chunk_overlap` 表示下一个 chunk 会重复上一个 chunk 最后 200 字符的内容，方便给每个 chunk 做衔接，这样可以让找相似性的时候尽量多找几个 chunk--找到更多的上下文。

### 2.6. 向量化！

最重要的一步，利用 openapi 对之前拆分好的文本 chunk 做向量化：

```python
embeddings = OpenAIEmbeddings()
docsearch = FAISS.from_texts(texts, embeddings)
```

就是这么简单，`docsearch` 是一个封装对象，在这一步已经循环调用了若干次 openapi 接口将文本转化为非常长的向量。

文本向量化又是一个深水区，可以看下这个 [介绍视频](https://www.youtube.com/watch?v=bof9EdygMSo&ab_channel=What%27sAIbyLouisBouchard)，简单来说就是一把文本转化为一系列数字，表示 N 维的向量，利用数学计算相似度，可以把文字处理转化为连续的数字进行数学处理，甚至进行文字加减法（比如 北京-中国+美国=华盛顿）。

总之这一步之后，我们本地就拿到了各段文本与其向量的对应关系，比如 “这是一段文字” 对应的向量为 `[-0.231, 0.423, -0.2347831, ...]`。

### 2.7. 利用 Chain 生成问答服务
接下来要串起完整流程了，初始化一个 QA chain 表示与 GPT 使用 chat 模型进行问答：

```python
from langchain.chains.question_answering import load_qa_chain
from langchain.llms import OpenAI
chain = load_qa_chain(OpenAI(), chain_type="stuff")
```

接下来就可以问他 PDF 相关问题了：

```python
query = "who are the main author of the article?"
docs = docsearch.similarity_search(query)
chain.run(input_documents=docs, question=query)
#  The main authors of the article are Yuvanesh Anand, Zach Nussbaum, Brandon Duderstadt, Benjamin Schmidt, and Andriy Mulyar.
```

当然也可以用中文提问，openapi 会调用内置模块翻译给你：

```python
query = "训练 GPT4ALL 的成本是多少？"
docs = docsearch.similarity_search(query)
chain.run(input_documents=docs, question=query)
#  根据文章，大约四天的工作，800美元的GPU成本（包括几次失败的训练）和500美元的OpenAI API开销。我们发布的模型gpt4all-lora大约在Lambda Labs DGX A100 8x 80GB上需要八个小时的训练，总成本约为100美元。
```

### 2.8. QA 环节发生了什么？

根据我的理解，当你问出 `who are the main author of the article?` 这个问题时，发生了如下几步。

第一步：调用 openapi 将问题进行向量化，得到一堆向量。

第二步：利用数学函数与本地向量数据库进行匹配，找到匹配度最高的几个文本 chunk（之前我们拆分的 PDF 文本内容）。

第三步：把这些相关度最高的文本发送给 openapi，让他帮我们归纳。

当然，如果问题需要结合 PDF 所有内容才能概括出来，这种向量匹配的方式就不太行了，因为他总是发送与问题最相关的文本片段。

## 3. 总结
结合向量化与 GPT 这两个能力可以解决任意场景模糊化匹配以及需要人性化回答问题的场景。
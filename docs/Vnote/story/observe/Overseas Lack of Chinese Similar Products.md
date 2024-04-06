尝试让LLM帮忙找一下海外缺少的中国产品，没找到..

| Chinese Product | Overseas Similar(may be absent) | Field                   | Differences(list)                                                                                          |
| --------------- | ------------------------------- | ----------------------- | ---------------------------------------------------------------------------------------------------------- |
| 58同城            | Craigslist                      | Local classifieds       | 58同城 focuses on services, recruitment, and real estate, while Craigslist has a broader range of categories |
| 虎扑              | Reddit                          | Online community        | 虎扑 is a sports-focused community, while Reddit covers various topics                                       |
| 哔哩哔哩 (Bilibili) | YouTube, Twitch                 | Video platform          | Bilibili is known for its "bullet screen" feature and focuses on ACG (Anime, Comics, and Games) content    |
| 豆瓣 (Douban)     | Goodreads, IMDb                 | Media review and rating | Douban covers books, movies, and music, while Goodreads focuses on books and IMDb on movies                |
| 企鹅FM            | Spotify, Apple Podcasts         | Podcasts                | 企鹅FM is a podcast platform with more interactive features, such as live chat and virtual gifts             |
| 懒人听书            | Audible                         | Audiobooks              | 懒人听书 offers a wider range of free content, while Audible focuses on paid content                           |
| 作业帮             | Chegg                           | Education               | 作业帮 provides homework help and exam preparation, while Chegg focuses on textbook solutions and tutoring    |
| 丁香园             | WebMD                           | Medical information     | 丁香园 is a professional medical community and platform, while WebMD is a general health information website  |
| 马蜂窝             | TripAdvisor                     | Travel                  | 马蜂窝 focuses on user-generated travel content and guides, while TripAdvisor is more review-based            |
| 飞猪              | Expedia, Booking.com            | Travel booking          | 飞猪 offers a more comprehensive range of travel services, including local tours and activities              |


## Prompt
```markdown
- task: 分析海外缺乏中国有的哪些种类或有本质不同的互联网产品,尽可能关注细小的垂直领域？如果你在 ${result_ignore} 后没有结果,重复 ${task}.
- background:我希望发现潜在的中国中小企业互联网产品出海机会
- requirement
	- language: English
	- result_ignore: 中国大公司或者依赖实体的互联网产品可以忽略，比如微信、快手、抖音、支付宝、淘宝、拼多多、阿里云、网易云音乐、美团、小红书、喜马拉雅等
	- format: markdown table
		- table_column: Chinese Product、Overseas Similar(may be absent)、field、differences(list)
- example: 
	- 微博和Twitter 是同一种类产品，所以海外不缺乏微博
	- 知乎和Quora 是同一类产品；知识星球是粉丝经济的一个代表产品，海外有 OnlyFans，尽管它们设计和运营思路不一样，但是没有本质不同
	- 微信是一款社交应用，但集成了多种功能，如支付、小程序、小视频等，和Facebook 有本质不同，所以海外缺乏微信
```


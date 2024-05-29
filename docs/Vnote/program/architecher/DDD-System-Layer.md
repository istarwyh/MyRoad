|     | Command     | Query          | Event     |
| --- | ----------- | -------------- | --------- |
| 语意  | 希望触发的操作     | 条件查询           | 已经发生的事件通知 |
| 读/写 | 写           | 只读             | 取决于下游，通常写 |
| 返回值 | DTO/Boolean | DTO/Collection | Void      |

CQE 规范
- 创建：当只有单个ID查询时可省略CQE，否则应该创建对应CQE对象
- 校验：校验应该前置，字段上可定义注解，并在Controller/Facade 层校验
- 避免复用：CQE有自己的语义，可以基于基类产生它们，但应该避免复用
- 模型转化：以下类型推荐转为DP-- Domain Primitive, 也是`Value Object`,不可变对象后再使用，如果有需要在DP中进一步校验
	- 有限制的String、Integer
	- 复杂的数据结构，比如 `Map<String,Object>`
	- 

|            | Data Transfer Object       | Domain Primitive                              | Entity |
| ---------- | -------------------------- | --------------------------------------------- | ------ |
| 组成       | 基础字段                   | Type(独一的数据类型)与Class（逻辑聚合到类中） | DP     |
| 功能       | 数据传输，属于技术细节     | 业务域中的概念                                | 业务概念       |
| 数据的关联 | 只是一堆字段在一起，弱关联 | 字段高度相关                                  |DP的组合        |
| 行为       | 无行为                     | 有行为和业务逻辑                              | 有行为和业务逻辑        |


DDD不是什么特殊的架构，而是任何传统项目经过反复合理重构后会抵达的终点。

没有抵达之前糅合了MVC的代码架构可能是这样的：

<iframe frameborder="0" style="width:100%;height:433px;" src="https://viewer.diagrams.net/?tags=%7B%7D&highlight=0000ff&edit=_blank&layers=1&nav=1&title=#R7Ztbc6M2FIB%2FjWa2D9WgK9Ij2KR9aCe7TWf3mdhamwkGDyaX7a%2BvhMEIC8du10lwgifjgSMJgz6do3MhgExWT78V8Xr5Zz5XKcDe%2FAmQKcBYCKK%2FjeDHVsA9thUsimS%2BFaFWcJP8o2qhV0vvk7nadDqWeZ6WybornOVZpmZlRxYXRf7Y7fY9T7u%2Fuo4XyhHczOLUlX5L5uWyfizmtfLfVbJYNr%2BMvLrlNp7dLYr8Pqt%2FD2Byha6CKNo2r%2BLmWnX%2FzTKe54%2BWiESATIo8L7dHq6eJSs3UNtO2HXd1oHV334XKylMGUH874iFO71Vzy9WNlT%2BayTDPs3YvXP%2FWgypK9dRHJb5trtA%2BmV4wKl%2Bpsvih%2BzWjSD0Zu8WyPX1sZ541C2NpzTptxsU17cXu0u0T64P6oQ9MgPv8kQ%2FCEAhhDoIQBFcg4kAKEEaVhIIwABEDIQLCnayKvTIXR4CEj8ukVDfreGZaH7WqaNmyXKV18%2BZOlTPziJ4%2B%2BZ5n5SRP86K6EEFTwphvOpVFfqesFsr8UE7NiCRNLXkgpsF0ouUuqx3mfVgHoUgCuU8ol0T4PmON%2FlpMEKZQSF8yiQQxfbGLiHvQsz%2BI%2Fjwx1kNM05gA4VWgaAWKAxGBgJkmcQUC8m5BYSqhBYEL6pJiHCLdkda4fPY6pHgPKWH0KfQNoK0CaUBapfRda92SHhB8p1ufvqnb3ckv7xbgAU2rjaHY5zIYuj07R6R1TwBZmUcRgACBSBq11OY0quynRC3dG1U8JBrXRyeM%2FC4clzBBGLZ8BXklwKIHsLavGldlaMUUBFtDq00vqthr2KwFPA2uPzxcgvfA8MHso41LZfGdpImeiU8fgBoWHFo2s3HkamrYHy415FDr10rtpErP4afjjbU5nMdlvCnzQh2n2CL3hs3Ux1AwH1Fcf3cVUbpRhSAQc0Y8Xm%2BfwkVI%2B7ffnyKIR4KHIkGOILGRnMBQwJ3tRV6PEr4EQdJDsHJjJa88HgIC0ROCGImo%2BugwcmL%2Btg6TkNUC4GYDNaZXgpDUo8LDCyDLyxPYtwmJ6%2FsyTTJVy%2BdxcXetRyWlmVw9R%2BySFoox0Nxyej16dKEgn8N2nTQDXnql9ASrh3hukmyRqsBkrwyfpFCzMskz05Lfm6d4N2pOKOlR2ibgYce2Xoogt62E7EEpoex8zoDyhEzZZaEs8jKub%2BtXJLxzqaZEcLcF6%2B%2FLoItdZ%2FgvtblPS4B5vDJ09CEJv17vzhflbsKsFaAnqXRgZLkxux0OtchCbrjWiXBMm%2FP6wqbdEEhmcRqkycIgWyXzuWkM41ow09BU8f8Wz9ncr9Yf9lEHPDvgC9sW2qPQE2hHH%2FFXsdHY3c0nX6KRrGWvOYVYmk1zi4f5z6MdDFk3uT%2F9PIK1wUqonWzSJDB4h6vcs9VkMFxdr0ob63WebdT079ZCZ7eb9Wiju5szoRBZSYvnFXk4wHsqCZfte50D5qHEf5PEOo7zjXyt0ZM%2Bje8BZRs8357CwWXzPcteS0WnUNBhSQgfaFwk3xvLl4p6eQdPE1QMnS9x494L53seXX0mQYXPwpLsGW%2FPPwPMntrQZcN8GWUlHoJt0OP7mF0KYOwADq8dxB84vmEY7pUCsLdnNH0H5hsFNcTNO03H4PUY3BYU6xYHsdgDPRjObhZqVNpu1ZdDqwqvY9lL0WA3D%2FU5LuLVCBecWtIfrta6gc9YFeh6UYJB1BYF%2BF4Jfk9rxWCSidQNeW6%2B%2FDFuvM%2FoMKKQ2i%2B77SUv%2BLEXWN%2BKtOthjXWC09IZGmmr2QzLbuqRoJ73a96I8bgHH7fUpn5rvRrp%2FedCwVvBdes%2Bo%2Bvc1VMGrZCIe%2BRCyDJXbUeyHZ3Fxn86VJpHZKi1edTzJtVI1t1bLbJ0mGj1aft%2FyFWb9b%2FeJPoX"></iframe>

[[SystemLayer.drawio]]
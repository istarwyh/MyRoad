[commit的重要性](https://mp.weixin.qq.com/s/SDkOdyLpRkcf0epxhPQEVg)
[自带commit](https://mp.weixin.qq.com/s/5v6kmsi9dBSjy8U4Fy9kmg)
[sqlSession](https://mp.weixin.qq.com/s/J6RD25sV_EIA6xuiomJC2g)
[代理SqlSession](https://www.cnblogs.com/yougewe/p/10072740.html)


默认单例的sqlSession并不会被释放，但是每次操作都会被代理生成新的sqlSession,那个会被释放


怎么样在生成代理类的时候自动绑定xml中的SQL语句

使用了数据库连接池，mybatis的sqlsession还要关闭吗
https://blog.csdn.net/u011649691/article/details/116045188 SqlSession与其他的关系
ReuseExecutor：顾名思义就是重复使用执行，其定义了一个Map<String, Statement>，将执行的sql作为key，将执行的Statement作为value保存，这样执行相同的sql时就可以使用已经存在的Statement，就不需要新创建了
https://xie.infoq.cn/article/80dd2bc5cad87320de3fa0478?source=app_share JDBC
https://juejin.cn/post/6844904002421800967 理论上来说，自己使用sqlSession的时候怎么开启事务？不开启的话，每一次都会被自动提交掉？
对于需要事务的引擎，如果不提交，是不能改变数据库的 https://louluan.blog.csdn.net/article/details/37992171?spm=1001.2101.3001.6650.10&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-10-37992171-blog-121394975.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-10-37992171-blog-121394975.pc_relevant_default&utm_relevant_index=14


## JPA Vs Mybatis
优势：

MyBatis 是一个可以灵活编写 sql 语句

MyBatis 避免了几乎全部的 JDBC 代码和手动设置参数以及获取结果集，相比 JDBC 更方便


MyBatis 与 JPA 的差异点：

设计哲学不同，MyBatis 偏向于面向过程，JPA 则将面向对象发挥到极致；

MyBatis 定制起来更加灵活，支持高度定制化的 sql 语句，支持任意编写 sql 语句；JPA 相对更注重对已有高频简单操作场景的封装，简化开发人员的重复操作，虽然 JPA 也支持定制 SQL 语句，但是相比 MyBatis 灵活度略差。


至此，到底如何在 JPA 与 MyBatis 之间抉择，就比较清晰了：

如果你的系统中对 DB 的操作没有太多额外的深度定制、对 DB 的执行性能也不是极度敏感、不需要基于 SQL 语句做一些深度的优化，大部分场景都是一些基础 CRUD 操作，则无疑 Spring Data JPA 是比较理想的选择，它将大大降低开发人员在 DB 操作层面的投入精力。

如果你的业务中对 DB 高阶逻辑依赖太深，比如大部分场景都需要额外定制复杂 SQL 语句来实现，或者系统对性能极其敏感，需要基于 Table 甚至 column 维度进行深度优化，或者数据量特别巨大的场景，则相比较而言，MyBatis 提供的调优定制灵活性上要更有优势一些。
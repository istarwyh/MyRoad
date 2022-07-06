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
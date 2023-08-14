假设我有如下 Bean：
@Service
public class A {
    @Autowired
    B b;
}
@Service
public class B {
    @Autowired
    A a;
}
就这样一个简单的循环依赖，默认情况下，A 会被先加载，然后在 A 中做属性填充的时候，去创建了 B，创建 B 的时候又需要 A，之后呢？请详细描述 Spring 如何解决这个问题的。


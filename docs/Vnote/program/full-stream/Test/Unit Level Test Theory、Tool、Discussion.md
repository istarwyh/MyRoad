## 为什么写测试？
### 测试左移
[[Java Fuzz Test]]
### 方便重构
每次commit 之后自动执行测试。
### 描述功能
真正随着代码变更的“PRD”。
## 面向功能开发- TDD（Test Driven Development)
>测试应当与生产代码解耦 --Bob大叔《匠艺整洁之道:程序员的职业修养》

[使用JUnit5与芝加哥学派TDD驱动产出一个简单的Stack类]( https://www.bilibili.com/video/BV1pd4y147MP/?share_source=copy_web)

- 伦敦学派
	- 预先设计
	- 调用栈
- 芝加哥学派
	- 不假设任何实现，演进式重构

伦敦学派强调按调用栈自外而内的写测试，芝加哥强调按功能写测试然后再从内重构。如果一开始就胸有成竹、有能力顶层设计的好就按伦敦学派来，不然一步步按芝加哥学派重构也可以得到更好的代码。
🌰
`foo()` 处理业务数据之前有一些校验逻辑
- 伦敦学派会设计好一个`validator`的接口，再专注于实现这个`validator`
- 芝加哥学派则会选择直接先在`foo()`中实现各种校验逻辑，再视情况逐渐重构出一个`validator`
## 测试工具和框架介绍
### [[JUnit5 & Mockito]] 好基友

### TestMe 少写重复的测试
IDEA上的`testme`插件，相比有人可能用过`squareTest`这些测试代码自动生成工具,但是先不说它是收费的,有时候它默认生成的数据类真的一言难尽,实在不如自己写的好用,这里我提供一个用于包装上游调用接口的通用模板,看一下应该不难看懂:)

```java
#parse("TestMe macros.java")
#set($hasMocks=$MockitoMockBuilder.hasMockable($TESTED_CLASS.fields))
#if($PACKAGE_NAME)
package ${PACKAGE_NAME};
#end

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Nested;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.extension.ExtendWith;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import java.util.stream.Stream;
#if($hasMock)
import org.mockito.junit.jupiter.MockitoExtension;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.when;
#end

#parse("File Header.java")
#if($hasMock)
@ExtendWith(MockitoExtension.class)
#end
class ${CLASS_NAME}{
#renderMockedFields($TESTED_CLASS.fields)
#renderTestSubjectInit($TESTED_CLASS,$TestSubjectUtils.hasTestableInstanceMethod($TESTED_CLASS.methods),$hasMocks)

private String testErrorCode;
private String testErrorMsg;

@BeforeEach
void setUp(){
    testErrorCode = "testErrorCode";
    testErrorMsg = "testErrorMsg";
}

#foreach($method in $TESTED_CLASS.methods)
    #if($TestSubjectUtils.shouldBeTested($method))

        @Nested
        @TestInstance(TestInstance.Lifecycle.PER_CLASS)
        class #renderTestMethodName($method.name){
            
            //exception
            @Test
            void should_throw_exception_when_upstream_input_null(){}
            
            //exception
            @Test
            void should_throw_exception_when_upstream_invoke_failed(){}
            
            //exception
            @Test
            void should_throw_exception_when_upstream_return_null(){}
            
            //default value
            @Test
            void should_set_default_value_to_empty_optional_or_list_when_not_success(){}
            
            @ParameterizedTest()
            @MethodSource("resultOfUpstream")
            void should_return_the_same_when_get_normal_data(Object resultOfUpstream){}
            
            Stream<Arguments> resultOfUpstream(){
                return Stream.of();
            }
        }    

    #end
#end
}
```

### [ junit5-extension](https://github.com/istarwyh/TestMuseum/tree/main/junit-extensions): 致力于一切想测的都能测
[[Test Need Geek]]

## 最后讨论一下

### GWT ？
1. Given:情景/条件
2. When:采取什么行动
3. Then:得到什么结果

### 集成测试 Vs 单元测试？
之所以写测试，是因为正确的测试确实可以提高开发效率，所以选择怎样写测试的判断标准之一就是要找到`ROI`(Return Of Investment) 最高的截面。柳胜认为测试的ROI计算公式为[^3KU]
>![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202309091459177.png)

在测试金字塔模型下，越往底部，测试的ROI越高。又因为UI 测试关注功能场景测试，易用性测试和可执行性测试；而接口测试关注不同数据的循环，接口的性能和错误恢复能力；单元测试关注算法的正确性和性能。所以各个测试专注的范围应该如下图所示：
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202309091458922.png)

在我的理解中，上图中的UI测试通常也被称为`端到端测试`,对外的`接口测试`对后端通常也就是`集成测试`。而单元测试是什么呢？是针对一个类或者某个方法的测试吗？不是，徐昊指出`单元测试`（Unit Test）是一个具有误导性的提法，在TDD中不应该有这样的说法，应代之以`单元级别功能测试`（Unit Level Functional Test）,[^IntegrationVsUnit]其本质是**能提供快速反馈的低成本的研发测试**[^Speed]。柳胜之所以认为测试金字塔模型下，越往底部，测试的ROI越高，是因为越往下手工运行时间是越短的，运行次数是越多的；而越往上开发测试和维护测试的成本都越高。

同时柳胜认为[^HappyPath]

>集成测试是处在单元测试和端到端测试中间的一个状态。如果所有的外部服务都 Mock 了，集成测试就变成了单元测试，往另外一个方向，如果所有的外部服务都是真实的，集成测试又变成了端到端的测试。

集成测试通常ROI不如单元测试，并且和单元测试测试内容有较多重合，所以两者使用需要追求平衡，[柳胜提出](https://time.geekbang.org/column/article/507443)
>**1. 在单元测试阶段验证尽可能多的业务逻辑，这样能让集成测试关注在外部依赖上。
>2. 在依赖可控的情况下，集成测试应走尽可能多的**真实的**外部依赖服务。**

可控的依赖比如数据库，就应该尽可能真实地去交互；不可控的如第三方接口，应该尽可能用定好的逻辑来Mock接口。
![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202307232321182.png)


### Mock Vs Real?
能Real 肯定Real。对于完全可控依赖的服务，虽然是应用之外的一个进程，但可以把跟它的交互当作开发的内部实现。
而不可控依赖服务，比如公共的服务，做Real 凭空增加服务方QPS，不是自己能决定的，基于交互契约来Mock即可。

![](https://xiaohui-zhangjiakou.oss-cn-zhangjiakou.aliyuncs.com/image/202307232327590.png)


### 是否需要测试私有方法?
这是一个还没有定论的[话题](https://jesseduffield.com/Testing-Private-Methods/).一般来说,抽象层次越高,对于测试越不友好,表现在:

- 更多时间运行
- mock更多的数据
- 需要mock更复杂的行为交互
- 更容易因为小改动而失败

但是也更容易**重构代码和测试**.
>The higher the level of encapsulation, the harder to test, but the lower the level of encapsulation, the harder to refactor.

另外,考虑到[[Code-or-Design-Pattern#SRP(Single Responsibility Program)|单一职责原则]],`Working With Legacy Code`中指出

>If we need to test a private method, we should make it public. If making it public bothers us, in most cases, it means that our class is doing too much and we ought to fix it .

如何测试私有方法便是在**易用性**,**重构性**和**SRP**作权衡.
通常来说:

>1. Try to have as slim a public interface as possible in your classes, by defaulting every method to private. 
>2. If you find yourself wanting to test a set of private methods directly, seriously consider extracting a class (or standalone function), but only if it makes sense independent of your testing desires. 
>3. If you want to test a single private method and don't see the point in extracting it out of the class, convert it into a pure function (no references to instance variables) and test that method. That way, if later on you decide to move the function somewhere else, moving the tests is as simple as copy+paste.

### 其他测试类型
![](https://substackcdn.com/image/fetch/w_550,c_limit,f_auto,q_auto:good,fl_lossy/https%3A%2F%2Fsubstack-post-media.s3.amazonaws.com%2Fpublic%2Fimages%2Ff4886cb0-e840-4d4a-84c7-8d9543d98101_1280x1664.gif)

[^3KU]:[3KU法则：如何找出最优自动化实施截面？](https://time.geekbang.org/column/article/497405)
[^IntegrationVsUnit]:[TDD中的测试（3）：集成测试还是单元测试？](https://time.geekbang.org/column/article/496699)
[^Speed]:[UnitTest.html](https://martinfowler.com/bliki/UnitTest.html)
[^HappyPath]:[集成测试（一）：一条Happy Path扫天下](https://time.geekbang.org/column/article/507443)
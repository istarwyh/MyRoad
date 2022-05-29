在JUnit4发布十年之后,2017年JUnit团队靠众筹推出了全新的[JUnit5](https://junit.org/junit5/docs/current/user-guide/#overview-what-is-junit-5).

<center>JUnit 5 = JUnit Platform + JUnit Jupiter + JUnit Vintage</center>

- JUnit Platform: 在 JVM 上[启动测试框架](https://junit.org/junit5/docs/current/user-guide/#launcher-api)的基础，不仅支持 Junit 的测试引擎[TestEngine](https://junit.org/junit5/docs/current/api/org.junit.platform.engine/org/junit/platform/engine/TestEngine.html)，其他测试引擎也都可以接入。

- JUnit Jupiter: JUnit Jupiter同时支持JUnit5新的[programming model ](https://junit.org/junit5/docs/current/user-guide/#writing-tests)以及老的[Extension Model](https://junit.org/junit5/docs/current/user-guide/#extensions).Jupiter实现了自己的TestEngine。

- JUnit Vintage: 为了向后兼容提供的TestEngine,支持运行JUnit4,Junit3的测试.注意类路径或者模块路径至少包含JUnit 4.12以以上版本.

![](https://gitee.com/istarwyh/images/raw/master/vnote/程序员练级之路/工程实践/测试/junit5&mockito.md/450935910220547.png)

虽然包括三个部分,不过最新版本引入`org.junit.jupiter:junit-jupiter`就可以了,核心注解都在`org.junit.jupiter.api`下.
## JUnit5特性
### 新的注解
下面按照我个人经验列举JUni5的新注解,更多的在[这里](https://junit.org/junit5/docs/current/user-guide/#overview-what-is-junit-5):


|     Annotation     |                                            描述                                             |
| ------------------ | ------------------------------------------------------------------------------------------- |
| @Test              | 和 JUnit4 的 @Test 不同，这个@Test不能声明任何属性，Jupiter会为不同的test extension提供专门注解 |
| @ParameterizedTest | 表示方法是参数化测试                                                                         |
| @RepeatedTest      | 表示方法可重复执行                                                                           |
| @DisplayName       | 为测试类或者测试方法设置展示名称,支持emoji😄                                                  |
| @BeforeEach        | 表示在每个单元测试之前执行                                                                    |
| @AfterEach         | 表示在每个单元测试之后执行                                                                    |
| @BeforeAll         | 表示在所有单元测试之前执行                                                                    |
| @AfterAll          | 表示在所有单元测试之后执行                                                                    |
| @Disabled          | 表示测试类或测试方法不执行，类似于 JUnit4 中的 @Ignore                                         |
| @Timeout           | 表示测试方法运行如果超过了指定时间将会返回错误                                                  |
| @Nested            | 该注解允许在测试类中定义非静态测试类.@BeforeAll与@AfterAll不直接适用于@Nested测试类              |
| @TestClassOrder    | 指定测试类的执行顺序                                                                         |
| @TestMethodOrder   | 指定测试方法的执行顺序                                                                        |
| @ExtendWith        | 为测试类或测试方法甚至字段提供一个或多个扩展环境                                                |
           
### 新的特性
#### 超时断言
```java
@Test
@DisplayName("超时测试")
public void timeoutTest() {
    //如果测试方法时间超过1s将会异常
    Assertions.assertTimeout(Duration.ofMillis(1000), () -> Thread.sleep(500));
}
```
#### 参数化测试
以下为部分介绍,更多细节[在这儿](https://junit.org/junit5/docs/current/user-guide/#writing-tests-parameterized-tests).
##### 通过注解属性传入参数
- @ValueSource: 为参数化测试指定入参来源，支持八大基础类以及 String 类型, Class 类型

- @EmptySource: 提供空白数组或空白集合,支持八大基础类及它们包装类以及 String 类型, 集合类型

- @NullSource: 表示为参数化测试提供一个 null 的入参

- @EnumSource: 表示为参数化测试提供一个枚举入参

```java
@ParameterizedTest
@ValueSource(strings = { "racecar", "radar", "able was I ere I saw elba" })
void palindromes(String candidate) {
    assertTrue(StringUtils.isPalindrome(candidate));
}
```

```java
@ParameterizedTest
@NullSource
@EmptySource
@ValueSource(strings = { " ", "   ", "\t", "\n" })
void nullEmptyAndBlankStrings(String text) {
    assertTrue(text == null || text.trim().isEmpty());
}
```
##### 通过静态方法名/指定接口类传入参数
- @MethodSource：读取静态方法的Stream流作为参数化测试入参

```java
@ParameterizedTest
@MethodSource("stringIntAndListProvider")
void testWithMultiArgMethodSource(String str, int num, List<String> list) {
    assertEquals(5, str.length());
    assertTrue(num >=1 && num <=2);
    assertEquals(2, list.size());
}

static Stream<Arguments> stringIntAndListProvider() {
    return Stream.of(
        arguments("apple", 1, Arrays.asList("a", "b")),
        arguments("lemon", 2, Arrays.asList("x", "y"))
    );
}
```
- @ArgumentsSource: 读取实现了ArgumentsProvider接口的类中方法返回流作为入参
    
```java
@ParameterizedTest
@ArgumentsSource(MyArgumentsProvider.class)
void testWithArgumentsSource(String argument) {
    assertNotNull(argument);
}
public class MyArgumentsProvider implements ArgumentsProvider {

    @Override
    public Stream<? extends Arguments> provideArguments(ExtensionContext context) {
        return Stream.of("apple", "banana").map(Arguments::of);
    }
}
```
##### 通过文件格式数据传入参数
- @CsvSource：表示读取指定 CSV内容作为参数化测试入参

```java
@ParameterizedTest
@CsvSource({
    "Jane, Doe, F, 1990-05-20",
    "John, Doe, M, 1990-10-22"
})
void testWithArgumentsAggregator(@AggregateWith(PersonAggregator.class) Person person) {
    // perform assertions against person
}
public static class PersonAggregator implements ArgumentsAggregator {
    @Override
    public Person aggregateArguments(ArgumentsAccessor arguments, ParameterContext context) {
        return new Person(arguments.getString(0),
                          arguments.getString(1),
                          arguments.get(2, Gender.class),
                          arguments.get(3, LocalDate.class));
    }
}
```
- @CsvFileSource：表示读取指定 CSV 文件内容作为参数化测试入参

```java
@ParameterizedTest
@CsvFileSource(resources = "/two-column.csv", numLinesToSkip = 1)
void testWithCsvFileSourceFromClasspath(String input, int output) {
    assertNotNull(input);
    assertEquals(input, output);
}
```

##### 评价
1. 参数化测试相当于是合并了多个单元测试输入输出数据的"缩写",所以通常会有代表input和output的输入输出.当input都对应相同的output时,可以省略output.

2. 通过外部文件作为参数构造文件,就可以**将测试逻辑与准备数据充分解耦**.具体实现除了官方支持的CSV ,想支持其他格式,如JSON/YAML

    1. 可以转成对应的CSV
    2. 自己从文件路径中读取文件,再转成Stream,通过`@MethodSource`或`@ArgumentsSource`实现入参

#### 对类中单元测试分组
如果一个Service类中方法较多,单纯写单元测试也会很多.@Nested 可以允许以静态内部成员类的形式对测试用例类进行逻辑分组.\
下面是一个测试Stack功能的例子
```java
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.EmptyStackException;
import java.util.Stack;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Nested;
import org.junit.jupiter.api.Test;

@DisplayName("A stack")
class TestingAStackDemo {

    Stack<Object> stack;

    @Test
    @DisplayName("is instantiated with new Stack()")
    void isInstantiatedWithNew() {
        new Stack<>();
    }

    @Nested
    @DisplayName("when new")
    class WhenNew {

        @BeforeEach
        void createNewStack() {
            stack = new Stack<>();
        }

        @Test
        @DisplayName("is empty")
        void isEmpty() {
            assertTrue(stack.isEmpty());
        }

        @Test
        @DisplayName("throws EmptyStackException when popped")
        void throwsExceptionWhenPopped() {
            assertThrows(EmptyStackException.class, stack::pop);
        }

        @Test
        @DisplayName("throws EmptyStackException when peeked")
        void throwsExceptionWhenPeeked() {
            assertThrows(EmptyStackException.class, stack::peek);
        }

        @Nested
        @DisplayName("after pushing an element")
        class AfterPushing {

            String anElement = "an element";

            @BeforeEach
            void pushAnElement() {
                stack.push(anElement);
            }

            @Test
            @DisplayName("it is no longer empty")
            void isNotEmpty() {
                assertFalse(stack.isEmpty());
            }

            @Test
            @DisplayName("returns the element when popped and is empty")
            void returnElementWhenPopped() {
                assertEquals(anElement, stack.pop());
                assertTrue(stack.isEmpty());
            }

            @Test
            @DisplayName("returns the element when peeked but remains not empty")
            void returnElementWhenPeeked() {
                assertEquals(anElement, stack.peek());
                assertFalse(stack.isEmpty());
            }
        }
    }
}
```
![](https://junit.org/junit5/docs/current/user-guide/images/writing-tests_nested_test_ide.png)
### JUnit5原理
单独的JUnit5其实是难以使用的,通常IDE或者代码管理工具,比如IntelliJ IDEA, Eclipse, NetBeans, Visual Studio Code, Gradle, Maven都会对JUnit5进行集成,从而让测试对开发更友好.所以以IDEA+JUnit5为例,第一步其实是从IDEA[内部插件](https://github.com/JetBrains/intellij-community/tree/61fb94acd0e337972338618b58c38a4509aefcff/plugins/junit5_rt/src/com/intellij/junit5)代码开始的.

1. 触发测试进入插件源码,com.intellij.rt.junit.JUnitStarter::main
2. IDEA构造DefaultDiscoveryRequest对象传入org.junit.platform.launcher.core::execute

```java
/**
 * {@code DefaultDiscoveryRequest} is the default implementation of the
 * {@link EngineDiscoveryRequest} and {@link LauncherDiscoveryRequest} APIs.
 *
 * @since 1.0
 */
final class DefaultDiscoveryRequest implements LauncherDiscoveryRequest {

  // Selectors provided to the engines to be used for discovering tests
  private final List<DiscoverySelector> selectors;

  // Discovery filters are handed through to all engines to be applied during discovery.
  private final List<DiscoveryFilter<?>> discoveryFilters;

  }
```
3. 解析测试用例生成测试计划
- ![](https://gitee.com/istarwyh/images/raw/master/vnote/程序员练级之路/工程实践/测试/junit5&mockito.md/492251015238973.png)
4. 选择具体的测试引擎执行用例,以JupiterTestEngine为例,会构造JupiterEngineDescriptor
- ![](https://gitee.com/istarwyh/images/raw/master/vnote/程序员练级之路/工程实践/测试/junit5&mockito.md/90941715226840.png)
5. 生成NodeTeskTask然后交给ExecutorService去执行(反射调用具体方法)
6. 实际执行时会根据注解先去找实现的扩展类,比如启动Spring时的SpringExtension、Mock依赖的 MockitoExtension

## Mockito
### Mockito原理
### 常用注解
#### 介绍
|  Annotation  |                                         描述                                          |
| ------------ | ------------------------------------------------------------------------------------ |
| @Mock        | @Mock修饰的对象都是null,用到的每个方法都需要打桩模拟执行结果: Mockito.when().thenReturn() |
| @Spy         | @Spy的对象会被无参实例化,在需要的时候可以打桩模拟执行结果: Mockito.doReturn().when()       |
| @MockBean    | 启动Spring容器,替换Spring原本加载的Bean,但是默认对象没有行为                              |
| @SpyBean     | 启动Spring容器,替换Spring原本加载的Bean,对象拥有默认行为                                 |
| @InjectMocks | 注入mock代理对象;必须修饰实现类,修饰接口会报错                                            |
| @Captor      | 捕获调用时的参数值                                                                      |

其他说明:

1. 使用`@Spy`的前提是对象可以被使用无参构造器初始化,因为需要得到一个空对象然后来执行它的方法.这也意味着`@Spy`修饰的属性不能被注入mock代理对象。
2. `@Spy` 修饰接口不会报错,不过因为接口没有实现逻辑,所以不打桩模拟的时候,接口方法永远返回`null`。

@Spy 与 @Mock 测试案例:

```java
    @Test
    public void whenCreateMock_thenCreated() {
        List mockedList = Mockito.mock(ArrayList.class);

        mockedList.add("one");
        Mockito.verify(mockedList).add("one");

        assertEquals(0, mockedList.size());
    }

    @Test
    public void whenCreateSpy_thenCreate() {
        List spyList = Mockito.spy(new ArrayList());

        spyList.add("one");
        Mockito.verify(spyList).add("one");

        assertEquals(1, spyList.size());
    }
```
#### 使用建议
[Mockito Patterns](https://stackoverflow.com/questions/11462697/forming-mockito-grammars): 
> When/Then: when(yourMethod()).thenReturn(5);
Given/Will: given(yourMethod()).willThrow(OutOfMemoryException.class);
Do/When: doReturn(7).when(yourMock.fizzBuzz());
Will/Given/Do: willReturn(any()).given(yourMethod()).doNothing();
Verify/Do: verify(yourMethod()).doThrow(SomeException.class);

1. 一般来说,`@Spy`修饰实现类、`@InjectMocks`修饰需要mock属性的实现类、`@Mock`修饰接口
2. 默认使用`@Spy`或`@SpyBean`,有需要打桩模拟返回结果的情况可以自定义模拟返回结果,尽可能的覆盖更多的代码逻辑
3. 对无法直接实例化三方依赖,比如下游接口、Redis等使用`@Mock`;没有Mock到的依赖会NPE,逐个Mock即可
5. 检查`void`方法的执行情况可以使用`verify/times`校验次数和`@Captor`校验参数
4. 私有方法和静态方法希望mock可以使用powermock
5. 使用这种测试框架最麻烦的在于真实生产代码中测试用例中复杂对象的构造,链路录制工具可以帮助生成请求与返回结构体

关于`when/then`以及`doxxx/when`,根据这个[回答](https://stackoverflow.com/questions/20353846/mockito-difference-between-doreturn-and-when),以下情况都应该使用后者:

1. mock `void`方法时,使用`doNothing/when`(因为这时并不会真的执行when中的方法)
2. spy对象的时候
3. 对于方法不止一次打桩(?)
## 测试建议与不足
### 一般测试流程
#### GWT 
Given:情景/条件
When:采取什么行动
Then:得到什么结果
#### 是否需要测试私有方法?
https://jesseduffield.com/Testing-Private-Methods/

抽象层次越高,对于测试:

- 更多时间运行
- mock更多的数据
- 需要mock更复杂的行为交互
- 测试更容易因为小改动而失败

但是也更容易重构代码和测试.
The higher the level of encapsulation, the harder to test, but the lower the level of encapsulation, the harder to refactor.
另外,考虑到Single Responsibility Principle,`Working With Legacy Code` 中指出
>If we need to test a private method, we should make it public. If making it public bothers us, in most cases, it means that our class is doing too much and we ought to fix it .

如何测试私有方法便是在**易用性**,**重构性**和**SRP**作权衡.
通常来说:

>1. Try to have as slim a public interface as possible in your classes, by defaulting every method to private. 
>2. If you find yourself wanting to test a set of private methods directly, seriously consider extracting a class (or standalone function), but only if it makes sense independent of your testing desires. 
>3. If you want to test a single private method and don’t see the point in extracting it out of the class, convert it into a pure function (no references to instance variables) and test that method. That way, if later on you decide to move the function somewhere else, moving the tests is as simple as copy+paste.

### 需要完善的测试框架
(或者还没发现的测试工具)


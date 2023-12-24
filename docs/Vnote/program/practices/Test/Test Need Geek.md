## 1. How to Get an Object Instance Limitlessly?
### 1.1. 利用Unsafe直接分配内存
```java
<T> T allocateInstance(Class<T> type) {
    Field unsafe = Unsafe.class.getDeclaredField("theUnsafe");
    unsafe.setAccessible(true);
    Unsafe u = (Unsafe)unsafe.get(Unsafe.class);
    return (T) u.allocateInstance(type);
}
```

但是如果类加载过程中就会有报错如下,Unsafe也是没办法的:
```java
public class CannotInitialDueToLoadingClassError {
    public static final Integer id;

    static {
        id = 1 / 0;
    }
}
```
### 1.2. 修改加载时字节码
如果可以直接替换类加载器，从而可以在类加载的过程中修改其字节码，那么就无论什么类都可以被实例化了。通过这样的方法，实际上也可以对类做任意的动作了。
比如通过 [TestMuseum](https://github.com/istarwyh/TestMuseum):tdd.args.geek.modifier.Modifier 来实现`CannotInitialDueToLoadingClassError` 的实例化：
```java
public class CannotInitialByLoadingClassErrorModifier implements Modifier {

    @Override
    @SneakyThrows
    public void afterLoadClass(String className, CtClass ctClass) {
        Class<?> startClass = CannotInitialDueToLoadingClassError.class;
        while (startClass != Object.class){
            if(startClass.getName().equals(className)){
                wipeOriginalByteCode(ctClass);
            }
            startClass = startClass.getSuperclass();
        }
    }
}

```
## 2. How to Set a Field Limitlessly?
### 2.1. 一般的反射
我想要将instance塞到field中:

```java
var declareField = TestClass.getDeclaredField("fieldName");
declareField.setAccessible(true);
declareField.set(testClass,instance);
```
### 2.2. 无限制的反射
[TestMuseum](https://github.com/istarwyh/TestMuseum):tdd.args.utils.ReflectionUtil

## 3. How to Mock Middleware Locally?
在公司里，与公司还有日常环境数据库可以提供测试之外，通常公司中的中间件都没有本地能够启动的版本，比如阿里的Redis、RocketMq,所以就和内存数据库H2可以作为日常环境数据库的替身一样，各种中间件的本地版本该怎么Mock呢？
如果只是方法中少次用到，每次mock返回值是可行的。但如果是并发或者循环调用这种中间件就得有个真实执行的mock。
一种思路是直接转发本地请求到本地的Mock类里面，这个思路的问题就在于工作量太大，得写一个完整的Mock类。
## How to Use JSON as Parameterized Test?
[TestMuseum](https://github.com/istarwyh/TestMuseum):tdd.args.utils.ReflectionUtil
🌰
[[Practices-for-Processing-l0-Billion-Bill-data#2.2.3. 参数化测试]]
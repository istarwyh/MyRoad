## How to get an Object Instance limitlessly?
### 利用Unsafe直接分配内存
```java
    <T> T allocateInstance(Class<T> type) {
        Field unsafe = Unsafe.class.getDeclaredField("theUnsafe");
        unsafe.setAccessible(true);
        Unsafe u = (Unsafe)unsafe.get(Unsafe.class);
        return (T) u.allocateInstance(type);
    }
```

但是如果类加载过程中就会有报错如下,unsafe也是没办法的:
```java
    private static final Integer id;

    static {
        id = 1/0;
    }
```
### JDK serviceLoader
清空字节码,实例化空对象(连成员变量都被干掉?)
## How to set a field limitlessly?
我想要将instance塞到field中:
```java
var declareField = TestClass.getDeclaredField("fieldName");
declareField.setAccessible(true);
declareField.set(testClass,instance);
```

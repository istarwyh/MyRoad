

## 1. 初识字符串源码
### 1.1. JDK8
```java
public final class String
    implements java.io.Serializable, Comparable<String>, CharSequence  {
    private final char value[];
    public String(){
        this.value = "".value;
    }
}

```
两个`final`，一个`private`，并且不提供`set()`决定了String是一个不可变类。为何这样设计呢？

- 为了复用（字符串常量池）
- 安全：
    - String在Java中被广泛应用于类的参数，包括网络请求，打开文件等操作，若String是可变的，String字面量的更改可能会引起连接或文件被错误访问
    - String这样的核心类，需要通过`JNI`(Java Native Interface)结合操作系统完成，如果String可以被继承并重写方法，很有可能对操作系统带来威胁[^String设计]
- 并发：String的方法均没有加任何锁，但由于它的不可变性，使得String对象可以在多个线程之间自由共享

### 1.2. JDK 11
```java
public final class String
    implements java.io.Serializable, Comparable<String>, CharSequence  {
    private final byte value[];
    // The identifier of the encoding used to encode the bytes in value. The supported values in this implementation are LATIN1 UTF16
    private final byte coder;
        public String() {
        this.value = "".value;
        this.coder = "".coder;
    }
}
```

**1char = 2byte**,而在经过调查后发现程序中大多只包含英文字母、数字这种1byte就可以表示的字符(比如采用LATIN1编码格式),这意味着:当String是奇数个英文字母或者[-128-127]时,存储为`char[]`至少浪费一位`byte`.

因此JDK11相比8将`char[]`数组改为了`byte[]`数组,并且引入coder以指明String的编码格式:

- LATIN1:1byte存储,存储传统ASCII字符
- UTF16: 2byte或4byte存储

Java会根据字符串的内容自动设置为相应的编码,要么Latin-1要么UTF16. 改进后程序在存储英文字母或[-128-127]内数字时,可以减少内存占用,进而也减少GC次数.
那么为什么不采用UTF8呢?
因为UTF8允许一个字符串中使用2个byte或3个byte或4个byte来存储,这样没有办法将字符串表示为多少多少char的形式,从而 对于substring、charAt这种以char为基本操作单位的方法就很不友好;而UTF16虽然也是变长的,但是因为都是2n(n<>0)个byte存储,所以对String的各种以char为基础的操作没有影响.

## 2. 字符串间什么时候相等?
### 2.1. 创建字符串
```java
String str = "abc";
```
is equivalent to:
```java
char data[] = {'a', 'b', 'c'};
String str = new String(data);
```
### 2.2. 字符串常量池

>**字符串常量池本质上是个哈希表，它存储的是字符串实例对象的引用![^jvm]**

```java
/**
 * @date 2020年7月1日
 * @JDK 8
 */
public class Solution {

    public static void main(String[] a){
     /**
         * 如果是通过new关键字加StringBuilder/StringBuffer的方式创建字符串对象,字符串常量池中一开始没有它的引用
         */
        String s1 = new StringBuilder("se").append("ven").toString();
        String s2 = new StringBuilder("se").append("ven").toString();

//        但是在JDK7之后，调用intern()时会发现堆上已有“seven”实例，故复制s1这个引用加入字符串常量池中，并返回这个引用
        System.out.println(s1.intern() == s1); // 2. true
//        s2.intern()与2.同理返回的也是s1
        System.out.println(s2.intern() == s1); // 3. true
//        但是s2本身通过了new关键字又会在堆上创建一个同样的“seven”实例，但是有着不一样的引用
        System.out.println(s1.intern() == s2); // 4. false

        /**
         * 而直接赋值,其实执行的是以下代码:
         *             String s;
         *             s = findInStringTable("dija");
         *             if(s!=null) return s;
         *             s = new String("dija");
         *             s.intern();
         *             return s;
         *  可以发现一定会有这个字符串实例的引用被加到了字符串常量池中
         */

        String s3 = "dijia";

        String s4 = new StringBuilder("di").append("jia").toString();
//        因为字符串常量池中已经存在了s3的引用，故返回的是s3
        System.out.println(s4.intern() == s3); // 5. true
//        与4.同理
        System.out.println(s4.intern() == s4); // 6. false，

        /**
         * 使用String的构造函数反而是很低效的创建字符串的方式
         *      因为在解析常量“daina”的时候是第一次解析，JVM会在堆中创建“daina”这个实例，并把其引用添加到字符串常量池中（{1}）
         *      而当遇到new时，则又会在堆中创建一个“daina”实例并返回这个实例的引用（{2}）
         */
        String s5 = new String("daina");
        s5.intern();
        String s6 = "daina";
//        s6是JVM自动创建的对象实例引用，s5是new关键字创建的对象实例引用({2})
        System.out.println(s5 == s6); //7. false

        String s7 = new StringBuilder("tai").append("luo").toString();
        s7.intern();
        String s8 = "tailuo";
        System.out.println(s7 == s8);//8. true

//        算是我学习中的彩蛋吧:)
//        和《深入理解JVM》Page63的实验不一样,这里结果是true
//        明明说好的sum.misc.Version加载时“java”字符串的引用已进入常量池
        String str = new StringBuilder("ja").append("va").toString();
        System.out.println(str.intern() == str); // 1. true

    }

}

```

### 2.3. 字符串拼接中“+”的重载

>The implementation of the string concatenation operator is left to the discretion of a Java compiler, as long as the compiler ultimately conforms to The Java™ Language Specification. For example, the javac compiler may implement the operator with StringBuffer, StringBuilder, or java.lang.invoke.StringConcatFactory depending on the JDK version. 

如何重载依赖于编译器的具体实现,可以利用` StringBuffer`, `StringBuilder`, 或` java.lang.invoke.StringConcatFactory`,只要符合Java语言规范就好.字符串之间如何相等实验如下:

```java
public class Test {
    // 字符串常量会被放入字符串常量池中
    public static final String R = "lianxiang";
    public static String a = "lian";
    public static String b = "xiang";
    public static final String A = "lian";
    public static final String B  = "xiang";
    public static void main(String[] args) {
        String i = "lian"+"xiang";
        String j = "lian";String k = "xiang";
        String h = new StringBuilder("lian").append("xiang").toString();
        String y = R.substring(0);
        // 字符串字面量会被解析为字符串常量
        /** 
         * "+"号对于  字符串常量  会优先查找字符串常量池中有没有对应的字符串引用({1})
         */
        System.out.println( i == R); // true
        // 与{1}同理
        System.out.println( (A+B) == R); // true

        // StringBuilder与StringBuffer底层的可变数组是一定会在堆中创建新对象并返回引用的({3})
        System.out.println( h == R); // false
        /** 
         * "+"号对于  字符串引用 则实际上等于调用append()方法({2})
         */
        System.out.println( (a+b) == R); // false
        // 与{2}同理
        System.out.println( (j+k) == R); // false

        // 与{3}同理
        System.out.println( y == R); // true
    }
}

```

[^String设计]:[深入理解String、StringBuffer与StringBuilder](https://blog.csdn.net/qq_40401156/article/details/108464386)
[^jvm]:[从字符串到常量池，一文看懂String类](https://mp.weixin.qq.com/s?__biz=MzU5ODg2Njk4OA==&mid=2247484037&idx=1&sn=5f0805bd6c62f690ffa06c3982959889&chksm=febcefc6c9cb66d0cb70d70fac2e4fee73ef2fa32d91f16cb0c6510f5b32ae78f629bb612ddb&mpshare=1&scene=1&srcid=&sharer_sharetime=1593613925350&sharer_shareid=f059618cb093f5efb49a39cd6562e90e&key=7a6ffc80620031bc540a4b92b391e87157b55c88e52904cbb835bda526e0ff1a586d1c6101fb542490828c08cdfce2866392d015927be5907d84463dc26371ef31dd9fe02f79e0fb09eb956dbbf22976&ascene=1&uin=MTM2NzczNTcyNQ%3D%3D&devicetype=Windows+10+x64&version=62090070&lang=zh_CN&exportkey=A8edtILws1QNJsxnTLC8Iqs%3D&pass_ticket=aFoiqjOTcc8UhC9qVxDI%2BkM0NqWcoqXdVcdpjeCgXbTcKBGqej6Xds48IfMw5j8m)
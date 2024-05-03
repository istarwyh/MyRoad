实践中发现蹊跷,Java中的引用不能实现C/C++中指针同样的swap():

## 1. C/C++与Java中实现Swap()
### 1.1. C中的swap()
对于C/C++，语言的操作是在内存级别上的。
如果确定输入参数是`int`型可以通过指针间接访问内存交换，
```c
void swap(int* x,int* y){
    int tmp = *x;
    *x = *y;
    *y =tmp;
}
```
对于更多不确定的类型如`double`乃至`泛型`(`void*`,可以简单理解为可以被任何类型赋值[^void*与Object])可以直接内存拷贝：
[^void*与Object]:void*表示通用指针，可以被任何类型赋值；Java中`Object`同理，并且Java中对象在运行时擦除后也都是Object.

```c
void swap(void* x,void* y,size_t size){
    char tmp[size];
    //memory copy的意思
    memcpy(tmp,x,size);
    memcpy(x,y,size);
    memcpy(y,tmp,size);
}
```
### 1.2. C++中使用引用（`&`）的swap()
C++中的引用变量是一个别名，封装了**对引用指向的变量本身的操作**但是不把指针暴露在外。
与指针的区别便是除了对变量本身的操作，不能在内存上操作。
与Java中引用的不同，在于Java中的引用其实是被剥夺了访问能力的指针。

```c
void swap(int& x, int& y)
{
   int tmp = x; /* 保存被引用变量 x 本身的值 */
   x = y;    /* 把 y 赋值给 x */
   y = tmp; /* 把 x 赋值给 y  */
  
   return;
}
```

### 1.3. Java中的swap()
```java
class SwapTest { 

    // int x=5,y=6;就不试了，本来就是传递值被拷贝。
    Integer x = 5, y = 6;
    
    public static void swap(Object x, Object y){
        Object t = x; 
        x = y; 
        y = t; 
    }
    public static void main(String[] args){ /
        // 尽管传递参数是对象
        Integer x1 = 5, y1 = 6; 
        swap(x1, y1); 
        // 结果没有变,只能解释为swap()函数内部的引用变量依然是对传入引用的拷贝
        System.out.println(x1+" "+y1); // 输出的是5 6 
        
        // 主函数中引用互换
        Object t = x1; x1 = y1; y1 = (Integer) t;
        System.out.println(x1+" "+y1); // 输出的是 6 5

        // 使用实例变量交换
        SwapTest s = new SwapTest(); 
        swap(s.x, s.y);
        // 实际上这里传进去的依然还是引用，这个引用通过解引用（后文有解释）得到这个引用指向在堆上的实例变量
        System.out.println(s.x+" "+s.y); // 输出的是5 6
    } 
}
```
### 1.4. 其他swap()
#### 1.4.1. 动态类型或者语法糖支持的语言
类型是对内存的抽象:类型系统的存在是为了避免代码以无效的使用方式编译或运行。
`go`中swap()甚至可以：

```go
int b,a;
b，a = a, b;
```
#### 1.4.2. 特殊情况下不需中间变量的swap()
```java
void swap(int[] a,int i,int j){
    a[i] = a[i] + a[j];
    a[j] = a[i] - a[j];
    a[i] = a[i] - a[j];
}
```

## 2. 从swap()窥函数调用的传参策略
调用函数时,传递不同类型的参数,比如基本数据类型(如int,char,double等，又称为`内建类型`)、结构体/对象、引用、指针给函数时,不同语言中使用这些参数的方式并不一致.一般,`parameter-passing strategy`也可以当作概念更宽泛的`evaluation strategy`的主要部分,并用`evaluation strategy`代指它.
### 赋值策略(Evaluation Strategy)

下表摘录自:[Evaluation_strategy](https://en.wikipedia.org/wiki/Evaluation_strategy)

| Evaluation strategy          | Representative Languages      | Year first introduced |
| ---------------------------- | ----------------------------- | --------------------- |
| Call by reference            | FORTRAN II, PL/I              | 1958                  |
| Call by value                | ALGOL, C, Scheme              | 1960                  |
| Call by name                 | ALGOL 60, Simula              | 1960                  |
| Call by copy-restore         | Fortran IV, Ada               | 1962                  |
| Call by need                 | Haskell, R                    | 1971                  |
| Call by reference parameters | C++, PHP C# Visual Basic .NET | ?                     |
| Call by reference to const   | C, C++                        | ?                     |
| Call by sharing              | Java, Python, Ruby            | ?                     |

### 2.1. call by value 整个副本
C/C++传递参数一般是值传递，但是也可以传递指针和引用。
值传递的意思是传入的参数在函数体内部会被**拷贝**一份而变成局部变量,其生命周期也即在函数内部,而所有的函数体内部的操作都是针对这个**拷贝**的操作,它影响不到作为输入参数的变量。
当其为值传递时，对于`外建类型`（自定义的数据结构），依然是一个深度拷贝。这一方面带来时间和空间上的开销，一方面也避免了内存共享的问题。
### call by reference
通常被翻译为引用传递.
`void swap(int& x, int& y)` 中在函数内部操作的`x`和`y` 就是变量的一个别名,即变量本身,函数内部对`x`的任何操作都会影响函数外的变量.
### 2.2.  call by sharing 特殊的值传递
call by sharing 也被称为`call by object` 或者 `call by object-sharing`.

1. 当把Java的基本数据类型作为入口参数传给函数体使用"值传递"    
2. 但是在Java中用对象（`外建类型`）作为入口参数的传递则为**引用值的传递**,方法内部会对实参的引用值做一次拷贝给形参使用.通常这被称作`call by sharing`
3. Java社区并不常使用`call by sharing`,根据其传递的是引用值的特性,依然称其为值传递
4. 参考"值传递",在方法内部无论对对象的这个引用值做了何种操作，都不会改变实参对象的*引用* .但是如果在方法内部**通过这个引用修改其所指向内存处的值**时,实质上就是在**对这个对象的直接操作**：
```java 
public void change(int[] source) {
       source[1]=13579;//source[1]的值被真正改变为13579
    }
```

因为Java是基于**内存共享**来操作内存变量。这大大避免了内存复制带来的麻烦并提高了效率，但这同样带来并发变量不确定以及野引用的问题。不过Java中的引用设计为**不支持**`+`/`-`这类算数运算,即Java的引用只能指向程序当前限定的对象,又引入垃圾回收机制(引用计数和根可达性分析等）,解决了野引用的问题

### 总结
Java中的引用和C++中的引用很像,都不允许寻址.不过C++语言规定,引用变量在定义的时候必须初始化,即引用变量必须一开始就与被引用变量强绑定.这种引用关系一旦确定就不允许改变.对于以下代码
```c
int i = 5;
int &r = i;
r =8;

```

`&r`代表的是`i`的地址,`r`代表的却是`i`本身.对于函数 `void foo(int &r)`,因为引用传递的关系,函数内部对于`r`的操作就是对实参的操作.
Java中`void foo(Object o)` o 对应的实参就是`&r`或者说`const *r`,这一步如果我们说传递的是引用,从字面意思上是没有问题的.
但是针对方法获取参数的策略(`evaluation strategy`)本身,这个引用会被拷贝一份给形参,并且在Java 中是不可能直接操作被引用对象本身`r`的,只可能通过解引用访问它的属性.所以这就是Java中的特殊的值传递,在Python中也被称为按共享传递(`call by sharing`).


### 2.3. `=`与拷贝
#### 2.3.1. `=`对对象只传递引用
1. 除了对象作参数时是"引用传递",在任何时候用"`=`"向对象变量赋值时也都是引用传递.这意味着当`objA = objB`时,此后任意对objB的改变都等同于对objA的改变,反之亦然；
#### 2.3.2. 浅拷贝可能造成拷贝后对象与原对象纠缠不清
2. Java为了实现C++的拷贝构造提供了`prototype`设计模式，即为Object类提供了clone()。但是直接使用`Object.clone()`是**浅拷贝**，对于clone的对象本身而言依然是引用传递，其与`=`区别在于它**取决于覆写的clone()方法对对象变量本身的clone实现**。在默认的`super.clone()`中，除了**内建类型**和`final`修饰类(如`String`类)的对象不会受原被clone对象的改变而改变，其他对象会随之修改(从而有可能造成不可料的后果)：

```java
public class Refer_vs_Point{
    public static void main(String[] args) throws CloneNotSupportedException {
        Node n1 = new Node(1,1,"壹", new Node(10,10,"十",null) );
        out("--- 原始的n1 ---");
        out( "n1:   "+n1.toString() );

        Node n2 = n1;
        Node n3 = n1.clone();
        Node n4 = n1.deepClone();

        out("n3与n1 地址是否相等:");out(  n3 == n1 );
        out("n3与n1 对象实例数据类型/内容是否相同:");out( n3.equals( n1 ) );

        n1.setId( 2 );n1.setNo( 2 );n1.setName( "贰");

        n1.getNext().setId(20);n1.getNext().setNo( 20 );n1.getNext().setName("廿");
        out( "\n--- 通过解引用的Set()之后 ---");
        out( "n1:  "+n1.toString() );
        out( "n2随n1改变:   \n"+"     "+ n2.toString() );
        out( "n3的 不可变对象(name),基本数据和基本数据装箱后的对象 不随n1改变:   \n"+"     "+ n3.toString() );
        out( "n4完全不随n1改变，实现了C++中的拷贝构造:   \n"+"     "+n4.toString() );

        n1.setNext( new Node( 100,100,"栢",null));
        out( "\n--- 对复合对象没有采取解引用的Set() ---");
        out( "n3不随n1改变:   \n"+"     "+ n3.toString() );

    }
    public static void out(Object o){
        System.out.println(o);
    }
    @Setter
    @ToString
    @AllArgsConstructor
   static  class Node implements Cloneable{
        public int no;
        public Integer id;
        public String name;
        public Node next;

        @Override
        protected Node clone() throws CloneNotSupportedException {
            return (Node)super.clone();
        }

        protected Node deepClone() throws CloneNotSupportedException {
//            因为Node的父类直接就是Object，因此clone之后也是Object,需要向下转型
            Node res = (Node)super.clone();
//            复杂对象时，这样递归调用可能栈溢出
            if (res.next != null) {
                res.next = next.clone();
            }
            return res;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            Node node = (Node) o;
            return no == node.no &&
                    Objects.equals(id, node.id) &&
                    Objects.equals(name, node.name) &&
                    Objects.equals(next, node.next);
        }

        @Override
        public int hashCode() {
            return Objects.hash(no, id, name, next);
        }

        public Node getNext(){
            return  this.next;
        }

        @Override
        public String toString() {
            return "{" +
                    "no=" + no +
                    ", id=" + id +
                    ", name='" + name + '\'' +
                    ", next=" + next +
                    '}';
        }
    }

}
```
最后结果

```json
--- 原始的n1 ---
n1:   {no=1, id=1, name='壹', next={no=10, id=10, name='十', next=null}}
n3与n1 地址是否相等:
false
n3与n1 对象实例数据类型/内容是否相同:
true

--- 通过解引用的Set()之后 ---
n1:  {no=2, id=2, name='贰', next={no=20, id=20, name='廿', next=null}}
n2随n1改变:   
     {no=2, id=2, name='贰', next={no=20, id=20, name='廿', next=null}}
n3的 不可变对象(name),基本数据和基本数据装箱后的对象 不随n1改变:   
     {no=1, id=1, name='壹', next={no=20, id=20, name='廿', next=null}}
n4完全不随n1改变，实现了C++中的拷贝构造:   
     {no=1, id=1, name='壹', next={no=10, id=10, name='十', next=null}}

--- 对复合对象没有采取解引用的Set() ---
n3不随n1改变:   
     {no=1, id=1, name='壹', next={no=20, id=20, name='廿', next=null}}
```

**注意**
`obj.str`的操作被称为解引用,可以理解为通过像句柄的`obj`来找到指向所要成员变量的引用（这里是`str`)；实验证明解引用对性能的影响其实很小(《Code Complete2》)。

#### 2.3.3. 深拷贝与实验解释

1. 从上述实验可以看到要使得objB诞生于objA,但是objB之后与objA又没有直接的关系,只能对每个**可变的引用类型对象**对显式调用它自己的clone()方法借助`Object.clone()`实现深拷贝,即让原始对象和拷贝之后的对象引用以及对象中嵌套的所有对象引用不指向同一块堆内存. 简单总结一下
    - `Object.clone()`介于`new`一个对象与`=`之间，从引用和堆中对象的关系： `=`$\leq$ 浅拷贝 $\leq$ `new`$\leq$深拷贝.
       - 对于 不可变对象(name),基本数据和基本数据装箱后的对象：浅拷贝 `==` 深拷贝
       - 对于可变对象，通过解引用对对象的更改：`=` `==` 浅拷贝

[^Note]: 让类实现 Cloneable 接口调用对象的 clone 方法，并覆写 clone 方法

2. 对于内建类型浅拷贝之后与原对象独立，是因为浅拷贝之后并不与原对象共享引用；另一方面对于String这样的不可变类型，是因为如果原对象更改不可变的成员变量实际上只能生成新的对象或者 复用字符串常量池已有的字符串常量，然后更换引用；实际上，实验中也看到如果不是通过解引用的方式更新数据而是直接生成新对象更换原有引用，那么浅拷贝后的对象也不会受到原有对象更改的影响。

3. 《Effective Java》中提到，通常实现接口是为了表明类的能力，而对于Cloneable接口，它改变了超类中`protected`方法的行为，使得无需调用构造器就可以创建对象。但是本身Cloneable接口对实现者无任何约束（就是一个空接口）：

```java
 * @see     java.lang.CloneNotSupportedException
 * @see     java.lang.Object#clone()
 */
public interface Cloneable {
}
```
这使得Cloneable的实现对于希望中拷贝功能的实现是缺乏确定性而危险的。因此不建议开发者使用Cloneable接口，可以代之以拷贝构造器或者其拷贝工厂方法。
## 3. 值传递与值类型
王垠认为[^inlining],
>Java 继承了 Scheme/Lisp 的衣钵，它们在“语义”上其实是没有值类型的。以Java中`int`这种传统认为是值类型的变量类型为例，`int`因为是原始类型,无法用deref操作符`*`访问内存地址或者`.`访问成员变量,所以它们在**实现上**是传递值还是引用对于程序员没有区别。实际上，所有的数据都是引用类型就是 Scheme 和 Java 最初的设计原理。原始类型用值来传递数据只是一种性能优化(`inlining`)。
>一个在语义上有值类型的语言（比如 C#，Go 和 Swift）必须具有以下两种特性之一（或者两者都有），程序员才能感觉到值类型的存在：
deref 操作。这使得你可以用 *x = 2 这样的语句来改变引用指向的内容，导致共享地址的其它引用看到新的值。你没法通过 x = 2 让其他值变量得到新的值，所以你感觉到值类型的存在。
像 struct 这样的“值组合类型”。你可以通过 x.foo = 2 这样的成员赋值改变引用数据（比如 class object）的一部分，使得共享地址的其它引用看到新的值。你没法通过成员赋值让另一个 struct 变量得到新的值，所以你感觉到值类型的存在。

上面提到的`x = 2`这样的方式之所以不能让其他值变量得到2这个值是因为值类型本身是**一个在栈上分配的整体**，本身是不可变的。对于值类型的赋值和参数传递都和值传递一样，会将原值变量拷贝一份。使用者没法让两个变量指向同一个值类型。
栈上存储的数据，很大机会会被虚拟机分配至物理机器的高速寄存器中存储，并且生命周期随着其所在栈桢出栈就结束，因此比堆效率高.

[^inlining]:http://www.yinwang.org/blog-cn/2016/06/08/java-value-type

那`null`是引用类型,不能转换为基本类型,这又该如何解释Java最开始设计没有值类型这回事？
### 3.1. 特殊的null类型
- `null`运行时没有任何类型，当然也不是任何类型的子类，但却可以被转换为任何类型
- 尽管null可以被赋值给任何类型，同时还是任何引用类型的缺省值，但是不能使用任何类型的方法或变量(比如继承Object的类都有的`hashCode()`、`getClass()`)

事实上，遍历整个字节码指令集，与null有关的字节码只有`aconst_null`,Zero解释器对其的实现为[^aconst_null]：

```c++
CASE(_aconst_null):
 SET_STACK_OBJECT(NULL, 0);
 UPDATE_PC_AND_TOS_AND_CONTINUE(1, 1);
```

基本意思是将常量NULL push到操作数栈顶，然后把它赋值给本地变量。这里的`NULL`也就是`HotSpot VM`宿主语言C++中的NULL，代表指针值为`0x0000`,指向内存中的第0个内存位置(但不被操作系统允许用户访问和读写)。
null现在被认为一种广泛存在于编程语言中的设计错误[^designerror]。

[^designerror]:https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Tony_Hoare%23Apologies_and_retractions

[^aconst_null]:https://www.zhihu.com/question/59085383/answer/162185403

### 3.2. 基本类型与其包装类
`Integer`之于`int`是完全的语法糖，xxx自动装箱前端编译后就等于`Intger.value(xxx)`，自动拆箱亦然。那么为什么还要这样做呢？
私以为正是因为Java没有实现值类型，而又想使用对象之于基本类型的优点，比如泛型、高频区间数据缓存(高速寄存器？)、序列化、类型转化，导致为每个基本类型都配套了一个包装类型。也有人说这是因为“Java一切皆是对象”的设计理念，不过null与基本类型的存在本身就是对这种理念的挑战。

## 4. References

实践中发现蹊跷,java中的引用不能实现C/C++中指针同样的swap():

## 1. C/C++与Java中实现Swap()
### 1.1. C中的swap()
对于C/C++，语言的操作是在内存级别上的，这赋予的能力是超过寻常语法的。
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
```c++
void swap(int& x, int& y)
{
   int tmp = x; /* 保存地址 x 的值 */
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
        Object t = x; x = y; y = t; 
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
        // 实际上这里传进去的依然还是引用，这个引用通过解引用（后文有解释）得到
        // 这个引用指向在堆上的实例变量
        System.out.println(s.x+" "+s.y); // 输出的是5 6
    } 
}
```
### 1.4. 其他swap()
#### 1.4.1. 动态类型或者语法糖支持的语言

类型是对内存的抽象；类型系统的存在是为了避免代码以无效的使用方式编译或运行
迭代器是为了泛型编程对不同数据结构**遍历**操作的抽象。

swap()甚至可以：
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


## 2. 从语法实验窥背后设计
### 2.1. C/C++传递参数一般同样是值传递（整个副本）
>通过指针，可以对内存进行低级控制，从而可以基于系统底层处理细节写出精细的代码，但是这同样加大了编程复杂度。--陈皓

C/C++传递参数一般同样是值传递，但是也可以传递指针。
当其为值传递时，对于`外建类型`（自定义的数据结构），依然是一个深度拷贝。这一方面带来时间和空间上的开销，一方面也避免了内存共享的问题。
### 2.2. Java中对象只有引用传递
1. 当把Java的**基本数据类型(如int,char,double等，又称为`内建类型`)作为入口参数**传给函数体的时候,传入的参数在函数体内部会被**拷贝**一份而变成局部变量,其生命周期也即在函数内部,而所有的函数体内部的操作都是针对这个**拷贝**的操作,它影响不到作为输入参数的变量.这种参数传递的方式也即"值传递".
2. 但是在Java中用**对象（`外建类型`）作为入口参数的传递**则缺省为**引用传递**,尽管实际上传递的是引用**值**.参考"值传递",在方法内部无论对对象的这个引用值做了何种操作，都不会改变实参对象的*引用* .
3. 但是如果在方法内部**通过这个引用修改其所指向内存处的值**时,实质上就是在**对这个对象的直接操作**.
```java 
public void change(int[] source) {
       source[1]=13579;//source[1]的值被真正改变为13579
    }
```

**传递引用带来的问题**

4. 因为Java是基于**内存共享**来操作内存变量。这大大避免了内存复制带来的麻烦并提高了效率，但这同样带来并发变量不确定以及野引用的问题。
5.  Java中的引用设计为**不支持**`+`/`-`这类算数运算,即Java的引用只能指向程序当前限定的对象,又引入垃圾回收机制(引用计数和根可达性分析等）,解决了野引用的问题
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
## 3. 思考
王垠认为,Java中`int`这种基本类型变量因为是原始类型,无法用`*`或者`.`访问成员变量,所以它们在**实现上**是传递值还是引用类型对于程序员没有区别.而在实际实现时,因为`innling`,将基本类型如`int`优化成值类型的传递.那`null`是引用类型,不能转换为基本类型,这又该如何解释?
## 4. References

# 引用vs指针
## 1. [C/C++的存在](https://blog.csdn.net/JianZhiZG/article/details/1427086)
```java
class SwapTest 
{ Integer x = 5, y = 6;
    public static void swap(Object x, Object y)
    {Object t = x; x = y; y = t; 
    }
   public static void main(String[] args)
    { Integer x = 5, y = 6; //局部变量x, y
         swap(x, y); 
        System.out.println(x+" "+y); // 输出的是5 6 

        Object t = x; x = y; y = (Integer) t; 
        System.out.println(x+" "+y); // 输出的是 6 5

         // 使用实例变量交换 
        SwapTest s = new SwapTest(); swap(s.x, s.y);
         System.out.println(x+" "+y); // 输出的是6 5
    } 
}
```
## 2. [java中只有值传递](https://blog.csdn.net/qq_35923749/article/details/79703700)
然而神奇的是...
```java
import java.util.Date;
public class Test{
    Date x = new Date(1234567);
    Date z = new Date(7654321);
    public static void main(String[] args){
        Date y = new Date(1234567);
        
        //局部变量传入
        m1(y);
        System.out.println(y.getTime());//不能改变

        //主函数调用
        y = new Date(7654321);
        System.out.println(y.getTime());// 可以改变

        // 实例变量传入-1-函数内部有局部变量
        Test t = new Test();
        m1(t.x);
        System.out.println(t.x.getTime());//不能改变
        
        // 实例变量传入-2-y是局部变量
        m2(t.x,y);
        System.out.println(t.x.getTime());//不能改变

        // 实例变量传入-3
        m3(t.x,t.z);
        System.out.println(t.x.getTime());//不能改变
        
        // 局部变量传入-4
        m4(y);
        System.out.println(y.getTime());// 可以改变


    }
    public static void m1(Date data){
        // data = new Date(7654321);
        Date date = new Date(7654321);
        data = date;
    }
    public static void m2(Date data,Date date){
        data = date;
    }
    public static void m3(Date data,Date date){
        Date tmp = data;
        data = date;
        date = tmp;
    }

    public static void m4(Date data){
        data.setTime(7654321);
    }
}
```
## 3. 总结
1. 当把Java的**基本数据类型(如int,char,double等)作为入口参数**传给函数体的时候,传入的参数在函数体内部会被拷贝一份而变成局部变量,其生命周期也即在函数内部,而所有的函数体内部的操作都是针对这个**拷贝**的操作,它影响不到作为输入参数的变量.这种参数传递的方式也即"值传递".
2. 但是在Java中用**对象作为入口参数的传递**(如[数组](https://blog.csdn.net/dadoneo/article/details/6577976?reload)) 则缺省为 **"引用传递"** ,即传递的是对象的引用(值),这个"引用"的概念与C/C++中的指针引用是一样的.但因为仍是值传递的缘故,此时无论对传入的引用(值)做了何种操作，都不会改变实参对象的*引用* :
```java
   public void swap(int[] source, int[] target) {
       int[] tmp = source;
       source = target;// 真正的实参变量source所指向的数组变量不会改变
       target = tmp;
    }
```
另一方面,当在函数体内部通过被传递的这个引用的值改变对象值时，这个引用值会被程序解释(或者查找)到JVM堆中的对象，从而实质上就是在**对这个对象的直接操作**.如
```java 
public void change(int[] source) {
       source[1]=13579;//source[1]的值被真正改变为13579
    }
```

3. 除了对象作参数时是"引用传递",在任何时候用"`=`"向对象变量赋值时也都是引用传递.这意味着当`objA = objB`时,此后任意对objB的改变都等同于对objA的改变,反之亦然:  
```java
       
       class ChangeObj{
    String str = "Hello";
}
public class Test{

    public static void main(String[] args) {
        ChangeObj objA = new ChangeObj();
        ChangeObj objB = objA;
        objA.str = "World";
        System.out.println(objB.str);
        objB.str = "Kitty";
        System.out.println(objA.str);
    }
}
```
4. 要使得objB诞生于objA,但是objB之后与objA又没有直接的关系,可以使用`Object.clone()`[^Note]实现深拷贝[^深拷贝],即让原始对象和拷贝之后的对象引用不指向同一块堆内存.

5. 从引用和堆中对象的关系,`=`$\iff$浅拷贝 $\leq$ `new`$\leq$深拷贝.

[^Note]: 调用对象的 clone 方法，必须要让类实现 Cloneable 接口，并且覆写 clone 方法

[^深拷贝]:将每个引用类型都拆分为基本类型，分别进行Object.clone()[^浅拷贝]

[^浅拷贝]:[Java的深拷贝和浅拷贝](https://www.cnblogs.com/ysocean/p/8482979.html)


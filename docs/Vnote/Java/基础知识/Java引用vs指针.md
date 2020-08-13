---
app:markdown
---
实践中发现蹊跷,java中的引用不能实现C/C++中指针同样的swap():
## 1. 引用与C++中的指针
```java
class SwapTest { 
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
        // 实例变量涉及的是引用所指向内容的调换
        SwapTest s = new SwapTest(); 
        swap(s.x, s.y);
        System.out.println(s.x+" "+s.y); // 输出的是5 6
    } 
}
```
## 2. java中只有值传递吗?

 java中只有值传递[^凌凌小博客],着重测试引用传递的情况
[^凌凌小博客]: [java函数（方法）中的值传递和引用传递问题](https://blog.csdn.net/qq_35923749/article/details/79703700)

```java
import java.util.Date;
public class Test{
    Date x = new Date(1234567);
    Date z = new Date(7654321);
    
    public static void main(String[] args){
        Date y = new Date(7654321);
        
        //主函数调用
        y = new Date(1234567);
        System.out.println(y.getTime());// 可以改变
        
        //引用值传入
        m1(y);
        System.out.println(y.getTime());//不能改变        

        // 实例变量传入-01-函数内部有局部变量
        Test t = new Test();
        m1(t.x);
        System.out.println(t.x.getTime());//不能改变
        
        // 实例变量传入-02
        m2(t.x,y);
        System.out.println(t.x.getTime());//不能改变

        // 实例变量传入-03-参数都是实例变量
        m3(t.x,t.z);
        System.out.println(t.x.getTime()+" "+t.z.getTime());//不能改变
        
        // 局部变量传入-04
        // 唯一能改变的情况是改变对象实际内容
        m4(y);
        System.out.println(y.getTime());// 可以改变


    }
    // 测试方法内部引用被改变,主函数内部是否改变
    public static void m1(Date data){
        // data = new Date(7654321);
        Date date = new Date(7654321);
        data = date;
    }

    // 同上,只是参数来自于传入的引用而不是函数内部产生的
    public static void m2(Date data,Date date){
        data = date;
    }

    // 测试参数都是实例变量的引用的互换是否成立
    public static void m3(Date data,Date date){
        Date tmp = data;
        data = date;
        date = tmp;
    }

    // 测试是否可以在函数内部改变对象的内容
    public static void m4(Date data){
        data.setTime(7654321);
    }
}
```
## 3. 总结

1. 当把Java的**基本数据类型(如int,char,double等)作为入口参数**传给函数体的时候,传入的参数在函数体内部会被拷贝一份而变成局部变量,其生命周期也即在函数内部,而所有的函数体内部的操作都是针对这个**拷贝**的操作,它影响不到作为输入参数的变量.这种参数传递的方式也即"值传递".
2. 但是在Java中用**对象作为入口参数的传递**则缺省为**引用传递**,尽管实际上传递的是引用**值**.参考"值传递",在方法内部无论对对象的这个引用值做了何种操作，都不会改变实参对象的*引用* ;但是如果在方法内部**通过这个引用修改其所指向内存处的值**时,实质上就是在**对这个对象的直接操作**.
### 3.1. 拓展
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
        System.out.println(objB.str);// 输出World
        objB.str = "Kitty";
        System.out.println(objA.str);// 输出Kitty
    }
}
```

4. 要使得objB诞生于objA,但是objB之后与objA又没有直接得关系,可以使用Object.clone()方法;这个方法介于`new`一个对象与`=`之间.
5. Java中的引用设计为不支持`+`/`-`这类算数运算,即Java的引用只能指向程序当前限定的对象,从而保证了安全.

**注意**
6. 对于`String`类,在方法内部通过引用值(在方法调用栈中)也无法修改之前在堆区申请的那段内存,因为它的底层是char数组.  

## 4. References
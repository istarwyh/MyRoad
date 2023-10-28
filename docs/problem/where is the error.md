## problem
```java
public class Test{
    public static void main(String[] args){
        f();
    }
    public static void f(){
        String[] a = new String[2];
        Object[] b = a;
        a[0] = "hi";
        b[1] = Integer.valueOf(42);
    }
}
```


## References
[深入理解Java：String](https://zhuanlan.zhihu.com/p/29500711)
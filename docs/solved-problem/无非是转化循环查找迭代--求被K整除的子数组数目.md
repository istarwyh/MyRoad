## 1. 从[题目](https://leetcode-cn.com/problems/subarray-sums-divisible-by-k/)到思路

关于前缀和的同余公式只能记住，有了公式就完成了第一步转化；第二步转化是 前缀和余K数组  中排列组合的计算。
之后便是把相关的数据原料准备好，设计循环与基于值对的查找。最后遍历值对计算。

## 2. 从思路到步骤

~

## 3. 从步骤到代码
```java
import java.util.HashMap;
import java.util.Collection;
import java.util.Iterator;
class Solution {
    public int subarraysDivByK(int[] A, int K) {
        int len=A.length;
        // 定义为从索引0到i的和
        int preSum[] = new int[len];

        //利用hash表能够寻找组合的数目
        HashMap<Integer,Integer> map = new HashMap<>();
        
        preSum[0] = A[0];
        int tmp = preSum[0] % K;
        int preSumModK = tmp >=0 ? tmp : tmp+K;  
        map.put(preSumModK,1);

        //为了利用递推公式，从i=1开始.上面先对i==0的情况处理.
        for(int i=1;i<len;i++){

            preSum[i] = preSum[i-1] +  A[i];

            // 防止计算机计算preSumModK为负数的情况
            // 真的前置知识比较多！！
            tmp = preSum[i] % K;
            preSumModK = tmp >=0 ? tmp : tmp+K;

            if(map.containsKey(preSumModK)){
                map.put(preSumModK,map.get(preSumModK)+1);
            }else{
                map.put(preSumModK,1);
            }
        }

        int count =0;
        // 当presumModK为0，自己是本身就是一种满足条件的子数组组合；其他preSumModK只可能与相减为0 才能满足中间子数组被K整除
        // 所以加上其本身的组合数情况
        if(map.containsKey(0)){
           count +=  map.get(0);
        }

        Collection c = map.values();
        Iterator itr = c.iterator();
        while(itr.hasNext()){
            int cur = (Integer)itr.next();
            count += Combination(cur,2);
        }
        
        return count;
    }

    private static long Combination(int n,int m){

        if(m==0)
            return 1;
        if (m==1) 
            return n;
        if(m>n/2)
            return Combination(n,n-m);
        if(n>1)
            return Combination(n-1,m)+Combination(n-1,m-1);  
        return 0;
    }
}
```

## 4.从可用到健壮
## 5.从健壮到高效
重新看上诉的解答，会发现它不过就是 

<center>题目转化->循环-> 查找 -> 遍历</center>

并且从查找到遍历还需要经历一个Map到Collection的转化，基本原理是遍历元素生成Collection，因此是个巨大的浪费。
所以**分拆了步骤之后**,只是着眼于提高各步效率，就能轻松优化。而在这个案例中还发现优化了查找连着后续遍历也简单多了。

```java
class Solution {
    public int subarraysDivByK(int[] A, int K) {
        int len=A.length;
        int preSum[] = new int[len];

        preSum[0] = A[0];      
        int tmp = preSum[0] % K;
        int preSumModK = tmp >=0 ? tmp : tmp+K;  

// K小于10000，因此preSumModK不可能超过10000，提供了其作为数组下标映射的条件。
//通过将preSumModK作为数组下标，充分利用了数组查找O(1)的特性。
        int[] preSumModKs = new int[10000];
        preSumModKs[preSumModK]++;

        for(int i=1;i<len;i++){
            preSum[i] = preSum[i-1] +  A[i];

            tmp = preSum[i] % K;
            preSumModK = tmp >=0 ? tmp : tmp+K;

            preSumModKs[preSumModK]++;
        }

        int count =0;
        count += preSumModKs[0];
        for(int a : preSumModKs)
            count += Combination(a,2);
        
        return count;
    }

    private static long Combination(int n,int m){

        if(m==0)
            return 1;
        if (m==1) 
            return n;
        if(m>n/2)
            return Combination(n,n-m);
        if(n>1)
            return Combination(n-1,m)+Combination(n-1,m-1);  
        return 0;
    }
}
```
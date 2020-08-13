# 1. HashMap Use
*八种实现类,当数据是10-100-1000-100000的时候,各自运行的效率(耗费的时间)如何?*
>如果不是第一个,题目也好做,计数的时候放弃第一个?
## 1.1. 只用一个标志就可以找到这个字符

```java
import java.util.Scanner;
public class Main4 {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        String str = input.nextLine();
        if (str == null) {
            System.out.println(-1);
            input.close();
            return;
        }
        int count=0;
        for(int i = 0; i < str.length(); i++) {
            for(int j = 0; j <str.length(); j++) {
                if (str.charAt(i)==str.charAt(j)){
                    //大家容易想到的就是用一个数组记录各个字符的出现次数,然后数组没有办法与字符映射,于是想到ASCII值对应数组下标,但是这样会丢失第一个字符出现的次序,于是想到用HashMap的key-value对,这个很好,只是如果不用数组呢?
                    count++;
                }
                if(count>1) break;
            }
            if(count ==1){
                System.out.println(i);
                input.close();
                return;
            }
            count=0;
            //所以啊,就是这么简单,count>1的判断只是为了少走几步
        System.out.println(-1);
        input.close();
    }
}
```
## 1.2. 用key-value对直观记录出现次数多开心

```java
import java.util.HashMap;
import java.util.HashSet;
import java.util.Scanner;
public class Main4 {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        String str = input.nextLine();
        if (str == null) {
            System.out.println("-1");
            input.close();
            return;
        }
        int len = str.length();
        HashMap<Character, Integer> map = new HashMap<>();
 
        for (int i = 0; i < len; ++i) {
            char c = str.charAt(i);
            if (map.containsKey(c)) {
                int cnt = map.get(c);
                cnt++;
                map.put(c, cnt);// key是str.charAt(i),value就是cnt了,这样就构造处理key-value对:一个key只能对应一个value,  
                // key不可重复,value可重复,故而value值可以很好的反应key出现的次数
            } else {
                // 如果不存在这样的字符,设置出现次数为1并存入map
                map.put(c, 1);
            }
        }
        // 尽管构造了key-value,已经可以遍历判断了
        // for(int i=0;i<len;i++){
        // if(map.get(str.charAt(i))==1){
        // System.out.println(i);
        // input.close();
        // return;
        // }else{
        // System.out.println(-1);
        // input.close();
        // return;
        // }
        // }
        // 但是还是说有O(1)的查询方法?
        //有,HashSet是一个封装的集合类,集合中元素具有无序性,唯一性
        HashSet<Character> keys = new HashSet<>();
        //这种表示方法真的很奇怪(主要我没见过),大概意思是把调用keySet()方法,把map中的char类型变量都集中一下,以便于后面根据某些条件(比如这里的map.get(c)==1)来把char类型的变量放入keys中
        for(char c:map.keySet()){
            if(map.get(c)==1)  keys.add(c);
        }
        //把value值为1的所有元素再全部放入另一个集合keys当中,这样后面查找这些元素是否只出现一次的时候,O(1);只是放入的过程不是遍历吗?-->看源码吧
        if(keys.isEmpty()){
            System.out.println(-1);
        }else{
            for(int i=0;i<len;i++){
                if(keys.contains(str.charAt(i))){
                    System.out.println(i);
                    break;
                }
            }
        }
        input.close();
    }
}
```
## 1.3. 要是能用数组存储对应字母下标,然后出现次数++ ,也很开心啊

```c
int firstUniqChar(char* s) {
    int hashtable[256];
    int i,l=strlen(s),ret=-1;
    memset(hashtable,0,sizeof(int)*256);
    if(l){
            
        for(i=0;i<l;i++)
        {
            hashtable[s[i]]++;
        }
        for(i=0;i<l;i++)
        {
            if(hashtable[s[i]]==1){          
                ret = i;
                break;
            }
        }
    }
 
    return ret;
}
```
## 1.4. 另一个将count作为了属性
[remove-duplicates-from-sorted-array-ii](https://leetcode-cn.com/problems/remove-duplicates-from-sorted-array-ii/)
```
class Solution {
    public int removeDuplicates(int[] nums) {
        // 对特殊情况进行优化,这里的特殊情况是长度小于3不用判断
        if(nums.length < 3 ) return nums.length;
        int p = 0;//[0,p]中存储次数至多为2的元素,p是区间尾
        int count=1;// 元素出现的次数.
        for(int i=1;i<nums.length;i++){
                
            if(nums[i] == nums[i-1]){
                count++;
            }else
                count=1;
            // 当它满足出现次数不超过2时,相当于给了nums[i]这个元素一个属性,当满足这个属性时
            // 我把它添加在[0,p)这个集合中
            if(count <= 2){
                // p与i指向同一个位置时,避免多一次复制
                if(p != i)
                    nums[++p] = nums[i];
                else    
                    ++p;
            }
           
        }
        // 长度比区间尾的下标大1,[0,2]长度为3
        return p+1;
    }
}
```
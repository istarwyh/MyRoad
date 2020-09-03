## 1. HashMap整体认识
按照组合模式的设计模式进行设计，划分接口定义功能，抽象类实现部分通用功能，HashMap作为Map子类之一实现自己的方法。接下来实现接口对Map的操作同于Map对于自己子结点的操作。HashMap作为容器，存储叶子结点Node<K,V>.对于叶子结点的内部类又是一个容器，包括了K和V；K和V是泛型，可以由用户自己装填任意类型，最终实现俄罗斯套娃的设计。
HashMap首先要解决的问题是Node的Key应该怎么映射，使得放进去的效率高同时查找的时候效率依然高。首先还是利用取余的分类功能，选取的是hash(key)的二进制尾数，为了保留高位的特征减少碰撞的概率，设计了高位取余算法；其次如果选取Hash的地址空间和扩容的阈值，初始定为16个桶，并且当达到12个桶的时候，即为了平衡发生碰撞时put元素的平均效率 和 空间很大更少碰撞但是空间紧张 之间的矛盾，默认负载因子为0.75；最后设计Node存在的数据结构形式：采用开地址法，每个Hash桶连接着链表--尾插法，并通过实验选择了当链表长度为8时转化为红黑树，又因为需要防止链表与红黑树在8这个长度处因为增加与删除导致反复转化，引起时间复杂度的震荡，选择红黑树元素存储为6时转化回链表。
而还有一些细节问题需要考虑，能不能提高取余操作本身的效率？因此权衡后强制每次hashmap的大小都是2的合数，这样方便使用位运算完成取余操作。
能不能在resize的时候尽量不要rehash，因为都再搞一遍真的感觉累。所以找rehash后的hash2与原始hash1之间的关系，发现大小都是2的合数时真的可以用更简单的方法完成hash取余操作。
[扰动函数](https://blog.csdn.net/mrchaochao/article/details/106340059?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-7.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-7.channel_param)
## 2. HashMap Use

### 2.1. 另一个将count作为了属性
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
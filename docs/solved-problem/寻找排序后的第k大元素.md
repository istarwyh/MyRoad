215. Kth Largest Element in an Array
## Warning
对于基本排序算法还是要慢慢地深刻理解,如
- 原地排序常常使用swap(),因为交换本身不涉及额外的空间操作,亦不会丢失原本双方数据
- `i++`vs`++i`
    - `++i`需要元素初始值无意义,然后`i=i+1`直接第一个有意义的元素;
    - `i++`的区别则第一个元素就有意义,但是最后它的元素会无意义.

### Code
```java
class Solution{
    /**
    *利用快速排序中partition()操作总会把元素放到正确位置上的特点
    */
    private static Random rand = new Random();
    public int findKthLargest(int[] nums, int k) {
        if(nums == null || nums.length==0 || K<1    ) return -1;
        int lo =0;
        int hi=nums.length-1; 

        // 第k大,则索引应是nums.length-k
		int requiredIndex = nums.length-k;
       
        while(true){
            int j = partition(nums,lo,hi);

            // 如果j大于排序后的指定索引,则在[lo,j-1]中找,舍去nums[j]以及它的另一半
            if(j>requiredIndex){
                hi = j-1;
            }else if(j<requiredIndex){
                lo = j+1;
            }else{
                return nums[j];
            }
        }
    }

    public static int partition(int[] a,int lo,int hi){
        // 做边界判断,以防后续遍历元素(++i和--j)越界
        if(lo>=hi)
            return lo;

        // 将数组切分为a[lo,i-1],a[j]和a[j+1,hi]
        // 初始值使得区间没有意义,是因为开始遍历时才有意义
        int i=lo;
        int j=hi+1;

         // 打乱数组不如随机选择轴值
        //  注意必须选取当前这个区间内的值,从而加"hi-lo"予以限定
        int randomNumber = lo+rand.nextInt(hi-lo);
        swap(a,lo,randomNumber);
        int pivot = a[lo];

        while(true){
            // 因为pivot总是会选第一个,++i总是会跳过对pivot本身的判断
            // i初始化为lo正是为了保持这种设计上的一致性
            while(less(a[++i],pivot)) 
                if(i == hi)
                    break;
            while(less(pivot,a[--j]))
                if(j == lo)
                    break;
            // 或许会奇怪,++i不一定会经过j么?为什么还要i>j也作为条件
            // 答:因为++i与--j可能会互相错过;假如一个数组长度为1,则i初始为0,j初始为1,++i==1,--j==0,这样两者就错过了,所以必须引入i>j的判断条件
            if(i >= j)
                break;
            // 当i==j,同一个元素自然没有必要交换;当i>j,
            swap(a,i,j);
            
        }
        // Key:为什么这里是j而不是i?这种不对称的感觉让人很奇怪
        // 那么回归j所代表的意义,j所指向的一定是等于或小于pivot的值,而选择的轴值是最左边的!即a[j]调换后一定是在最左边
        // 换句话来说,如果轴值选择是a[hi],那么这里就是swap(a,hi,i)--当然其他的相关操作都要对应改变
        swap(a,lo,j);
        // 达成a[lo,j-1] <= a[j] <= a[j+1,hi]
        return j;
    }
    public static boolean less(int a ,int b){
        return a >= b ? false : true; 
    }
    public static void swap(int[] a,int i,int j){
        int tmp=a[i];
        a[i]=a[j];
        a[j]=tmp;
    }
    /**
     * 轴值选择是最右边值的partition
    */
     public static int partition_Reverse(int[] nums,int lo,int hi){
        if(lo>=hi){
            return lo;
        }
        int randNum=lo+rand.nextInt(hi-lo);
        swap(nums,hi,randNum);
        // 注意,因为固定位置的值被随机调换的关系,每一轮的pivot值都不一样
        int pivot =nums[hi];
        
        // [lo,i-1][i][i+1,j]
        int i = lo-1;
        int j = hi;
        while(i<j){
            while(less(nums[++i],pivot))
                if(i==hi)
                    break;
                    // 总是会跳过最右边的pivot值
            while(less(pivot,nums[--j]))
                if(j==lo)
                    break;
            if(i>=j)
                break;
            swap(nums,i,j);
        }
        swap(nums,hi,i);
        return i;
    }
    // public static void main(String[] args){
	// 	ArrayList<Integer> list = new ArrayList<Integer>();
	// 	Scanner input = new Scanner(System.in);
	// 	while (input.hasNextLine()){
	// 		String t = input.nextLine();
	// 		System.out.println("<>"+t);
	// 		if(t.equals(""))
	// 			break;
	// 		list.add(Integer.parseInt(t));
			
	// 	}
	// 	int[] arr = list.stream().mapToInt(Integer::valueOf).toArray();
	// 	System.out.println(new Solution().findKthLargest(arr, 4));
	// 	input.close();
	// }
}
	
```
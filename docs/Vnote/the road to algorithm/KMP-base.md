## 1. KMP-base
>利用之前的工作**努力**是十分重要的.

>如何快速从一个主串中找出某一模式的子串?

其实这个问题这样描述就无形又增加了难度,换个说法:
>如何将一个模式子串匹配主串,并返回其起始下标?



```java
import java.util.Scanner;

//如何将一个模式字串匹配主串,并返回其起始下标

class KMP{
    //暴力直接:先逐个匹配,合适则继续,不合适让j回到0继续匹配.
    
    //当pat失配的时候,i应当回退到原本匹配开始时的后一个位置,但是事实上不必这么麻烦,我们让i作为开始匹配时的位置,在每次试配失败后才+1作为下一次的试配开始位置即可.那么怎么实现pat与txt逐个匹配呢?好办法便是i+j,这样使得txt是与pat中的元素同步比较的.
 
    public static int direct_right(String txt,String pat){
        int m = txt.length();int n = pat.length();
        Boolean flag = true;
        for(int i=0;i<m-n;++i){
            for(int j=0;j<n;++j){
                if(txt.charAt(i+j)!=pat.charAt(j)){
                    flag=false;
                    break;
                }
            }
            if(flag){
                return i;
            }
            flag=true;
        }
        return -1;

    }

    //如果上面是用j作为状态判断条件,是看到了j变化的实质是pat逐渐接近匹配完成的过程,更好!
    public static int direct_right_plus(String txt,String pat){
        int m = txt.length();int n = pat.length();
        for(int i=0;i<m-n;i++){
            int j;
            for(j=0;j<n;j++){
                if(txt.charAt(i+j)!=pat.charAt(j)){
                    break;
                }
            }
            if(j==n) return i;//这一步为名字里多的plus
        }
        return -1;
    }
    public static int KMP_autoState(String txt,String pat){
        int[][] dp;//帮助pat在匹配中移动的数组
        int m = txt.length();int n=pat.length();
        dp=new int[n][256];
        //代表着处于第一个字符匹配状态下的dp数组的状态(1=0+1,代表j的状态已经往后滑动进一位)
        //dp数组是只与pat有关的,是其特征表示之一
        //java把数组初始化为0,现在所有状态都是0,下面针对匹配的情况进行改写 
        dp[0][pat.charAt(0)]=1;
        //影子状态初始为0;引入X是为了通过X的状态转移图来获得重启位置,这样做可以减少匹配长度的回退,避免了暴力法那样一次回到原点"解放前"的情况,也正是KMP优于暴力法的核心所在
        int X=0;
        //j=0的状态从第一个字符能匹配开始已经结束,故j从1开始
        for(int j=1;j<n;j++){
            for(int c=0;c<256;c++){
                //不用管被匹配的txt如何,用256个字符模拟可能遇到的全部字符情况
                if(pat.charAt(j)==c)    
                    dp[j][c]=j+1;//如果匹配j的状态进一位
                else 
                    dp[j][c] = dp[X][c];
            }
            //初始的X状态从j=1,即pat中的第二个字母开始,这是因为至少当pat处于状态1才可能回退(到0,因为遇到了不匹配的字符).
            X = dp[X][pat.charAt(j)];
            //事实上,状态X设计开始便是为了和j具有相同的前缀(包括自身?),而在j需要回退的时候减少j回退的长度,因而X总是落后状态j一个状态,待命j遇到不匹配字符的时候回退
        }
        int j = 0;
        for(int i=0;i<m;i++) {
            //模拟逐个匹配的过程,调用dp数组里面的规则可直接判断失配或者匹配成功了j的状态,也就是说pat离完全匹配成功还有多远的状态
            j = dp[j][txt.charAt(i)];
            if(j==n) return i-n+1;
            //i是匹配完成后txt的下标,因为题目要求的是返回被匹配字串的中的起始索引;
            //注意j==m,因为从dp[][]求法总的第一个if可知,当遇到最后一个相匹配字符时,j仍会+1
        }
        //匹配失败,返回-1 
        return -1;
    }
    public static int KMP_traditon(String txt,String pat){
        int m=txt.length();int n = pat.length();
        //先计算Next数组--最长相等前后字串长度,同样为pat的特征表示,只与pat有关
        int[] next = new int[n];
        //不对第一位进行比较,直接置为0,从pat的第一位前缀与第一位后缀开始循环下面步骤
        next[0]=0;
        //TODO:设置一个数代表新增加的后缀,再设置一个数代表最长的前缀?-->大概是这个意思.
        int i,k;
        for(i=1,k=0;i<n;i++){
            //next[i-1]为i-1处的最长相等前缀后缀长度,将k首先置为它,是代表k从可能是最长前缀串的理想情况来考虑,这也保证了不出现前缀与后缀为重合的无意义情况
            //第一个最长前缀串其实也就是第一个字符,然后包括第二个字符,然后包括第三个字符...
            k = next[i-1];//i-1!!!
            while(pat.charAt(i)!=pat.charAt(k) && k>0)
                k  = next[k-1];//k-1~!!!
            //k从最长前缀串往回退,每一次从上一次的前缀尾部开始,从而探寻新增加的后缀尾部(pat.charAt(i))是否与前缀尾部(pat.charAt(k),在动态往回退中)相同--->
            //如果前后缀匹配则下一步next[i]在原有k已经代表前缀除了新增加的尾部已经匹配过的情况下(无论是否匹配)加1,代表最长相等前后缀又增加了一位,否则next[i]一路回退到0
            if(pat.charAt(i)==pat.charAt(k)) 
                next[i]=k+1;
            else 
                next[i]=0;
                //为什么不让next[i]=k?因为新增加的后缀如果失配前缀尾部,最长的前缀后缀字串只能是0,比如aaac-->0120
        }
        int j=0;
        //这里不能用i,避免与上面混淆
        for(int p=0;p<m;p++){
            while(j>0 && pat.charAt(j) != txt.charAt(p))
                j=next[j-1];
                //在不匹配的时候,j不停地回溯上一个状态以找到从j开始时会匹配的情况---利用上一个状态的努力,这种努力存放在next数组当中
            if(pat.charAt(j) == txt.charAt(p))
                j++;
            if(j==n) return p-n+1;//注意j=n!!,因为遇见最后一匹配相等只有,j还会++!从而从j所在的位置m-1加1变成了j的状态所在值m
        }
        return -1;
    }
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        String txt = input.nextLine();
        String pat = input.nextLine();
        System.out.println(direct_right(txt,pat));
        System.out.println(direct_right_plus(txt,pat));
        System.out.println(KMP_autoState(txt,pat));
        System.out.println(KMP_traditon(txt,pat));

        //下面主要考虑到txt串中可能有多个子串的情况
        int p = KMP_autoState(txt,pat);
         //p+pat.length()是本身某位字符的对应的txt中的下标,只有当它小于m-n的时候,后面才可能再出现字串
         while(p>0 && (p+pat.length())<txt.length()-pat.length()){
            //如果是空的0或者失配的-1,所以p>0作为不继续查询的终止条件
            p=KMP_autoState(txt,pat);
            if(p!=-1){//因为进入循环已经说明没有失配,现在只是检测后面还有没有字串
                System.out.printf("%-3d ",p);
            }
        }
        System.out.print("\n");
        input.close();
    }                
}


```
### 1.1. 理解KMP_tradiion的几问:
#### 1.1.1. 怎么求next[]? next[]究竟是什么?
一顿诡异的操作之后求得next[],而令人迷惑的无疑是next[]多重身份的一致性:
     next[]先后扮演了三个角色:
     - 最长前缀串长度-->未知
     - 最长相等前缀后缀长度-->寻找过程决定的
     - pat失配后应该去的位置(next[j-1]指示的位置)-->技巧
#### 1.1.2. 最长相等前缀后缀部分是怎么起作用的?
pat失配后,肯定要向右滑动,暴力法是每次只滑动一位,并把j=0,然后再逐个匹配.
对于KMP,它知道前面已经匹配过的最长相等前缀后缀是必然匹配的,因此**j的状态**不必从0开始,直接从**最长相等前缀后缀部分**的**下一位**开始即可--这样成功利用了**上一次**`(j-1)`已经匹配的"**努力**",省去了再把上一次已经匹配过的最长相等前缀后缀(如图中的abab)再匹配一次的步骤.而这个**下一位**的下标正是next数组所找的`j-1`状态下的*最长相等前缀后缀部分的长度*(因为正好下标是从0开始的,这个长度的下一位的下标就是这个长度,否则这个长度对应的数值还得+1)
![@KMP笔记|left|300*0](https://i.loli.net/2019/10/31/8k1wATYCZ2NrdbX.jpg =500x)

#### 1.1.3. 为什么pat在j处失配后,却需要j-1处最长相等前缀后缀部分的长度,即得到next[j-1]来作为j下一位置的状态?
我们刚刚解释了这个部分是怎么起作用的,但是这个作用为什么会是这样不知道.我认为这是因为next[j-1]正好就是下一位置应该在的状态,**技巧而已**.至于数学上对next[]的递归定义,以及更为广泛的字符串特征向量的本质---非我所能关心了.
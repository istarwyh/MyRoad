import java.util.Scanner;
import java.io.*;
class Solution {
    public static void main(String[] args) throws Exception {
        Scanner input = new Scanner(System.in);
        URL url = new URL(URLString);
        // 创建磁盘对象
        // 如果不封装成File,直接将路径用于构造接下来的节点流也可以
        File source = new File("JavaTest"+File.separator+"in.txt");
        File target = new File("JavaTest"+File.separator+"out.txt");

        //字符缓冲输入流
        BufferedReader br = new BufferedReader(new FileReader(source));
        //字符缓冲输出流
        BufferedWriter bw = new BufferedWriter(new FileWriter(target));

        char[] buffer = new char[10];
        //len表示读取的字节数
        int len = -1;
        while((len=br.read(buffer))!=-1){
            bw.write(buffer,0,len);
        }

        // 关闭IO资源
        br.close();
        bw.close();
    }
    /**
     * 以集合{a,b,c}为例，
     * 
     * 设i为数组下标，i从0~2
     * 1<<0 == 001
     * 1<<1 == 010
     * 1<<2 == 100 
     * 可以使用 1<<i 来分别表示a,b,c:
     * 001 == a
     * 010 == b
     * 100 == c
     * 对应的子集则是a,b,c的组合，二进制编码中1出现的位置对应子集中元素的出现位置，
     * 于是对所有的子集也可以采用二进制编码的方式：
     * 000 == null
     * 001 == a
     * 010 == b
     * ...
     * 111 == cba
     * 如果想要提取出这样二进制编码代表的子集信息（如打印出来），
     * 只需要元素编码与子集编码做“&”运算，来判断在每个被编码的子集中元素是否出现，
     * 001 & 011 == 001 == a[1<<i的i] == a[0] == a
     * 010 & 011 == 010 == a[1<<i的i] == a[1] == b
     * 100 & 011 == 000
     * 所以这个编码为011的子集则为 a，b
     * 
     */
    public void getAllSubset(char[] arr){
        int AllSetNum = (1 << arr.length) - 1;
        // 子集的编码范围从0~2^n-1
        for(int mask= 0;mask <= AllSetNum;mask++){
            for(int i=0;i<arr.length;i++){
                int tmp = 1<<i;
                boolean hasPresented = (tmp & mask) > 0 ? true : false;
                if(hasPresented)
                    System.out.print(arr[i]+" ");
            }
            System.out.println();
        }
    }

}

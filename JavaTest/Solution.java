import java.util.Scanner;
import java.util.*;
public class Solution {
    //零散序列,由n个正整数,满足子序列中任意两个数之和不能被k整除
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int k = sc.nextInt();
        int[] a = new int[n];
        for(int i=0;i<n;i++){
            a[i] = sc.nextInt();
        }

        //1  2 3 4
        //0001 0010 0100 1000 <-- 1<<i
        //序列如134--> 1101,一共2^n-1个序列可以遍历 
        //1101 & mask 
        //mask = 1<< i
        //然后再双重循环对不被k整除的子序列计数
        long count = 0;
        
        long num = (2 << n) -1;
        ArrayList<Integer> list = new ArrayList<>();
        for(long i=0; i< num; i++){
            for(int j=0;j<n;j++){
                boolean isCorrect =( i & (1 << j) )> 0 ? true : false;
                if(isCorrect){
                    list.add(a[j]);
                }
            }
            boolean flag =true;
            for(int l=0;l<list.size();l++){
                for(int h = l;h<list.size();h++){
                    if((list.get(l) + list.get(h)) % k == 0){
                        flag = false;
                        break;
                    }
                }
                if(flag == false)
                    break;
            }   
            if(flag)
                count++;
        }
        System.out.println(count%(1000000007));
    }
}
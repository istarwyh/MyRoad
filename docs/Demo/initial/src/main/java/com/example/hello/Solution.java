package com.example.hello;

import java.util.Scanner;

public class Solution {
    public static void main(String[] args) {
    /*    Scanner in = new Scanner(System.in);
        while(in.hasNextLine()){
            String s = in.nextLine();
            String t = in.nextLine();
            System.out.println("----line已读完----");
            System.out.println(new Solution().minWindow(s,t));
        }*/
        System.out.println(8 >>> 2);
        System.out.println(Integer.MAX_VALUE);

        System.out.println(-1 >>> 1);

        System.out.println(-1 >>> 2);

        System.out.println(-1 >>> 20);
        System.out.println(-1 >>> 22);
        System.out.println(-1 >>> 23);
        System.out.println(-1 >>> 24);

        System.out.println(-1 >>> 27);
        System.out.println(-1 >>> 28);
        System.out.println(-1 >>> 29);
        System.out.println(-1 >>> 31);

    }
    public String minWindow(String s, String t) {
        int l = 0;
        int r = -1;
        Pattern p4Window = new Pattern(s);

        int[] freq = new int[256];
        int groove =0;
        Pattern p4T = new Pattern(t,freq,groove);

        int[] resIndex =  getWindow1(p4Window,p4T,l,r);
        if(resIndex[1]  == -1){ return ""; }

        String res = s.substring(resIndex[0],resIndex[1]+1);

        while( resIndex[0] < p4Window.s.length()){
            resIndex = getWindow2(p4Window,p4T,resIndex[0], resIndex[1]);
            //从底层返回消息-1，表示窗口已经不需要蠕动了
            if(resIndex[1] == -1) {return  res;}

            int resLen = resIndex[1]-resIndex[0]+1;
            if(res.length() > resLen ) {
                res = s.substring(resIndex[0],resIndex[1]+1);
            }
        }
        return res;
    }

    /**
     * 保证从当前l开始，满足坑位条件下尽可能小
     */
    private static int[] getWindow1(Pattern p4Window,Pattern p4T, int l ,int r){
        // 维护窗口的属性,先大而全
        while(r+1 < p4Window.s.length()){
            r++;
            int next = p4Window.s.charAt(r);
            p4Window.freq[next]++;
            if( p4Window.freq[next] == p4T.freq[next] ){
                p4Window.groove++;
            }
            if(p4Window.groove == p4T.groove){
                break;
            }
        }

        int tail = p4Window.s.charAt(l);

        // 然后 不断缩小直到满足窗口条件，不能再去掉任何一个
        // 以及 没有踩中槽说明它是多余的，可以去掉
        while ( l< r && (p4Window.freq[tail] > p4T.freq[tail] || p4T.freq[tail] == 0 ) ){
            p4Window.freq[tail]--;
            if( p4Window.freq[tail] == -1){
                p4Window.freq[tail] = 0;
            }
            l++;
            tail = p4Window.s.charAt(l);
        }


        if(p4Window.groove == p4T.groove){
            return new int[]{l,r};
        }else{
            return new int[]{-1,-1};
        }
    }

    /**
     * 破坏掉现在满足条件的窗口条件，开启下一轮寻找最小的
     */
    private static int[] getWindow2(Pattern p4Window,Pattern p4T, int l ,int r){
        // l与r已经相遇
        if( l == r ){
            return new int[]{-1,-1};
        }
        int tail = p4Window.s.charAt(l);
        if( p4Window.freq[tail] == p4T.freq[tail]){
            p4Window.groove--;
        }
        p4Window.freq[tail]--;
        l++;
        return getWindow1(p4Window,p4T,l,r);
    }
    class Pattern {
        String s;
        int[] freq;
        int groove;
        Pattern(String s) {
            this.s = s;
            this.freq  = new int[256];
            this.groove = 0;
        }
        Pattern(String s,int[] freq, int groove){
            this.s =s;

            for(int i=0;i<s.length();i++){
                freq[s.charAt(i)]++;
            }
            this.freq = freq;

            for(int a : freq){
                if( a != 0) {
                    groove++;
                }
            }
            this.groove = groove;
        }

    }
}

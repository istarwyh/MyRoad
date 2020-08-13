/*
 * @Descripttion:
 * @version: 1.0
 * @Author: Yihui
 * @Date: 2019-10-21 09:11:21
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-21 11:05:08
 */
//摩尔投票法

import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int n = input.nextInt();
        int[] a = new int[n];
        for (int i = 0; i < n; i++) {
            a[i] = input.nextInt();
        }
        int count = 0;
        int[] a1 = new int[99];
        for (int i = 0; i < 99; i++) a1[i] = 0;
        int k = 0;
        Boolean flag = true;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (a[i] == a[j]) {
                    count++;
                }
            }
            //java会自动从int变成double的计算,很贴合实际!
            if (count > n / 3.0) {
                for (int j = 0; j < k; j++) {
                    if (a1[j] == a[i]) {
                        flag = false;
                    }
                }
                if (flag) {
                    a1[k] = a[i];
                    k++;
                }
                flag = true;
            }
            count = 0;
        }
        for (int i = 0; i < k; i++) {
            System.out.print(a1[i] + " ");
        }
        System.out.println(n / 3.0);
        input.close();
    }

}                 

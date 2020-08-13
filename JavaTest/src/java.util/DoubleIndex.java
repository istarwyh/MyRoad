/*
 * @Descripttion:
 * @version: 1.0
 * @Author: Yihui
 * @Date: 2019-10-21 08:19:48
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-21 10:57:23
 */

import java.util.*;
//冒泡,让右指针不停的与左指针相邻比较,将左指针与右指针交换

public class DoubleIndex {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        //System.out.println("请输入数组的长度:");
        int n = input.nextInt();
        int[] a1 = new int[n];

        int i;
        for (i = 0; i < n; i++) {
            a1[i] = input.nextInt();
        }
        int[] a2 = new int[n];
        int[] a3 = new int[n];
        int slow = 0, fast = 0;
        while (fast < n) {
            if (a1[fast] % 2 != 0) {
                a2[slow] = a1[fast];
                slow++;
            }
            fast++;
        }

        int oslow = 0;
        fast = 0;
        while (fast < n) {
            if (a1[fast] % 2 == 0) {
                a3[oslow] = a1[fast];
                oslow++;
            }
            fast++;
        }
        for (i = 0; i < slow; i++) {
            System.out.print(a2[i] + " ");
        }
        for (i = 0; i < oslow; i++) {
            System.out.print(a3[i] + " ");
        }
        input.close();
    }
}
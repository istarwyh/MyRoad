/*
 * @Descripttion:
 * @version:
 * @Author: Yihui
 * @Date: 2019-09-18 00:43:39
 * @LastEditors: Yihui
 * @LastEditTime: 2019-09-18 14:28:58
 */

import java.util.Scanner;

public class exercise_03_08 {
    public static void sort(int[] array) {
        int i, j;
        int temp;
        for (i = 1; i <= array.length - 1; i++) {
            for (j = 1; j <= array.length - i; j++) {
                if (array[i] < array[i - 1]) {
                    temp = array[i];
                    array[i] = array[i - 1];
                    array[i - 1] = temp;
                }
            }
        }
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int[] a = new int[3];
        for (int i = 0; i < 3; i++) {
            a[i] = in.nextInt();
        }
        sort(a);
        for (int i = 0; i < 2; i++) {
            System.out.print(a[i] + " ");
        }
        System.out.print(a[2]);
        in.close();
    }
}
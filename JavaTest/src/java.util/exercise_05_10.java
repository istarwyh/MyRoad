/*
 * @Descripttion:
 * @version:
 * @Author: Yihui
 * @Date: 2019-09-18 18:57:11
 * @LastEditors: Yihui
 * @LastEditTime: 2019-09-20 23:34:38
 */
public class exercise_05_10 {
    public static void main(String[] args) {
        int i, j;
        int[] a = new int[100];
        for (i = 100, j = 0; i <= 1000; i++) {
            if (i % 5 == 0 && i % 6 == 0) {
                a[j] = i;
                j++;
            }
        }
        for (i = 0, j = 1; i < a.length; i++) {
            if (a[i] == 0) break;
            //System.out.print(a[i]);
            if (i / 10 >= j) {
                System.out.println();
                j++;
            } else if (i != a.length - 1 && i != 0)
                System.out.print(" ");
            System.out.print(a[i]);
        }
    }
}
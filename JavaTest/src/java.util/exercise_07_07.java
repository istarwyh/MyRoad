/*
 * @Descripttion:
 * @version:
 * @Author: Yihui
 * @Date: 2019-09-20 23:30:53
 * @LastEditors: Yihui
 * @LastEditTime: 2019-09-21 15:48:20
 */
public class exercise_07_07 {
    public static void main(String[] args) {
        int[] count = new int[100];
        int[] counts = new int[10];
        for (int i = 0; i < 100; i++) {
            count[i] = (int) (Math.random() * 10);
        }
        for (int i = 0; i < 100; i++) {
            if (count[i] == 0) {
                counts[0]++;
            } else if (count[i] == 1) {
                counts[1]++;
            } else if (count[i] == 2) {
                counts[2]++;
            } else if (count[i] == 3) {
                counts[3]++;
            } else if (count[i] == 4) {
                counts[4]++;
            } else if (count[i] == 5) {
                counts[5]++;
            } else if (count[i] == 6) {
                counts[6]++;
            } else if (count[i] == 7) {
                counts[7]++;
            } else if (count[i] == 8) {
                counts[8]++;
            } else {
                counts[9]++;
            }
        }
        for (int i = 0; i < 10; i++) {
            System.out.println("出现" + i + "的次数是:" + counts[i]);
        }
    }
}
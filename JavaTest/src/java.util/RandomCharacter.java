/*
 * @Descripttion:
 * @version:
 * @Author: Yihui
 * @Date: 2019-09-17 10:21:10
 * @LastEditors: yihui
 * @LastEditTime: 2019-09-17 10:53:18
 */
public class RandomCharacter {
    public static char getRandomCharacter(char ch1, char ch2) {
        return (char) (ch1 + Math.random() * (ch2 - ch1 + 1));
    }

    public static char getRandomCharacter() {
        return getRandomCharacter('A', 'Z');
    }

    public static void main(String[] args) {
        int j = 1;
        for (int i = 0; i < 500; i++) {
            if (i / 100 >= j) {
                System.out.println();
                j++;
            }

            System.out.print(getRandomCharacter());
        }


    }
}






























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































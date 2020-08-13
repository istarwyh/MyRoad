import java.util.Scanner;

/*
 * @Descripttion:
 * @version:
 * @Author: Yihui
 * @Date: 2019-09-16 19:05:48
 * @LastEditors: yihui
 * @LastEditTime: 2019-09-17 09:26:59
 */
/*也可以从2开始判断是否为num1和num2的最大公约数,依此类推,直到大于num1 或num2*/
public class greatestCommonDivisor {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int num1 = input.nextInt();
        int num2 = input.nextInt();
        int temp;
        //从大到小遍历
        if (num1 > num2) temp = num2;
        else temp = num1;

        if (temp <= 1) System.out.println("There is no greatest common divisor");
        else {
            while (temp > 1) {
                if (num1 % temp == 0 && num2 % temp == 0) {
                    System.out.println(temp);
                    break;
                }

                temp--;
            }
            if (temp <= 1) {
                System.out.println("There is no greatest common divisor");
            }
        }

        input.close();
    }

    //System.out.println((num%2)==0?num+" is even":num+" is odd");

}
/*
 * @Descripttion:
 * @version:
 * @Author: Yihui
 * @Date: 2019-09-17 22:51:28
 * @LastEditors: Yihui
 * @LastEditTime: 2019-09-20 23:34:30
 */

import java.util.Scanner;

public class exercise_04_21 {
    public static void main(String[] args) {

        Scanner input = new Scanner(System.in);
        System.out.print("Enter your SSN:");
        String securityNumber = input.nextLine();
        char s4 = securityNumber.charAt(3);
        char s7 = securityNumber.charAt(6);
        int flag = 1;
        if (s4 == '-' && s7 == '-' && securityNumber.length() == 11) {
            for (int i = 0; i < 10; i++) {
                char si = securityNumber.charAt(i);
                if (si >= '0' && si <= '9' || si == '-') {
                } else {
                    flag = 0;
                    break;
                }
            }
        } else {
            flag = 0;
        }
        if (flag == 1) {
            System.out.println(securityNumber + " is a valid social security number");
        } else {
            System.out.print(securityNumber + " is an invalid social security number");
        }
        input.close();
    }
}
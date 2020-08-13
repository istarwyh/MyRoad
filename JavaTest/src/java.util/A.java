/*
 * @Descripttion:
 * @version: 1.0
 * @Author: Yihui
 * @Date: 2019-10-19 22:52:07
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-19 22:58:49
 */
public class A {

    public static void main(String[] args) {
        int year = 124;
        String str = String.valueOf(year);
        StringBuilder sb = new StringBuilder(str);
        sb.append("World!");
        System.out.println(sb);
        System.out.println(str);


    }

}

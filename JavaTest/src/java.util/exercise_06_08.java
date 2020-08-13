/*
 * @Descripttion: 为了让它猜编码方式
 * @version:
 * @Author: Yihui
 * @Date: 2019-09-18 19:52:56
 * @LastEditors: Yihui
 * @LastEditTime: 2019-09-20 23:32:24
 */
public class exercise_06_08 {
    public static double celsiusToFahrenheit(double celsius) {
        double fahrenheit;
        fahrenheit = (9.0 / 5.0) * celsius + 32.0;
        return fahrenheit;
    }

    public static double fahrenheitToCelsius(double fahrenheit) {
        double celsius;
        celsius = (5.0 / 9.0) * (fahrenheit - 32.0);
        return celsius;
    }

    public static void main(String[] args) {
        double celsius, fahrenheit;

        System.out.println("摄氏度         华氏度         华氏度         摄氏度");
        System.out.print("----------------------------------------------------\n");
        for (double i = 40.0, j = 120.0; i >= 31.0 && j >= 30.0; i--) {
            celsius = celsiusToFahrenheit(i);
            fahrenheit = fahrenheitToCelsius(j);
            System.out.printf("%-15.1f", i);
            //System.out.print("           ");
            System.out.printf("%-15.1f", celsius);
            //System.out.print("           ");
            System.out.printf("%-15.1f", j);
            //System.out.print("           ");
            System.out.printf("%-15.1f", fahrenheit);
            System.out.println();
            j -= 10.0;
        }

    }
}
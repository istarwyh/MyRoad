import java.util.Scanner;

/*
 * @Descripttion:
 * @version:
 * @Author: Yihui
 * @Date: 2019-09-21 18:47:59
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-05 21:30:56
 */
public class exercise_08_05 {
    public static double[][] Matrix(int i) {
        Scanner in = new Scanner(System.in);
        System.out.println("Enter matrix" + i + ":");
        double[][] m = new double[3][3];
        int j = 0;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                m[i][j] = in.nextDouble();
            }
        }
        in.close();
        return m;
    }

    public static double[][] addMatrix(double[][] a, double[][] b) {
        int i = 0, j = 0;
        double[][] c = new double[3][3];
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                c[i][j] = a[i][j] + b[i][j];
            }
        }
        return c;
    }

    public static void main(String[] args) {
        double[][] c1 = new double[3][3];
        double[][] c2 = new double[3][3];
        double[][] c = new double[3][3];
        c1 = Matrix(1);
        c2 = Matrix(2);
        c = addMatrix(c1, c2);
        System.out.println("The matrices are added as follows");
        int i = 0, j;
        for (; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                System.out.printf("%-5.1f", c1[i][j]);
            }
            if (i == 1) {
                System.out.print("   +   ");
            }
            if (i == 0 || i == 2) {
                System.out.print("       ");
            }

            for (j = 0; j < 3; j++) {
                System.out.printf("%-5.1f", c2[i][j]);
            }
            if (i == 1) {
                System.out.print("  =   ");
            }

            if (i == 0 || i == 2) {
                System.out.print("      ");
            }

            for (j = 0; j < 3; j++) {
                System.out.printf("%-5.1f", c[i][j]);
            }
            if (i == 0) {
                System.out.println();
            }
            if (i == 1) {
                System.out.println();
            }
        }
    }
}
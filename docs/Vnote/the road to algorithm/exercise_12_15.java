import java.io.FileNotFoundException;
import java.util.Scanner;

/*
 * @Descripttion:
 * @version: 1.0
 * @Author: Yihui
 * @Date: 2019-10-20 07:31:11
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-20 09:01:19
 */
public class exercise_12_15 {
    public static void main(String[] args) throws FileNotFoundException {
        java.io.File file = new java.io.File("exercise_12_15.txt");

        if (file.exists()) {
            System.out.println("File already exists");
            System.exit(1);
        }
        java.io.PrintWriter output = new java.io.PrintWriter(file);
        int[] array = new int[100];
        for (int i = 0; i < 100; i++) {
            array[i] = (int) (1 + Math.random() * (100 - 1 + 1));
        }
        for (int i = 0; i < 100; i++) {
            output.print(array[i] + " ");
        }


        Scanner input = new Scanner(file);
        int j = 0;
        while (input.hasNext() && j < 100) {
            int i = input.nextInt();
            array[j++] = i;
        }
        exercise_12_15 s = new exercise_12_15();
        s.BubbleSort(array);
        output.println();
        for (int i = 0; i < 100; i++) {
            output.printf("%-4d", array[i]);
        }
        input.close();
        output.close();
    }

    public void BubbleSort(int[] a) {
        int i, j, tmp;
        for (i = 0; i < a.length - 1; i++) {
            for (j = 0; j < a.length - i - 1; j++) {
                if (a[j] > a[j + 1]) {
                    tmp = a[j + 1];
                    a[j + 1] = a[j];
                    a[j] = tmp;
                }
            }
        }
    }
}
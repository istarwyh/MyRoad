import java.util.Arrays;
import java.util.HashSet;
import java.util.Random;
import java.util.Scanner;

public class Test2 {
    // 声明一个静态的Scanner工具类,以便调用同时最后结束时才关闭
    public static Scanner input = new Scanner(System.in);

    public static void main(final String[] args) {
        int[] range = new int[2];
        Test2 t = new Test2();

        System.out.println("====得到正确的输入范围====");
        // 输入范围检查,并返回正确的输入范围
        int[] inputRange = t.test1(range);

        System.out.println("====等价类划分技术====");

        // 使用等价类划分技术返回三个区间
        int[][] equalClass = t.test2(inputRange);
        for (int[] row : equalClass)
            System.out.println(Arrays.toString(row));

        System.out.println("====边界值划分技术====");
        // 边界值分析返回六个边界值数组
        int[][] criticValues = t.test3(equalClass);
        for (int[] row : criticValues)
            System.out.println(Arrays.toString(row));
        t.alert(3);
        t.alert(4);

        System.out.println("====随机数生成技术====");
        //随机数生成返回随机数组
        Object[] randomValues = t.test4(equalClass);
        for (int i = 0; i < randomValues.length; i++) {
            HashSet<Integer> row = (HashSet<Integer>) randomValues[i];
            System.out.println(row.toString());
        }

        input.close();

    }

    public int[] test1(int[] range) {
        // Scanner input = new Scanner(System.in);
        int errorType = 0;
        do {
            System.out.println("请输入下界与上界");
            String stringLow = input.next();
            // System.out.println("low"+low);
            String stringHigh = input.next();
            // System.out.println("high"+high);

            if (this.isInteger(stringLow) && this.isInteger(stringHigh)) {
                int low = Integer.parseInt(stringLow);
                int high = Integer.parseInt(stringHigh);
                if (this.isProper(low, high)) {
                    range[0] = low;
                    range[1] = high;
                    break;
                } else {
                    errorType = 2;
                    this.alert(errorType);
                }
            } else {
                errorType = 1;
                this.alert(errorType);
            }
        } while (errorType != 0);
        return range;
    }

    public int[][] test2(int[] inputRange) {
        int[][] rangeArray = new int[3][2];
        rangeArray[0] = new int[]{Integer.MIN_VALUE, inputRange[0] - 1};
        rangeArray[1] = inputRange;
        rangeArray[2] = new int[]{inputRange[inputRange.length - 1] + 1, Integer.MAX_VALUE};
        return rangeArray;
    }

    // 边界值分析,防止重复设置了两个非1的常量
    public int[][] test3(int[][] rangeArray) {
        final int Ten = 10;
        final int Hundred = 100;
        int[][] criticValues = new int[6][3];
        criticValues[0] = new int[]{rangeArray[0][0] - 1, rangeArray[0][0], rangeArray[0][0] + 1};
        criticValues[1] = new int[]{rangeArray[0][1] - 1, rangeArray[0][1], rangeArray[0][1] + Ten};
        criticValues[2] = new int[]{rangeArray[1][0] - Hundred, rangeArray[1][0], rangeArray[1][0] + 1};
        criticValues[3] = new int[]{rangeArray[1][1] - 1, rangeArray[1][1], rangeArray[1][1] + Hundred};
        criticValues[4] = new int[]{rangeArray[2][0] - Ten, rangeArray[2][0], rangeArray[2][0] + 1};
        criticValues[5] = new int[]{rangeArray[2][1] - 1, rangeArray[2][1], rangeArray[2][1] + 1};
        return criticValues;
    }

    // 随机数生成
    public Object[] test4(int[][] rangeArray) {
        System.out.print("请输入要生成的随机数数目: ");
        int n = input.nextInt();
        Object[] randomValues = new Object[3];
        for (int i = 0; i < randomValues.length; i++)
            randomValues[i] = this.randomValueSet(i, n, rangeArray);

        return randomValues;
    }

    public HashSet<Integer> randomValueSet(int index, int n, int[][] rangeArray) {
        // 使用HashSet来去重
        HashSet<Integer> randomValueSet = new HashSet<Integer>();
        // 如果正常用int,这里减去一个Integer.MIN_VALUE就很容易整数溢出
        long gap = (long) rangeArray[index][1] - (long) rangeArray[index][0];
        while (randomValueSet.size() < n) {
            // temp最大也就取到给的区间的最大值,因而可以用int
            int temp = (int) (Math.random() * gap + rangeArray[index][0]) + 1;
            randomValueSet.add(temp);
        }
        return randomValueSet;
    }

    // 是否是整数
    public boolean isInteger(String input) {
        // 检测整数的正则表达式
        final String regex = "^-?[1-9]\\d*$|0{0,1}";
        return input.matches(regex);
    }

    // 范围适合low<high
    public boolean isProper(int low, int high) {
        if (low > high)
            return false;
        return true;
    }

    // 错误报告
    public void alert(int errorType) {
        if (errorType == 1) {
            System.out.println("不是整数,请重新输入");
        }
        if (errorType == 2) {
            System.out.println("下界大于上界,请重新输入");
        }
        if (errorType == 3) {
            System.out.println("Error: -2147483648-1 " + "负整数溢出");
        }
        if (errorType == 4) {
            System.out.println("Error: 2147483647+1 " + "正整数溢出");
        }
    }
}
package com.itheima.domain;

import java.io.IOException;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Scanner;

public class Test2 {
    /**
     * 声明一个静态的Scanner工具类,以便调用同时最后结束时才关闭
     */
    private static Scanner input = new Scanner(System.in);

    public static void main(final String[] args) throws IOException {
        int[] range = new int[2];
        Test2 t = new Test2();
        try {
            System.out.println("====得到正确的输入范围====");
            // 输入范围检查,并返回正确的输入范围
            int[] inputRange = t.test1(range);
            System.out.println("====等价类划分技术====");

            // 使用等价类划分技术返回三个区间
            int[][] equalClass = t.test2(inputRange);
            for (int[] row : equalClass) {
                System.out.println(Arrays.toString(row));
            }
            System.out.println("====边界值划分技术====");

            // 边界值分析返回六个边界值数组
            int[][] criticValues = t.test3(equalClass);
            for (int[] row : criticValues) {
                System.out.println(Arrays.toString(row));
            }
            t.alert(3);
            t.alert(4);

            System.out.println("====随机数生成技术====");
            System.out.print("请输入要生成的随机数数目: ");
            int n = input.nextInt();
            //随机数生成返回随机数组
            for (HashSet<?> row : (HashSet<Integer>[]) t.test4(equalClass, n)) {
                System.out.println( row.toString() );
            }

            input.close();
        } catch (ArithmeticException e) {
            System.out.println(e.getClass().getName() + e.getMessage() + "\n请重新运行程序输入合理数值");
        }


    }

    public int[] test1(int[] range) throws IOException {
        int errorType = 0;
        do {
            System.out.println("请输入下界与上界");
            String stringLow = input.next();
            String stringHigh = input.next();

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
        if (inputRange[0] <= Integer.MIN_VALUE || inputRange[1] >= Integer.MAX_VALUE)
            throw new ArithmeticException("Integer overflow");
        int[][] rangeArray = new int[3][2];
        rangeArray[0] = new int[]{Integer.MIN_VALUE, inputRange[0] - 1};
        rangeArray[1] = inputRange;
        rangeArray[2] = new int[]{inputRange[inputRange.length - 1] + 1, Integer.MAX_VALUE};
        return rangeArray;
    }

    /**
     * 边界值分析,防止重复设置了两个非1的常量
     *
     * @param rangeArray
     * @return
     */
    public int[][] test3(int[][] rangeArray) {
        int limit = 3;
        if (rangeArray[0][1] - rangeArray[0][0] == limit || rangeArray[1][1] - rangeArray[1][0] == limit || rangeArray[2][1] - rangeArray[2][0] == limit) {
            throw new RuntimeException("区间长度为3不能生成不重复的边界值");
        }
        int[][] criticValues = new int[6][3];
//        6个边界值的加一减一
        criticValues[0] = new int[]{rangeArray[0][0] - 1, rangeArray[0][0], rangeArray[0][0] + 1};
        criticValues[1] = new int[]{rangeArray[0][1] - 1, rangeArray[0][1], rangeArray[0][1] + 3};
        criticValues[2] = new int[]{rangeArray[1][0] - 3, rangeArray[1][0], rangeArray[1][0] + 1};
        criticValues[3] = new int[]{rangeArray[1][1] - 1, rangeArray[1][1], rangeArray[1][1] + 3};
        criticValues[4] = new int[]{rangeArray[2][0] - 3, rangeArray[2][0], rangeArray[2][0] + 1};
        criticValues[5] = new int[]{rangeArray[2][1] - 1, rangeArray[2][1], rangeArray[2][1] + 1};
        return criticValues;
    }

    /**
     * 随机数生成
     *
     * @param rangeArray
     * @param n
     * @return
     */
    public Object[] test4(int[][] rangeArray, int n) {
        if (!isLegalRangeArray(rangeArray, n)) {
            throw new RuntimeException("区间过小不足以生成指定数目的随机数");
        }
        HashSet<?>[] randomValues = new HashSet[3];
        for (int i = 0; i < randomValues.length; i++) {
            randomValues[i] = this.randomValueSet(i, n, rangeArray);
        }

        return randomValues;
    }

    /**
     * 用index控制生成第几行的随机数
     */
    public HashSet<Integer> randomValueSet(int index, int n, int[][] rangeArray) {
        // 使用HashSet来去重
        HashSet<Integer> randomValueSet = new HashSet<>();
        // 如果正常用int,这里减去一个Integer.MIN_VALUE就很容易整数溢出
        long gap = (long) rangeArray[index][1] - (long) rangeArray[index][0];
        while (randomValueSet.size() < n) {
            // temp最大也就取到给的区间的最大值,因而可以用int
            int temp = (int) (Math.random() * gap + rangeArray[index][0]) + 1;
            randomValueSet.add(temp);
        }
        return randomValueSet;
    }

    /**
     * 是否是整数
     */
    public boolean isInteger(String input) {
        // 检测整数的正则表达式
        final String regex = "^-?[1-9]\\d*$|0?";
        return input.matches(regex);
    }

    /**
     * 范围适合 low<high
     */
    public boolean isProper(int low, int high) {
        if (low > high)
            return false;
        return true;
    }

    /**
     * 错误报告
     */
    public void alert(int warningType) {
        switch (warningType) {
            case 1:
                System.out.println("不是整数!请重新输入");
                return;
            case 2:
                System.out.println("下界大于上界!请重新输入");
                return;
            case 3:
                System.out.println("Warning: Integer下界越界!");
                return;
            case 4:
                System.out.println("Warning: Integer上界越界!");
                return;
            default:
                throw new IllegalStateException("Unexpected value: " + warningType);
        }
    }

    public boolean isLegalRangeArray(int[][] rangeArray, int limit) {
        if (limit > rangeArray[0][1] - rangeArray[0][0] || limit > rangeArray[1][1] - rangeArray[1][0] || limit > rangeArray[2][1] - rangeArray[2][0]) {
            return false;
        }
        return true;
    }
}
package com.itheima.domain;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;

import static org.junit.Assert.*;

import java.util.Arrays;
import java.util.Collection;

// 将所有需要运行的测试用例集中起来，一次性地运行所有测试用例
// 为准备使用参数化测试的测试类指定特殊的运行器 org.junit.runners.Parameterized
@RunWith(Parameterized.class)
public class Test2test2 {
    // 分别用于存放期望值和测试所用数据
    private int[][] expected = new int[3][2];
    private int[] target = new int[2];

    // 在java.util.Collection 的公共静态方法初始化所有需要测试的参数对
    @Parameterized.Parameters
    public static Collection<Integer[]> getTestParas() {
        return Arrays.asList(new Integer[][]{
                {-2147483648, -6, -5, 10, 11, 2147483647, -5, 10},// 测试一般情况
                {-2147483648, -1, 0, 1, 2, 2147483647, 0, 1},//测试边界
                {-2147483648, -2, -1, 0, 1, 2147483647, -1, 0},//测试边界
                {-2147483648, 2147483647, -2147483648, 2147483647, -2147483648, 2147483647, -2147483648, 2147483647}// 测试边界与错误处理路径
        });
    }

    //构造函数依次调用Collection中的数组元素构造
    public Test2test2(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8) {
        this.expected[0][0] = n1;
        this.expected[0][1] = n2;
        this.expected[1][0] = n3;
        this.expected[1][1] = n4;
        this.expected[2][0] = n5;
        this.expected[2][1] = n6;
        this.target[0] = n7;
        this.target[1] = n8;
    }

    @Before
    public void setUp() throws Exception {
        System.out.println("===一个测试" + "开始===");

    }

    @After
    public void tearDown() throws Exception {
        System.out.println("===测试" + "已结束===");
    }

    @Test
    public void test2() {
//        System.out.println("===测试等价类划分===");
        assertArrayEquals("整数溢出", expected, new Test2().test2(target));

    }
}
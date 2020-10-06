package com.domain;

import org.junit.jupiter.api.Test;

import static org.junit.Assert.assertArrayEquals;

class Test2test3 {
    Test2 tmpObject = new Test2();

    @Test
    public void test3() {
        int[][] t1 = {{2147483647, -2147483648, -2147483647}, {-7, -6, -3}, {-8, -5, -4}, {9, 10, 13}, {8, 11, 12}, {2147483646, 2147483647, -2147483648}};
        assertArrayEquals(t1, tmpObject.test3(new int[][]{{-2147483648, -6}, {-5, 10}, {11, 2147483647}}));
    }

    @Test
    public void test3_2() {
        int[][] t2 = {{2147483647, -2147483648, -2147483647}, {-2, -1, 2}, {-3, 0, 1}, {0, 1, 4}, {-1, 2, 3}, {2147483646, 2147483647, -2147483648}};
        assertArrayEquals(t2, tmpObject.test3(new int[][]{{-2147483648, -1}, {0, 1}, {2, 2147483647}}));
    }

    @Test
    public void test3_3() {
        int[][] t3 = {{2147483647, -2147483648, -2147483647}, {-3, -2, 1}, {-4, -1, 0}, {-1, 0, 3}, {-2, 1, 2}, {2147483646, 2147483647, -2147483648}};
        assertArrayEquals(t3, tmpObject.test3(new int[][]{{-2147483648, -2}, {-1, 0}, {1, 2147483647}}));
    }

    /**
     * 测试错误抛出路径
     */
    @Test
    public void test3_4() {
        assertArrayEquals(null, tmpObject.test3(new int[][]{{-2147483648, 3}, {4, 7}, {8, 2147483647}}));
    }


}
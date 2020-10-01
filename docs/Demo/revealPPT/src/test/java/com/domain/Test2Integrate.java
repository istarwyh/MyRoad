package com.domain;


import org.junit.jupiter.api.Test;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;

public class Test2Integrate {
    @Test
    public void test2_test3() {
        int[] t1 = new int[]{-5, 10};
        Test2 tmpObject = new Test2();
        int[][] t_test3 = {{2147483647, -2147483648, -2147483647}, {-7, -6, -3}, {-8, -5, -4}, {9, 10, 13}, {8, 11, 12}, {2147483646, 2147483647, -2147483648}};
        assertArrayEquals(t_test3, tmpObject.test3(tmpObject.test2(t1)));
    }

    @Test
    public void test2_test4() {
        int[] t1 = new int[]{-5, 10};
        Test2 tmpObject = new Test2();
        /**随机数模块已无问题,这里用长度参数反映程序造件正常*/
        assertEquals(3, tmpObject.test4(tmpObject.test2(t1), 5).length);
    }
}

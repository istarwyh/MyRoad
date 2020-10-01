package com.domain;

import org.junit.jupiter.api.Test;

import java.util.HashSet;
import java.util.Iterator;

import static org.junit.jupiter.api.Assertions.*;

class Test2test4 {

    @Test
    void test4I() {
        Test2 tmpObj = new Test2();
        // 输入正常的测试条件
        Object[] t1 = tmpObj.test4(new int[][]{{-2147483648, -6}, {-5, 10}, {11, 2147483647}}, 5);
        assertEquals(3, t1.length);
    }

    @Test
    void test4II() {
        Test2 tmpObj = new Test2();
        // 输入中间区间很小的边界条件
        Object[] t2 = tmpObj.test4(new int[][]{{-2147483648, -2}, {-1, 1}, {2, 2147483647}}, 5);
        assertEquals(3, t2.length);
    }

    @Test
    void test4III() {
        Test2 tmpObj = new Test2();
        // 输入左边区间很小的边界条件
        Object[] t3 = tmpObj.test4(new int[][]{{-2147483648, -2147483647}, {-2147483646, 1}, {2, 2147483647}}, 5);
        assertEquals(3, t3.length);
    }

    @Test
    void test4IV() {
        Test2 tmpObj = new Test2();
        // 输入右边区间很小的边界条件
        Object[] t4 = tmpObj.test4(new int[][]{{-2147483648, -1}, {0, 2147483645}, {2147483646, 2147483647}}, 5);
        assertEquals(3, t4.length);
    }

    @Test
    void test4V() {
        Test2 tmpObj = new Test2();
        // 覆盖独立路径集{113.1-113.2-113.3-116-117.1-117.2-125-126,127,128-133-117.3-117.2-120}
        Object[] t5 = tmpObj.test4(new int[][]{{-2147483648, -2}, {-1, 1}, {2, 2147483647}}, 3);
        assertEquals(3, t5.length);
    }

    // 输入正常的测试条件
    @Test
    void randomValueSet() {

        Test2 tmpObject = new Test2();
        HashSet<Integer> t1 = tmpObject.randomValueSet(0, 5, new int[][]{{-2147483648, -6}, {-5, 10}, {11, 2147483647}});
        // 测试元素个数
        assertEquals(5, t1.size());
        // 测试元素重复
        assertEquals(1, this.repeatTimeK(t1));
        // 测试元素区间内
        Iterator<Integer> it = t1.iterator();
        it.forEachRemaining(i ->
                assertEquals(true, i > Integer.MIN_VALUE & i < Integer.MAX_VALUE)
        );
    }

    // 覆盖独立路径集{113.1-113.2-113.3-116-117.1-117.2-125-126,127,128-133-117.3-117.2-120}
    @Test
    void randomValueSet5() {

        Test2 tmpObject = new Test2();
        HashSet<Integer> t1 = tmpObject.randomValueSet(0, 3, new int[][]{{-2147483648, -2}, {-1, 1}, {2, 2147483647}});
        // 测试元素个数
        assertEquals(3, t1.size());
        // 测试元素重复
        assertEquals(1, this.repeatTimeK(t1));
        // 测试元素区间内
        Iterator<Integer> it = t1.iterator();
        it.forEachRemaining(i ->
                assertEquals(true, i > Integer.MIN_VALUE & i < Integer.MAX_VALUE)
        );
    }

    public int repeatTimeK(HashSet<Integer> t1) {
        // Set -> 数组
        Integer[] t = t1.toArray(new Integer[t1.size()]);
        int k = 1;
        for (int i = 0; i < t.length; i++) {
            for (int j = i + 1; j < t.length; j++) {
                if (t[i].equals(t[j]))
                    k++;
            }
        }
        return k;
    }
}

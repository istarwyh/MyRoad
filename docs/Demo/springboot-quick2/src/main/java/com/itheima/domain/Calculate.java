package com.itheima.domain;

import java.util.Collection;
import java.util.HashSet;

public class Calculate {
    public static void main(String[] args) {
        Collection<Integer> t = new HashSet<>();
        t.add(1);
        t.add(2);
        t.add(3);
        t.add(4);
        Integer[] Int = t.toArray(new Integer[t.size()]);
        if (Int[0] == Int[1])
            System.out.println("true");
        else
            System.out.println("false");
        Integer a = 100;
        Integer b = 100;
        if (a == b)
            System.out.println("true");

    }
}

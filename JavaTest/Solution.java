import java.util.Scanner;
import java.util.*;
public class Solution {
    static int x = 1;
    static int y = 2;
    public static void main(String[] args) {
        swap(x, y);
        System.out.println(x + " "+y);
    }
    public static void swap(int x, int y) {
        int tmp = x;
        x=y;
        y=tmp;
    }
}
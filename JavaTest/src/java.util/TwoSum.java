/*
 * @Descripttion:
 * @version:
 * @Author: Yihui
 * @Date: 2019-09-09 10:25:18
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-07 21:24:26
 */

import java.util.HashMap;
import java.util.Scanner;

public class TwoSum {

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int n, target;
        System.out.printf("Enter the length of array:");
        n = input.nextInt();
        int[] nums = new int[n];
        System.out.printf("Enter the elements of array:");
        for (int i = 0; i < n; i++) {
            nums[i] = input.nextInt();
        }
        System.out.printf("Enter the target:");
        target = input.nextInt();
        int[] t = new int[2];
        t = twoSum(nums, target);
        // for(int i=0;i<2;i++){
        //     //System.out.printf("%-2d",t[i]);
        // }
        System.out.print(t[0] + " ");
        System.out.print(t[1]);
        input.close();
    }

    public static int[] twoSum(int[] nums, int target) {
        if (nums == null || nums.length == 0) return new int[]{-1, -1};
        HashMap<Integer, Integer> map = new HashMap<Integer, Integer>();
        for (int i = 0; i < nums.length; i++)
            map.put(nums[i], i);
        for (int i = 0; i < nums.length; i++) {
            int temp = target - nums[i];
            if (map.containsKey(temp) && map.get(temp) != i)
                return new int[]{i, map.get(temp)};
        }
        return new int[]{-1, -1};

    }
}
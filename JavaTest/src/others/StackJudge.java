import java.util.*;
import java.util.Stack;
import java.util.Scanner;
import java.util.ArrayList;
import java.io.*;

public class StackJudge {
    public static void main(String[] args) throws FileNotFoundException {
        File sourceFile = new File("../../in.txt");
        File targetFile = new File("../..//out.txt");
        Scanner in = new Scanner(sourceFile);
        PrintWriter out = new PrintWriter(targetFile);

        List<Integer> list_push = new ArrayList<Integer>();
        out.println("请输入压栈序列:");
        int n = in.nextInt();
        while (n != -1) {
            list_push.add(n);
            n = in.nextInt();
        }
        // for (int i=0;i<list_push.size();i++)
        //     System.out.print(list_push.get(i));
        List<Integer> list_pop = new ArrayList<Integer>();
        out.println("请输入弹栈序列:");
        n = in.nextInt();
        while (n != -1) {
            list_pop.add(n);
            n = in.nextInt();
        }
        int[] pushorder = new int[list_push.size()];
        for (int i = 0; i < list_push.size(); i++) {
            pushorder[i] = list_push.get(i);
        }
        int[] poporder = new int[list_pop.size()];
        for (int i = 0; i < list_pop.size(); i++)
            poporder[i] = list_pop.get(i);
        out.println("出栈顺序是正确的吗?:\n" + judgePop(pushorder, poporder, list_push.size()));
        in.close();
        out.close();
    }

    public static Boolean judgePop(int[] pushorder, int[] poporder, int length) {
        if (pushorder != null && poporder != null && length > 0) {
            Stack<Integer> stack_judge = new Stack<Integer>();
            Stack<Integer> stack_push = new Stack<Integer>();
            Stack<Integer> stack_pop = new Stack<Integer>();
            //注意从length-1到i>=0,对于这种栈的题目,把数组放入栈之后真的就不需要数组了
            for (int i = length - 1; i >= 0; i--)
                stack_push.push(pushorder[i]);
            for (int i = length - 1; i >= 0; i--)
                stack_pop.push(poporder[i]);
            // for(int i=length-1; i>=0; i--)
            //     System.out.println(poporder[i]);
            int push_location = 0;
            //刚开始judge是空的,因此必须先把push中的元素入judge栈
            for (int i = 0; i < length; i++) {// 把flag的那个放入的话,当i=5的时候就已经越界了,尽管不会进入循环
                stack_judge.push(stack_push.peek());
                // System.out.println("<"+stack_judge.peek()+">");
                stack_push.pop();
                push_location++;
                if (stack_judge.peek() == stack_pop.peek()) break;
            }

            while (!stack_judge.empty()) {
                if (stack_judge.peek() == stack_pop.peek() && !stack_judge.empty()) {
                    // System.out.println("<>"+stack_judge.peek()+"<>");
                    stack_judge.pop();
                    stack_pop.pop();
                } else if (stack_judge.peek() != stack_pop.peek()) {

                    if (!stack_push.empty()) {
                        for (int i = push_location; i < length; i++) {
                            stack_judge.push(stack_push.peek());
                            // push_location用来辅助标记把push栈加到了什么地方
                            push_location++;
                            // System.out.println("<<"+stack_judge.peek()+">>");
                            stack_push.pop();
                            if (stack_judge.peek() == stack_pop.peek()) break;
                        }
                    } else
                        return false;
                }
            }
            if (stack_judge.empty()) return true;

        }
        return false;
    }
}
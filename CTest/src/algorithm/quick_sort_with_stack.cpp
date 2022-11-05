#include <stack>

using namespace std;
typedef struct l_h_f {
    int l;
    int h;
    int flag;
} l_h_f;

void quick_sort_with_stack(int a[], int low, int high) {
    stack<l_h_f> cs;
    l_h_f ini = {0, 0, -1};
    cs.push(ini);//指令初始化,栈底
    l_h_f n1 = {low, high, 0};
    cs.push(n1);//第一次划分开始指令
    while (true) {//用无限循环模拟递归调用
        l_h_f n2 = cs.top();
        cs.pop();
        low = n2.l;
        high = n2.h;
        // flag指示low与high的比较状态,以便跳到下一次或者跳出
        if (n2.flag == 0) {
            if (low >= high) {
                l_h_f n3 = {0, 0, 1};
                cs.push(n3);
                continue;
            }
            int i = low;
            int j = high;
            int pivot = a[low];
            // 以下为快排扫描调换
            while (i < j) {
                while (i < j && a[j] >= pivot) --j;
                if (i < j) a[i] = a[j];
                while (i < j && a[i] < pivot) ++i;
                if (i < j) a[j] = a[i];
            }
            a[i] = pivot;
            l_h_f n4 = {0, 0, 0};
            n4.l = low;
            n4.h = i - 1;
            cs.push(n4);// 给定下一次划分的范围
            n4.l = i + 1;
            n4.h = high;
            cs.push(n4);
        } else if (n2.flag == 1)
            continue;
        else if (n2.flag == -1)
            break;
    }
}
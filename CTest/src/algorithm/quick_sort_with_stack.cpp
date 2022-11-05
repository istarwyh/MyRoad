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
    cs.push(ini);//ָ���ʼ��,ջ��
    l_h_f n1 = {low, high, 0};
    cs.push(n1);//��һ�λ��ֿ�ʼָ��
    while (true) {//������ѭ��ģ��ݹ����
        l_h_f n2 = cs.top();
        cs.pop();
        low = n2.l;
        high = n2.h;
        // flagָʾlow��high�ıȽ�״̬,�Ա�������һ�λ�������
        if (n2.flag == 0) {
            if (low >= high) {
                l_h_f n3 = {0, 0, 1};
                cs.push(n3);
                continue;
            }
            int i = low;
            int j = high;
            int pivot = a[low];
            // ����Ϊ����ɨ�����
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
            cs.push(n4);// ������һ�λ��ֵķ�Χ
            n4.l = i + 1;
            n4.h = high;
            cs.push(n4);
        } else if (n2.flag == 1)
            continue;
        else if (n2.flag == -1)
            break;
    }
}
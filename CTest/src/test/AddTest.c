//
// Created by mac on 2022/9/25.
// <a href=https://zhuanlan.zhihu.com/p/164863339></a>
//
#include <stdio.h>
static int Add(int a, int b) { return a + b; }

#define Test(funcName) void Test##funcName(void)
#define Context(...) printf(__VA_ARGS__);
#define It(...) printf(__VA_ARGS__);

#define IsTrue(a) (a) ? printf("\n -> true\n") : printf("\n -> false\n")

Test(Add) {
    //第一个测试用例
    Context("when input 1 and 1") {
        It(" then returns a 2") {
            IsTrue(
                    Add(1, 1) == 2
            );
        }
    }
}

//int main(void) {
//    TestAdd();
//    return 0;
//}
// 问题1.6 - 判断是否为2的幂次
// 输入：正整数n
// 输出：判断n是否为2的幂次

#include <iostream>
#include <iomanip>
using namespace std;

// 算法1.6A（直接判定）
bool test(int n) {
    return !(n & n-1);
}

// 算法1.6B（递归）
bool test2(int n) {
    if (n % 2 == 1) { 
        return n == 1;
    }
    return test2(n / 2);
}

// 算法1.6C（循环）
bool test3(int n) {
    int exp2 = 1;
    while (exp2 < n) {
        exp2 *= 2;
    }
    return exp2 == n;
}

// 算法1.6B2（递归改循环）
bool test2_l(int n) {
    while (n % 2 == 0) {
        n /= 2;
    }
    return n == 1;
}

int main() {
    auto testN = [](int n) -> void {
        cout << "test power2(" << setw(6) << n << ") : ";
        cout << (int)test(n) << (int)test2(n) << (int)test3(n) << (int)test2_l(n) << endl;
    };
    for (int i = 1; i < 0x7FFF; i <<= 2) {
        testN(i);
        testN(i+2);
    }
    return 0;
}
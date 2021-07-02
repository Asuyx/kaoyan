// 问题1.2 - 求最大公因数
// 输入：正整数a、b
// 输出：a和b的最大公因数

#include <iostream>
#include "gcd.h"
using namespace std;

// 算法1.2A
GcdEuclidRecursion gcd;

// 测试样例
int main() {
    auto test = [](int a, int b) -> void {
        cout << "gcd(" << a << "," << b << ") = " << gcd(a, b) << endl;
    };
    pair<int, int> testCases[] {
        {1, 1}, {0, 6}, {6, 0},
        {24, 18}, {720, 1024},
        {-8, 6},  // 未定义行为
        {8, -6}   // 未定义行为
    };
    for (auto pair : testCases) {
        test(pair.first, pair.second);
    }
    return 0;
}
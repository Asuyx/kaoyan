// 问题1.8 - 幂
// 输入：正整数a、b
// 输出：a的b次幂

#include <iostream>
#include "power.h"
using namespace std;

// 算法1.8A
int power(int a, int b) {
    int prod = 1;
    for (int i = 0; i < b; ++i) {
        prod *= a;
    }
    return prod;
}

// 算法1.8B
PowerRecursion power_q;

int main() {
    auto test = [](int a, int b) -> void {
        cout << "power(" << a << "," << b << ") =\t";
        cout << power(a, b) << "\t" << power_q(a, b) << endl;
    };
    test(2, 7);
    test(3, 4);
    test(-3, 3);
    return 0;
}
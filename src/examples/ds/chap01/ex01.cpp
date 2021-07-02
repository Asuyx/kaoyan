// 问题1.1 - 从1加到n
// 输入：正整数n
// 输出：1 + 2 + ... + n的和

// 算法1.1
// 1. 输入正整数n
// 2. 计算A = n*(n+1)/2
// 3. 输出结果A

#include <iostream>
#include <iomanip>
using namespace std;

// 算法1.1A
inline int f1(int n) {
    return n*(n+1)/2;
}

// 算法1.1B
inline int f2(int n) {
    return n % 2 == 1 ? (n+1)/2*n : n/2*(n+1);
}

// 算法1.1C
// 在算法1.1B的基础上，如果结果超出int_max，则返回int_max而不会溢出
inline int f3(int n) {
    const int int_max = 2147483647;
    const int threshold = 65536;
    return n >= threshold ? int_max : f2(n); 
}

// 测试样例
int main() {
    auto testN = [](int n) -> void {
        cout << "f1(" << setw(5) << n << ")=" << setw(11) << f1(n) << "\t";
        cout << "f2(" << setw(5) << n << ")=" << setw(11) << f2(n) << "\t";
        cout << "f3(" << setw(5) << n << ")=" << setw(11) << f3(n) << "\n";
    };
    testN(10);  // f1正确，f2正确，f3正确
    testN(5e4); // f1溢出，f2正确，f3正确
    testN(7e4); // f1溢出，f2溢出，f3输出上界
    testN(-8);  // 虽然从程序的角度，这样输入n是可行的，但从算法的角度，这样的输入没有意义。
                // 因此，即使无法正确处理这类输入，算法仍然是正确的。
                // 但是，如果无法正确处理这类输入，程序的稳健性可能是存在缺陷的。
                // 因为如果开放算法供其他程序使用，则无法保证调用者一定会给出有意义的值。
                // 如果要提升程序的稳健性，在输入非正数的n时，可以返回同样“无意义的值”，如0。
                // 但这并不是《数据结构》或《算法设计》需要讨论的问题，因此以后也不会讨论。
    return 0;
}
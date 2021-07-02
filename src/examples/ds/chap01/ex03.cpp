// 问题1.3 - 数组求和
// 输入：数组A[0:n]
// 输出：A[0]+A[1]+...+A[n-1]的和

#include <iostream>
using namespace std;

// 算法1.3A
int f1(int* A, int n) {
    if (n == 0) { return 0; }
    if (n == 1) { return A[0]; }
    return f1(A, n/2) + f1(A + n/2, n - n/2);
}

// 算法1.3B
// 用于对照试验，测试算法1.3A的正确性
int f2(int* A, int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += A[i];
    }
    return sum;
}

// 用随机数生成测试样例
int* generate(int n) {
    int* A = new int[n];
    for (int i = 0; i < n; ++i) {
        A[i] = rand() & 0x7FFF; // 大多数的标准C库都将rand范围设成了0-32767
    }
    return A;
}

int main() {
    auto testA = [](int* A, int n) -> void {
        cout << "f1 = " << f1(A, n) << "\t";
        cout << "f2 = " << f2(A, n) << "\n";
    };
    testA(new int[3] { 1, 2, 3 }, 3);
    testA(generate(100), 100);
    testA(generate(10000), 10000);
    testA(generate(1000000), 1000000); // 可能溢出，溢出的情况下两种算法仍然能返回一样的值
    return 0;
}
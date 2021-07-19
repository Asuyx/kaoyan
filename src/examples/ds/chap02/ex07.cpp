// 例2.7 - 问题2.12 - 循环位移
// 给定：向量V[0:n]，位移量k（0<k<n）
// 要求：将向量V循环左移k个单位

#include <iostream>
#include "vector.h"
#include "gcd.h"
using namespace std;

GcdEuclidRecursion gcd;

// 算法2.12A
template<typename T>
void cyclicLeftShiftA(const Vector<T>& V, int k) {
    T* B = new T[k], * A = V.data();
    int n = V.size();
    arrayCopy(B, A, k);     // B[0:k] = V[0:k]
    arrayCopy(A, A+k, n-k); // V[0:n-k] = V[k:n]
    arrayCopy(A+n-k, B, k); // V[n-k:n] = B[0:k]
    delete[] B;
}

// 算法2.12B
template<typename T>
void cyclicLeftShiftB(const Vector<T>& V, int k) {
    auto A = V.data(), n = V.size();
    int d = gcd(n, k), n1 = n / d;      // 计算最大公约数
    T temp;                             // 辅助空间
    for (int i = 0; i < d; ++i) {       // 外层循环
        temp = A[i];                    // 放入辅助空间
        for (int j = 0; j < n1-1; ++j) {  // 内层循环，以k为步长位移
            A[(i+j*k) % n] = A[(i+(j+1)*k) % n];
        }
        A[(i+(n1-1)*k) % n] = temp;       // 最后一步，从辅助空间取出
    }
}

// 算法2.12B2（将i+j*k作为循环变量，正文中未给出）
template<typename T>
void cyclicLeftShiftB2(const Vector<T>& V, int k) {
    auto A = V.data(), n = V.size();
    int d = gcd(n, k), n1 = n / d;      // 计算最大公约数
    T temp;                             // 辅助空间
    for (int i = 0; i < d; ++i) {       // 外层循环
        temp = A[i];                    // 放入辅助空间
        int r, r2 = i;                  // r2表示r的下一个取值
        do {                            // 用r = i+j*k作为循环变量
            r = r2;
            A[r] = A[r2 = (r+k)%n];
        } while (r2 != i);
        A[r] = temp;                    // 最后一步，从辅助空间取出
    }
}

// 算法2.12C
template <typename T>
void cyclicLeftShiftC(const Vector<T>& V, int k) {
    auto A = V.data(), n = V.size();
    reverse(A, k);   // -> rV[0:k] + V[k:n]
    reverse(A, n);   // -> rV[k:n] + V[0:k]
    reverse(A, n-k); // -> V[k:n] + V[0:k]
}

const int n = 12;
const int k = 9;

int main() {
    auto V = range(1, 1+n);
    auto printV = [&V]() -> void {
        cout << "V   = " << V << endl;
    };
    printV();
    auto testShift = [&](function<void(const Vector<int>&, int)> shift) -> void {
        cout << "    cyclic shift(" << k << ")" << endl;
        shift(V, k); printV();
    };
    testShift(cyclicLeftShiftA<int>);
    testShift(cyclicLeftShiftB<int>);
    testShift(cyclicLeftShiftB2<int>);
    testShift(cyclicLeftShiftC<int>);
    return 0;
}
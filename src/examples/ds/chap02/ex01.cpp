// 例2.1 - 向量的扩容和缩容
// 展示向量扩容和缩容的过程

#include <iostream>
#include "vector.h"
using namespace std;

// 算法2.1B1 - 固定扩容
template<typename T>
void expandByAP(const Vector<T>& V, int d) {
    V.expand([=](int m) -> int { return m + d; });
}
// 算法2.1B2 - 比例扩容
template<typename T>
void expandByGP(const Vector<T>& V, double q) {
    V.expand([=](int m) -> int { return (int)(m * q); });
}

int main() {
    auto V = range(1, 2);
    for (int i = 2; i <= 9; ++i) {
        cout << "capacity = " << V.capacity() << " V = " << V << endl;
        V.push_back(i);
    }
    for (int i = 1; i <= 9; ++i) {
        cout << "capacity = " << V.capacity() << " V = " << V << endl;
        V.pop_back();
    }
    cout << "capacity = " << V.capacity() << " V = " << V << endl;
    return 0;
}

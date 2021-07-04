// 问题2.1 - 向量扩容
// 分析合适的方式让向量扩容

#include <iostream>
#include "vector.h"
using namespace std;

// 步骤1 - 改变向量的容量

// 步骤2 - 设计扩容算法
// 算法2.1B1 - 固定扩容
template<typename T>
void expandByAP(const Vector<T>& V, int d) {
    V.expand([=](int m) -> { return m + d; });
}
// 算法2.1B2 - 比例扩容
template<typename T>
void expandByGP(const Vector<T>& V, double q) {
    V.expand([=](int m) -> { return (int)(m * q); });
}

int main() {
    return 0;
}
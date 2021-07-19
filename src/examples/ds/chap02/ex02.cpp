// 例2.2 - 向量的扩容方式对比

#include <iostream>
#include "vector.h"
#include "utility.h"
using namespace std;

// 算法2.1B1 - 固定扩容
template<typename T>
void expandByAP(Vector<T>& V, int d) {
    V.expand([=](int m) -> int { return m + d; });
}
// 算法2.1B2 - 比例扩容
template<typename T>
void expandByGP(Vector<T>& V, double q) {
    V.expand([=](int m) -> int { return (int)(m * q); });
}

int main() {
    int start_capacity = 8;
    int end_capacity = 1<<20;
    int d = 1024, q = 2;
    // 规定d和q，固定起始容量和终止容量，对比两种扩容方式的时间
    // 注意，这里统计的时间还包括“插入元素”的时间
    auto testStrategy = [=](function<void(Vector<int>&)> strategy) -> double {
        auto V = range(0, start_capacity);
        auto t = calculateTime([=, &V]() -> void {
            while (V.capacity() < end_capacity) {
                strategy(V);
                while (V.size() < V.capacity()) {
                    V.push_back(0);
                }
            }
        });
        return t;
    };
    cout << "AP time = " << testStrategy([d](Vector<int>& V) -> void {
        expandByAP(V, d);
    }) << endl;
    cout << "GP time = " << testStrategy([q](Vector<int>& V) -> void {
        expandByGP(V, q);
    }) << endl;
    return 0;
}

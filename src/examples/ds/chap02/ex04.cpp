// 例2.4 - 向量置乱和排序

#include <iostream>
#include "vector.h"
#include "utility.h"
using namespace std;

const int start_size = 20;       // 用于展示的向量长度
const int long_size = 1<<20;     // 用于验证排序的向量长度

int main() {
    srand(time(0));
    // 第一部分：展示置乱和排序效果
    auto V = range(1, start_size+1);
    auto printV = [&V]() -> void {
        cout << "V   = " << V << endl;
    };
    printV();
    cout << "    shuffled" << endl;
    V.shuffle(); printV();
    cout << "    sorted" << endl;
    V.mergeSort(); printV();
    // 第二部分：对一个长的随机生成的向量进行排序
    Vector<int> VL(long_size, long_size, [](Rank r) -> int {
        return rand()*rand();
    });
    auto judgeSorted = [&VL]() -> bool { // 验证向量是否有序
        for (Rank r = 1; r < VL.size(); ++r) {
            if (VL[r-1] > VL[r]) { return false; }
        }
        return true;
    };
    cout << "sorted? = " << judgeSorted() << endl;
    auto t = calculateTime([&VL]() -> void {
        VL.mergeSort();
    });
    cout << "sort t  = " << t << endl;
    cout << "sorted? = " << judgeSorted() << endl;
    return 0;
}

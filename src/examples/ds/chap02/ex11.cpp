// 例2.11 - 分块表的循秩访问

#include <iostream>
#include "listlist.h"
using namespace std;

// 这个n对于O(n2)的普通列表来说，逐一循秩访问的总时间是非常夸张的
// 但是使用分块表之后，就能够在比较短的时间里完成访问工作
const int n = 1<<20;

int main() {
    auto V = range(0, n);              // 生成向量
    ListList<int> L = List<int>(V);    // 将向量转换成分块表
    bool equal = true;
    auto t = calculateTime([&]() -> void {
        for (int i = 0; i < n; ++i) {  // 逐个判断分块表的循秩访问是否和向量等价
            if (L[i]->value != V[i]) { // 这里向量的循秩访问是O(1)的，时间可以忽略
                equal = false;         // 因此统计的时间基本上就是L[i]循秩访问的时间
            }
        }
    });
    cout << "equal test  = " << equal << endl;
    cout << "time        = " << t << endl; // 输出逐一循秩访问的时间
    return 0;
}

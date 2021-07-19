// 例2.3 - 向量插入和删除元素

#include <iostream>
#include "vector.h"
using namespace std;

const int start_size = 4;       // 向量的初始长度
const int op_count = 20;        // 连续操作序列的长度
const double removeRatio = 0.3; // 在连续操作序列中“删除”的占比

int main() {
    srand(time(0));
    auto V = range(0, start_size);                     // 初始向量
    cout << "original = \t" << V << endl;
    int e = start_size;                                // 下一个插入的元素
    for (int i = 0; i < op_count; ++i) {               // 连续进行若干次操作
        bool remove = rand() < RAND_MAX * removeRatio; // 随机决定是插入还是删除
        if (remove && V.size() > 0) {
            int r = rand() % V.size();
            V.remove(r);
            cout << "remove(e @ " << r << ") = \t";
        } else {
            int r = rand() % (V.size() + 1);
            V.insert(e, r);
            cout << "insert(" << e++ << " @ " << r << ") = \t"; 
        }
        cout << V << endl;
    }
    return 0;
}

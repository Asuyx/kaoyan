// 3.2 - 队列
// 基于向量的队列
// 扩容、缩容、搬移
#define EXAMPLE_MODE
#include <iostream>
#include <iomanip>
#include "queue.h"
using namespace std;

Queue<int> Q;
const int operationCount = 10000000;

int main() {
    // 这个例子展示：连续进行若干次enqueue，结束后做dequeue
    // 观察在这个操作序列下的队列规模变化
    // 只有触发了扩容、缩容或搬移的时候，才输出
    int vs = Q.vectorSize();
    int vc = Q.vectorCapacity();
    auto output = [&](int i) -> void {
        if (i >= operationCount) {
            cout << "deq " << setw(9) << i - operationCount;
        } else {
            cout << "enq " << setw(9) << i;
        }
        cout << " Qs=" << setw(9) << Q.queueSize();
        cout << " Vs=" << setw(9) << (vs = Q.vectorSize());
        cout << " Vc=" << setw(9) << (vc = Q.vectorCapacity()); 
        cout << endl;
    };
    for (int i = 0; i < operationCount; ++i) {
        Q.enqueue(i);
        if (Q.vectorCapacity() != vc) {      // 触发了扩容
            output(i);
        }
    }
    for (int i = 0; i < operationCount; ++i) {
        Q.dequeue();
        if (Q.vectorCapacity() != vc || Q.vectorSize() != vs) { // 触发了缩容 
            // 这里使用的搬移的阈值是25%；但是每次缩容，只会将容量减半
            // 所以这连续的搬移操作结束之后，容量仍然有n^0.5的级别
            output(i + operationCount);
        }
    }
    return 0;
}
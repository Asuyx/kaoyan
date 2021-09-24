// 3.2 - 队列
// 循环队列 示例
#define EXAMPLE_MODE
#include <iostream>
#include <ctime>
#include "circular_queue.h"
using namespace std;

const int start_capacity = 4;
const int operation_count = 32;
const double enqueue_ratio = 0.8;

int main() {
    srand(time(0));
    CircularQueue<int> Q(start_capacity);
    int enqueueCount = 0;
    for (int i = 0; i < operation_count; i++) {
        bool enqueue = rand() < RAND_MAX * enqueue_ratio;
        cout << "================================ OPERATION " << i << " ================================" << endl;
        cout << "capcity = " << Q.vectorCapacity() << endl;
        cout << "front   = " << Q.queueFront() << endl;
        cout << "rear    = " << Q.queueRear() << endl;
        if (enqueue) {
            cout << "enqueue = " << enqueueCount << endl;
            Q.enqueue(enqueueCount++);
        } else {
            cout << "dequeue = " << Q.dequeue() << endl;
        }
        cout << Q << endl;
    }
    return 0;
}
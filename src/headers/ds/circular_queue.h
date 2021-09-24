// 3.2 - 队列 的代码
// 循环队列

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <iostream>
#include "vector.h"
using namespace std;

template <typename T>
class CircularQueue : protected Vector<T> { // 利用protected继承做访问限制
    int _front = 0;                         // 存储队头的秩，对应Q[d:n]的d
    int _rear  = 0;                         // 存储队尾的秩，对应Q[d:n]的n
    bool _empty = true;                     // 标识队列是否为空
public:
    T& front() const { return Vector<T>::data()[_front]; }
    void enqueue(T e);
    T dequeue();

    CircularQueue(int _capacity) : Vector<T>(_capacity) { // 重载构造函数
        Vector<T>::resize(_capacity);       // 直接修改向量的规模，因为n不再需要用向量的规模表示
    }

private:
    void expandQueue();

    // ----------------------------------以下内容不在笔记正文中----------------------------
public:
    bool empty() const { return _front == _rear && _empty; }
    bool full() const  { return _front == _rear && !_empty; }
    template <typename T1> friend ostream& operator<< (ostream& out, const CircularQueue<T1>& Q); // 使用cout方式打印

// 暴露给例子的接口，展示队列中的元素数量
#ifdef EXAMPLE_MODE
    int vectorCapacity() const { return Vector<T>::capacity(); }
    int queueRear() const { return _rear; }
    int queueFront() const { return _front; }
#endif
};

// 算法3.4B - 扩容
template <typename T>
void CircularQueue<T>::expandQueue() {
    int capacity = Vector<T>::capacity(); 
    Vector<T>::resize(capacity << 1);     // 加倍扩容
    arrayCopy(                            // 注意队列的尾部在向量的开头
        Vector<T>::data() + capacity,     // 即Q[d:n ; 0:d]，扩容后需要变为Q[d:n+d]
        Vector<T>::data(),                // 因此要把Q[0:d]移动到Q[n:n+d]去
        _rear                             // 并即时更新队尾的秩为n+d
    );
    _rear += capacity;
}

// 算法3.4A - 循环队列的入队
template <typename T>
void CircularQueue<T>::enqueue(T e) {
    if (full()) {                  // 如果已满，先扩容
        expandQueue();
    }
    Vector<T>::data()[_rear++] = e;
    _rear %= Vector<T>::capacity(); // 如果溢出，会回到0
    _empty = false;                // 插入元素之后肯定不会是空表
}

// 算法3.5 - 循环队列的出队
template <typename T>
T CircularQueue<T>::dequeue() {
    T e = Vector<T>::data()[_front++];    // 暂存队头的元素
    _front %= Vector<T>::capacity();      // 如果溢出，会回到0
    _empty = _front == _rear;             // 出队导致_front追上_rear，则为空表
    return e;
}

//  ----------------------------------以下内容不在笔记正文中----------------------------

template <typename T>
ostream& operator<< (ostream& out, const CircularQueue<T>& Q) {
    out << "Q<";
    if (!Q.empty()) {
        for (int i = Q._front; ; ) {
            if (i != Q._front) { out <<","; }
            out << Q[i];
            ++i, i %= Q.capacity();
            if (i == Q._rear) { break; }
        }
    }
    out << "<";
}

#endif
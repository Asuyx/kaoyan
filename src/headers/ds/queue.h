// 3.2 - 队列 的代码

#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "vector.h"
using namespace std;

template <typename T>
class Queue : protected Vector<T> { // 利用protected继承做访问限制
    int _front = 0;                 // 存储队头的秩，相当于Q[d:n]的d
public:
    T front() const { return Vector<T>::data()[_front]; }
    void enqueue(T e) { Vector<T>::push_back(e); }
    T dequeue() { T e = Vector<T>::data()[_front++]; moveElements(); return e; }

private:
    const double moveThreshold = 0.25;
    void moveElements(); // 算法3.3 - 搬移元素

    // ----------------------------------以下内容不在笔记正文中----------------------------
public:
    bool empty() const { return Vector<T>::size() == _front; }
    template <typename T1> friend ostream& operator<< (ostream& out, const Queue<T1>& V); // 使用cout方式打印

// 暴露给例子的接口，展示队列中的元素数量
#ifdef EXAMPLE_MODE
    int vectorCapacity() const { return Vector<T>::capacity(); }
    int vectorSize() const { return Vector<T>::size(); }
    int queueSize() const { return Vector<T>::size() - _front; }
#endif
};

template <typename T>
ostream& operator<< (ostream& out, const Queue<T>& V)
{
    out << "Q<";
    V.traverse([&](Rank r, const T& e) -> void {
        if (r > 0) { out <<","; }
        out << e;
    });
    out << "<";
}

// 算法3.3 - 搬移元素
template <typename T>
void Queue<T>::moveElements() {
    if (_front > Vector<T>::size() * (1 - moveThreshold)) {
        arrayCopy(
            Vector<T>::data(),
            Vector<T>::data() + _front,
            Vector<T>::size() - _front
        );                           // Q[d:n]搬移到V[0:n-d]
        Vector<T>::resize(Vector<T>::size() - _front); 
        _front = 0;                  // 向量缩容，队头秩被重置为0
    }
}


#endif
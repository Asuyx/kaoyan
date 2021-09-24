// 3.1 - 栈 的代码

#ifndef STACK_H
#define STACK_H

#include <iostream>
#include "vector.h"
using namespace std;

#ifdef STACK_OF_LIST
#include "list.h"
template <typename T>
class Stack : protected List<T> {
public:
    T& top() const { return List<T>::tail()->pred->value; }
    void push(T e) { List<T>::push_back(e); }
    T pop() { return List<T>::pop_back(); }
};
#endif

template <typename T>
class Stack : protected Vector<T> { // 利用protected继承做访问限制
public:
    T& top() const { return Vector<T>::data()[Vector<T>::size()-1]; }
    void push(T e) { Vector<T>::push_back(e); }
    T pop() { return Vector<T>::pop_back(); }

    // ----------------------------------以下内容不在笔记正文中----------------------------
    bool empty() const { return Vector<T>::size() == 0; }
    template <typename T1> friend ostream& operator<< (ostream& out, const Stack<T1>& V); // 使用cout方式打印
};

template <typename T>
ostream& operator<< (ostream& out, const Stack<T>& V)
{
    out << "S[";
    V.traverse([&](Rank r, const T& e) -> void {
        if (r > 0) { out <<","; }
        out << e;
    });
    out << ">";
}

#endif

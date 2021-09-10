// 3.1 - 栈 的主要代码

#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <functional>
#include "vector.h"
using namespace std;

template <typename T>
class Stack : protected Vector<T> { // 利用protected继承做访问限制
public:
    T top() const { return data()[size()-1]; }
    void push(T e) { push_back(e); }
    T pop() { return pop_back(); }
};

#endif
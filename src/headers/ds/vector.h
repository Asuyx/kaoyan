// 2.1 - 向量 的主要代码

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <functional>
#include "utility.h"
using namespace std;

typedef int Rank;

template <typename T>
class Vector {
    T* _data;      // 向量所基于的数组
    int _capacity; // 向量的容量
    int _size;     // 向量的规模
public:
    int capacity() { return _capacity; }
    int size() { return _size; }

    // ----------------------------------以下内容不在笔记正文中----------------------------

    Vector(); // 构造函数，生成空向量
    Vector(int capacity); // 构造函数，生成空向量，但开辟一定数组空间
    Vector(int capacity, int size, T* const data); // 构造函数，基于给定数组（复制数据）
    Vector(int capacity, int size, function<T(int)> fill); // 构造函数，基于填充规则
    Vector(const Vector<T>& V); // 复制构造函数

    template <typename T1> friend ostream& operator<< (ostream& out, const Vector<T1>& V); // 使用cout方式打印

    // -----------------------------------以下开始是示例代码的函数---------------------------
    // 循秩访问
    T& operator[](int index) {
        return _data[index];
    }

    void set_capacity(int new_capacity); // 算法2.1（向量扩容）
};

// 算法2.1A（改变向量的容量）
template <typename T>
void Vector<T>::set_capacity(int new_capacity) {
    T* old = _data;
    _data = new T[_capacity = new_capacity];
    _size = min(_size, _capacity); // 用于提升稳健性
    arrayCopy(_data, old, _size);
    delete[] old;
}


//  ----------------------------------以下内容不在笔记正文中----------------------------

template <typename T>
Vector<T>::Vector() {
	_data = nullptr;
	_size = _capacity = 0;
}

template <typename T>
Vector<T>::Vector(int capacity) {
	_data = new T[_capacity = capacity];
	_size = 0;
}

template<typename T>
Vector<T>::Vector(int capacity, int size, T* const data) {
    size = min(capacity, size);
    _data = new T[_capacity = capacity];
    arrayCopy(_data, data, _size = size);
}

template<typename T>
Vector<T>::Vector(int capacity, int size, function<T(int)> fill) {
    size = min(capacity, size);
    _data = new T[_capacity = capacity];
    for (_size = 0; _size < size; ++_size) {
        _data[_size] = fill(_size);
    }
}

template <typename T>
Vector<T>::Vector(const Vector<T>& V) {
    _data = new T[_capacity = V._capacity];
    arrayCopy(_data, V._data, _size = V._size);
}

template <typename T>
ostream& operator<< (ostream& out, const Vector<T>& V)
{
    out << "[";
    for (int i = 0; i < V._size; ++i) {
        if (i > 0) { out << ","; }
        out << V._data[i];
    }
    out << "]";
}

// 仿Python的range，生成向量
Vector<int> range(int start, int stop, int step=1) {
    if (step * (stop-start) <= 0) { return Vector<int>(); }
    int size = (abs(stop-start)+abs(step)-1)/abs(step);
    return Vector<int>(size, size, [=](int index) -> int { 
        return start + index * step;
    });
}

#endif
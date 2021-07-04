// 2.1 - 向量 的主要代码

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <functional>
#include "utility.h"
using namespace std;

typedef int Rank;

// 默认的扩容策略（加倍扩容）
static int expandBy2(int m) {
    return m << 1;
}

template <typename T>
class Vector {
    T* _data;      // 向量所基于的数组
    int _capacity; // 向量的容量
    int _size;     // 向量的规模

    static function<int(int)> capacity_expand_strategy = expandBy2; // 向量扩容的默认方法

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
    void expand(function<int(int)> strategy = nullptr); // 向量自动扩容

    void insert(T e, Rank r); // 算法2.2A - 向量插入元素
    void insert(const Vector<T>& V, Rank r); // 算法2.3A - 向量合并
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

// 算法2.1B - 基本框架
template <typename T>
void Vector<T>::expand(function<int(int)> strategy) {
    if (strategy == nullptr) {
        strategy = capacity_expand_strategy;
    }
    set_capacity(strategy(_capacity));
}

// 算法2.2A - 向量插入元素（单元）
template <typename T>
void Vector<T>::insert(T e, Rank r) {
    if (r < 0) { r = _size - r; } // 仿Python的记法，如果r是负数，从后向前计数
    if (_size + 1 > _capacity) { expand(); }    // 插入后会超出容量，需要扩容
    arrayCopy(_data+r+1, _data+r, _size-r, -1); // 从后向前，依次移动V[r:n]中的每个元素
    _data[r] = e;
    ++_size; // 更新向量的规模
}

// 算法2.3A - 向量插入元素（批量）
template <typename T>
void Vector<T>::insert(const Vector<T>& V, Rank r) {
    if (r < 0) { r = _size - r; } // 仿Python的记法，如果r是负数，从后向前计数
    int new_size = _size + V._size;                   // 计算插入之后的向量规模
    if (new_size > _capacity) { expand([=](int m) -> int {
        return max(new_size, capacity_expand_strategy(m));
    }); }        // 插入后会超出容量，需要扩容。这里省略指定的capacity_expand_strategy
    arrayCopy(_data+r+V._size, _data+r, _size-r, -1); // 从后向前，依次移动V[r:n]中的每个元素
    arrayCopy(_data+r, V._data, V._size);             // 依次插入V1中的元素
    _size = new_size; // 更新向量的规模
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
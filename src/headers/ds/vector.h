// 2.1 - 向量 的主要代码

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <functional>
#include "utility.h"
#include "sort.h"
#include "search.h"
using namespace std;

typedef int Rank;

const int min_vector_capacity = 4;    // 向量的最小容量

// 默认的扩容策略（加倍扩容）
inline static int expandBy2(int m) {
    return m << 1;
}

// 默认的缩容策略（减半缩容，缩容阈值25%）
inline static int shrinkBy2(int m, int n) {
    if (n < m >> 2) {
        return max(m >> 1, min_vector_capacity);
    } else {
        return m;
    }
}

// 设置默认的扩容、缩容策略
static function<int(int)> default_expand_strategy = expandBy2;
static function<int(int,int)> default_shrink_strategy = shrinkBy2;

// 向量的模板定义开始
template <typename T>
class Vector {
    T* _data;      // 向量所基于的数组
    int _capacity; // 向量的容量
    int _size;     // 向量的规模

public:
    int capacity() const { return _capacity; }
    int size() const { return _size; }
    T* data() const { return _data; }

    // ----------------------------------以下内容不在笔记正文中----------------------------

    Vector(); // 构造函数，生成空向量
    Vector(int capacity); // 构造函数，生成空向量，但开辟一定数组空间
    Vector(int capacity, int size, T* const data); // 构造函数，基于给定数组（复制数据）
    Vector(int capacity, int size, function<T(int)> fill); // 构造函数，基于填充规则
    Vector(const Vector<T>& V); // 复制构造函数

    template <typename T1> friend ostream& operator<< (ostream& out, const Vector<T1>& V); // 使用cout方式打印

    void push_back(T e); // 在尾部插入元素
    T pop_back();  // 删除尾部的元素
    T& back() const { return _data[_size-1]; } // 获取尾部的元素

    void resize(int size); // 修改规模
    template <typename T1> Vector<T1> changeAll(function<T1(T)> f) {
        return Vector<T1>(_capacity, _size, [&](int i) -> T1 {
            return f(_data[i]);
        });
    }

    // -----------------------------------以下开始是示例代码的函数---------------------------
    // 循秩访问
    T& operator[](Rank index) const {
        return _data[index];
    }

    void set_capacity(int new_capacity); // 算法2.1（向量扩容）
    void expand(function<int(int)> strategy = nullptr); // 向量自动扩容
    void shrink(function<int(int,int)> strategy = nullptr); // 向量自动缩容

    void insert(T e, Rank r); // 算法2.2A - 向量插入元素
    void insert(const Vector<T>& V, Rank r); // 算法2.3A - 向量批量插入元素
    void remove(Rank r);      // 算法2.4A - 向量删除元素
    void remove(Rank r1, Rank r2); // 向量批量删除元素（正文中未给出）

    Rank find(T e) const; // 算法2.5A - 向量查找元素
    Rank find(function<bool(Rank, const T&)> filter) const; // 向量查找元素（按条件）（正文中未给出）
    void traverse(function<void(Rank, const T&)> visit) const; // 算法2.6A - 向量遍历
    Vector<Rank> findAll(function<bool(Rank, const T&)> filter) const; // 算法2.6B - 批量查找
    void removeAll(function<bool(Rank, const T&)> filter); // 算法2.7 - 向量批量删除元素（按条件）

    void shuffle();    // 算法2.8 - 随机置乱
    void mergeSort(function<bool(const T&, const T&)> cmp);  // 算法2.9 - 归并排序
    void mergeSort();  // 重载函数，用默认<=

    Rank binarySearch(T e, function<bool(const T&, const T&)> cmp) const; // 算法2.10 - 折半查找
    Rank binarySearch(T e) const; // 重载函数，用默认<=

    void duplicate();  // 算法2.11A - 无序唯一化
    void duplicateSorted(); // 算法2.11B - 有序唯一化
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
        strategy = default_expand_strategy;
    }
    set_capacity(strategy(_capacity));
}

// 缩容框架（在笔记正文省略）
template <typename T>
void Vector<T>::shrink(function<int(int,int)> strategy) {
    if (strategy == nullptr) {
        strategy = default_shrink_strategy;
    }
    int new_capacity = strategy(_capacity, _size);
    if (new_capacity < _capacity) {
        set_capacity(new_capacity);
    }
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
        return max(new_size, default_expand_strategy(m));
    }); }        // 插入后会超出容量，需要扩容。这里省略指定的default_expand_strategy
    arrayCopy(_data+r+V._size, _data+r, _size-r, -1); // 从后向前，依次移动V[r:n]中的每个元素
    arrayCopy(_data+r, V._data, V._size);             // 依次插入V1中的元素
    _size = new_size; // 更新向量的规模
}

// 算法2.4A - 向量删除元素（单元）
template<typename T>
void Vector<T>::remove(Rank r) {
    if (r < 0) { r = _size - r; } // 仿Python的记法，如果r是负数，从后向前计数
    arrayCopy(_data+r, _data+r+1, _size-r-1); // 这里可以从前向后依次前移
    --_size;  // 更新向量的规模
    shrink(); // 如果有必要，则缩容
}

// 向量删除元素（批量）
template <typename T>
void Vector<T>::remove(Rank r1, Rank r2) {
    if (r1 < 0) { r1 = _size - r1; }
    if (r2 < 0) { r2 = _size - r2; }
    if (r1 > r2) { swap(r1, r2); } // 保证r1 <= r2
    arrayCopy(_data+r1, _data+r2, _size-r2); // 类似单元素删除的前移
    _size -= r2 - r1; // 更新向量的规模
    shrink();         // 如果有必要，则缩容
}

// 算法2.5A - 向量查找元素（单元）
template<typename T>
Rank Vector<T>::find(T e) const {
    for (Rank i = 0; i < _size; ++i) { // 检测每个元素是否等于e
        if (_data[i] == e) {
            return i;                 // 如果相等则返回秩
        }
    }
    return -1;                        // e不在向量中，返回-1
}

// 向量查找元素（单元）（按条件）
template <typename T>
Rank Vector<T>::find(function<bool(Rank, const T&)> filter) const {
    for (Rank i = 0; i < _size; ++i) {
        if (filter(i, _data[i])) {
            return i;
        }
    }
    return -1;
}

// 算法2.6A - 向量遍历（顺序遍历）
template<typename T>
void Vector<T>::traverse(function<void(Rank, const T&)> visit) const {
    for (Rank i = 0; i < _size; ++i) {
        visit(i, _data[i]);
    }
}

// 算法2.6B - 批量查找
template <typename T>
Vector<Rank> Vector<T>::findAll(function<bool(Rank, const T&)> filter) const {
    Vector<Rank> temp;
    traverse([=, &temp](Rank index, const T& e) -> void {
        if (filter(index, e)) {
            temp.push_back(index);
        }
    });
    return temp;
}

#define ALGORITHM_2_7_C_2

#ifdef ALGORITHM_2_7_A
// 算法2.7A（逐个查找-逐个删除）
template <typename T>
void Vector<T>::removeAll(function<bool(Rank, const T&)> filter) {
    Rank index;
    while ((index = find(filter)) >= 0) {
        remove(index);
    }
}
#endif
#ifdef ALGORITHM_2_7_B
// 算法2.7B（一次查找-逐个删除）
template <typename T>
void Vector<T>::removeAll(function<bool(Rank, const T&)> filter) {
    for (Rank i = 0; i < _size; ) {
        if (filter(i, _data[i])) {
            remove(i);
        } else {
            ++i; // 注意，如果remove了则不需要++i，否则会跳过1个元素
        }
    }
}
#endif
#ifdef ALGORITHM_2_7_B_2
// 算法2.7B2（一次查找-逐个删除，使用findAll）（正文中未给出）
template <typename T>
void Vector<T>::removeAll(function<bool(Rank, const T&)> filter) {
    auto indexes = findAll(filter);
    for (int i = 0; i < indexes._size; ++i) {
        remove(indexes[i] - i); // 注意删除之后的下标和findAll出的下标会差一个i
    }
}
#endif
#ifdef ALGORITHM_2_7_C
// 算法2.7C（一次查找-一次删除）
template <typename T>
void Vector<T>::removeAll(function<bool(Rank, const T&)> filter) {
    int k = 0;      // 用来记录偏移量，即V[0:i]中满足filter的数量
    for (Rank i = 0; i < _size; ++i) {
        if (filter(i, _data[i])) {
            ++k;    // 满足filter条件，记录偏移量
        } else {
            _data[i-k] = _data[i]; // 不满足filter，移动元素
        }
    }
    _size -= k;     // 直接缩减_size抛弃掉末尾的元素
    shrink();       // 如果有必要，则缩容
}
#endif
#ifdef ALGORITHM_2_7_C_2
// 算法2.7C2（一次查找-一次删除，去掉无意义赋值）（正文中未给出）
template <typename T>
void Vector<T>::removeAll(function<bool(Rank, const T&)> filter) {
    Rank i = 0;
    for (; i < _size; ++i) {
        if (filter(i, _data[i])) {
            break;  // 找到第1个满足filter的点，此前是不需要移动元素的
        }
    }
    if (i == _size) { return; }    // 全部不满足filter，直接返回
    int k = 1;      // 用来记录偏移量，即V[0:i]中满足filter的数量
    for (++i; i < _size; ++i) {
        if (filter(i, _data[i])) {
            ++k;    // 满足filter条件，记录偏移量
        } else {
            _data[i-k] = _data[i]; // 不满足filter，移动元素
        }
    }
    _size -= k;     // 直接缩减_size抛弃掉末尾的元素
    shrink();       // 如果有必要，则缩容
}
#endif

// 算法2.8A - 随机置乱
template <typename T>
void Vector<T>::shuffle() {
    for (Rank i = _size; i > 0; --i) {
        swap(_data[i-1], _data[rand() % i]);
    }
}

// 算法2.9A - 归并排序
template <typename T>
void Vector<T>::mergeSort(function<bool(const T&, const T&)> cmp) {
    MergeSort<T> sort;
    sort(_data, _size, cmp);
}

template <typename T>
void Vector<T>::mergeSort() {
    mergeSort(less_equal<T>());
}

// 算法2.10A - 折半查找
template <typename T>
Rank Vector<T>::binarySearch(T e, function<bool(const T&, const T&)> cmp) const {
    BinarySearch<T> search;
    return search(_data, _size, e, cmp);
}

template <typename T>
Rank Vector<T>::binarySearch(T e) const {
    return binarySearch(e, less_equal<T>());
}

// 算法2.11A - 无序向量唯一化
template <typename T>
void Vector<T>::duplicate() {
    Rank i = 1, k = 1;                  // 快指针i检索，慢指针k填充
    while (i < _size) {                 // V[0:k]始终是V[0:i]唯一化后的结果
        bool existInPrefix = false;     // 要判断V[i]是否在V[0:i]中
        for (Rank j = 0; j < k; ++j) {  // 只需要判断是否在V[0:k]中即可
            if (_data[i] == _data[j]) {
                existInPrefix = true; break;
            }
        }
        if (!existInPrefix) {           // 如果V[i]不在它的前缀中
            _data[k++] = _data[i++];    // 移动元素，并同时移动快慢指针
        } else { ++i; }                 // 否则只需要移动快指针
    }
    _size = k;                          // 修改规模，和慢指针对齐
    shrink();                           // 如果有必要，则缩容
}

// 算法2.11B - 有序向量唯一化
template <typename T>
void Vector<T>::duplicateSorted() {
    Rank i = 1, k = 1;                  // 快指针i检索，慢指针k填充
    while (i < _size) {                 // V[0:k]始终是V[0:i]唯一化后的结果
        bool existInPrefix = _data[i] == _data[k-1];  // 改进后的计算方法
        if (!existInPrefix) {           // 如果V[i]不在它的前缀中
            _data[k++] = _data[i++];    // 移动元素，并同时移动快慢指针
        } else { ++i; }                 // 否则只需要移动快指针
    }
    _size = k;                          // 修改规模，和慢指针对齐
    shrink();                           // 如果有必要，则缩容
}

//  ----------------------------------以下内容不在笔记正文中----------------------------

template <typename T>
Vector<T>::Vector() {
	_data = new T[_capacity = min_vector_capacity];
	_size = 0;
}

template <typename T>
Vector<T>::Vector(int capacity) {
	_data = new T[_capacity = max(min_vector_capacity, capacity)];
	_size = 0;
}

template<typename T>
Vector<T>::Vector(int capacity, int size, T* const data) {
    capacity = max(min_vector_capacity, capacity);
    size = min(capacity, size);
    _data = new T[_capacity = capacity];
    arrayCopy(_data, data, _size = size);
}

template<typename T>
Vector<T>::Vector(int capacity, int size, function<T(int)> fill) {
    capacity = max(min_vector_capacity, capacity);
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
ostream& operator<< (ostream& out, const Vector<T>& V) {
    out << "[";
    V.traverse([&](Rank r, const T& e) -> void {
        if (r > 0) { out <<","; }
        out << e;
    });
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

template <typename T>
void Vector<T>::push_back(T e) {
    insert(e, _size);
}

template <typename T>
T Vector<T>::pop_back() {
    T temp = _data[_size-1];
    remove(_size-1);
    return temp;
}

template <typename T>
void Vector<T>::resize(int size) {
    if (_size < size) { // 增加规模
        if (size > _capacity) { expand([=](int m) -> int {
            return max(size, default_expand_strategy(m));
        }); }           // 如果需要扩容
        _size = size;
    } else if (_size > size) { // 减少规模
        _size = size;
        shrink();
    }
}

#endif

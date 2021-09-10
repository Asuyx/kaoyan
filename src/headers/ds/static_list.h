// 2.2 - 列表 中的 静态链表
// 双链线性静态链表

#ifndef STATIC_LIST_H
#define STATIC_LIST_H

#include <iostream>
#include <functional>
#include "vector.h"
#include "sort.h"
using namespace std;

typedef int Rank;

template <typename T>
class StaticList {
private:
    int _size;
    static const Rank _head = 0;
    static const Rank _tail = 1;
    Vector<T> V;    // 将value、pred和succ拆成三个向量
    Vector<Rank> Vpred;
    Vector<Rank> Vsucc;
public:
    int size() const { return _size; }
    Rank head() const { return _head; }
    Rank tail() const { return _tail; }
    T& get(Rank r) const { return V[r]; }
    Rank& getPred(Rank r) const { return Vpred[r]; }
    Rank& getSucc(Rank r) const { return Vsucc[r]; }

    // ----------------------------------以下内容不在笔记正文中----------------------------
private:
    void make_vector(T* A, int n);
public:
    StaticList(T* A, int n); // 利用数组生成列表
    StaticList(const Vector<T>& V); // 利用向量生成列表
    StaticList(const StaticList<T>& L); // 复制构造函数

    Rank forward(Rank start, int step) const;
    Rank backward(Rank start, int step) const;
    T& operator[](Rank index) const; // "循秩访问"

    void traverse(function<void(Rank)> visit) const; // 遍历

    template <typename T1> friend ostream& operator<< (ostream& out, const StaticList<T1>& L); // 使用cout方式打印

    void push_back(T e); // 在尾部插入元素
    T pop_back();  // 删除尾部的元素
    void push_front(T e); // 在头部插入元素
    T pop_front(); // 删除头部的元素

private:
    void mergeSort(Rank& s, Rank t, int n, function<bool(const T&, const T&)> cmp);
public:
    void mergeSort(function<bool(const T&, const T&)> cmp);
    void mergeSort();
    
    template <typename T1> friend void mergeSortDirectly(StaticList<T1>& L, function<bool(const T1&, const T1&)> cmp);

    // -----------------------------------以下开始是示例代码的函数---------------------------
    StaticList(); // 构造函数，生成空列表
    
    void insertAsPred(T e, Rank r); // 列表插入元素（前插，正文中未给出）
    void insertAsSucc(T e, Rank r); // 列表插入元素（后插，正文中未给出）
    void remove(Rank r); // 列表删除元素
    Rank find(T e) const; // 列表查找元素（正文中未给出）
};

template <typename T>
StaticList<T>::StaticList() {
    _size = 0;
    V.resize(2); Vpred.resize(2); Vsucc.resize(2);
    Vsucc[_head] = _tail; Vpred[_head] = -1;
    Vpred[_tail] = _head; Vsucc[_tail] = -1;
}

template <typename T>
void StaticList<T>::insertAsPred(T e, Rank r) {
    Rank x = V.size(), s = Vpred[r];
    V.push_back(e);                 // 创建新节点
    Vpred.push_back(s);             // 将新节点的链子挂在s和r之间
    Vsucc.push_back(r);
    Vsucc[s] = Vpred[r] = x;        // 断开p和q之间的两条链子
    ++_size;                        // 更新列表规模
}

template <typename T>
void StaticList<T>::insertAsSucc(T e, Rank r) {
    Rank x = V.size(), s = Vsucc[r];
    V.push_back(e);                 // 创建新节点
    Vpred.push_back(r);             // 将新节点的链子挂在s和r之间
    Vsucc.push_back(s);
    Vsucc[r] = Vpred[s] = x;        // 断开p和q之间的两条链子
    ++_size;                        // 更新列表规模
}

// 算法2.16B
template <typename T>
void StaticList<T>::remove(Rank r) {
    Vsucc[Vpred[r]] = Vsucc[r];        // 先执行列表中的删除
    Vpred[Vsucc[r]] = Vpred[r]; 
    --_size;                           // 算法2.16A到此为止
    if (r != V.size()-1) {
        Rank last = V.size() -1;       // 再执行向量中的删除
        V[r] = V[last];                // 将V[last]移动到V[r]的位置上
        Vsucc[Vpred[last]] = r;        // 连接V[last]的链子转为连到V[r]上
        Vpred[Vsucc[last]] = r;
    }
    V.pop_back();                      // 删除V的最后一个节点
    Vpred.pop_back();
    Vsucc.pop_back();
}

template <typename T>
Rank StaticList<T>::find(T e) const {
    for (Rank r = Vpred[_tail]; r != _head; r = Vpred[r]) {
        if (V[r] == e) { return r; }
    }
    return -1;
}

//  ----------------------------------以下内容不在笔记正文中----------------------------

template <typename T>
void StaticList<T>::make_vector(T* A, int n) {
    V.resize(n+2); Vpred.resize(n+2); Vsucc.resize(n+2);
    for (int i = 0; i < n; ++i) {
        V[i+2] = A[i];
    }
    for (int i = 0; i < n+2; ++i) {
        Vpred[i] = i-1; Vsucc[i] = i+1;
    }
    Vsucc[_tail] = -1;
    if (n > 0) {
        Vpred[Vsucc[_head] = 2] = _head;
        Vsucc[Vpred[_tail] = n+1] = _tail;
    }
}

template <typename T>
StaticList<T>::StaticList(T* A, int n) {
    _size = n;
    make_vector(A, n);
}

template <typename T>
StaticList<T>::StaticList(const Vector<T>& V) {
    _size = V.size();
    make_vector(V.data(), V.size());
}

template <typename T>
StaticList<T>::StaticList(const StaticList<T>& L): V(L.V), Vpred(L.Vpred), Vsucc(L.Vsucc) {
    _size = L._size; // 利用向量的复制构造函数直接深复制
}

template <typename T>
Rank StaticList<T>::forward(Rank start, int step) const {
    for (; step > 0; --step) {
        start = Vsucc[start];
    }
    return start;
}

template <typename T>
Rank StaticList<T>::backward(Rank start, int step) const {
    for (; step > 0; --step) {
        start = Vpred[start];
    }
    return start;
}

template <typename T>
T& StaticList<T>::operator[](Rank index) const {
    if (index <= _size / 2) {
        return V[forward(_head, index+1)];
    } else {
        return V[backward(_tail, _size-index)];
    }
}

template <typename T>
void StaticList<T>::traverse(function<void(Rank)> visit) const {
    for (Rank r = Vsucc[_head]; r != _tail; r = Vsucc[r]) {
        visit(r);
    } 
}

template <typename T>
ostream& operator<< (ostream& out, const StaticList<T>& L)
{
    out << "L(";
    L.traverse([&](Rank r) -> void {
        if (r != L.Vsucc[L._head]) { out <<","; }
        out << L.V[r];
    });
    out << ")";
}

template <typename T>
void StaticList<T>::push_back(T e) {
    insertAsPred(e, _tail);
}

template <typename T>
void StaticList<T>::push_front(T e) {
    insertAsSucc(e, _head);
}

template <typename T>
T StaticList<T>::pop_back() {
    Rank r = Vpred[_tail];
    T temp = V[r];
    remove(r);
    return temp;
}

template <typename T>
T StaticList<T>::pop_front() {
    Rank r = Vsucc[_head];
    T temp = V[r];
    remove(r);
    return temp;
}

template <typename T> // 归并排序，仿照动态列表的形式
void StaticList<T>::mergeSort(Rank& s, Rank t, int n, function<bool(const T&, const T&)> cmp) {
    if (n <= 1) { return; }
    int m = n / 2;
    Rank mid = forward(s, m);
    mergeSort(s, mid, m, cmp);
    mergeSort(mid, t, n-m, cmp);
    Rank sp = Vpred[s];
    Rank p = s, q = mid;
    while (p != q && q != t) {
        if (cmp(V[p], V[q])) {
            p = Vsucc[p];
        } else {
            Rank t = q;
            q = Vsucc[q]; // 这里一定需要将6次链子的赋值展开写，不能用remove，否则会发生问题
            Vsucc[Vpred[q] = Vpred[t]] = q; // 而动态列表的删除是安全的，所以不展开只会降低效率，不会出错
            Vsucc[Vpred[t] = Vpred[p]] = t;
            Vsucc[t] = p; Vpred[p] = t;
        }
    }
    s = Vsucc[sp];
}

template <typename T>
void StaticList<T>::mergeSort(function<bool(const T&, const T&)> cmp) {
    Rank start = Vsucc[_head];
    mergeSort(start, _tail, _size, cmp);
}

template <typename T>
void StaticList<T>::mergeSort() {
    mergeSort(less_equal<T>());
}

// 这里展示了另一个排序静态列表的方式
// 无视静态列表L中的内在逻辑次序，直接对它的数据向量L.V进行排序
// 然后重新指定静态列表的前驱/后继指针
// 由于无视了内在逻辑次序，因此相等的数据无法保持排序前的相对位置，即此种排序方法不具有稳定性。
// 但它确实比上面实现的mergeSort要快，参见例2.9
template <typename T>
void mergeSortDirectly(StaticList<T>& L, function<bool(const T&, const T&)> cmp) {
    if (L._size == 0) { return; }
    MergeSort<T> sortFunction;
    sortFunction(L.V.data()+2, L._size, cmp);
    for (int i = 0; i < L._size+2; ++i) {
        L.Vpred[i] = i-1; L.Vsucc[i] = i+1;
    }
    L.Vsucc[L._tail] = -1;
    L.Vpred[L.Vsucc[L._head] = 2] = L._head;
    L.Vsucc[L.Vpred[L._tail] = L._size+1] = L._tail;
}

#endif
